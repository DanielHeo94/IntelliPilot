//
//  config.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://droix.kr. All rights reserved.
//

#ifndef _CONFIG_H_
#define _CONFIG_H_

// Turn on/off tasks you need to
#define TASK_GET_ATTITUDE           1 //Warning!!! must NOT be turned off unless you want copter down!
#define TASK_GET_ALTITUDE           0 //Warning!!! must NOT be turned off unless you want copter down!
#define TASK_GET_BEARING            0 //Warning!!! must NOT be turned off unless you want copter down!
#define TASK_GET_POSITION           0
#define TASK_GET_COMMANDS           1
#define TASK_COMM_GCS               0
#define TASK_FC_IDLE                0
#define TASK_FC_MANUAL_CONTROL      0
#define TASK_LED_INDICATOR          0
#define TASK_BATTERY_CHECK          0

#define DEBUG_READABLE_QUATERNION   0
#define DEBUG_READABLE_EULER        0
#define DEBUG_READABLE_YAWPITCHROLL 1
#define DEBUG_RADIO_COMMANDS        1
#define DEBUG_BATTERY               0

#define COMMUNICATE_GCS_WIRE        0
#define COMMUNICATE_GCS_WIRELESS    1

// Frequency of tasks, Unit: Hz
#define FREQUENCY_TASK_GET_ATTITUDE           100
#define FREQUENCY_TASK_GET_ALTITUDE           50
#define FREQUENCY_TASK_GET_BEARING            1
#define FREQUENCY_TASK_GET_POSITION           0
#define FREQUENCY_TASK_GET_COMMANDS           100
#define FREQUENCY_TASK_COMM_GCS               0
#define FREQUENCY_TASK_FC_IDLE                200
#define FREQUENCY_TASK_FC_MANUAL_CONTROL      100
#define FREQUENCY_TASK_LED_INDICATOR          0
#define FREQUENCY_TASK_BATTERY_CHECK          0

#endif
