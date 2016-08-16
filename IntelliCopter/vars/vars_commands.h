//
//  vars_commands.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_COMMANDS_H_
#define _VARS_COMMANDS_H_

typedef struct _commands {

  double raw[5];
  double manual[5];
} Commands;

extern Commands __commands;

#endif
