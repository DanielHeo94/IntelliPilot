//
//  IC_Radio.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "IC_Radio.h"

#include <FreeRTOS_ARM/src/FreeRTOS_ARM.h>
#include <IC_State/IC_State.h>

double ch1, ch2, ch3, ch4, ch5;
double ch1Last, ch2Last, ch3Last, ch4Last, ch5Last;

boolean radioInterruptLock = false;

unsigned long radioLastChange1 = micros();
unsigned long radioLastChange2 = micros();
unsigned long radioLastChange3 = micros();
unsigned long radioLastChange4 = micros();
unsigned long radioLastChange5 = micros();

IC_Radio::IC_Radio() {
}

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

inline void radioInterruptHandler5() {
								if (!radioInterruptLock) ch5 = micros() - radioLastChange5;
								radioLastChange5 = micros();
}


void IC_Radio::begin() {

								Serial.print("\t\tAttach interrupt CH1..."); attachInterrupt(RC_CH1, radioInterruptHandler1, CHANGE); Serial.println("\t\tSuccess.");
								Serial.print("\t\tAttach interrupt CH2..."); attachInterrupt(RC_CH2, radioInterruptHandler2, CHANGE); Serial.println("\t\tSuccess.");
								Serial.print("\t\tAttach interrupt CH3..."); attachInterrupt(RC_CH3, radioInterruptHandler3, CHANGE); Serial.println("\t\tSuccess.");
								Serial.print("\t\tAttach interrupt CH4..."); attachInterrupt(RC_CH4, radioInterruptHandler4, CHANGE); Serial.println("\t\tSuccess.");
								Serial.print("\t\tAttach interrupt CH5..."); attachInterrupt(RC_CH5, radioInterruptHandler5, CHANGE); Serial.println("\t\tSuccess.");
}

// NOTICE!! this function MUST be in loop.
void IC_Radio::getCommands(double *data) {

								IC_Radio::acquireLock();

								ch1 = floor(ch1 / ROUNDING_BASE) * ROUNDING_BASE;
								ch2 = floor(ch2 / ROUNDING_BASE) * ROUNDING_BASE;
								ch3 = floor(ch3 / ROUNDING_BASE) * ROUNDING_BASE;
								ch4 = floor(ch4 / ROUNDING_BASE) * ROUNDING_BASE;
								ch5 = floor(ch5 / ROUNDING_BASE) * ROUNDING_BASE;

								if ((ch1 > RC_CH1_LOW) || (ch1 < RC_CH1_HIGH)) ch1 = ch1Last;
								if ((ch2 > RC_CH2_LOW) || (ch2 < RC_CH2_HIGH)) ch2 = ch2Last;
								if ((ch3 < RC_CH3_LOW) || (ch3 > RC_CH3_HIGH)) ch3 = ch3Last;
								if ((ch4 > RC_CH4_LOW) || (ch4 < RC_CH4_HIGH)) ch4 = ch4Last;
								if ((ch5 > RC_CH5_LOW) || (ch5 < RC_CH5_HIGH)) ch5 = ch5Last;

								ch1Last = ch1;
								ch2Last = ch2;
								ch3Last = ch3;
								ch4Last = ch4;
								ch5Last = ch5;

								data[0] = ch1;
								data[1] = ch2;
								data[2] = ch3;
								data[3] = ch4;
								data[4] = ch5;

								IC_Radio::releaseLock();
								vTaskDelay((5L * configTICK_RATE_HZ) / 1000L);
								IC_Radio::acquireLock();
}

void IC_Radio::acquireLock() {
								radioInterruptLock = true;
}

void IC_Radio::releaseLock() {
								radioInterruptLock = false;
}
