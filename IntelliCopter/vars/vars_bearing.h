//
//  vars_bearing.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_BEARING_H_
#define _VARS_BEARING_H_

struct sRadDeg {
        double radians;
        double degrees;
};

typedef struct Bearing_t {
        struct sRadDeg heading;
        double declinationAngle;
} Bearing_t;

static Bearing_t bearingBox;

#endif
