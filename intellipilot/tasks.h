//
//  tasks.hpp
//  intellipilot
//
//  Created by 허진수 on 2016. 7. 11..
//  Copyright © 2016년 dronix.kr. All rights reserved.
//

#ifndef _TASKS_h
#define _TASKS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class tasks{
public:
    
    tasks();
    
    static void getEulerAnglesGyroThread(void* arg);
    static void getHeightThread(void* arg);
    static void getCommandsThread(void* arg);
    static void getBaroThread(void* arg);
    static void getCompThread(void* arg);
    static void getGpsThread(void* arg);
    
    static void computePidThread(void* arg);
    static void runMotorsThread(void* arg);
    
private:
};

#endif
