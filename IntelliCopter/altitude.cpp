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

                altitudeBox.lidar = lidarlitev2.getVerDistance();
                altitudeBox.barometer = bmp085.readAltitude();
                altitudeBox.gps = _gps.altitude.meters();

                barometerBox.temparature = bmp085.readTemperature();
                barometerBox.pressure = bmp085.readPressure();
                barometerBox.sealevelpressure = bmp085.readSealevelPressure();
        }
}

LIDARLiteV2 lidarlitev2;
BMP085 bmp085;
gps _gps;
