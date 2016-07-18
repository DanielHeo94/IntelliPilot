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
float lat, lng, speed = 0;

// $GPGSA sentence
char mode;
int numsats = 0;
int pdop, vdop, hdop = 0;
int fixtype, fixquality = 0;

// $GPGSV sentence

int nummessages, messagenum, numsatsinview = 0;
int prn, snr = 0;
int elevation, azimuth = 0;

// $GPGSV sentence
float cog;
uint64_t timestamp;


unsigned long _age, date, chars = 0;
unsigned short sentences, failed = 0;

int year;
byte month, day, hour, minute, second, hundredths;
unsigned long __age;

// Compass vars
float heading, headingDegrees, declinationAngle = 0;

// PID Computation vars
double rollAngleError, rollRateError = 0;
double pitchAngleError, pitchRateError = 0;
double yawRateError = 0;

// Motors vars
int force1, force2, force3, force4 = 0;

// Remote Control vars
double cmd[4] = { 0, }; // Pulses signal from the radio.
double manualCmd[4] = { 0, }; // Attitude Commands into manual control.

// Rotor status
int status = 0;
int statusLast = 0; // detect change of status

// Mavlink
int mavMode = 0;



#endif /* vars_h */
