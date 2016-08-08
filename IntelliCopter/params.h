// params.h

#ifndef _PARAMS_h
#define _PARAMS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define PREFLIGHT_MODE_ARM_COUNT_MAX	200

// Rotor States
#define STATE_PREFLIGHT_ARMED		1
#define STATE_PREFLIGHT_DISARMED	2
#define STATE_MANUAL_ARMED			3
#define STATE_MANUAL_DISARMED		4
#define STATE_HOLD_ARMED			5
#define STATE_HOLD_DISARMED			6
#define STATE_AUTO_ARMED			7
#define STATE_AUTO_DISARMED			8

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
#define MOTOR_PULSE_MIN		800
#define MOTOR_PULSE_MAX		2200
#define MOTOR_PULSE_TAKEOFF	850

// Angle offset
#define PITCH_ANG_OFFSET    1
#define ROLL_ANG_OFFSET     -0.9

#endif
