// lidar.h

#ifndef _LIDARLiteV2_H_
#define _LIDARLiteV2_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#define DEFAULT_TRIGGER_PIN   34
#define DEFAULT_MONITOR_PIN   46

#define LOWEST_ALTITUDE   0
#define HIGHEST_ALTITUDE  1500

class LIDARLiteV2 {
public:
								LIDARLiteV2();

								int trigger_pin, monitor_pin;

								double ver_distance, ver_distanceLast;
								double ver_velocity, ver_velocityLast;

								void begin(int trigger_pin = DEFAULT_TRIGGER_PIN, int monitor_pin = DEFAULT_MONITOR_PIN);

								double getVerDistance();
								double getVerVelocity();

private:

								void acquireLock();
								void releaseLock();
};

#endif
