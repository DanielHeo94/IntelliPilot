//
//  EBIMU9DOFV2.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _EBIMU9DOFV2_H_
#define _EBIMU9DOFV2_H_

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define AHRS_SERIAL_PIN 1

class EBIMU9DOFV2 {
public:
								EBIMU9DOFV2();

								void begin(int baudrate);

								bool stabilize();

								bool getEulerAngles(double *data); //default setting
								bool getQuaternion(double *data);
								bool getGyro(double *data);
								bool getEulerAnglesGyro(double *data, double *data2);

private:
};

#endif
