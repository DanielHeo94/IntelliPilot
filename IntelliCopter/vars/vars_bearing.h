//
//  vars_bearing.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_BEARING_H_
#define _VARS_BEARING_H_

typedef struct type {

  double radians;
  double degrees;
} Type;

typedef struct _bearing {

  Type heading;
  double declination_angle;
} Bearing;

static Bearing __bearing;

#endif
