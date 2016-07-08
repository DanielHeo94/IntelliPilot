// math.h

#ifndef _MATH_h
#define _MATH_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define M_PI 3.14159265358979323846

class math {
public:
	math();

	void limit(float *val, float min, float max);
	void limitInt(int *val, int min, int max);

	float degreetoradian(float val);
	float radiantodegree(float val);

	float floor(float val);
};

#endif

