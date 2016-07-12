// 
// 
// 

#include "motors.h"
#include "math.h"

math mMath;

Servo m1, m2, m3, m4;

motors::motors(){}

bool motors::start() {
	Serial.println("Starting motors");

	m1.attach(QUAD_MOTOR_A);
	m2.attach(QUAD_MOTOR_B);
	m3.attach(QUAD_MOTOR_B);
	m4.attach(QUAD_MOTOR_D);

	delay(1000);

	m1.writeMicroseconds(MOTOR_PULSE_MIN);
	m2.writeMicroseconds(MOTOR_PULSE_MIN);
	m3.writeMicroseconds(MOTOR_PULSE_MIN);
	m4.writeMicroseconds(MOTOR_PULSE_MIN);

	delay(MOTOR_START_DELAY);

	if (m1.attached() && m2.attached() && m3.attached() && m4.attached()) {
		Serial.println("Done");
		return true;
	}
	else {
		Serial.println("Motor start fail");
		return false;
	}
}

void motors::rotate(int pulse_1, int pulse_2, int pulse_3, int pulse_4) {

	mMath.limitInt(&pulse_1, MOTOR_PULSE_MIN, MOTOR_PULSE_MAX);
	mMath.limitInt(&pulse_2, MOTOR_PULSE_MIN, MOTOR_PULSE_MAX);
	mMath.limitInt(&pulse_3, MOTOR_PULSE_MIN, MOTOR_PULSE_MAX);
	mMath.limitInt(&pulse_4, MOTOR_PULSE_MIN, MOTOR_PULSE_MAX);

	m1.writeMicroseconds(pulse_1);
	m2.writeMicroseconds(pulse_2);
	m3.writeMicroseconds(pulse_3);
	m4.writeMicroseconds(pulse_4);
}
