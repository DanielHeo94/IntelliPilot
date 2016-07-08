// gains.h

#ifndef _GAINS_h
#define _GAINS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Gains
#define PITCH_OUTER_P_GAIN  4.750
#define PITCH_INNER_P_GAIN  2.933
#define PITCH_INNER_I_GAIN  0.440
#define PITCH_INNER_D_GAIN  0.035

#define ROLL_OUTER_P_GAIN   4.750
#define ROLL_INNER_P_GAIN   2.833
#define ROLL_INNER_I_GAIN   0.440
#define ROLL_INNER_D_GAIN   0.035

#define YAW_P_GAIN          3.325
#define YAW_I_GAIN          1.550
#define YAW_D_GAIN			0

#define ALT_P_GAIN          1.750
#define ALT_I_GAIN          4.250
#define ALT_D_GAIN			0

#endif

