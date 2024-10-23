#include <Arduino.h>
#include "config.h"

#include "Utils.hpp"
#include "Stoplight.hpp"
#include "Timer.hpp"

#if MODBUS_ENABLED
#include "StoplightControllerModbus.hpp"
#endif

// Holds the state of the two stoplights
Stoplight northSouth(YELLOW_DURATION_S);
Stoplight eastWest(YELLOW_DURATION_S);

// --- Timers ---

// Times when both stoplights are red. This must occur for
// ALL_RED_DURATION_S seconds before one light is transition
// to green.
Timer allRedTimer(ALL_RED_DURATION_S);

// This timer is started when the east-west vehicle sensor 
// detects a vehicle and fires once it reaches EW_TRIGGER_THRESHOLD_S.
// If a vehicle moves off the sensor, then the timer is reset.
Timer eastWestTriggerTimer(EW_TRIGGER_THRESHOLD_S);

// This timer tracks how long the east-west direction has been active.
Timer eastWestActiveTimer(EW_ACTIVE_DURATION_S);

#if MODBUS_ENABLED
StoplightControllerModbus mb(SLAVE_ID, northSouth, eastWest);
#endif

// The last time we ran the loop (most operations only run every 500ms)
unsigned long ts = 0;

// 0 = north-south, 1 = east-west
unsigned short nextLight = 0;

// We track this for the requirement:
//  - The east-west direction should be given the green light only once in an X second period
unsigned long lastEastWestActive = 0;

// --- System-level helpers ---

bool allRed()
{
  return eastWest.isRedOn() && northSouth.isRedOn();
}

bool northSouthActive()
{
  return northSouth.isGreenOn() && eastWest.isRedOn();
}

bool eastWestActive()
{
  return northSouth.isRedOn() && eastWest.isGreenOn();
}

bool eastWestEligibleForActive()
{
  return EW_ACTIVE_PERIOD_S == 0 || ((millis() - lastEastWestActive) / 1000) >= EW_ACTIVE_PERIOD_S;
}

bool northSouthPriority()
{
#if MODBUS_ENABLED
  return mb.hasNorthSouthPriority();
#else
  return false;
#endif
}

bool eastWestPriority()
{
#if MODBUS_ENABLED
  return mb.hasEastWestPriority();
#else
  return false;
#endif
}

void transitionNorthSouthToStop()
{
  // next light is east-west
  nextLight = 1;
  northSouth.transitionToStop();
}

void transitionEastWestToStop()
{
  // next light is north-south
  nextLight = 0;
  eastWest.transitionToStop();
}

void triggerNextLight()
{
  if (nextLight == 0)
  {
    northSouth.readyToGo();
  }
  else
  {
    eastWest.readyToGo();
    lastEastWestActive = millis();
  }
}

// --- Arduino control ---

void setup()
{
  pinMode(EW_RED, OUTPUT);
  pinMode(EW_YELLOW, OUTPUT);
  pinMode(EW_GREEN, OUTPUT);

  pinMode(NS_RED, OUTPUT);
  pinMode(NS_YELLOW, OUTPUT);
  pinMode(NS_GREEN, OUTPUT);

  pinMode(EW_TRIGGER, INPUT);

  Serial.begin(BAUD);
  while (!Serial)
    ;
  delay(50);
  serialLog("Ready.");

#if MODBUS_ENABLED
  mb.setup(Serial, BAUD);
#endif
}

void loop()
{
  bool ewTriggered = digitalRead(EW_TRIGGER);
#if MODBUS_ENABLED
  mb.tick(ewTriggered);
#endif

  if (millis() - ts >= 500)
  {
    ts = millis();

    if (allRed())
    {
      // The "allRed" state: make sure we're red for the right amount of time
      // and then trigger the next light green.
      allRedTimer.tick(true);
      if (allRedTimer.isActive())
      {
        allRedTimer.reset();
        serialLog("'All red' duration reached. Triggering next light");
        triggerNextLight();
      }
    }
    else if (northSouthPriority())
    {
      if (eastWestActive())
      {
        serialLog("'North/south' priority signal received. Triggering next light.");
        transitionEastWestToStop();
      }
    }
    else if (eastWestPriority())
    {
      if (northSouthActive())
      {
        serialLog("'East/west' priority signal received. Triggering next light.");
        transitionNorthSouthToStop();
      }
    }
    else if (northSouthActive())
    {
      eastWestTriggerTimer.tick(ewTriggered);
      if (eastWestTriggerTimer.isActive() && eastWestEligibleForActive())
      {
        eastWestTriggerTimer.reset();
        serialLog("'East/west' trigger threshold reached. Triggering next light.");
        transitionNorthSouthToStop();
      }
    }
    else if (eastWestActive())
    {
      eastWestActiveTimer.tick(true);
      if (eastWestActiveTimer.isActive())
      {
        eastWestActiveTimer.reset();
        serialLog("'East/west' has been active its maximum duration. Triggering next light.");
        transitionEastWestToStop();
      }
    }
    
    eastWest.tick();
    northSouth.tick();

    // Update the physical outputs

    digitalWrite(NS_RED, northSouth.isRedOn());
    digitalWrite(NS_YELLOW, northSouth.isYellowOn());
    digitalWrite(NS_GREEN, northSouth.isGreenOn());

    digitalWrite(EW_RED, eastWest.isRedOn());
    digitalWrite(EW_YELLOW, eastWest.isYellowOn());
    digitalWrite(EW_GREEN, eastWest.isGreenOn());

    // Let the serial monitor know what's going on

    serialLog("EW TRIGGERED " + String(ewTriggered == HIGH));
    serialLog("NORTH-SOUTH  " + northSouth.statusString());
    serialLog("EAST-WEST    " + eastWest.statusString());
    serialLog("");
  }
}