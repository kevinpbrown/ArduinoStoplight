#include <Arduino.h>

#include "Timer.hpp"

void Timer::tick(bool enabled)
{
  unsigned long now = millis();
  if (!active)
  {
    if (enabled && lastTrigger == -1)
    {
      lastTrigger = now;
    }
    else if (enabled && ((now - lastTrigger) / 1000) >= triggerDuration)
    {
      active = true;
      lastTrigger = -1;
    }
    else if (!enabled && lastTrigger >= 0)
    {
      lastTrigger = -1;
    }
  }
}

void Timer::reset()
{
  active = false;
  lastTrigger = -1;
}

bool Timer::isActive()
{
  return active;
}