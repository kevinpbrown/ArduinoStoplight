# Arduino Stoplight

This project simulates a simple stoplight for a two-way intersection. It
operates as a Modbus slave so that its current status can be read by Ignition
using the COM port established for the Arduino via USB. A control centre is also
able to use a Modbus holding register to force one of the two streets in the
intersection to be green.

The specification for this proof-of-concept are:

* The intersection has two roads, one running north-south, the other east-west
* Both intersections have outputs for red, yellow, and green lights
* There are no pedestrian crossings 
* Assume that whenever switching between green and red:
    * Yellow will be illuminated for 4s
    * Both directions will remain red for 3s before green is displayed
* North-south stoplights will take precedence if no one is waiting to travel
  east-west
* There is a sensor to determine that someone is waiting to travel east-west
* The east-west stoplight should wait 10s to turn green after sensing a vehicle
* The east-west direction should be given the green light only once in a 30
  second period
* The east-west direction should only be green for 7s before giving the right of
  way back to the north-south stoplights
* A control centre should be given the ability to hold either of the two
  directions green temporarily

### Inputs

* East-west vehicle sensor: mechanical switch
* Control centre override: Modbus holding register with 0=no override,
  1=north-south, 2=east-west

### Outputs

* East-west red, yellow, and green lights
* North-south red, yellow, and green lights

## Modbus Configuration

See `src/config.h` for configuration details. Currently, it is set up with seven
coils and one holding register.

### Coils

| Coil | Value |
| ---- | ----- |
| `0`  | North-south red light            |
| `1`  | North-south yellow light         |
| `2`  | North-south green light          |
| `3`  | East-west red light              |
| `4`  | East-west yellow light           |
| `5`  | East-west green light            |
| `6`  | East-west vehicle trigger switch |

### Holding registers

| Register | Value |
| -------- | ----- |
| `0` | Control centre override to be written by Ignition: `0`=no override, `1`=north-south, `2`=east-west |
