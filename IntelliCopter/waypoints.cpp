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

int seq;

void System::Setup::loadWaypoints() {
								byte* bMissionCount = storage.readAddress(4);
								memcpy(&communicate.missionCount, bMissionCount, sizeof(mavlink_mission_count_t));

								byte* bMissionItem = storage.readAddress(8);
								mavlink_mission_item_t temp[communicate.missionCount.count];
								memcpy(temp, bMissionItem, communicate.missionCount.count * sizeof(mavlink_mission_item_t));

								waypointsList.clear();
								waypointsList.begin();
								for(int i = 0; i < communicate.missionCount.count; i++) waypointsList.pushBack(temp[i]);
}

void System::Communicate::processMissionCount() {

								mavlink_mission_request_t missionRequest;
								seq = -1;
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

								mavlink_msg_mission_item_decode(&receivedMsg, &missionItem);
								if((int)missionItem.seq - seq > 0) waypointsList.pushBack(missionItem);
								seq = (int)missionItem.seq;

								// All waypoints item received
								if (missionItem.seq == missionCount.count - 1) {
																mavlink_mission_ack_t missionAck;
																missionAck.target_system = 255;
																missionAck.type = MAV_MISSION_ACCEPTED;

																mavlink_msg_mission_ack_encode(SYSTEM_ID, COM_ID, &protocolMsg, &missionAck);
																isTimeoutEnabled = false;

																byte bMissionCount[sizeof(mavlink_mission_count_t)];
																memcpy(bMissionCount, &missionCount, sizeof(mavlink_mission_count_t));
																storage.write(4, bMissionCount, sizeof(mavlink_mission_count_t));

																mavlink_mission_item_t temp[missionCount.count];
																waypointsList.begin();

																for(int i = 0; i < missionCount.count; i++) waypointsList.getElement(temp[i]);
																byte bMissionItem[missionCount.count * sizeof(mavlink_mission_item_t)];
																memcpy(bMissionItem, temp, missionCount.count * sizeof(mavlink_mission_item_t));
																storage.write(8, bMissionItem, missionCount.count * sizeof(mavlink_mission_item_t));
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

								mavlink_msg_mission_request_list_decode(&receivedMsg, &missionRequestList);
								missionCount.target_system = 255;

								mavlink_msg_mission_count_encode(SYSTEM_ID, COM_ID, &protocolMsg, &missionCount);
								isTimeoutEnabled = true;

								sendMessage(protocolMsg);
}

void System::Communicate::processMissionRequest() {

								mavlink_mission_request_t missionRequest;
								mavlink_mission_item_t missionItem;

								mavlink_msg_mission_request_decode(&receivedMsg, &missionRequest);
								waypointsList.getElementAt(missionItem, (int)missionRequest.seq);
								missionItem.target_system = 255;

								mavlink_msg_mission_item_encode(SYSTEM_ID, COM_ID, &protocolMsg, &missionItem);
								isTimeoutEnabled = true;

								sendMessage(protocolMsg);
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

IC_Storage storage;
