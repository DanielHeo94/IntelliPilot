// 
// 
// 

#include "lidar.h"

#define SAMPLING_TIME	0.01
#define ALPHA			0.96

unsigned long pulse_width;

lidar::lidar() {}

void lidar::begin(int trigger_pin, int monitor_pin) {
	pinMode(trigger_pin, OUTPUT);
	pinMode(monitor_pin, INPUT);
	digitalWrite(trigger_pin, LOW);
}

float lidar::getVerDistance() {
	pulse_width = pulseIn(monitor_pin, HIGH);
	if (pulse_width != 0) {
		pulse_width = pulse_width / 10;

		ver_distance = pulse_width;
		ver_distance = ALPHA * ver_distanceLast + (1 - ALPHA) * ver_distance;

		ver_distanceLast = ver_distance;
	}
	return ver_distance;
}

float lidar::getVerVelocity() {
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