//
//  vars_commands.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_COMMANDS_H_
#define _VARS_COMMANDS_H_

typedef struct Commands_t {
        double pulseWidth[5];
        double controlManual[5];
} Commands_t;

static Commands_t commandsBox;

#endif
