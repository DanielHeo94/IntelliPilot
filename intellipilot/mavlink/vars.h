// gains.h

#ifndef _VARS_h
#define _VARS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

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

#endif

