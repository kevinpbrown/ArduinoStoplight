#include "config.h"
#include "StoplightControllerModbus.hpp"

void StoplightControllerModbus::setup(Stream &stream, unsigned long baud)
{
  if (mb != nullptr)
  {
    // TODO: Error state
    return;
  }
  
  mb = new ModbusSerial(stream, slaveId, -1);
  mb->config(baud);
  mb->setAdditionalServerData("Arduino_Stoplight");

  mb->addCoil(NS_RED_COIL);
  mb->addCoil(NS_YELLOW_COIL);
  mb->addCoil(NS_GREEN_COIL);

  mb->addCoil(EW_RED_COIL);
  mb->addCoil(EW_YELLOW_COIL);
  mb->addCoil(EW_GREEN_COIL);

  mb->addCoil(EW_TRIGGER_COIL);

  mb->addHreg(OVERRIDE_HR);
}

void StoplightControllerModbus::tick(bool ewTriggered)
{
  mb->task();

  mb->setCoil(NS_RED_COIL, northSouth.isRedOn());
  mb->setCoil(NS_YELLOW_COIL, northSouth.isYellowOn());
  mb->setCoil(NS_GREEN_COIL, northSouth.isGreenOn());
  
  mb->setCoil(EW_RED_COIL, eastWest.isRedOn());
  mb->setCoil(EW_YELLOW_COIL, eastWest.isYellowOn());
  mb->setCoil(EW_GREEN_COIL, eastWest.isGreenOn());

  mb->setCoil(EW_TRIGGER_COIL, ewTriggered);
}

bool StoplightControllerModbus::hasNorthSouthPriority()
{
  return mb->hreg(OVERRIDE_HR) == OVERRIDE_NS;
}

bool StoplightControllerModbus::hasEastWestPriority()
{
  return mb->hreg(OVERRIDE_HR)  == OVERRIDE_EW;
}
