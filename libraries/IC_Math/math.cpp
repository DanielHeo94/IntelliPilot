// 
// 
// 

#include "math.h"

math::math() { }

void math::limit(float *val, float min, float max)
{
	if (*val < min) *val = min;
	else if (*val > max) *val = max;
}

void math::limitInt(int *val, int min, int max)
{
	if (*val < min) *val = min;
	else if (*val > max) *val = max;
}

float math::degreetoradian(float val) {
	return val * M_PI / 180;
}

float math::radiantodegree(float val) {
	return val * 180 / M_PI;
}

float math::floor(float val) {
	return (float) ((int)val);
}