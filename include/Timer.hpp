#pragma once

class Timer
{
public:
  Timer(unsigned long _triggerDuration, unsigned long _activeDuration)
      : triggerDuration(_triggerDuration), activeDuration(_activeDuration)
  {
  }
  Timer(unsigned long _triggerDuration) : Timer(_triggerDuration, -1)
  {
  }

  void tick(bool enabled);
  void reset();
  bool isActive();

private:
  unsigned long triggerDuration;
  unsigned long activeDuration;

  long lastTrigger = -1;
  bool active = false;
};
