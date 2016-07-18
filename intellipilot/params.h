// params.h

#ifndef _PARAMS_h
#define _PARAMS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define PREFLIGHT_MODE_ARM_COUNT_MAX	300

// Rotor States
#define STATE_PREFLIGHT_ARMED		1		
#define STATE_PREFLIGHT_DISARMED	2
#define STATE_MANUAL_ARMED			3
#define STATE_MANUAL_DISARMED		4
#define STATE_HOLD_ARMED			5
#define STATE_HOLD_DISARMED			6
#define STATE_AUTO_ARMED			7
#define STATE_AUTO_DISARMED			8

#endif

