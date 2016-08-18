//
//  bearing.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//
#include "System.h"

void System::Setup::bearing() {

        while (hmc5883l.begin()) { Serial.println("\t\tCould not find a valid compass sensor, check wiring!"); delay(500); }
        // Set measurement range
        hmc5883l.setRange(HMC5883L_RANGE_1_3GA);
        // Set measurement mode
        hmc5883l.setMeasurementMode(HMC5883L_CONTINOUS);
        // Set data rate
        hmc5883l.setDataRate(HMC5883L_DATARATE_30HZ);
        // Set number of samples averaged
        hmc5883l.setSamples(HMC5883L_SAMPLES_8);
        // Set calibration offset. See HMC5883L_calibration.ino
        hmc5883l.setOffset(0, 0);
}

void System::Publish::bearing(void *arg) {

        for (;; ) {
                Vector norm = hmc5883l.readNormalize();

                __bearing.heading.radians = atan2(norm.YAxis, norm.XAxis);

                __bearing.declination_angle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
                __bearing.heading.radians += __bearing.declination_angle;

                if (__bearing.heading.radians < 0)
                {
                        __bearing.heading.radians += 2 * PI;
                }

                if (__bearing.heading.radians > 2 * PI)
                {
                        __bearing.heading.radians -= 2 * PI;
                }

                __bearing.heading.degrees = math.radiantodegree(__bearing.heading.radians);
        }
}

HMC5883L hmc5883l;
