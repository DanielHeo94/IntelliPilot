//
//  IC_Math.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "IC_Math.h"

IC_Math::IC_Math() {
}

void IC_Math::limit(float *val, float min, float max)
{
								if (*val < min) *val = min;
								else if (*val > max) *val = max;
}

void IC_Math::limitInt(int *val, int min, int max)
{
								if (*val < min) *val = min;
								else if (*val > max) *val = max;
}

float IC_Math::degreetoradian(float val) {
								return val * M_PI / 180;
}

float IC_Math::radiantodegree(float val) {
								return val * 180 / M_PI;
}

float IC_Math::floor(float val) {
								return (float) ((int)val);
}
