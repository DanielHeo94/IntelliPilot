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

	bool getEulerAngles(double *data); //default setting
	bool getQuaternion(double *data);
	bool getGyro(double *data);
	bool getEulerAnglesGyro(double *data, double *data2);

private:
};

#endif /* _AHRS_H_ */