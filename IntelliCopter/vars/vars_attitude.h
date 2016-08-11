//
//  vars_attitude.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_ATTITUDE_H_
#define _VARS_ATTITUDE_H_

typedef struct c {

  float radians[3];
  float degrees[3];
} C;

typedef struct datatype {

  C raw;
  C dmp;
} Datatype;

typedef struct _attitude {

  Datatype ypr;
  Datatype euler;
  Datatype gyro;
} Attitude;

Attitude __attitude;

#endif
