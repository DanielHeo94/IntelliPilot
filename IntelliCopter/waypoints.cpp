//
//  waypoints.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Being modified by Minchan Kim since 2016. 8. 30.
//	Modified by Daniel Heo on 2016. 8. 31.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"
/*
   HACK: #include "list.h" was deleted.
      "list.h" has been moved to '/libraries' then renamed as 'IC_DataStructure/List.h'.
    List.h is included in System.h.
 */

uint8_t System::Communicate::Waypoints::waitMessage(mavlink_message_t &msg, mavlink_status_t &status) {
								if (timeout) {
																char buf;

																if (Serial3.readBytes(&buf, 1) == 0) {
																								sendMessage();
																} else {
																								return (mavlink_parse_char(MAVLINK_COMM_0, buf, &msg, &status));
																}
								} else {
																if (Serial3.available()) {
																								return (mavlink_parse_char(MAVLINK_COMM_0, (char)Serial3.read(), &msg, &status));
																}
								}
								return 0;
}

void System::Communicate::Waypoints::processCommandInt(const mavlink_message_t &msg) {

								mavlink_command_int_t command_int;
								mavlink_command_ack_t command_ack;

								mavlink_msg_command_int_decode(&msg, &command_int);
								switch (command_int.command) {
								case MAV_CMD_DO_SET_MODE:
																command_int.param1;
																break;

								case MAV_CMD_DO_PARACHUTE:
																break;

								default:
																break;
								}

								command_ack.command = command_int.command;
								command_ack.result = MAV_RESULT_ACCEPTED;

								mavlink_msg_command_ack_encode(0, 0, &(this->msg), &command_ack);
								timeout = false;

								sendMessage();
}

void System::Communicate::Waypoints::processMissionCount(const mavlink_message_t &msg) {

								mavlink_mission_count_t mission_count;
								mavlink_mission_request_int_t mission_request_int;

								mavlink_msg_mission_count_decode(&msg, &mission_count);
								count = mission_count.count;
								mission_request_int.seq = 0;

								mavlink_msg_mission_request_int_encode(0, 0, &(this->msg), &mission_request_int);
								timeout = true;

								sendMessage();
}

void System::Communicate::Waypoints::processMissionItemInt(const mavlink_message_t &msg) {

								static uint16_t seq = 0;
								mavlink_mission_item_int_t mission_item_int;

								mavlink_msg_mission_item_int_decode(&msg, &mission_item_int);
								waypointsList.pushBack(mission_item_int);
								seq++;
								if (seq == count) {
																mavlink_mission_ack_t mission_ack;
																seq = 0;
																mission_ack.type = MAV_MISSION_ACCEPTED;
																mavlink_msg_mission_ack_encode(0, 0, &(this->msg), &mission_ack);
																timeout = false;
								} else {
																mavlink_mission_request_int_t mission_request_int;
																mission_request_int.seq = seq;
																mavlink_msg_mission_request_int_encode(0, 0, &(this->msg), &mission_request_int);
																timeout = true;
								}

								sendMessage();
}

void System::Communicate::Waypoints::processMissionRequestList(const mavlink_message_t &msg) {

								mavlink_mission_request_list_t mission_request_list;
								mavlink_mission_count_t mission_count;

								mavlink_msg_mission_request_list_decode(&msg, &mission_request_list);
								mission_count.count = waypointsList.getSize();
								waypointsList.begin();

								mavlink_msg_mission_count_encode(0, 0, &(this->msg), &mission_count);
								timeout = true;

								sendMessage();
}

void System::Communicate::Waypoints::processMissionRequestInt(const mavlink_message_t &msg) {

								mavlink_mission_request_int_t mission_request_int;
								static mavlink_mission_item_int_t mission_item_int;
								static int seq = -1;

								try {
																mavlink_msg_mission_request_int_decode(&msg, &mission_request_int);
																if (mission_request_int.seq != static_cast<uint16_t>(seq)) {
																								if (waypointsList.getElement(mission_item_int)) throw 1;
																								seq = static_cast<int>(mission_request_int.seq);
																}

																mavlink_msg_mission_item_int_encode(0, 0, &(this->msg), &mission_item_int);
																timeout = true;

																sendMessage();
								} catch (int expn) {
																// TODO : Error handling
								}
}

void System::Communicate::Waypoints::processMissionAck(const mavlink_message_t &msg) {

								mavlink_mission_ack_t mission_ack;

								mavlink_msg_mission_ack_decode(&msg, &mission_ack);
								timeout = false;
}

void System::Communicate::Waypoints::processMissionClearAll(const mavlink_message_t &msg) {

								mavlink_mission_clear_all_t mission_clear_all;
								mavlink_mission_ack_t mission_ack;

								mavlink_msg_mission_clear_all_decode(&msg, &mission_clear_all);
								waypointsList.clear();
								mission_ack.type = MAV_MISSION_ACCEPTED;

								mavlink_msg_mission_ack_encode(0, 0, &(this->msg), &mission_ack);
								timeout = false;

								sendMessage();
}

void System::Communicate::Waypoints::sendMessage(void) {

								uint8_t buf[MAVLINK_MAX_PACKET_LEN];
								uint16_t length = mavlink_msg_to_send_buffer(buf, &msg);

								Serial3.write(buf, length);
}
