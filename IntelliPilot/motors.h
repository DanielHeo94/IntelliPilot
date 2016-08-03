// motors.h

#ifndef _MOTORS_h
#define _MOTORS_h

#include <Servo.h>
#include "config.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

// Motors pin configuration
#define QUAD_MOTOR_A	9
#define QUAD_MOTOR_B	6
#define QUAD_MOTOR_C	5
#define QUAD_MOTOR_D	3

#define MOTOR_START_DELAY	3000

class motors {
public:
	motors();

	bool start();

	void rotate(int pulse_1, int pulse_2, int pulse_3, int pulse_4);
	void stop(int pulse_1 = MOTOR_PULSE_MIN, int pulse_2 = MOTOR_PULSE_MIN, int pulse_3 = MOTOR_PULSE_MIN, int pulse_4 = MOTOR_PULSE_MIN);
};

#endif

