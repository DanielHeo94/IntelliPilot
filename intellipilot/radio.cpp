// 
// 
// 
#include <Arduino.h>
#include "radio.h"

#include <FreeRTOS_ARM.h>

double ch1, ch2, ch3, ch4;
double ch1Last, ch2Last, ch3Last, ch4Last;

boolean radioInterruptLock = false;

unsigned long radioLastChange1 = micros();
unsigned long radioLastChange2 = micros();
unsigned long radioLastChange3 = micros();
unsigned long radioLastChange4 = micros();

int armCnt, disarmCnt;
boolean isArmed = false;

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

void radio::begin() {

    Serial.print("\t\tAttach interrupt CH1..." ); attachInterrupt(RC_CH1, radioInterruptHandler1, CHANGE); Serial.println("\t\tSuccess.");
    Serial.print("\t\tAttach interrupt CH2..." ); attachInterrupt(RC_CH2, radioInterruptHandler2, CHANGE); Serial.println("\t\tSuccess.");
    Serial.print("\t\tAttach interrupt CH3..." ); attachInterrupt(RC_CH3, radioInterruptHandler3, CHANGE); Serial.println("\t\tSuccess.");
    Serial.print("\t\tAttach interrupt CH4..." ); attachInterrupt(RC_CH4, radioInterruptHandler4, CHANGE); Serial.println("\t\tSuccess.");
}

// NOTICE!! this function MUST be in loop.
uint8_t radio::getCommands(double *data) {
		
		radio::acquireLock();

		ch1 = floor(ch1 / ROUNDING_BASE) * ROUNDING_BASE;
		ch2 = floor(ch2 / ROUNDING_BASE) * ROUNDING_BASE;
		ch3 = floor(ch3 / ROUNDING_BASE) * ROUNDING_BASE;
		ch4 = floor(ch4 / ROUNDING_BASE) * ROUNDING_BASE;

        if ((ch1 > RC_CH1_LOW) || (ch1 < RC_CH1_HIGH))		ch1 = ch1Last;
        if ((ch2 > RC_CH2_LOW) || (ch2 < RC_CH2_HIGH))		ch2 = ch2Last;
        if ((ch3 < RC_CH3_LOW) || (ch3 > RC_CH3_HIGH))		ch3 = ch3Last;
        if ((ch4 > RC_CH4_LOW) || (ch4 < RC_CH4_HIGH))      ch4 = ch4Last;
    
        ch1Last = ch1;
        ch2Last = ch2;
        ch3Last = ch3;
		ch4Last = ch4;
            
		data[0] = ch3;
		data[1] = ch2;
		data[2] = ch1;
		data[3] = ch4;

		radio::sCounter();

		radio::releaseLock();
		vTaskDelay((5L * configTICK_RATE_HZ) / 1000L);
		radio::acquireLock();

		if (armCnt == 300 && !isArmed) {
			isArmed = true;
			return STATE_ARMED;
		}
		else if (disarmCnt == 300 && isArmed) {
			isArmed = false;
			return STATE_DISARMED;
		}
		
		return 0;
}

void radio::sCounter() {
	if ((int)ch1 <= (RC_CH1_HIGH + 10) && (int)ch4 >= (RC_CH4_LOW - 10) && !isArmed) armCnt++;
	else if ((int)ch1 >= (RC_CH1_LOW - 10) && (int)ch4 >= (RC_CH4_LOW - 10) && isArmed) disarmCnt++;
	else { armCnt = 0; disarmCnt = 0; }
}

void radio::acquireLock() {
	radioInterruptLock = true;
}

void radio::releaseLock() {
	radioInterruptLock = false;
}