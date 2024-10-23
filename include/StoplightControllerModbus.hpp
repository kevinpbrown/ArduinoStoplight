#pragma once

#include <Arduino.h>

#include <Modbus.h>
#include <ModbusSerial.h>

#include "Stoplight.hpp"

class StoplightControllerModbus
{
public:
  StoplightControllerModbus(byte _slaveId, Stoplight &_northSouth, Stoplight &_eastWest) 
    : slaveId(_slaveId), northSouth(_northSouth), eastWest(_eastWest)
  {
  }

  ~StoplightControllerModbus()
  {
    delete mb;
  }

  void setup(Stream &stream, unsigned long baud);
  void tick(bool ewTriggered);

  bool hasNorthSouthPriority();
  bool hasEastWestPriority();
private:
  byte slaveId;
  Stoplight &northSouth;
  Stoplight &eastWest;

  ModbusSerial *mb;

};
