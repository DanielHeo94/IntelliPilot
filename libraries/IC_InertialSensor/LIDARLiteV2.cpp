//
//
//

#include "LIDARLiteV2.h"

#include <FreeRTOS_ARM/src/FreeRTOS_ARM.h>

#define SAMPLING_TIME		0.01
#define ALPHA						0.35

int pulseWidth;
int pulseWidthLast;

boolean _InterruptLock = false;

unsigned long _LastChange = micros();

LIDARLiteV2::LIDARLiteV2() {}

inline void _InterruptHandler() {
	if (!_InterruptLock) pulseWidth = micros() - _LastChange;
	_LastChange = micros();
}

void LIDARLiteV2::begin(int trigger_pin, int monitor_pin) {

    Serial.print("\t\tSet trigger pin."); pinMode(trigger_pin, OUTPUT); Serial.println("\t\t\tSuccess.");
		Serial.print("\t\tAttach monitor pin."); attachInterrupt(monitor_pin, _InterruptHandler, CHANGE); Serial.println("\t\t\tSuccess.");
    Serial.print("\t\tSet trigger pin LOW state."); digitalWrite(trigger_pin, LOW); Serial.println("\t\tSuccess.");
}

double LIDARLiteV2::getVerDistance() {

	LIDARLiteV2::acquireLock();

	pulseWidth = floor(pulseWidth / 10) * 10;

	if ((pulseWidth < LOWEST_ALTITUDE) || (pulseWidth > HIGHEST_ALTITUDE))      pulseWidth = pulseWidthLast;

	pulseWidthLast = pulseWidth;

	ver_distance = pulseWidth / 10;

	ver_distance = ALPHA * ver_distanceLast + (1 - ALPHA) * ver_distance;
	ver_distanceLast = ver_distance;

	LIDARLiteV2::releaseLock();
	vTaskDelay((5L * configTICK_RATE_HZ) / 1000L);
	LIDARLiteV2::acquireLock();

	return ver_distance;
}

double LIDARLiteV2::getVerVelocity() {

		ver_velocity = (ver_distance - ver_distanceLast) / SAMPLING_TIME;
		ver_velocity = ALPHA * ver_velocityLast + (1 - ALPHA) * ver_velocity;

		ver_velocityLast = ver_velocity;

	return ver_velocity;
}

void LIDARLiteV2::acquireLock() {
	_InterruptLock = true;
}

void LIDARLiteV2::releaseLock() {
	_InterruptLock = false;
}
