//
//  sys.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _SYS_H_
#define _SYS_H_

#include <Arduino.h>

#include <FreeRTOS_ARM/src/FreeRTOS_ARM.h>

#include <stdio.h>

#include <IC_GPS/IC_GPS.h>
#include <IC_GPS/parser.h>

#include <IC_InertialSensor/BMP085.h>
#include <IC_InertialSensor/EBIMU9DOFV2.h>
#include <IC_InertialSensor/HMC5883L.h>
#include <IC_InertialSensor/LIDARLite2.h>
#include <IC_InertialSensor/MPU6050.h>

#include <IC_LED/IC_LED.h>
#include <IC_Math/IC_Math.h>
#include <IC_Motors/IC_Motors.h>
#include <IC_PID/IC_PID.h>
#include <IC_Radio/IC_Radio.h>

class sys{
public:

    sys();

    static void config();
    static void start();

private:

    // Sensors
    static void configAhrs();
	  static void configImu();
    static void configLidar();
    static void configBaro();
    static void configComp();
    static void configGps();

    // Communication
    static void configRadio();

	// Motors
	static void startMotors();
};

#endif /* sys_h */
