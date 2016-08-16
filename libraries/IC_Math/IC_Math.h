//
//  IC_Math.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _IC_MATH_H_
#define _IC_MATH_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

class IC_Math {
public:
								IC_Math();

								void limit(float *val, float min, float max);
								void limitInt(int *val, int min, int max);

								float degreetoradian(float val);
								float radiantodegree(float val);

								float floor(float val);
};

#endif
