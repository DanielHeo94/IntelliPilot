//
//  vars_altitude.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_ALTITUDE_H_
#define _VARS_ALTITUDE_H_

typedef struct _altitude {

  double lidar;     // centimeters
  double barometer; // meters
  double gps;       // meters
} Altitude;

extern Altitude __altitude;

#endif
