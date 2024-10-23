#pragma once

enum StoplightActiveLight
{
  RED = 0,
  YELLOW = 1,
  GREEN = 2
};

class Stoplight
{
public:
  Stoplight(unsigned int _yellowDuration)
      : yellowDuration(_yellowDuration)
  {
    activeLight = RED;
    lastStateChange = -1;
  }

  void tick();
  void transitionToStop();
  void readyToGo();

  String statusString();

  bool isRedOn()
  {
    return activeLight == RED;
  }
  bool isYellowOn()
  {
    return activeLight == YELLOW;
  }
  bool isGreenOn()
  {
    return activeLight == GREEN;
  }
private:
  const unsigned int yellowDuration;
  
  StoplightActiveLight activeLight;
  long lastStateChange;
};