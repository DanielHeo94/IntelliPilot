//
//  sys.hpp
//  intellipilot
//
//  Created by 허진수 on 2016. 7. 12..
//  Copyright © 2016년 dronix.kr. All rights reserved.
//

#ifndef _SYS_H_
#define _SYS_H_

#include <stdio.h>

class sys{
public:
    
    sys();
    
    static void config();
    static void start();
    
private:
    
    // Sensors & Actuator
    static void configAhrs();
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
