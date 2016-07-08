// config.h

#ifndef _CONFIG_h
#define _CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Attitude commands config
#define PITCH_ANG_MIN   -30
#define PITCH_ANG_MAX   30

#define ROLL_ANG_MIN    -30
#define ROLL_ANG_MAX    30

#define YAW_SPEED_MIN    -20
#define YAW_SPEED_MAX    20

#define CLIMB_RATE_MIN  -50
#define CLIMB_RATE_MAX  50

// Motor input config
#define MOTOR_PULSE_MIN		1100
#define MOTOR_PULSE_MAX		1800
#define MOTOR_PULSE_TAKEOFF	1200

// Angle offset
#define PITCH_ANG_OFFSET    1
#define ROLL_ANG_OFFSET     -0.9

#endif

