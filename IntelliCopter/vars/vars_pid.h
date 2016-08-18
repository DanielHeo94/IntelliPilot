//
//  vars_pid.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_PID_H_
#define _VARS_PID_H_


typedef struct a {

  double angle;
  double speed;
} A;

typedef struct _pid_error {

  A yaw;
  A pitch;
  A roll;
} PID_error;

static PID_error __pid_error;

#endif
