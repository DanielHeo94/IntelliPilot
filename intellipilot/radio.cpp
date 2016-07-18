// 
// 
// 
#include <Arduino.h>
#include "radio.h"

#include <FreeRTOS_ARM.h>

double ch1, ch2, ch3, ch4;
const double ch5 = RC_CH5_LOW;
double ch1Last, ch2Last, ch3Last, ch4Last, ch5Last;

boolean radioInterruptLock = false;

unsigned long radioLastChange1 = micros();
unsigned long radioLastChange2 = micros();
unsigned long radioLastChange3 = micros();
unsigned long radioLastChange4 = micros();
//unsigned long radioLastChange5 = micros();

int preflightModeArmCnt, preflightModeDisarmCnt;

boolean armedMode[4] = { false , }; // Pre-Flgiht Mode, Manual Mode, Guided Mode, Auto Mode.

radio::radio() {}

inline void radioInterruptHandler1() {
	if (!radioInterruptLock) ch1 = micros() - radioLastChange1;
	radioLastChange1 = micros();
}

inline void radioInterruptHandler2() {
	if (!radioInterruptLock) ch2 = micros() - radioLastChange2;
	radioLastChange2 = micros();
}

inline void radioInterruptHandler3() {
	if (!radioInterruptLock) ch3 = micros() - radioLastChange3;
	radioLastChange3 = micros();
}

inline void radioInterruptHandler4() {
	if (!radioInterruptLock) ch4 = micros() - radioLastChange4;
	radioLastChange4 = micros();
}

/*
inline void radioInterruptHandler5() {
	if (!radioInterruptLock) ch5 = micros() - radioLastChange5;
	radioLastChange5 = micros();
}
*/

void radio::begin() {

	Serial.print("\t\tAttach interrupt CH1..."); attachInterrupt(RC_CH1, radioInterruptHandler1, CHANGE); Serial.println("\t\tSuccess.");
	Serial.print("\t\tAttach interrupt CH2..."); attachInterrupt(RC_CH2, radioInterruptHandler2, CHANGE); Serial.println("\t\tSuccess.");
	Serial.print("\t\tAttach interrupt CH3..."); attachInterrupt(RC_CH3, radioInterruptHandler3, CHANGE); Serial.println("\t\tSuccess.");
	Serial.print("\t\tAttach interrupt CH4..."); attachInterrupt(RC_CH4, radioInterruptHandler4, CHANGE); Serial.println("\t\tSuccess.");
	//Serial.print("\t\tAttach interrupt CH5..."); attachInterrupt(RC_CH5, radioInterruptHandler5, CHANGE); Serial.println("\t\tSuccess.");
}

// NOTICE!! this function MUST be in loop.
uint8_t radio::getCommands(double *data) {

	radio::acquireLock();

	ch1 = floor(ch1 / ROUNDING_BASE) * ROUNDING_BASE;
	ch2 = floor(ch2 / ROUNDING_BASE) * ROUNDING_BASE;
	ch3 = floor(ch3 / ROUNDING_BASE) * ROUNDING_BASE;
	ch4 = floor(ch4 / ROUNDING_BASE) * ROUNDING_BASE;
	//ch5 = floor(ch5 / ROUNDING_BASE) * ROUNDING_BASE;

	if ((ch1 > RC_CH1_LOW) || (ch1 < RC_CH1_HIGH))		ch1 = ch1Last;
	if ((ch2 > RC_CH2_LOW) || (ch2 < RC_CH2_HIGH))		ch2 = ch2Last;
	if ((ch3 < RC_CH3_LOW) || (ch3 > RC_CH3_HIGH))		ch3 = ch3Last;
	if ((ch4 > RC_CH4_LOW) || (ch4 < RC_CH4_HIGH))      ch4 = ch4Last;
	//if ((ch5 > RC_CH5_LOW) || (ch5 < RC_CH5_HIGH))      ch5 = ch5Last;

	ch1Last = ch1;
	ch2Last = ch2;
	ch3Last = ch3;
	ch4Last = ch4;

	data[0] = ch3;
	data[1] = ch2;
	data[2] = ch1;
	data[3] = ch4;

	/*
	Serial.print(data[0]);
	Serial.print("\t");
	Serial.print(data[1]);
	Serial.print("\t");
	Serial.print(data[2]);
	Serial.print("\t");
	Serial.println(data[3]);
	*/

	radio::sCounter();

	radio::releaseLock();
	vTaskDelay((5L * configTICK_RATE_HZ) / 1000L);
	radio::acquireLock();

	if (preflightModeArmCnt == PREFLIGHT_MODE_ARM_COUNT_MAX
		&& !armedMode[0]) {
		armedMode[0] = true;
		armedMode[1] = false;
		armedMode[2] = false;
		armedMode[3] = false;
		return STATE_PREFLIGHT_ARMED;
	}
	else if (preflightModeDisarmCnt == PREFLIGHT_MODE_ARM_COUNT_MAX 
		&& armedMode[0]) {
		armedMode[0] = false;
		armedMode[1] = false;
		armedMode[2] = false;
		armedMode[3] = false;
		return STATE_PREFLIGHT_DISARMED;
	}

	if (ch5 == RC_CH5_LOW
		&& !armedMode[1]
		&& armedMode[0]) {
		armedMode[0] = true;
		armedMode[1] = true;
		armedMode[2] = false;
		armedMode[3] = false;
		return STATE_MANUAL_ARMED;
	}
	else if (ch5 == RC_CH5_MID
		&& !armedMode[2]
		&& armedMode[0]) {
		armedMode[0] = true;
		armedMode[1] = false;
		armedMode[2] = true;
		armedMode[3] = false;
		return STATE_HOLD_ARMED;
	}
	else if (ch5 == RC_CH5_HIGH
		&& !armedMode[3]
		&& armedMode[0]) {
		armedMode[0] = true;
		armedMode[1] = false;
		armedMode[2] = false;
		armedMode[3] = true;
		return STATE_AUTO_ARMED;
	}
		
		return 0;
}

void radio::sCounter() {
	if ((int)ch1 <= (RC_CH1_HIGH + 20) && (int)ch4 >= (RC_CH4_LOW - 20) && !armedMode[0]) preflightModeArmCnt++;
	else if ((int)ch1 >= (RC_CH1_LOW - 20) && (int)ch4 >= (RC_CH4_LOW - 20) && armedMode[0]) preflightModeDisarmCnt++;
	else { preflightModeArmCnt = 0; preflightModeDisarmCnt = 0; }
}

void radio::acquireLock() {
	radioInterruptLock = true;
}

void radio::releaseLock() {
	radioInterruptLock = false;
}