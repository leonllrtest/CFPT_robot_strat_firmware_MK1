#include <Arduino.h>

void initCommunication();
void sendCommand(String cmd);
void emptySerialQueue();

void sendCommandBlocking(String cmd); // always use for move commands if possible
void waitForCmdReturnBlocking();

uint8_t getLineSensorReadingBlocking();