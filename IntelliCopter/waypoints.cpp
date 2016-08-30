//
//  waypoints.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Being modified by Minchan Kim since 2016. 8. 30.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"
#include "vars/vars_waypoints.h"
#include "list.h"

void System::Communicate::Waypoints::read() {

	
}

void System::Communicate::Waypoints::writeWaypoint() {
}

Process::Process(void)
{

}

void Process::processCommandInt(const mavlink_message_t &msg)
{
	mavlink_command_int_t command_int;
	mavlink_msg_command_int_decode(&msg, &command_int);
	switch (command_int.command)
	{
	case MAV_CMD_DO_SET_MODE:
		break;
	}
}

void Process::processMissionCount(const mavlink_message_t &msg)
{
	mavlink_mission_count_t mission_count;
	mavlink_msg_mission_count_decode(&msg, &mission_count);
}

void Process::processMissionItemInt(const mavlink_message_t &msg)
{
	mavlink_mission_item_int_t mission_item_int;
	mavlink_msg_mission_item_int_decode(&msg, &mission_item_int);
}

void Process::processMissionRequestList(const mavlink_message_t &msg)
{
	mavlink_mission_request_list_t mission_request_list;
	mavlink_msg_mission_request_list_decode(&msg, &mission_request_list);
}

void Process::processMissionRequestInt(const mavlink_message_t &msg)
{
	mavlink_mission_request_int_t mission_request_int;
	mavlink_msg_mission_request_int_decode(&msg, &mission_request_int);
}

void Process::processMissionAck(const mavlink_message_t &msg)
{
	mavlink_mission_ack_t mission_ack;
	mavlink_msg_mission_ack_decode(&msg, &mission_ack);
}

void Process::processMissionClearAll(const mavlink_message_t &msg)
{
	mavlink_mission_clear_all_t mission_clear_all;
	mavlink_msg_mission_clear_all_decode(&msg, &mission_clear_all);
}

void Process::sendMessage(const mavlink_message_t &msg)
{
	uint16_t length = mavlink_msg_to_send_buffer(buf, &msg);
	Serial3.write(buf, length);
}

Process::~Process()
{

}