// gains.h

#ifndef _VARS_h
#define _VARS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define SYSTEM_ID		20                   ///< ID 20 for this airplane
static int COM_ID =		MAV_COMP_ID_IMU;     ///< The component sending the message is the IMU, it could be also a Linux process
static int TYPE	  =		MAV_TYPE_QUADROTOR;   ///< This system is an airplane / fixed wing

								 // Define the system type, in this case an airplane
static uint8_t SYSTEM_TYPE =	MAV_TYPE_FIXED_WING;
static uint8_t AUTOPILOT_TYPE = MAV_AUTOPILOT_GENERIC;

static uint8_t SYSTEM_MODE =	MAV_MODE_PREFLIGHT; ///< Booting up
static uint32_t CUSTOM_MODE =	0;                 ///< Custom mode, can be defined by user/adopter
static uint8_t SYSTEM_STATE =	MAV_STATE_STANDBY; ///< System ready for flight
										  // Initialize the required buffers
mavlink_message_t _heartbeat_msg;
mavlink_message_t _attitude_msg;
mavlink_message_t _gps_pos_msg;

uint8_t _heartbeat_buf[MAVLINK_MAX_PACKET_LEN];
uint8_t _attitude_buf[MAVLINK_MAX_PACKET_LEN];
uint8_t _gps_pos_buf[MAVLINK_MAX_PACKET_LEN];

uint16_t _heartbeat_len;
uint16_t _attitude_len;
uint16_t _gps_pos_len;

#endif

