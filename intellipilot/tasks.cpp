//
//  tasks.cpp
//  intellipilot
//
//  Created by 허진수 on 2016. 7. 11..
//  Copyright © 2016년 dronix.kr. All rights reserved.
//

#include <Wire.h>

#include "tasks.h"
#include "vars.h"
#include "gains.h"

#include "math.h"

#include "ahrs.h"
#include "baro.h"
#include "comp.h"
#include "gps.h"
#include "lidar.h"
#include "motors.h"
#include "nmea.h"
#include "pid.h"
#include "radio.h"

math _math;

ahrs _ahrs;
baro _barometer;
comp _compass;
gps _gps;
lidar _lidar;
motors _motors;
nmea _nmea;
radio _radio;

pid rollAngleReg(&angle[0], &rollAngleError, &cmd[0], ROLL_OUTER_P_GAIN, 0, 0, DIRECT);
pid rollRateReg(&gyro[0], &rollRateError, &rollAngleError,
                ROLL_INNER_P_GAIN,
                ROLL_INNER_I_GAIN,
                ROLL_INNER_D_GAIN,
                DIRECT);

pid pitchAngleReg(&angle[1], &pitchAngleError, &cmd[1], PITCH_OUTER_P_GAIN, 0, 0, REVERSE);
pid pitchRateReg(&gyro[1], &pitchRateError, &pitchAngleError,
                PITCH_INNER_P_GAIN,
                PITCH_INNER_I_GAIN,
                PITCH_INNER_D_GAIN,
                REVERSE);

pid yawRateReg(&angle[2], &yawRateError, &cmd[2],
               YAW_P_GAIN,
               YAW_I_GAIN,
               YAW_D_GAIN,
               DIRECT);

tasks::tasks() {}

void tasks::getEulerAnglesGyroThread( void* arg ){
    for(;;) {
        while(!_ahrs.getEulerAnglesGyro(angle, gyro)){
            /*
             *  Waiting for data ready
             */
        }
    }
}

void tasks::getAccAltThread( void* arg ){
    for(;;) {
        lidarAlt = _lidar.getVerDistance();
        //lidarVel = _lidar.getVerVelocity();
    }
}

void tasks::getCommandsThread( void* arg ){
    for(;;) {
        _radio.getCommands(cmd);
        Serial.println(cmd[0]);
    }
}

void tasks::getPosThread( void* arg ) {
    for(;;) {
        unsigned long start = millis();
        do
        {
            while (Serial2.available())
                _nmea.encode(Serial2.read());
        } while (millis() - start < 100);
        
        if( (numsats = (int) _nmea.satellites()) == nmea::GPS_INVALID_SATELLITES ) {}
        if( (hdop = (int) _nmea.hdop()) == nmea::GPS_INVALID_HDOP ) {}
        _nmea.f_get_position( &latitude, &longitude, &_age);
        if(latitude == nmea::GPS_INVALID_F_ANGLE) {}
        if(longitude == nmea::GPS_INVALID_F_ANGLE) {}
        if(_age == nmea::GPS_INVALID_AGE) {}
        _nmea.crack_datetime( &year, &month, &day, &hour, &minute, &second, &hundredths, &__age);
        if( __age == nmea::GPS_INVALID_AGE) {}
        if( (gpsAlt = _nmea.f_altitude()) == nmea::GPS_INVALID_ALTITUDE) {}
        if( (speed = _nmea.f_speed_kmph()) == nmea::GPS_INVALID_F_SPEED) {}

        }
}

void tasks::getBaroThread( void* arg ) {
    for(;;) {
        temparature = _barometer.readTemperature();
        pressure = _barometer.readPressure();
        baroAlt = _barometer.readAltitude();
        sealevelpressure = _barometer.readSealevelPressure();
    }
}

void tasks::getCompThread( void* arg ) {
    for(;;) {
        Vector norm = _compass.readNormalize();
        
        heading = atan2(norm.YAxis, norm.XAxis);
        
        declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
        heading += declinationAngle;
        
        if (heading < 0)
        {
            heading += 2 * PI;
        }
        
        if (heading > 2 * PI)
        {
            heading -= 2 * PI;
        }
        
        headingDegrees = _math.radiantodegree(heading);
    }
}

void tasks::manualControlThread( void* arg ) {
    
    rollAngleReg.Compute();
    rollRateReg.Compute();
    
    pitchAngleReg.Compute();
    pitchRateReg.Compute();
    
    yawRateReg.Compute();
    
    force1 = (-pitchRateError + rollRateError) * ( 1 / 2 ) + yawRateError + cmd[3];
    force2 = (-pitchRateError - rollRateError) * ( 1 / 2 ) - yawRateError + cmd[3];
    force3 = (pitchRateError - rollRateError) * ( 1 / 2 ) + yawRateError + cmd[3];
    force4 = (pitchRateError + rollRateError) * ( 1 / 2 ) - yawRateError + cmd[3];
}


