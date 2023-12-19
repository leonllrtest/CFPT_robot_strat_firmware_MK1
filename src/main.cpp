#include <Arduino.h>
#include "communication.h"

#define LED_PIN 8

#define BASE_SPEED 20

const unsigned char COEFF_0 = 0;
const unsigned char COEFF_1 = 1;
const unsigned char COEFF_2 = 4;
const unsigned char COEFF_3 = 8;

unsigned char reverseBits(unsigned char num);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Serial for usb debug

  pinMode(LED_PIN, OUTPUT);

  initCommunication();
  emptySerialQueue();

  Serial.println("Begun USB serial port");

  sendCommand("PW" + String(BASE_SPEED));
  delay(200);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t sensorReadingLine = getLineSensorReadingBlocking();

  uint8_t lineLeft = (sensorReadingLine & 0xF0) >> 4;
  uint8_t lineRight = reverseBits((sensorReadingLine & 0x0F));

  if(lineLeft > lineRight) // select only the most used line
         {

            uint8_t coeffBit1 = (lineLeft & 0x02) >> 1;
            uint8_t coeffBit2 = (lineLeft & 0x04) >> 2;
            uint8_t coeffBit3 = (lineLeft & 0x08) >> 3;

            uint8_t angleCorrection = (coeffBit1 * COEFF_1) + (coeffBit2 * COEFF_2) + (coeffBit3 + COEFF_3);


            sendCommand("PG" + String(BASE_SPEED));

            sendCommand("PD" + String(BASE_SPEED - angleCorrection));

         }
         else if (lineLeft < lineRight)
         {

            uint8_t coeffBit1 = (lineRight & 0x02) >> 1;
            uint8_t coeffBit2 = (lineRight & 0x04) >> 2;
            uint8_t coeffBit3 = (lineRight & 0x08) >> 3;

            uint8_t angleCorrection = (coeffBit1 * COEFF_1) + (coeffBit2 * COEFF_2) + (coeffBit3 + COEFF_3);

            sendCommand("PD" + String(BASE_SPEED));

            sendCommand("PG" + String(BASE_SPEED - angleCorrection));
         }

	delay(10);
}

unsigned char reverseBits(unsigned char num) {
    unsigned char reversedNum = 0;
    unsigned char temp;
   
   int i = 0;

    // Reverse the first 4 bits
    for (; i < 4; i++) {
        temp = (num & (1 << i));
        if(temp)
            reversedNum |= (1 << ((4 - 1) - i));
    }

    // Copy the last 4 bits
    reversedNum |= (num & (0xF0));

    return reversedNum;
}