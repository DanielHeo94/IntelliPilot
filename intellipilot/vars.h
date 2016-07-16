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
int lidarAlt = 0;
int lidarVel = 0;

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
float PITCH_ANG_ERR, PITCH_RATE_ERR = 0;
float PITCH_RATE_ERR_PRE = 0;
float PITCH_RATE_ERR_P, PITCH_RATE_ERR_I = 0;
float PITCH_RATE_ERR_D = 0;
float PITCH_BALANCE = 0;

float ROLL_ANG_ERR, ROLL_RATE_ERR = 0;
float ROLL_RATE_ERR_PRE = 0;
float ROLL_RATE_ERR_P, ROLL_RATE_ERR_I = 0;
float ROLL_RATE_ERR_D = 0;
float ROLL_BALANCE = 0;

float YAW_RATE_ERR, YAW_RATE_ERR_P = 0;
float YAW_RATE_ERR_I = 0;
float YAW_TORQUE = 0;

// Motors vars
int force1, force2, force3, force4 = 0;

// Remote Control vars
double cmd[4] = { 0, }; // Roll, Pitch, Yaw, Throttle Commands.

// Rotor status
int status = 0;
int statusLast = 0;



#endif /* vars_h */
