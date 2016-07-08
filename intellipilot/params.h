// params.h

#ifndef _PARAMS_h
#define _PARAMS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

// System status
boolean isArmed = false;

// RC status
#define LSTICK_RIGHT	19
#define LSTICK_LEFT		-19
#define LSTICK_DOWN		-50

#define STATE_ARMING	1
#define STATE_ARMED		2
#define STATE_DISARMED	3

#endif

