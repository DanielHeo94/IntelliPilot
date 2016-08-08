// 
// 
// 
#include <Arduino.h>
#include "remote.h"

float ch1, ch2, ch3, ch4;
float ch1Last, ch2Last, ch3Last, ch4Last;

boolean interruptLock = false;

unsigned long rcLastChange1 = micros();
unsigned long rcLastChange2 = micros();
unsigned long rcLastChange3 = micros();
unsigned long rcLastChange4 = micros();

remote::remote() {}

inline void rcInterrupt1() {
  if (!interruptLock) ch1 = micros() - rcLastChange1;
  rcLastChange1 = micros();
}

inline void rcInterrupt2() {
  if (!interruptLock) ch2 = micros() - rcLastChange2;
  rcLastChange2 = micros();
}

inline void rcInterrupt3() {
  if (!interruptLock) ch3 = micros() - rcLastChange3;
  rcLastChange3 = micros();
}

inline void rcInterrupt4() {
  if (!interruptLock) ch4 = micros() - rcLastChange4;
  rcLastChange4 = micros();
}

void acquireLock() {
  interruptLock = true;
}

void releaseLock() {
  interruptLock = false;
}

void remote::begin() {
  
	Serial.println("Initializing RC");

	attachInterrupt(RC_CH1, rcInterrupt1, CHANGE);
	attachInterrupt(RC_CH2, rcInterrupt2, CHANGE);
	attachInterrupt(RC_CH3, rcInterrupt3, CHANGE);
	attachInterrupt(RC_CH4, rcInterrupt4, CHANGE);

	Serial.println("Done");
}

// NOTICE!! this function MUST be in loop.
void remote::getCommands(float *data) {
		acquireLock();

		ch1 = floor(ch1 / ROUNDING_BASE) * ROUNDING_BASE;
		ch2 = floor(ch2 / ROUNDING_BASE) * ROUNDING_BASE;
		ch3 = floor(ch3 / ROUNDING_BASE) * ROUNDING_BASE;
		ch4 = floor(ch4 / ROUNDING_BASE) * ROUNDING_BASE;

		ch1 = map(ch1, RC_LOW_YAW_CMD, RC_HIGH_YAW_CMD, YAW_RATE_MIN, YAW_RATE_MAX) + YAW_CMD_OFFSET;
		ch2 = map(ch2, RC_LOW_PITCH_CMD, RC_HIGH_PITCH_CMD, PITCH_ANG_MIN, PITCH_ANG_MAX) + PITCH_CMD_OFFSET;
		ch3 = map(ch3, RC_LOW_ROLL_CMD, RC_HIGH_ROLL_CMD, ROLL_ANG_MIN, ROLL_ANG_MAX) + ROLL_CMD_OFFSET;
		ch4 = map(ch4, RC_LOW_ALT_CMD, RC_HIGH_ALT_CMD, CLIMB_RATE_MIN, CLIMB_RATE_MAX) + ALT_CMD_OFFSET;

		if ((ch1 < YAW_RATE_MIN) || (ch1 > YAW_RATE_MAX)) ch1 = ch1Last;
		if ((ch2 < PITCH_ANG_MIN) || (ch2 > PITCH_ANG_MAX)) ch2 = ch2Last;
		if ((ch3 < ROLL_ANG_MIN) || (ch3 > ROLL_ANG_MAX)) ch3 = ch3Last;
		if ((ch4 < CLIMB_RATE_MIN) || (ch4 > CLIMB_RATE_MAX)) ch4 = ch4Last;

		ch1Last = ch1;
		ch2Last = ch2;
		ch3Last = ch3;
		ch4Last = ch4;

		data[0] = ch1;
		data[1] = ch2;
		data[2] = ch3;
		data[3] = ch4;

		releaseLock();

		delay(5);
}
