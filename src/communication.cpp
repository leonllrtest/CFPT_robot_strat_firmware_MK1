#include <Arduino.h>
#include <HardwareSerial.h>
#include "communication.h"

HardwareSerial RobotSerial(1);

void initCommunication()
{
   RobotSerial.begin(115200, SERIAL_8N1, 20, 21);

   delay(500);
}

void sendCommand(String cmd)
{
   RobotSerial.print(cmd + "\r");
}


void sendCommandBlocking(String cmd)
{
   emptySerialQueue();

   sendCommand(cmd);
   waitForCmdReturnBlocking();
}

void emptySerialQueue()
{
   while(RobotSerial.available() > 0) RobotSerial.read();
}

void waitForCmdReturnBlocking()
{

   int timeoutCounter = 0;

   // wait until the char \n is recieved
   while(1)
   {
      if(RobotSerial.available())
      {
         if((char)RobotSerial.read() == '\n')
         {
            return;
         }
      }

      delay(10);
      timeoutCounter++;

      if(timeoutCounter > 500)
      {
         return; // timeout de 5s
      }
   }
}


uint8_t getLineSensorReadingBlocking()
{
   char cmd_buf[2] = {0, 0};
   uint8_t cmd_buf_pointer = 0;

   emptySerialQueue(); // pour éviter des mauvais résultats

   sendCommand("CL");
   delay(10);

   while(cmd_buf_pointer < 2)
   {
      while(RobotSerial.available() < 1);

      // Serial.println("Recieved char");

      cmd_buf[cmd_buf_pointer] = (char)RobotSerial.read();
      cmd_buf_pointer++;
   }

   return (uint8_t)strtol(cmd_buf, (char**)NULL, 16);
}