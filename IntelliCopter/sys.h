//
//  sys.hpp
//  intellipilot
//
//  Created by 허진수 on 2016. 7. 12..
//  Copyright © 2016년 dronix.kr. All rights reserved.
//

#ifndef _SYS_H_
#define _SYS_H_

#include <Arduino.h>

#include <stdio.h>

#include <IC_GPS/gps.h>
#include <IC_GPS/nmea.h>

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
