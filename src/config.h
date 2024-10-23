#ifndef CONFIG
#define CONFIG

// --- Arduino physical I/O ---
#define NS_RED 2
#define NS_YELLOW 3
#define NS_GREEN 4

#define EW_RED 5
#define EW_YELLOW 6
#define EW_GREEN 7

#define EW_TRIGGER 13

// --- Modbus and Serial Configuration ---

#define BAUD 38400

#define MODBUS_ENABLED 1
#define SLAVE_ID 2

#define NS_RED_COIL 0
#define NS_YELLOW_COIL 1
#define NS_GREEN_COIL 2

#define EW_RED_COIL 3
#define EW_YELLOW_COIL 4
#define EW_GREEN_COIL 5

#define EW_TRIGGER_COIL 6

#define OVERRIDE_HR 0
#define OVERRIDE_NONE 0
#define OVERRIDE_NS 1
#define OVERRIDE_EW 2

// --- Application Configuration ---

const unsigned int YELLOW_DURATION_S = 4;
const unsigned int ALL_RED_DURATION_S = 3;
const unsigned int EW_TRIGGER_THRESHOLD_S = 10;
const unsigned int EW_ACTIVE_DURATION_S = 7;
const unsigned int EW_ACTIVE_PERIOD_S = 30;

#endif