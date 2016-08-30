//
//  vars_waypoints.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_VARIABLES_H_
#define _VARS_VARIABLES_H_

class Process
{
public:
								Process(void);
								void processCommandInt(const mavlink_message_t &msg);
								// Write MAV Waypoint list
								void processMissionCount(const mavlink_message_t &msg);
								void processMissionItemInt(const mavlink_message_t &msg);
								// Read MAV Waypoint list
								void processMissionRequestList(const mavlink_message_t &msg);
								void processMissionRequestInt(const mavlink_message_t &msg);
								void processMissionAck(const mavlink_message_t &msg);
								// Clear MAV Waypoint list
								void processMissionClearAll(const mavlink_message_t &msg);
								~Process();
private:
								void sendMessage(const mavlink_message_t &msg);
								int state;
								int count;
								uint8_t buf[MAVLINK_MAX_PACKET_LEN];
};

#endif
