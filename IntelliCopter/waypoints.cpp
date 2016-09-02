//
//  waypoints.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Modified by Minchan Kim since 2016. 8. 30.
//	Modified by Daniel Heo on 2016. 8. 31.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

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

								mavlink_msg_command_ack_encode(SYSTEM_ID, COM_ID, &sendingMsg, &commandAck);
								isTimeoutEnabled = false;

								sendMessage();
}

void System::Communicate::processMissionCount() {

								mavlink_mission_request_t missionRequest;

								mavlink_msg_mission_count_decode(&receivedMsg, &missionCount);
								missionRequest.seq = 0;
								missionRequest.target_system = 255;

								mavlink_msg_mission_request_encode(SYSTEM_ID, COM_ID, &sendingMsg, &missionRequest);
								isTimeoutEnabled = true;

								sendMessage();
}

void System::Communicate::processMissionItem() {

								mavlink_mission_item_t missionItem;

								mavlink_msg_mission_item_decode(&receivedMsg, &missionItem);
								waypointsList.pushBack(missionItem);

								// All waypoints item received
								if (missionItem.seq == missionCount.count) {
																mavlink_mission_ack_t missionAck;
																missionAck.type = MAV_MISSION_ACCEPTED;
																mavlink_msg_mission_ack_encode(COM_ID, SYSTEM_ID, &sendingMsg, &missionAck);
																isTimeoutEnabled = false;
								} else {
																mavlink_mission_request_t missionRequest;
																missionRequest.seq = missionItem.seq + 1;
																missionRequest.target_system = 255;

																mavlink_msg_mission_request_encode(SYSTEM_ID, COM_ID, &sendingMsg, &missionRequest);
																isTimeoutEnabled = true;
								}

								sendMessage();
}

void System::Communicate::processMissionRequestList() {

								mavlink_mission_request_list_t missionRequestList;
								mavlink_mission_count_t sendingMissionCount;

								mavlink_msg_mission_request_list_decode(&receivedMsg, &missionRequestList);
								sendingMissionCount.count = waypointsList.getSize();
								waypointsList.begin();

								mavlink_msg_mission_count_encode(SYSTEM_ID, COM_ID, &sendingMsg, &sendingMissionCount);
								isTimeoutEnabled = true;

								sendMessage();
}

void System::Communicate::processMissionRequest() {

								mavlink_mission_request_t missionRequest;
								mavlink_mission_item_t missionItem;

								static int seq = -1;

								try {
																mavlink_msg_mission_request_decode(&receivedMsg, &missionRequest);
																if (missionRequest.seq != static_cast<uint16_t>(seq)) {
																								if (waypointsList.getElement(missionItem)) throw 1;
																								seq = static_cast<int>(missionRequest.seq);
																}

																mavlink_msg_mission_item_encode(SYSTEM_ID, COM_ID, &sendingMsg, &missionItem);
																isTimeoutEnabled = true;

																sendMessage();
								} catch (int expn) {
																// TODO : Error handling
								}
}

void System::Communicate::processMissionAck() {

								mavlink_mission_ack_t missionAck;

								mavlink_msg_mission_ack_decode(&receivedMsg, &missionAck);
								isTimeoutEnabled = false;
}

void System::Communicate::processMissionClearAll() {

								mavlink_mission_clear_all_t missionClearAll;
								mavlink_mission_ack_t missionAck;

								mavlink_msg_mission_clear_all_decode(&receivedMsg, &missionClearAll);
								waypointsList.clear();
								missionAck.type = MAV_MISSION_ACCEPTED;

								mavlink_msg_mission_ack_encode(SYSTEM_ID, COM_ID, &sendingMsg, &missionAck);
								isTimeoutEnabled = false;

								sendMessage();
}
