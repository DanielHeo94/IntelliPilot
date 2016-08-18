//
//  scheduler.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#pragma once

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <FreeRTOS_ARM.h>

static TaskHandle_t task_flight_control_pre_flight;
static TaskHandle_t task_flight_control_manual;
static TaskHandle_t task_flight_control_pos_hold;

#endif
