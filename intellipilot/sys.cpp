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

#include "radio.h"

#include "motors.h"

extern ahrs _ahrs;
extern baro _barometer;
extern lidar _lidar;
extern comp _compass;
extern gps _gps;

extern radio _radio;

extern motors _motors;

scheduler _scheduler;

sys::sys() {}

void sys::config() {
    
    Serial.begin(115200);
	Serial.flush();

	Serial.println(" ______   ______   ______   ______   ______   ______   ______   ______   ______ ");
	Serial.println("/_____/  /_____/  /_____/  /_____/  /_____/  /_____/  /_____/  /_____/  /_____/ ");
	Serial.println("._. .___        __         .__  .__  .____________.__.__          __    ._.     ");
	Serial.println("| | |   | _____/  |_  ____ |  | |  | |__\______   \__|  |   _____/  |_  | |     ");
	Serial.println("|_| |   |/    \   __\/ __ \|  | |  | |  ||     ___/  |  |  /  _ \   __\ |_|     ");
	Serial.println("|-| |   |   |  \  | \  ___/|  |_|  |_|  ||    |   |  |  |_(  <_> )  |   |-|     ");
	Serial.println("| | |___|___|  /__|  \___  >____/____/__||____|   |__|____/\____/|__|   | |     ");
	Serial.println("|_|          \/          \/                                             |_|     ");
	Serial.println(" ______   ______   ______   ______   ______   ______   ______   ______   ______ ");
	Serial.println("/_____/  /_____/  /_____/  /_____/  /_____/  /_____/  /_____/  /_____/  /_____/ ");

    Serial.println("System configuration start.");
    
    sys::configAhrs();
    //sys::configComp();
    sys::configLidar();
    //sys::configBaro();
    sys::configGps();
    
    sys::configRadio();
    
    Serial.println("System configuration complete.");
    
    _scheduler.taskCreate();
}

void sys::start() {
    _scheduler.taskStart();
}

void sys::configAhrs() {
    // AHRS
    Serial.println("configuring AHRS...");
    
    _ahrs.begin(115200);
    _ahrs.stabilize();
    
    Serial.println("\t\t\t Success.");
}

void sys::configComp() {
    // Compass
    Serial.println("configuring Compass...");
    
    while (!_compass.begin()) { Serial.println("\t\tCould not find a valid compass sensor, check wiring!"); delay(500); }
    // Set measurement range
    Serial.print("\t\tSet measurement range."); _compass.setRange(HMC5883L_RANGE_1_3GA); Serial.println("\t\t\t Success.");
    // Set measurement mode
    Serial.print("\t\tSet measurement mode."); _compass.setMeasurementMode(HMC5883L_CONTINOUS); Serial.println("\t\t\t Success.");
    // Set data rate
    Serial.print("\t\tSet data rate."); _compass.setDataRate(HMC5883L_DATARATE_30HZ); Serial.println("\t\t\t\t Success.");
    // Set number of samples averaged
    Serial.print("\t\tSet number of samples averaged."); _compass.setSamples(HMC5883L_SAMPLES_8); Serial.println("\t\t Success.");
    // Set calibration offset. See HMC5883L_calibration.ino
    Serial.print("\t\tSet calibration offset."); _compass.setOffset(0, 0); Serial.println("\t\t\t Success.");
}

void sys::configLidar() {
    // LiDAR
    Serial.println("configuring LiDAR...");
    
    _lidar.begin();
}

void sys::configBaro() {
    // Barometer
    Serial.println("configuring Barometer...");
    
    if (!_barometer.begin()) while (1) {}
}

void sys::configGps() {
    // GPS
    Serial.println("configuring GPS...");
    
    _gps.begin();
    _gps.configure();
}

void sys::configRadio() {
    // Radio
    Serial.println("configuring Radio...");
    
    _radio.begin();
}

void sys::startMotors() {
	Serial.println("Starting Motors...");
	_motors.start();
}