#ifndef _AHRS_H_
#define _AHRS_H_

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define AHRS_SERIAL_PIN	1

class ahrs{
public:
	ahrs();

	void begin(int baudrate);
	
	bool stabilize();

	bool getEulerAngles(float *data); //default setting
	bool getQuaternion(float *data);
	bool getGyro(float *data);
	bool getEulerAnglesGyro(float *data, float *data2);

private:
};

#endif /* _AHRS_H_ */