//
//  vars_pid.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_PID_H_
#define _VARS_PID_H_

struct sAngSpd {
        double angle;
        double speed;
};

typedef struct PidError_t {
        struct sAngSpd yaw;
        struct sAngSpd pitch;
        struct sAngSpd roll;
} PidError_t;

static PidError_t pidErrorBox;

#endif
