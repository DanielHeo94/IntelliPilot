//
//  tasks.cpp
//  intellipilot
//
//  Created by 허진수 on 2016. 7. 11..
//  Copyright © 2016년 dronix.kr. All rights reserved.
//

#include <Wire.h>

#include "tasks.h"
#include "vars.h"
#include "gains.h"
#include "params.h"

#include "math.h"

#include "ahrs.h"
#include "baro.h"
#include "comp.h"
#include "gps.h"
#include "lidar.h"
#include "motors.h"
#include "nmea.h"
#include "pid.h"
#include "radio.h"

#include "mavlink/common/mavlink.h"
#include "mavlink/vars.h"

#include <FreeRTOS_ARM.h>

math _math;

ahrs _ahrs;
baro _barometer;
comp _compass;
gps _gps;
lidar _lidar;
motors _motors;
nmea _nmea;
radio _radio;

SemaphoreHandle_t sem;

tasks::tasks() {}

/*######################################
				Sensors
######################################*/

void tasks::getEulerAnglesGyroThread( void* arg ){
    for(;;) {
        while(!_ahrs.getEulerAnglesGyro(angle, gyro)){
            /*
             *  Waiting for data ready
             */
        }
    }
}

void tasks::getAccAltThread( void* arg ){
    for(;;) {
        lidarAlt = _lidar.getVerDistance();
        //lidarVel = _lidar.getVerVelocity();
    }
}

void tasks::getPosThread(void* arg) {
	for (;;) {
		unsigned long start = millis();
		do
		{
			while (Serial2.available())
				_nmea.encode(Serial2.read());
		} while (millis() - start < 100);

		if ((numsats = (int)_nmea.satellites()) == nmea::GPS_INVALID_SATELLITES) {}
		if ((hdop = (int)_nmea.hdop()) == nmea::GPS_INVALID_HDOP) {}
		_nmea.f_get_position(&latitude, &longitude, &_age);
		if (latitude == nmea::GPS_INVALID_F_ANGLE) {}
		if (longitude == nmea::GPS_INVALID_F_ANGLE) {}
		if (_age == nmea::GPS_INVALID_AGE) {}
		_nmea.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &__age);
		if (__age == nmea::GPS_INVALID_AGE) {}
		if ((gpsAlt = _nmea.f_altitude()) == nmea::GPS_INVALID_ALTITUDE) {}
		if ((speed = _nmea.f_speed_kmph()) == nmea::GPS_INVALID_F_SPEED) {}

	}
}

void tasks::getBaroThread(void* arg) {
	for (;;) {
		temparature = _barometer.readTemperature();
		pressure = _barometer.readPressure();
		baroAlt = _barometer.readAltitude();
		sealevelpressure = _barometer.readSealevelPressure();
	}
}

void tasks::getCompThread(void* arg) {
	for (;;) {
		Vector norm = _compass.readNormalize();

		heading = atan2(norm.YAxis, norm.XAxis);

		declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
		heading += declinationAngle;

		if (heading < 0)
		{
			heading += 2 * PI;
		}

		if (heading > 2 * PI)
		{
			heading -= 2 * PI;
		}

		headingDegrees = _math.radiantodegree(heading);
	}
}

/*######################################
			Communication
######################################*/

void tasks::getCommandsThread( void* arg ){
    for (;;) {
		if ((status = _radio.getCommands(cmd))) xSemaphoreGive(sem);
    }
}

void tasks::commGcsThread(void* arg) {
	for (;;) {
		mavlink_msg_heartbeat_pack(sysid, compid, &_msg, type, autopilot_type, system_mode, custom_mode, system_state);
		mavlink_msg_attitude_pack(sysid, compid, &_msg2, 0, angle[0] * M_PI / 180, angle[1] * M_PI / 180, angle[2] * M_PI / 180, gyro[0] * M_PI / 180, gyro[1] * M_PI / 180, gyro[2] * M_PI / 180);

		uint16_t _len = mavlink_msg_to_send_buffer(_buf, &_msg);
		uint16_t _len2 = mavlink_msg_to_send_buffer(_buf2, &_msg2);

		Serial.write(_buf, _len);
		Serial.write(_buf2, _len2);

		vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
	}
}

/*######################################
		Commands Processing
######################################*/

void tasks::procCommands(int event) {
	
	xSemaphoreTake(sem, portMAX_DELAY);

	switch (status)
	{
	case STATE_ARMED:
		xTaskSuspend(idleHandle);
		xTaskResume(manualControlHandle);
		break;

	case STATE_DISARMED:
		xTaskSuspend(manualControlHandle);
		xTaskResume(idleHandle);
		break;

	default:
		break;
	}
}

/*######################################
			Flight Control
######################################*/

void tasks::idle(void* arg) {

	vTaskSuspend(manualControlHandle);
	_motors.stop();
}

void tasks::manualControlThread( void* arg ) {

	pid manualRollAngleReg(&angle[0], &rollAngleError, &cmd[0], ROLL_OUTER_P_GAIN, 0, 0, DIRECT);
	pid manualRollRateReg(&gyro[0], &rollRateError, &rollAngleError,
		ROLL_INNER_P_GAIN,
		ROLL_INNER_I_GAIN,
		ROLL_INNER_D_GAIN,
		DIRECT);

	pid manualPitchAngleReg(&angle[1], &pitchAngleError, &cmd[1], PITCH_OUTER_P_GAIN, 0, 0, REVERSE);
	pid manualPitchRateReg(&gyro[1], &pitchRateError, &pitchAngleError,
		PITCH_INNER_P_GAIN,
		PITCH_INNER_I_GAIN,
		PITCH_INNER_D_GAIN,
		REVERSE);

	pid manualYawRateReg(&angle[2], &yawRateError, &cmd[2],
		YAW_P_GAIN,
		YAW_I_GAIN,
		YAW_D_GAIN,
		DIRECT);

	for (;;) {
		manualRollAngleReg.Compute();
		manualRollRateReg.Compute();

		manualPitchAngleReg.Compute();
		manualPitchRateReg.Compute();

		manualYawRateReg.Compute();

		force1 = (-pitchRateError + rollRateError) * (1 / 2) + yawRateError + cmd[3];
		force2 = (-pitchRateError - rollRateError) * (1 / 2) - yawRateError + cmd[3];
		force3 = (pitchRateError - rollRateError) * (1 / 2) + yawRateError + cmd[3];
		force4 = (pitchRateError + rollRateError) * (1 / 2) - yawRateError + cmd[3];

		_motors.rotate(force1, force2, force3, force4);
	}  
}


