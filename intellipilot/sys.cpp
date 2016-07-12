//
//  sys.cpp
//  intellipilot
//
//  Created by 허진수 on 2016. 7. 12..
//  Copyright © 2016년 dronix.kr. All rights reserved.
//

#include "sys.h"
#include "scheduler.h"

#include "ahrs.h"
#include "baro.h"
#include "lidar.h"
#include "comp.h"
#include "gps.h"

extern ahrs _ahrs;
extern baro _barometer;
extern lidar _lidar;
extern comp _compass;
extern gps _gps;

scheduler _scheduler;

sys::sys() {}

void sys::config() {
    
    Serial.begin(115200);
    Serial.println("System configuration start.");
    
    sys::configAhrs();
    sys::configComp();
    sys::configLidar();
    sys::configBaro();
    sys::configGps();
    
    Serial.println("System configuration complete.");
    
    _scheduler.taskCreate();
}

void sys::start() {
    _scheduler.taskStart();
}

void sys::configAhrs() {
    // AHRS
    Serial.println("configuring AHRS.");
    
    _ahrs.begin(115200);
    _ahrs.stabilize();
    
    Serial.println("\t\t Success.");
}

void sys::configComp() {
    // Compass
    Serial.print("configuring Compass...");
    
    while (!_compass.begin()) { delay(500); }
    // Set measurement range
    _compass.setRange(HMC5883L_RANGE_1_3GA);
    // Set measurement mode
    _compass.setMeasurementMode(HMC5883L_CONTINOUS);
    // Set data rate
    _compass.setDataRate(HMC5883L_DATARATE_30HZ);
    // Set number of samples averaged
    _compass.setSamples(HMC5883L_SAMPLES_8);
    // Set calibration offset. See HMC5883L_calibration.ino
    _compass.setOffset(0, 0);
    
    Serial.println("\t\t Success.");
}

void sys::configLidar() {
    // LiDAR
    Serial.print("configuring LiDAR...");
    
    _lidar.begin();
    
    Serial.println("\t\t Success.");
}

void sys::configBaro() {
    // Barometer
    Serial.print("configuring Barometer...");
    
    if (!_barometer.begin()) while (1) {}
    
    Serial.println("\t Success.");
}

void sys::configGps() {
    // GPS
    Serial.println("configuring GPS...");
    
    _gps.begin();
    _gps.configure();
}