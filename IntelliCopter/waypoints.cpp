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

				Only Node_t type public variables can be declared in vars_waypoints.h.
*/

void System::Communicate::Waypoints::processCommandInt(const mavlink_message_t &msg) {

}

void System::Communicate::Waypoints::processMissionCount(const mavlink_message_t &msg) {

}

void System::Communicate::Waypoints::processMissionItemInt(const mavlink_message_t &msg) {

}

void System::Communicate::Waypoints::processMissionRequestList(const mavlink_message_t &msg) {

}

void System::Communicate::Waypoints::processMissionRequestInt(const mavlink_message_t &msg) {

}

void System::Communicate::Waypoints::processMissionAck(const mavlink_message_t &msg) {

}

void System::Communicate::Waypoints::processMissionClearAll(const mavlink_message_t &msg) {

}
