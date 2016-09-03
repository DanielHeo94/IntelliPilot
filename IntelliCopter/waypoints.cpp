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

								mavlink_msg_command_ack_encode(SYSTEM_ID, COM_ID, &protocolMsg, &commandAck);
								isTimeoutEnabled = false;

								sendMessage(protocolMsg);
}

void System::Communicate::processMissionCount() {

								mavlink_mission_request_t missionRequest;
								waypointsList.clear();
								waypointsList.begin();

								mavlink_msg_mission_count_decode(&receivedMsg, &missionCount);
								missionRequest.seq = 0;
								missionRequest.target_system = 255;

								mavlink_msg_mission_request_encode(SYSTEM_ID, COM_ID, &protocolMsg, &missionRequest);
								isTimeoutEnabled = true;

								sendMessage(protocolMsg);
}

void System::Communicate::processMissionItem() {

								mavlink_mission_item_t missionItem;
								static int seqTemp = -1;

								mavlink_msg_mission_item_decode(&receivedMsg, &missionItem);
								if((int)missionItem.seq - seqTemp > 0) {
																waypointsList.pushBack(missionItem);
																Serial.println(missionItem.seq);
								}
								seqTemp = (int)missionItem.seq;

								waypointsList.pushBack(missionItem);

								// All waypoints item received
								if (missionItem.seq == missionCount.count) {
																mavlink_mission_ack_t missionAck;
																missionAck.target_system = 255;
																missionAck.type = MAV_MISSION_ACCEPTED;
																mavlink_msg_mission_ack_encode(SYSTEM_ID, COM_ID, &protocolMsg, &missionAck);
																isTimeoutEnabled = false;
								} else {
																mavlink_mission_request_t missionRequest;
																missionRequest.seq = missionItem.seq + 1;
																missionRequest.target_system = 255;

																mavlink_msg_mission_request_encode(SYSTEM_ID, COM_ID, &protocolMsg, &missionRequest);
																isTimeoutEnabled = true;
								}

								sendMessage(protocolMsg);
}

void System::Communicate::processMissionRequestList() {

								mavlink_mission_request_list_t missionRequestList;
								mavlink_mission_count_t sendingMissionCount;

								mavlink_msg_mission_request_list_decode(&receivedMsg, &missionRequestList);
								sendingMissionCount.count = waypointsList.getSize();
								sendingMissionCount.target_system = 255;
								waypointsList.begin();

								mavlink_msg_mission_count_encode(SYSTEM_ID, COM_ID, &protocolMsg, &sendingMissionCount);
								isTimeoutEnabled = true;

								sendMessage(protocolMsg);
}

void System::Communicate::processMissionRequest() {

								mavlink_mission_request_t missionRequest;
								mavlink_mission_item_t missionItem;

								static int seqTemp = -1;

								try {
																mavlink_msg_mission_request_decode(&receivedMsg, &missionRequest);
																if ((int)missionRequest.seq - seqTemp > 0) {
																								if (waypointsList.getElement(missionItem)) {
																																missionItem.target_system = 255;
																																throw 1;
																								}
																								seqTemp = (int)missionRequest.seq;
																}

																mavlink_msg_mission_item_encode(SYSTEM_ID, COM_ID, &protocolMsg, &missionItem);
																isTimeoutEnabled = true;

																sendMessage(protocolMsg);
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

								mavlink_msg_mission_ack_encode(SYSTEM_ID, COM_ID, &protocolMsg, &missionAck);
								isTimeoutEnabled = false;

								sendMessage(protocolMsg);
}
