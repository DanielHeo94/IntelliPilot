//
//  gcs_mavlink.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Being modified by Minchan Kim since 2016. 8. 30.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

bool System::Communicate::isTimeoutEnabled;
mavlink_mission_count_t System::Communicate::missionCount;

mavlink_message_t System::Communicate::receivedMsg;
mavlink_status_t System::Communicate::receivedStatus;

mavlink_message_t System::Communicate::commonMsg;
mavlink_message_t System::Communicate::protocolMsg;

void System::Setup::gcs_mavlink() {
								Serial3.setTimeout(1000);
								Serial3.begin(COMMUNICATE_GCS_WIRELESS_BAUDRATE);
								Serial3.flush();
}

void System::Communicate::transferMsgToGcs(void *arg) {

								TickType_t xLastWakeTime = xTaskGetTickCount();
								const TickType_t xWakePeriod = FREQUENCY_TASK_COMM_GCS;

								for(;; ) {
																mavlink_msg_heartbeat_pack(SYSTEM_ID, COM_ID, &commonMsg, TYPE, AUTOPILOT_TYPE, (subscribe.status())->flightMode, CUSTOM_MODE, SYSTEM_STATE);
																sendMessage(commonMsg);

																mavlink_msg_attitude_pack(SYSTEM_ID, COM_ID, &commonMsg, 0, subscribe.attitude()->ypr.dmp.radians[2], subscribe.attitude()->ypr.dmp.radians[1], subscribe.attitude()->ypr.dmp.radians[0], subscribe.attitude()->gyro.dmp.radians[0], subscribe.attitude()->gyro.dmp.radians[1], subscribe.attitude()->gyro.dmp.radians[2]);
																sendMessage(commonMsg);

																//mavlink_msg_gps_raw_int_pack(SYSTEM_ID, COM_ID, &sendingMsg, 0, 2, (subscribe.position())->latitude * pow(10, 7), (subscribe.position())->longitude * pow(10, 7), (subscribe.altitude())->gps * 1000, (subscribe.gpsInfo())->hdop, UINT16_MAX, (subscribe.position())->speed * 100, (int)((subscribe.position())->cog) * 100, (subscribe.gpsInfo())->number_of_satellites);
																//sendMessage();

																//mavlink_msg_gps_status_pack(SYSTEM_ID, COM_ID, &sendingMsg, numsatsinview, prn, 1, elevation, (255 * azimuth) / 360, snr);
																//sendMessage();

																//mavlink_msg_battery_status_pack(SYSTEM_ID, COM_ID, &sendingMsg, BATTERY_ID, 0, 0, INT16_MAX, 0, -1, -1, -1, batteryPercent);
																//sendMessage();

																receiveMsgFromGcs();

																vTaskDelayUntil(&xLastWakeTime, xWakePeriod);
								}
}

void System::Communicate::receiveMsgFromGcs() {
								if (communicate.getParams()) {
																switch (receivedMsg.msgid) {
																case MAVLINK_MSG_ID_COMMAND_INT:
																								communicate.processCommandInt();
																								break;

																case MAVLINK_MSG_ID_COMMAND_LONG:
																								communicate.processCommandLong();
																								break;

																case MAVLINK_MSG_ID_MISSION_COUNT:
																								communicate.processMissionCount();
																								break;

																case MAVLINK_MSG_ID_MISSION_ITEM:
																								communicate.processMissionItem();
																								break;

																case MAVLINK_MSG_ID_MISSION_REQUEST_LIST:
																								communicate.processMissionRequestList();
																								break;

																case MAVLINK_MSG_ID_MISSION_REQUEST:
																								communicate.processMissionRequest();
																								break;

																case MAVLINK_MSG_ID_MISSION_ACK:
																								communicate.processMissionAck();
																								break;

																case MAVLINK_MSG_ID_MISSION_CLEAR_ALL:
																								communicate.processMissionClearAll();
																								break;

																case MAVLINK_MSG_ID_PARAM_REQUEST_READ:
																								break;

																case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
																								break;

																case MAVLINK_MSG_ID_PARAM_VALUE:
																								break;

																case MAVLINK_MSG_ID_PARAM_SET:
																								break;

																default:
																								break;
																}
								}
}

uint8_t System::Communicate::getParams() {
								if (isTimeoutEnabled) {
																char buf;

																if (Serial3.readBytes(&buf, 1) == 0) {
																								sendMessage(protocolMsg);
																} else {
																								return (mavlink_parse_char(MAVLINK_COMM_0, buf, &receivedMsg, &receivedStatus));
																}
								} else {
																if (Serial3.available()) {
																								return (mavlink_parse_char(MAVLINK_COMM_0, (char)Serial3.read(), &receivedMsg, &receivedStatus));
																}
								}
								return 0;
}

void System::Communicate::sendMessage(mavlink_message_t &msg) {

								uint8_t buf[MAVLINK_MAX_PACKET_LEN];
								uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

								#if (COMMUNICATE_GCS_WIRE == 1)
								Serial.write(buf, len);
								#endif

								#if (COMMUNICATE_GCS_WIRELESS == 1)
								Serial3.write(buf, len);
								#endif
}

void System::Communicate::processCommandInt() {

								mavlink_command_int_t commandInt;
								mavlink_command_ack_t commandAck;

								mavlink_msg_command_int_decode(&receivedMsg, &commandInt);

								switch (commandInt.command) {
								case MAV_CMD_DO_SET_MODE:
																commandInt.param1;
																break;

								case MAV_CMD_DO_PARACHUTE:
																break;

								default:
																break;
								}

								commandAck.command = commandInt.command;
								commandAck.result = MAV_RESULT_ACCEPTED;

								mavlink_msg_command_ack_encode(SYSTEM_ID, COM_ID, &protocolMsg, &commandAck);
								isTimeoutEnabled = false;

								sendMessage(protocolMsg);
}

void System::Communicate::processCommandLong() {

								mavlink_command_long_t commandLong;

								mavlink_msg_command_long_decode(&receivedMsg, &commandLong);

								switch (commandLong.command) {
								case MAV_CMD_COMPONENT_ARM_DISARM:
																statusBox.flightReady = true;
																break;

								default:
																break;
								}
}
