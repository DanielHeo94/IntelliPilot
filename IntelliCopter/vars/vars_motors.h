//
//  vars_motors.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_MOTORS_H_
#define _VARS_MOTORS_H_

typedef struct ServoOutput_t {
        int a, b, c, d;
} ServoOutput_t;

static ServoOutput_t servoOutputBox;

#endif
