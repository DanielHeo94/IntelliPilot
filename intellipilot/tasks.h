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
    
    // Sensors & Actuator
    static void getEulerAnglesGyroThread(void* arg);    // Get roll, pitch, yaw, gyro.
    static void getAccAltThread(void* arg);             // Get highly accurate altitude.
    static void getBaroThread(void* arg);               // Get temparature, pressure, altitude.
    static void getCompThread(void* arg);               // Get heading.
    static void getPosThread(void* arg);                // Get the num of sats, lat, lon, date, hdop, alt.
    
    // Communication
    static void getCommandsThread(void* arg);   // Communication Copter <-> Controller.
    static void commGcsThread(void* arg);       // Communication Copter <-> GCS ( Protocol : mavLink ).
    
    // Commands Processing
    static void procCommands(int event);
    
    // Flight Control

	
	static void idle(void* arg);

    static void takeoffSequenceThread(void* arg);   // Automatic Take-off.
    static void manualControlThread(void* arg);     // Manual.
    static void posHoldControlThread(void* arg);    // GPS Hold.
    static void waypointSequenceThread(void* arg);  // Autonomous Flight.
    static void returnSequenceThread(void* arg);    // Return to home.
    
    // LED indicator
    static void ledIndicatingThread(void* arg);     // Turn on or off as system state changes
    
private:
};

#endif
