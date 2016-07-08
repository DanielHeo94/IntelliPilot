#ifndef DronixCopter(RC)
#include "nmea.h"
#include "lidar.h"
#include "math.h"
#include "gains.h"
#include "pid.h"
#include "gps.h"
#include "config.h"
#include "params.h"
#include "remote.h"
#include "motors.h"
#define DronixCopter(RC)

#include <Servo.h>
#include <Wire.h>
#include "ahrs.h"
#include "mavlink/common/mavlink.h"









/* Offset values
*
*/



float angle[3];
float gyro[3];


/*  LED Indicators
*
*/



/*
* Yaw Error
*/
float YAW_RATE_ERR;
float YAW_RATE_ERR_P, YAW_RATE_ERR_I;
float YAW_TORQUE;

/*
* Pitch Error
*/
float PITCH_ANG_ERR, PITCH_RATE_ERR, PITCH_RATE_ERR_PRE;     // �ܺ� ���� ����
float PITCH_RATE_ERR_P, PITCH_RATE_ERR_I, PITCH_RATE_ERR_D;  // ���� ���� ����
float PITCH_BALANCE;

/*
* Roll Error
*/
float ROLL_ANG_ERR, ROLL_RATE_ERR, ROLL_RATE_ERR_PRE;        // �ܺ� ���� ����
float ROLL_RATE_ERR_P, ROLL_RATE_ERR_I, ROLL_RATE_ERR_D;     // ���� ���� ����
float ROLL_BALANCE;

/*
* Alt Error
*/
float CLIMB_RATE_ERR;
float CLIMB_RATE_ERR_P, CLIMB_RATE_ERR_I;
float Throttle, ThrottleLast;


int va, vb, vc, vd;                    //throt

Servo a, b, c, d;

/* Mavlink variables
*
*/
int sysid = 20;                   ///< ID 20 for this airplane
int compid = MAV_COMP_ID_IMU;     ///< The component sending the message is the IMU, it could be also a Linux process
int type = MAV_TYPE_QUADROTOR;   ///< This system is an airplane / fixed wing

								 // Define the system type, in this case an airplane
uint8_t system_type = MAV_TYPE_FIXED_WING;
uint8_t autopilot_type = MAV_AUTOPILOT_GENERIC;

uint8_t system_mode = MAV_MODE_PREFLIGHT; ///< Booting up
uint32_t custom_mode = 0;                 ///< Custom mode, can be defined by user/adopter
uint8_t system_state = MAV_STATE_STANDBY; ///< System ready for flight
										  // Initialize the required buffers
mavlink_message_t _msg;
mavlink_message_t _msg2;

uint8_t _buf[MAVLINK_MAX_PACKET_LEN];
uint8_t _buf2[MAVLINK_MAX_PACKET_LEN];

/*  Setup function
*
*/

void setup() {
	Serial.begin(9600);
	Serial3.begin(57600);
	Serial.flush();

	Serial.println("Initializing AHRS...");
	mahrs.begin(115200);
	Serial.println("Done");

	initLEDs();
}

void loop() {
	////////////////////////////////// Get angle & gyro ////////////////////////////////////////////////////////
	// if programming failed, don't try to do anything
	while (!mahrs.getEulerAnglesGyro(angle, gyro)) {
		/*
		* Waiting for data ready
		*/
	}
	angle[0] += ROLL_ANG_OFFSET;
	angle[1] += PITCH_ANG_OFFSET;

	/*
	Serial.print("Roll : ");
	Serial.print(angle[0]);
	Serial.print("\tPitch : ");
	Serial.print(angle[1]);
	Serial.print("\tYaw : ");
	Serial.print(angle[2]);

	Serial.print("\tRoll Gyro : ");
	Serial.print(gyro[0]);
	Serial.print("\tPitch Gyro : ");
	Serial.print(gyro[1]);
	Serial.print("\tYaw Gyro : ");
	Serial.println(gyro[2]);
	*/
	////////////////////////////////////////// Compute PID /////////////////////////////////////////////////////  
	if (isArmed) computePID();

	releaseLock();
	/////////////////////////////////////Throttle Calculation //////////////////////////////////////////////////

	if ((Throttle < ESC_MIN) || (Throttle > ESC_MAX)) Throttle = ThrottleLast;
	ThrottleLast = Throttle;

	/*
	Serial.print("Yaw cmd : ");
	Serial.print(ch1);
	Serial.print("\tPitch cmd : ");
	Serial.print(ch2);
	Serial.print("\tRoll cmd : ");
	Serial.print(ch3);
	Serial.print("\tTHRO cmd : ");
	Serial.println(Throttle);
	*/

	va = (-PITCH_BALANCE + ROLL_BALANCE) * 0.5 + YAW_TORQUE + (float)Throttle;
	vb = (-PITCH_BALANCE - ROLL_BALANCE) * 0.5 - YAW_TORQUE + (float)Throttle;
	vc = (PITCH_BALANCE - ROLL_BALANCE) * 0.5 + YAW_TORQUE + (float)Throttle;
	vd = (PITCH_BALANCE + ROLL_BALANCE) * 0.5 - YAW_TORQUE + (float)Throttle;

	if (Throttle < ESC_TAKEOFF_OFFSET) {
		va = ESC_MIN;
		vb = ESC_MIN;
		vc = ESC_MIN;
		vd = ESC_MIN;
	}




	/*
	Serial.print("Roll : ");
	Serial.print(angle[0]);
	Serial.print("\tPitch : ");
	Serial.println(angle[1]);
	*/
	/*
	Serial.print("Roll Gyro : ");
	Serial.print(gyro[0]);
	Serial.print("Pitch Gyro : ");
	Serial.print(gyro[1]);
	Serial.print("Yaw Gyro : ");
	Serial.println(gyro[2]);
	*/
	/*
	Serial.print("va : ");
	Serial.print(va);
	Serial.print("\tvb : ");
	Serial.print(vb);
	Serial.print("\tvc : ");
	Serial.print(vc);
	Serial.print("\tvd : ");
	Serial.println(vd);
	*/
	/////////////////////////////////// End of loop ////////////////////////////////////////////////////////////////
	mavlink_msg_heartbeat_pack(sysid, compid, &_msg, type, autopilot_type, system_mode, custom_mode, system_state);
	mavlink_msg_attitude_pack(sysid, compid, &_msg2, 0, angle[0] * M_PI / 180, angle[1] * M_PI / 180, angle[2] * M_PI / 180, gyro[0] * M_PI / 180, gyro[1] * M_PI / 180, gyro[2] * M_PI / 180);

	uint16_t _len = mavlink_msg_to_send_buffer(_buf, &_msg);
	uint16_t _len2 = mavlink_msg_to_send_buffer(_buf2, &_msg2);

	Serial3.write(_buf, _len);
	Serial3.write(_buf2, _len2);
}



inline void initLEDs() {

	Serial.println("Initializing LED indicators");

	pinMode(STATE_ARMED, OUTPUT);
	pinMode(STATE_DISARMED, OUTPUT);

	digitalWrite(STATE_DISARMED, HIGH);

	Serial.println("Done");
}

//ERR_I limit to prevent divergence


void computePID() {
	//Yaw Control
	YAW_RATE_ERR = (float)ch1 - gyro[2];

	YAW_RATE_ERR_P = YAW_RATE_ERR * YAW_P_GAIN;
	YAW_RATE_ERR_I += YAW_RATE_ERR * YAW_I_GAIN * SAMPLING_TIME;
	LIMIT_VALUE(&YAW_RATE_ERR_I, -500, 500);

	YAW_TORQUE = YAW_RATE_ERR_P + YAW_RATE_ERR_I;

	//Pitch Control
	PITCH_ANG_ERR = (float)ch2 + angle[1];
	PITCH_RATE_ERR = PITCH_ANG_ERR * PITCH_OUTER_P_GAIN + gyro[1];

	PITCH_RATE_ERR_P = PITCH_RATE_ERR * PITCH_INNER_P_GAIN;
	PITCH_RATE_ERR_I += PITCH_RATE_ERR * PITCH_INNER_I_GAIN * SAMPLING_TIME;
	LIMIT_VALUE(&PITCH_RATE_ERR_I, -300, 300);
	PITCH_RATE_ERR_D = (PITCH_RATE_ERR - PITCH_RATE_ERR_PRE) / SAMPLING_TIME * PITCH_INNER_D_GAIN;

	PITCH_RATE_ERR_PRE = PITCH_RATE_ERR;

	PITCH_BALANCE = PITCH_RATE_ERR_P + PITCH_RATE_ERR_I + PITCH_RATE_ERR_D;

	//Roll Control
	ROLL_ANG_ERR = (float)ch3 - angle[0];
	ROLL_RATE_ERR = ROLL_ANG_ERR * ROLL_OUTER_P_GAIN - gyro[0];

	ROLL_RATE_ERR_P = ROLL_RATE_ERR * ROLL_INNER_P_GAIN;
	ROLL_RATE_ERR_I += ROLL_RATE_ERR * ROLL_INNER_I_GAIN * SAMPLING_TIME;
	LIMIT_VALUE(&ROLL_RATE_ERR_I, -300, 300);
	ROLL_RATE_ERR_D = (ROLL_RATE_ERR - ROLL_RATE_ERR_PRE) / SAMPLING_TIME * ROLL_INNER_D_GAIN;

	ROLL_RATE_ERR_PRE = ROLL_RATE_ERR;

	ROLL_BALANCE = ROLL_RATE_ERR_P + ROLL_RATE_ERR_I + ROLL_RATE_ERR_D;

	//Alt Control
	CLIMB_RATE_ERR = (float)ch4 - (int)vel[0];

	CLIMB_RATE_ERR_P = CLIMB_RATE_ERR * ALT_P_GAIN;
	CLIMB_RATE_ERR_I += CLIMB_RATE_ERR * ALT_I_GAIN * SAMPLING_TIME;
	LIMIT_VALUE(&CLIMB_RATE_ERR_I, -1400, 1400);

	Throttle = ESC_TAKEOFF_OFFSET + CLIMB_RATE_ERR_P + CLIMB_RATE_ERR_I;
}

void resetPID() {
	YAW_RATE_ERR_I = 0;
	PITCH_RATE_ERR_I = 0;
	ROLL_RATE_ERR_I = 0;
	CLIMB_RATE_ERR_I = 0;

	YAW_TORQUE = 0;
	PITCH_BALANCE = 0;
	ROLL_BALANCE = 0;
	Throttle = 0;
}

#endif
