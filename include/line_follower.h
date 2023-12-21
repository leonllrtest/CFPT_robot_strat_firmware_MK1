#define BASE_SPEED 20

typedef unsigned char uint8_t;

// tuned a bit for base speed of 20
//const unsigned char ANGLE_COEFF_0 = 0; -> 0%
//const unsigned char ANGLE_COEFF_1 = 1; -> 1/20 ->  5%
//const unsigned char ANGLE_COEFF_2 = 6; -> 30%
//const unsigned char ANGLE_COEFF_3 = 10;

const float ANGLE_COEFF_0 = 0.0;
const float ANGLE_COEFF_1 = 0.05;
const float ANGLE_COEFF_2 = 0.3;
const float ANGLE_COEFF_3 = 0.5;


// NOTE: have each block be abs(most-less)-lesser
const unsigned char SPEED_COEFF_0 = 70;
const unsigned char SPEED_COEFF_1 = 50;
const unsigned char SPEED_COEFF_2 = 35;
const unsigned char SPEED_COEFF_3 = 15;

void lineFollowStartup();

void lineFollowerTick();

int getBaseSpeed(uint8_t lineUsed);
int getAngleCorrection(uint8_t baseSpeed, uint8_t lineUsed);
