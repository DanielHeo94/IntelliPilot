//
//  vars_battery.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_BATTERY_H_
#define _VARS_BATTERY_H_

const double MAX_BATTERY_VOLTAGE = 3.1;
const double MIN_BATTERY_VOLTAGE = 2.7;

typedef struct unit {

  int percents;
  int minutes;
} Unit;

typedef struct _battery {

  double voltages;
  Unit remain;
} Battery;

static Battery __battery;

#endif
