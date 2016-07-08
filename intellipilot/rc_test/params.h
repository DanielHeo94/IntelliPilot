// params.h

#ifndef _PARAMS_h
#define _PARAMS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

// Flight parameters
#define PITCH_ANG_MIN   -30
#define PITCH_ANG_MAX   30
#define ROLL_ANG_MIN    -30
#define ROLL_ANG_MAX    30
#define YAW_RATE_MIN    -20
#define YAW_RATE_MAX    20
#define CLIMB_RATE_MIN  -50
#define CLIMB_RATE_MAX  50

// Motor parameters
#define MOTOR_PULSE_MIN		1100
#define MOTOR_PULSE_MAX		1800
#define MOTOR_PULSE_TAKEOFF	1200
#endif

