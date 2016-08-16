//
//  altitude.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//
#include "System.h"

void System::Setup::altitude() {

        lidarlitev2.begin();
        if (!bmp085.begin()) while (1) {}
}

void System::Publish::altitude(void *arg) {

        for(;; ) {

                __altitude.lidar = lidarlitev2.getVerDistance();
                __altitude.barometer = bmp085.readAltitude();
                __altitude.gps = _gps.altitude.meters();

                __barometer.temparature = bmp085.readTemperature();
                __barometer.pressure = bmp085.readPressure();
                __barometer.sealevelpressure = bmp085.readSealevelPressure();
        }
}
