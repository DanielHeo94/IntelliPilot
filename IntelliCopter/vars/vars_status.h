//
//  vars_status.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_STATUS_H_
#define _VARS_STATUS_H_

struct Status_t {
        bool flightReady;
        int flightMode;
};

extern Status_t statusBox;

#endif
