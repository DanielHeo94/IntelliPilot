//
//  vars.h
//  intellipilot
//
//  Created by 허진수 on 2016. 7. 11..
//  Copyright © 2016년 dronix.kr. All rights reserved.
//

#ifndef vars_h
#define vars_h

// AHRS vars
double angle[3] = { 0, };
double gyro[3] = { 0, };

// LiDAR vars
double lidarAlt = 0;
double lidarVel = 0;

// Barometer vars
float temparature = 0;
float pressure = 0;
float baroAlt = 0;
float sealevelpressure = 0;

// GPS vars
float gpsAlt = 0;
float latitude, longitude, speed = 0;

int numsats, hdop = 0;

unsigned long _age, date, chars = 0;
unsigned short sentences, failed = 0;

int year;
byte month, day, hour, minute, second, hundredths;
unsigned long __age;

// Compass vars
float heading, headingDegrees, declinationAngle = 0;

// PID Computation vars
double pitchAngleError, pitchRateError = 0;
double rollAngleError, rollRateError = 0;
double yawRateError = 0;
double altRateError = 0;

// Motors vars
int force1, force2, force3, force4 = 0;



// Remote Control vars
double cmd[4] = { 0, }; // Roll, Pitch, Yaw, Throttle Commands.



#endif /* vars_h */
