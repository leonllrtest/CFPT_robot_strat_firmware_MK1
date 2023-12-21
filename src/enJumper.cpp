#include <Arduino.h>

#include "enJumper.h"

bool getIfJumperPresent()
{
   return digitalRead(JUMPER_PIN);
}