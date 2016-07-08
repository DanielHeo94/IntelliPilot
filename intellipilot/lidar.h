// lidar.h

#ifndef _LIDAR_h
#define _LIDAR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define DEFAULT_TRIGGER_PIN   34
#define DEFAULT_MONITOR_PIN   46

class lidar {
public:
	lidar();

	int trigger_pin, monitor_pin;
	
	float ver_distance, ver_distanceLast;
	float ver_velocity, ver_velocityLast;

	void begin(int trigger_pin = DEFAULT_TRIGGER_PIN, int monitor_pin = DEFAULT_MONITOR_PIN);

	float getVerDistance();
	float getVerVelocity();
};

#endif

