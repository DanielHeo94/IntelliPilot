//
//  config.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://droix.kr. All rights reserved.
//

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define TASK_GET_ATTITUDE           1
#define TASK_GET_ALTITUDE           1
#define TASK_GET_BEARING            1
#define TASK_GET_POSITI0N           1
#define TASK_GET_COMMANDS           1
#define TASK_COMM_GCS               1
#define TASK_FC_IDLE                1
#define TASK_FC_MANUAL_C0TROL       1
#define TASK_LED_INDICATOR          1
#define TASK_BATTERY_CHECK          1

#define DEBUG_RADIO_COMMAND         0
#define DEBUG_READABLE_QUATERNION   0
#define DEBUG_READABLE_EULER        0
#define DEBUG_READABLE_YAWPITCHROLL 1
#define DEBUG_BATTERY               0

#define COMMUNICATE_GCS_WIRE        0
#define COMMUNICATE_GCS_WIRELESS    1

#endif
