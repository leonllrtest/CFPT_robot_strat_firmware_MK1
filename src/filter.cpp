#include "filter.h"

bool filterLineFollowerValidate(uint8_t lastLine, uint8_t currentLine)
{
   uint8_t error = currentLine - lastLine;

   return (error < LF_THRESHOLD);
}