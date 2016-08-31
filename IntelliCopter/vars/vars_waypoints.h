//
//  vars_waypoints.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_VARIABLES_H_
#define _VARS_VARIABLES_H_

#include <IC_DataStructure/List.h>
#include <GCS_MAVLink/mavlink/common/mavlink.h>

list<mavlink_mission_item_int_t> waypointsList;

#endif
