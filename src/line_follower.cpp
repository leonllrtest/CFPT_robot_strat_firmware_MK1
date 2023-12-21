#include "line_follower.h"
#include "communication.h"

#include "utils.h"

#include <math.h>


void lineFollowStartup()
{
   sendCommand("PW" + String(BASE_SPEED));
   delay(200);
}


// TODO: implement PID system
void lineFollowerTick()
{
   uint8_t sensorReadingLine = getLineSensorReadingBlocking();

   uint8_t lineLeft = (sensorReadingLine & 0xF0) >> 4;
   uint8_t lineRight = reverseBits((sensorReadingLine & 0x0F));

   
   if(lineLeft > lineRight) // select only the most used line
   {
      uint8_t baseSpeed = getBaseSpeed(lineLeft);
      uint8_t angleCorrection = getAngleCorrection(baseSpeed, lineLeft);

      sendCommand("PG" + String(baseSpeed));

      sendCommand("PD" + String(baseSpeed - angleCorrection));
   }
   else if (lineLeft < lineRight)
   {
      uint8_t baseSpeed = getBaseSpeed(lineRight);
      uint8_t angleCorrection = getAngleCorrection(baseSpeed, lineRight);

      sendCommand("PD" + String(baseSpeed));

      sendCommand("PG" + String(baseSpeed - angleCorrection));
   }




            
}

int getBaseSpeed(uint8_t lineUsed)
{
   uint8_t coeffBit0 = (lineUsed & 0x01);
   uint8_t coeffBit1 = (lineUsed & 0x02) >> 1;
   uint8_t coeffBit2 = (lineUsed & 0x04) >> 2;
   uint8_t coeffBit3 = (lineUsed & 0x08) >> 3;

      return abs(abs(abs((coeffBit0 * SPEED_COEFF_0) - (coeffBit1 * SPEED_COEFF_1)) - (coeffBit2 * SPEED_COEFF_2)) - (coeffBit3 + SPEED_COEFF_3));

}

int getAngleCorrection(uint8_t baseSpeed, uint8_t lineUsed)
{

   uint8_t coeffBit0 = (lineUsed & 0x01);
   uint8_t coeffBit1 = (lineUsed & 0x02) >> 1;
   uint8_t coeffBit2 = (lineUsed & 0x04) >> 2;
   uint8_t coeffBit3 = (lineUsed & 0x08) >> 3;

   // should be inversed, if multiple line are triggered, the situation is less worst than
   // if only furthest if triggered

   return (((float)coeffBit0 * ANGLE_COEFF_0) + ((float)coeffBit1 * ANGLE_COEFF_1) + ((float)coeffBit2 * ANGLE_COEFF_2) + ((float)coeffBit3 + ANGLE_COEFF_3));
}