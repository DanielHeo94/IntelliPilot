// remote.h

#ifndef _REMOTE_h
#define _REMOTE_h

#include "config.h"
#include "params.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
// nums of RC channel
#define RC_CH_NUM	4

// RC pin configuration
#define RC_CH1	30
#define RC_CH2	31
#define RC_CH3	32
#define RC_CH4	33

// RC pulse width limits configuration
// ROLL
#define RC_HIGH_ROLL_CMD	2000
#define RC_LOW_ROLL_CMD		1100

// PITCH
#define RC_HIGH_PITCH_CMD	1050
#define RC_LOW_PITCH_CMD	1900

// YAW
#define RC_HIGH_YAW_CMD		1050
#define RC_LOW_YAW_CMD		1900

// ALT
#define RC_HIGH_ALT_CMD		1100
#define RC_LOW_ALT_CMD		1800


#define ROUNDING_BASE		50

// Offsets
#define YAW_CMD_OFFSET		-1
#define PITCH_CMD_OFFSET	-1
#define ROLL_CMD_OFFSET		0
#define ALT_CMD_OFFSET		0

class remote {
public:
	remote();

	void begin();
	uint8_t getCommands(float *data);

	void acquireLock();
	void releaseLcok();

private:

	void sCounter();
	
	void acquireLock();
	void releaseLock();

};

#endif

