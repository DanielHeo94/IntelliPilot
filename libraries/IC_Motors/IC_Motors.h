//
//  IC_Motors.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _IC_MOTORS_H_
#define _IC_MOTORS_H_

#include <Servo.h>
#include <IC_Math/IC_Math.h>

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

IC_Math _math;

// Motors pin configuration
#define QUAD_MOTOR_A	9
#define QUAD_MOTOR_B	6
#define QUAD_MOTOR_C	5
#define QUAD_MOTOR_D	3

// Motor input config
#define MOTOR_PULSE_MIN			1100
#define MOTOR_PULSE_MAX			1900
#define MOTOR_PULSE_TAKEOFF	1200

#define MOTOR_START_DELAY		3000

class IC_Motors {
public:
	IC_Motors();

	bool start();

	void rotate(int pulse_1, int pulse_2, int pulse_3, int pulse_4);
	void stop(int pulse_1 = MOTOR_PULSE_MIN, int pulse_2 = MOTOR_PULSE_MIN, int pulse_3 = MOTOR_PULSE_MIN, int pulse_4 = MOTOR_PULSE_MIN);
};

#endif
