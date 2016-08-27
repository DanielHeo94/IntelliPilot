//
//  vars_attitude.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_ATTITUDE_H_
#define _VARS_ATTITUDE_H_

#include <IC_InertialSensor/helper_3dmath.h>

struct sRadDeg3 {
        float radians[3];
        float degrees[3];
};

struct sRawDmp {
        struct sRadDeg3 raw;
        struct sRadDeg3 dmp;
};

struct Attitude_t {
        struct sRawDmp ypr;
        struct sRawDmp euler;
        struct sRawDmp gyro;

        Quaternion quaternion;
        VectorInt16 aa;
        VectorInt16 aaReal;
        VectorInt16 aaWorld;
        VectorFloat gravity;
};

static Attitude_t attitudeBox;

#endif
