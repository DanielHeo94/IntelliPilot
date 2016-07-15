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
    
        if ((ch1 > RC_LOW_YAW_CMD) || (ch1 < RC_HIGH_YAW_CMD))      ch1 = ch1Last;
        if ((ch2 > RC_LOW_PITCH_CMD) || (ch2 < RC_HIGH_PITCH_CMD))  ch2 = ch2Last;
        if ((ch3 > RC_HIGH_ROLL_CMD) || (ch3 < RC_LOW_ROLL_CMD))    ch3 = ch3Last;
        if ((ch4 > RC_LOW_ALT_CMD) || (ch4 < RC_HIGH_ALT_CMD))      ch4 = ch4Last;
    
        ch1Last = ch1;
        ch2Last = ch2;
        ch3Last = ch3;
        ch4Last = ch4;
        
        //ch1 = map(ch1, RC_LOW_YAW_CMD, RC_HIGH_YAW_CMD, YAW_SPEED_MIN, YAW_SPEED_MAX) + YAW_CMD_OFFSET;
		//ch2 = map(ch2, RC_LOW_PITCH_CMD, RC_HIGH_PITCH_CMD, PITCH_ANG_MIN, PITCH_ANG_MAX) + PITCH_CMD_OFFSET;
		//ch3 = map(ch3, RC_LOW_ROLL_CMD, RC_HIGH_ROLL_CMD, ROLL_ANG_MIN, ROLL_ANG_MAX) + ROLL_CMD_OFFSET;
		//ch4 = map(ch4, RC_LOW_ALT_CMD, RC_HIGH_ALT_CMD, MOTOR_PULSE_MIN, MOTOR_PULSE_MAX);
            
		data[2] = ch1;
		data[1] = ch2;
		data[0] = ch3;
		data[3] = ch4;

		radio::sCounter();

		radio::releaseLock();
		vTaskDelay((5L * configTICK_RATE_HZ) / 1000L);
		radio::acquireLock();

		if (armCnt == 300 && !isArmed) {
			isArmed = true; return STATE_ARMED;
		}
		else if (disarmCnt == 300 && isArmed) {
			isArmed = false;
			return STATE_DISARMED;
		}
}

void radio::sCounter() {
	if ((int)ch1 == LSTICK_RIGHT && (int)ch4 == LSTICK_DOWN && !isArmed) armCnt++;
	else if ((int)ch1 == LSTICK_LEFT && (int)ch4 == LSTICK_DOWN && isArmed) disarmCnt++;
	else { armCnt = 0; disarmCnt = 0; }
}

void radio::acquireLock() {
	radioInterruptLock = true;
}

void radio::releaseLock() {
	radioInterruptLock = false;
}