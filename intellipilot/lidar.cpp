// 
// 
// 

#include "lidar.h"

#define SAMPLING_TIME	0.01
#define ALPHA			0.96

unsigned long pulse_width;

lidar::lidar() {}

void lidar::begin(int trigger_pin, int monitor_pin) {
    Serial.print("\t\tSet trigger pin."); pinMode(trigger_pin, OUTPUT); Serial.println("\t\t\tSuccess.");
    Serial.print("\t\tSet monitor pin."); pinMode(monitor_pin, INPUT); Serial.println("\t\t\tSuccess.");
    Serial.print("\t\tSet trigger pin LOW state."); digitalWrite(trigger_pin, LOW); Serial.println("\t\tSuccess.");
}

double lidar::getVerDistance() {
	pulse_width = pulseIn(monitor_pin, HIGH);
	if (pulse_width != 0) {
		pulse_width = pulse_width / 10;

		ver_distance = pulse_width;
		ver_distance = ALPHA * ver_distanceLast + (1 - ALPHA) * ver_distance;

		ver_distanceLast = ver_distance;
	}
	return ver_distance;
}

double lidar::getVerVelocity() {
	pulse_width = pulseIn(trigger_pin, HIGH);
	if (pulse_width != 0) {
		pulse_width = pulse_width / 10;

		ver_distance = pulse_width;
		ver_distance = ALPHA * ver_distanceLast + (1 - ALPHA) * ver_distance;

		ver_velocity = (ver_distance - ver_distanceLast) / SAMPLING_TIME;
		ver_velocity = ALPHA * ver_velocityLast + (1 - ALPHA) * ver_velocity;

		ver_distanceLast = ver_distance;
		ver_velocityLast = ver_velocity;
	}
	return ver_velocity;
}