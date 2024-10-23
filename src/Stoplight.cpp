#include <Arduino.h>

#include "Stoplight.hpp"
#include "Utils.hpp"

void Stoplight::tick()
{
  if (activeLight == YELLOW && ((millis() - lastStateChange) / 1000) >= yellowDuration)
  {
    serialLog("Transitioning yellow to red in tick()");
    activeLight = RED;
    lastStateChange = 0;
  }
}

void Stoplight::transitionToStop()
{
  if (activeLight != GREEN)
  {
    // TODO: Introduce an error state
    serialLog("ERROR: NEED TO BE GREEN TO TRANSITION TO STOP");
    return;
  }

  lastStateChange = millis();
  activeLight = YELLOW;
}

void Stoplight::readyToGo()
{
  if (activeLight != RED)
  {
    // TODO: Introduce an error state
    serialLog("ERROR: NEED TO BE RED BEFORE WE CAN GO");
    return;
  }

  lastStateChange = millis();
  activeLight = GREEN;
}

String Stoplight::statusString()
{
  return "RED=" + String(isRedOn()) + " YELLOW=" + String(isYellowOn()) + " GREEN=" + String(isGreenOn());
}