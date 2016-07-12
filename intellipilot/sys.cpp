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

ahrs _ahrs;
baro _baro;
lidar _lidar;
comp _comp;
gps _gps;

scheduler _scheduler;

sys::sys() {}

void sys::config() {
    
    sys::configAhrs();
    sys::configComp();
    sys::configLidar();
    sys::configBaro();
    sys::configGps();
}

void sys::start() {
    _scheduler.taskStart();
}

void sys::configAhrs() {
    // AHRS
    _ahrs.begin(115200);
    _ahrs.stabilize();
}

void sys::configComp() {
    // Compass
    while (!_comp.begin()) { delay(500); }
    // Set measurement range
    _comp.setRange(HMC5883L_RANGE_1_3GA);
    // Set measurement mode
    _comp.setMeasurementMode(HMC5883L_CONTINOUS);
    // Set data rate
    _comp.setDataRate(HMC5883L_DATARATE_30HZ);
    // Set number of samples averaged
    _comp.setSamples(HMC5883L_SAMPLES_8);
    // Set calibration offset. See HMC5883L_calibration.ino
    _comp.setOffset(0, 0);
}

void sys::configLidar() {
    // LiDAR
    _lidar.begin();
}

void sys::configBaro() {
    // Barometer
    if (!_baro.begin()) while (1) {}
}

void sys::configGps() {
    // GPS
    _gps.begin();
    _gps.configure();
}