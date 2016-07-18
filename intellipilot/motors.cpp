// 
// 
// 

#include "motors.h"
#include "math.h"

math mMath;

Servo m1, m2, m3, m4;

motors::motors(){}

bool motors::start() {

	Serial.print("\t\tAttach M1."); m1.attach(QUAD_MOTOR_A); Serial.println("\t\tSuccess.");
	Serial.print("\t\tAttach M2."); m2.attach(QUAD_MOTOR_B); Serial.println("\t\tSuccess.");
	Serial.print("\t\tAttach M3."); m3.attach(QUAD_MOTOR_C); Serial.println("\t\tSuccess.");
	Serial.print("\t\tAttach M4."); m4.attach(QUAD_MOTOR_D); Serial.println("\t\tSuccess.");

	delay(1000);

	Serial.print("\t\tPre-Arming...");
	m1.writeMicroseconds(MOTOR_PULSE_MIN);
	m2.writeMicroseconds(MOTOR_PULSE_MIN);
	m3.writeMicroseconds(MOTOR_PULSE_MIN);
	m4.writeMicroseconds(MOTOR_PULSE_MIN);

	delay(MOTOR_START_DELAY);

	if (m1.attached() && m2.attached() && m3.attached() && m4.attached()) {
		Serial.println("\t\tSuccess.");
		return true;
	}
	else {
		Serial.println("\t\tMotor start fail");
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

void motors::stop(int pulse_1, int pulse_2, int pulse_3, int pulse_4) {
	
	m1.writeMicroseconds(pulse_1);
	m2.writeMicroseconds(pulse_2);
	m3.writeMicroseconds(pulse_3);
	m4.writeMicroseconds(pulse_4);
}
