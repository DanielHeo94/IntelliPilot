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

#define LOWEST_ALTITUDE			0
#define HIGHEST_ALTITUDE		1500

class lidar {
public:
	lidar();

	int trigger_pin, monitor_pin;
	
	double ver_distance, ver_distanceLast;
	double ver_velocity, ver_velocityLast;

	void begin(int trigger_pin = DEFAULT_TRIGGER_PIN, int monitor_pin = DEFAULT_MONITOR_PIN);

	double getVerDistance();
	double getVerVelocity();

private:

	static void acquireLock();
	static void releaseLock();
};

#endif