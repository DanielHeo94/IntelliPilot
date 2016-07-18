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

TaskHandle_t idleHandle;
TaskHandle_t manualControlHandle;
TaskHandle_t posHoldControlHandle;

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

		/*
		Serial.print(angle[0]);
		Serial.print("\t");
		Serial.print(angle[1]);
		Serial.print("\t");
		Serial.println(angle[2]);
		*/

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
		if ((speed = _nmea.f_speed_mps()) == nmea::GPS_INVALID_F_SPEED) {}
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

		status = _radio.getCommands(cmd);
		if ((status - statusLast) != 0) tasks::procCommands(status);
		statusLast = status;
    }
}

void tasks::commGcsThread(void* arg) {

	//TickType_t xLastWakeTime;
	//const TickType_t xFrequency = 5;

	//xLastWakeTime = xTaskGetTickCount();

	for (;;) {
		//vTaskDelayUntil(&xLastWakeTime, xFrequency);

		mavlink_msg_heartbeat_pack(SYSTEM_ID, COM_ID, &_heartbeat_msg, TYPE, AUTOPILOT_TYPE, mavMode, CUSTOM_MODE, SYSTEM_STATE);
		mavlink_msg_attitude_pack(SYSTEM_ID, COM_ID, &_attitude_msg, 0, angle[0] * M_PI / 180, angle[1] * M_PI / 180, angle[2] * M_PI / 180, gyro[0] * M_PI / 180, gyro[1] * M_PI / 180, gyro[2] * M_PI / 180);
		mavlink_msg_gps_raw_int_pack(SYSTEM_ID, COM_ID, &_gps_pos_msg, 0, 3, (latitude * pow(10, 7)), (longitude * pow(10, 7)), (gpsAlt * 1000), hdop, UINT16_MAX, (speed * 100), UINT16_MAX, numsats);

		_heartbeat_len = mavlink_msg_to_send_buffer(_heartbeat_buf, &_heartbeat_msg);
		_attitude_len = mavlink_msg_to_send_buffer(_attitude_buf, &_attitude_msg);
		_gps_pos_len = mavlink_msg_to_send_buffer(_gps_pos_buf, &_gps_pos_msg);

		// Wire
		//Serial.write(_heartbeat_buf, _heartbeat_len);
		//Serial.write(_attitude_buf, _attitude_len);
		//Serial.write(_gps_pos_buf, _gps_pos_len);

		// Wireless
		Serial3.write(_heartbeat_buf, _heartbeat_len);
		Serial3.write(_attitude_buf, _attitude_len);
		Serial3.write(_gps_pos_buf, _gps_pos_len);

		vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
	}
}

/*######################################
		Commands Processing
######################################*/

void tasks::procCommands(int event) {

		switch (event)
		{
		case STATE_PREFLIGHT_ARMED:
			mavMode = MAV_MODE_PREFLIGHT;
			break;

		case STATE_PREFLIGHT_DISARMED:
			mavMode = MAV_MODE_PREFLIGHT;
			vTaskResume(idleHandle);
			vTaskSuspend(manualControlHandle);
			break;

		case STATE_MANUAL_ARMED:
			mavMode = MAV_MODE_MANUAL_ARMED;
			vTaskSuspend(idleHandle);
			vTaskResume(manualControlHandle);
			break;

		case STATE_HOLD_ARMED:
			mavMode = MAV_MODE_GUIDED_ARMED;
			vTaskSuspend(manualControlHandle);
			_motors.stop();
			break;

		case STATE_AUTO_ARMED:
			mavMode = MAV_MODE_AUTO_ARMED;
			vTaskSuspend(manualControlHandle);
			_motors.stop();
			break;

		default:
			break;

		}
}

/*######################################
			Flight Control
######################################*/

void fcInit() {

	rollAngleError = 0;
	rollRateError = 0;
	pitchAngleError = 0;
	pitchRateError = 0;
	yawRateError = 0;
}

void tasks::idle( void* arg ) {

	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xWakePeriod = 1000 / portTICK_PERIOD_MS;

	for (;;) {
		_motors.stop();

		vTaskDelayUntil(&xLastWakeTime, xWakePeriod);
	}
}

void tasks::manualControlThread( void* arg ) {

	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xWakePeriod = 3 / portTICK_PERIOD_MS;

	fcInit();

	pid manualRollAngleReg(&angle[0], &rollAngleError, &manualCmd[0], ROLL_OUTER_P_GAIN, 0, 0, DIRECT);
	pid manualRollRateReg(&gyro[0], &rollRateError, &rollAngleError,
		ROLL_INNER_P_GAIN,
		ROLL_INNER_I_GAIN,
		ROLL_INNER_D_GAIN,
		DIRECT);

	pid manualPitchAngleReg(&angle[1], &pitchAngleError, &manualCmd[1], PITCH_OUTER_P_GAIN, 0, 0, REVERSE);
	pid manualPitchRateReg(&gyro[1], &pitchRateError, &pitchAngleError,
		PITCH_INNER_P_GAIN,
		PITCH_INNER_I_GAIN,
		PITCH_INNER_D_GAIN,
		REVERSE);

	pid manualYawRateReg(&gyro[2], &yawRateError, &manualCmd[2],
		YAW_P_GAIN,
		YAW_I_GAIN,
		YAW_D_GAIN,
		DIRECT);

	manualRollAngleReg.SetMode(AUTOMATIC);
	manualRollAngleReg.SetOutputLimits(-1000, 1000);
	manualRollRateReg.SetMode(AUTOMATIC);
	manualRollRateReg.SetOutputLimits(-700, 700);

	manualPitchAngleReg.SetMode(AUTOMATIC);
	manualPitchAngleReg.SetOutputLimits(-1000, 1000);
	manualPitchRateReg.SetMode(AUTOMATIC);
	manualPitchRateReg.SetOutputLimits(-700, 700);

	manualYawRateReg.SetMode(AUTOMATIC);
	manualYawRateReg.SetOutputLimits(-300, 300);

	for (;;) {

		manualCmd[0] = map(cmd[0], RC_CH3_LOW, RC_CH3_HIGH, ROLL_ANG_MIN, ROLL_ANG_MAX);
		manualCmd[1] = map(cmd[1], RC_CH2_LOW, RC_CH2_HIGH, PITCH_ANG_MIN, PITCH_ANG_MAX);
		manualCmd[2] = map(cmd[2], RC_CH1_LOW, RC_CH1_HIGH, YAW_SPEED_MIN, YAW_SPEED_MAX);
		manualCmd[3] = map(cmd[3], RC_CH4_LOW, RC_CH4_HIGH, MOTOR_PULSE_MIN, MOTOR_PULSE_MAX);

		/*
		Serial.print(manualCmd[0]);
		Serial.print("\t");
		Serial.print(manualCmd[1]);
		Serial.print("\t");
		Serial.print(manualCmd[2]);
		Serial.print("\t");
		Serial.println(manualCmd[3]);
		*/
		
		if (manualCmd[3] > MOTOR_PULSE_TAKEOFF) {

			manualRollAngleReg.Compute();
			manualRollRateReg.Compute();

			manualPitchAngleReg.Compute();
			manualPitchRateReg.Compute();

			manualYawRateReg.Compute();

			force1 = (-pitchRateError + rollRateError) * 0.5 - yawRateError + manualCmd[3];
			force2 = (-pitchRateError - rollRateError) * 0.5 + yawRateError + manualCmd[3];
			force3 = (pitchRateError - rollRateError) * 0.5 - yawRateError + manualCmd[3];
			force4 = (pitchRateError + rollRateError) * 0.5 + yawRateError + manualCmd[3];

			_motors.rotate(force1, force2, force3, force4);

			/*
			Serial.print(force1);
			Serial.print("\t");
			Serial.print(force2);
			Serial.print("\t");
			Serial.print(force3);
			Serial.print("\t");
			Serial.println(force4);
			*/
		}
		else {
			_motors.stop();
		}

		vTaskDelayUntil(&xLastWakeTime, xWakePeriod);
	}  
}


