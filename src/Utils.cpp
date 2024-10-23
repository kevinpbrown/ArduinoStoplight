#include <Arduino.h>

#include "Utils.hpp"
#include "config.h"

void serialLog(String m)
{
#if !MODBUS_ENABLED
  Serial.print(m + "\n");
#endif
}