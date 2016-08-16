//
//  vars_status.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_STATUS_H_
#define _VARS_STATUS_H_

typedef struct _status {

  int temporary, temporary_last;
  int flight_mode;
} Status;

extern Status __status;

#endif
