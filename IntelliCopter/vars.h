//
//  var.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef vars_h
#define vars_h

// AHRS vars
double angle[3];
double gyro[3];

// LiDAR vars
int lidarAlt;
int lidarVel;

// Barometer vars
float temparature;
float pressure;
float baroAlt;
float sealevelpressure;

// GPS vars
double gpsAlt;
double lat, lng, speed;

// $GPGSA sentence
const char *mode;
int numsats;
const char *pdop, *vdop;
int32_t hdop;
const char *fixtype, *fixquality;

// $GPGSV sentence

const char *nummessages, *messagenum;
const char *numsatsinview;
const char *prn, *snr;
const char *elevation, *azimuth;

// $GPGSV sentence
double cog;
const char *timestamp;


unsigned long _age, date, chars;
unsigned short sentences, failed;

int year;
byte month, day, hour, minute, second, hundredths;
unsigned long __age;

// Compass vars
float heading, headingDegrees, declinationAngle = 0;

// PID Computation vars
double rollAngleError, rollRateError;
double pitchAngleError, pitchRateError;
double yawRateError;

// Motors vars
int force1, force2, force3, force4;

// Remote Control vars
double cmd[4]; // Pulses signal from the radio.
double manualCmd[4]; // Attitude Commands into manual control.

// Rotor status
int status;
int statusLast; // detect change of status

const double MAX_BATTERY_VOLTAGE = 3.1; // Voltages
double batteryVoltage;					// Voltages
int batteryPercent;						// Min : 0%, Max : 100%
int batteryTimeRemain;					// Minutes

// Mavlink
int mavMode = 0;



#endif /* vars_h */
