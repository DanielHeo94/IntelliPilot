//
//  vars_position.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_POSITION_H_
#define _VARS_POSITION_H_

typedef struct _position {
public:
  double latitude;
  double longitude;
  double speed;
  const char *cog;
} Position;

static Position __position;

#endif
