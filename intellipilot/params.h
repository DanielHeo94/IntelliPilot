// params.h

#ifndef _PARAMS_h
#define _PARAMS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

// RC status
#define LSTICK_RIGHT	1080
#define LSTICK_LEFT		-19
#define LSTICK_DOWN		1800

#define STATE_ARMED			2
#define STATE_DISARMED		3
#define STATE_ALT_HOLD		4
#define STATE_POS_HOLD		5
#define STATE_LOITER 		6

#endif

