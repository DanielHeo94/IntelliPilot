// 
// 
// 

#include "lidar.h"

#include <FreeRTOS_ARM.h>

#define SAMPLING_TIME	0.01
#define ALPHA			0.35



double pulseWidth;
double pulseWidthLast;

boolean lidarInterruptLock = false;

unsigned long lidarLastChange = micros();

lidar::lidar() {}

inline void lidarInterruptHandler() {
	if (!lidarInterruptLock) pulseWidth = micros() - lidarLastChange;
	lidarLastChange = micros();
}

void lidar::begin(int trigger_pin, int monitor_pin) {
    Serial.print("\t\tSet trigger pin."); pinMode(trigger_pin, OUTPUT); Serial.println("\t\t\tSuccess.");
	Serial.print("\t\tAttach monitor pin."); attachInterrupt(monitor_pin, lidarInterruptHandler, CHANGE); Serial.println("\t\t\tSuccess.");
    Serial.print("\t\tSet trigger pin LOW state."); digitalWrite(trigger_pin, LOW); Serial.println("\t\tSuccess.");
}

double lidar::getVerDistance() {

	lidar::acquireLock();

	pulseWidth = floor(pulseWidth / 10) * 10;

	if ((pulseWidth < LOWEST_ALTITUDE) || (pulseWidth > HIGHEST_ALTITUDE))      pulseWidth = pulseWidthLast;

	pulseWidthLast = pulseWidth;

	ver_distance = pulseWidth / 10;

	ver_distance = ALPHA * ver_distanceLast + (1 - ALPHA) * ver_distance;
	ver_distanceLast = ver_distance;
	
	lidar::releaseLock();
	vTaskDelay((5L * configTICK_RATE_HZ) / 1000L);
	lidar::acquireLock();

	return ver_distance;
}

double lidar::getVerVelocity() {

		ver_velocity = (ver_distance - ver_distanceLast) / SAMPLING_TIME;
		ver_velocity = ALPHA * ver_velocityLast + (1 - ALPHA) * ver_velocity;

		ver_velocityLast = ver_velocity;
	
	return ver_velocity;
}

void lidar::acquireLock() {
	lidarInterruptLock = true;
}

void lidar::releaseLock() {
	lidarInterruptLock = false;
}