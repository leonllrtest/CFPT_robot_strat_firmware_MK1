#include "line_follower.h"
#include "communication.h"

#include "utils.h"

#include <math.h>

#include "GyverPID.h"

// https://towardinfinity.medium.com/pid-for-line-follower-11deb3a1a643-

// true=left, false=right
bool lastDirection;

float angleCorrection = 0;

//Define Variables we'll be connecting to
double Setpoint = 0;
double SensorInput, AngleOutput;

double variableKp = _Kp;
double variableKi = _Ki;
double variableKd = _Kd;


//Specify the links and initial tuning parameters
GyverPID lineFollowPID(variableKp, variableKi, variableKd);
int PIDCorrection = 0;


void lineFollowStartup()
{
   sendCommand("PW" + String(BASE_SPEED));
   sendCommand("PD" + String(BASE_SPEED));
   sendCommand("PG" + String(BASE_SPEED));

   uint8_t sensorReadingLine = getLineSensorReadingBlocking();

   uint8_t lineLeft = (sensorReadingLine & 0xF0) >> 4;
   uint8_t lineRight = reverseBits((sensorReadingLine & 0x0F));

   lineFollowPID.setpoint = 0;
   lineFollowPID.setDt(10);

   delay(200);
}


// TODO: implement PID system
void lineFollowerTick()
{
   uint8_t sensorReadingLine = getLineSensorReadingBlocking();

   uint8_t lineLeft = (sensorReadingLine & 0x0F);
   uint8_t lineRight = reverseBits(((sensorReadingLine & 0xF0) >> 4));

   
   /*if(lineLeft > lineRight) // select only the most used line
   {
      angleCorrection = -getAngleCorrection(lineLeft);

      lastDirection = false;
   }
   else if (lineLeft < lineRight)
   {
      angleCorrection = getAngleCorrection(lineRight);

      lastDirection = true;
   }
   else if(sensorReadingLine == 0x00)
   {
      // quited track
      if(lastDirection)
      {
         // recover to left
         int angleCorrection = -recoveryCoeff;
      }
      else
      {
         // recover to right
         angleCorrection = recoveryCoeff;
      }
   }*/

   angleCorrection = getAngleCorrection(lineLeft) - getAngleCorrection(lineRight);

   // go left
   //sendCommand("PD" + String(0));

   //sendCommand("PG" + String(0 - angleCorrection));


   lineFollowPID.input = angleCorrection;
   lineFollowPID.getResult();

   /*if(abs(AngleOutput) < PID_STRAIGHT_THRESHOLD)
   {
      sendCommand("PG" + String(BASE_SPEED));

      sendCommand("PD" + String(BASE_SPEED));
   }
   else
   {*/
   PIDCorrection = (int)((lineFollowPID.output) * ((float)BASE_SPEED*(float)PID_ANGLE_COEFF));

   PIDCorrection = CLAMP(PIDCorrection, -BASE_SPEED, BASE_SPEED);

   sendCommand("PD" + String(BASE_SPEED - PIDCorrection));

   sendCommand("PG" + String(BASE_SPEED + PIDCorrection));
   //}
   
   

   //previousLineLeft = lineLeft;
   //previousLineRight = lineRight;
}

/*
int getBaseSpeed(uint8_t lineUsed)
{
   uint8_t coeffBit0 = (lineUsed & 0x01);
   uint8_t coeffBit1 = (lineUsed & 0x02) >> 1;
   uint8_t coeffBit2 = (lineUsed & 0x04) >> 2;
   uint8_t coeffBit3 = (lineUsed & 0x08) >> 3;

   // select from cruise speed or coef (if only middle selected)
   return (lineUsed < 2) ? (coeffBit0 * SPEED_COEFF_0) : ((coeffBit1 * SPEED_COEFF_1) + (coeffBit2 * SPEED_COEFF_2) + (coeffBit3 + SPEED_COEFF_3));

}*/

float getAngleCorrection(uint8_t lineUsed)
{
   uint8_t coeffBit0 = (lineUsed & 0x01);
   uint8_t coeffBit1 = (lineUsed & 0x02) >> 1;
   uint8_t coeffBit2 = (lineUsed & 0x04) >> 2;
   uint8_t coeffBit3 = (lineUsed & 0x08) >> 3;

   uint8_t sensorNumber = countBits(lineUsed);

   uint8_t correction = 0;

   if(sensorNumber == 0)
   {
      // last angle Correction
   }
   else if (sensorNumber == 1)
   {
      correction  = (float)(((float)coeffBit0 * ANGLE_COEFF_0 * 1) + ((float)coeffBit1 * ANGLE_COEFF_1 * 1) + ((float)coeffBit2 * ANGLE_COEFF_2 * 1) + ((float)coeffBit3 + ANGLE_COEFF_3 * 1));
   }
   else if (sensorNumber > 1)
   {
      correction = (float)((((float)coeffBit0 * ANGLE_COEFF_0 * 1) + ((float)coeffBit1 * ANGLE_COEFF_1 * 1) + ((float)coeffBit2 * ANGLE_COEFF_2 * 1) + ((float)coeffBit3 + ANGLE_COEFF_3 * 1)) / (float)sensorNumber);
   }

   // should be inversed, if multiple line are triggered, the situation is less worst than
   // if only furthest if triggered

   // only apply correction if not going straight
   //have minium angleCorrection to be 0 when substraction happens
   return correction;// min(baseSpeed, correction);
}