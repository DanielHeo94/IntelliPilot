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

struct sPerMin {
        int percents;
        int minutes;
};

typedef struct Battery_t {
        double voltages;
        struct sPerMin remain;
} Battery_t;

static Battery_t batteryBox;

#endif
