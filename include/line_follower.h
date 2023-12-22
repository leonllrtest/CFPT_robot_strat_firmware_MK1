#define BASE_SPEED 10


typedef unsigned char uint8_t;

#define _Kp 1.3
#define _Ki 0.0
#define _Kd 0.0

#define PID_STRAIGHT_THRESHOLD 0.01

// tuned a bit for base speed of 20
//const unsigned char ANGLE_COEFF_0 = 0; -> 0%
//const unsigned char ANGLE_COEFF_1 = 1; -> 1/20 ->  5%
//const unsigned char ANGLE_COEFF_2 = 6; -> 30%
//const unsigned char ANGLE_COEFF_3 = 10;

const float ANGLE_COEFF_0 = 0.2;
const float ANGLE_COEFF_1 = 0.8;
const float ANGLE_COEFF_2 = 1.2;
const float ANGLE_COEFF_3 = 1.3;
const float recoveryCoeff = 1.5;

#define PID_ANGLE_COEFF 1

void lineFollowStartup();

void lineFollowerTick();

// int getBaseSpeed(uint8_t lineUsed);
float getAngleCorrection(uint8_t lineUsed);
