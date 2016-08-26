//
//  position.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

void System::Setup::position() {

        gps_setup.begin();
        gps_setup.configure();
}

void System::Publish::position(void *arg) {

        gpsCustom _mode(_gps, "GPGSA", 1);
        gpsCustom _fixtype(_gps, "GPGSA", 2);
        gpsCustom _pdop(_gps, "GPGSA", 15);
        gpsCustom _vdop(_gps, "GPGSA", 17);

        gpsCustom _nummessages(_gps, "GPGSV", 1);
        gpsCustom _messagenum(_gps, "GPGSV", 2);
        gpsCustom _numsatsinview(_gps, "GPGSV", 3);
        gpsCustom _prn(_gps, "GPGSV", 4);
        gpsCustom _elevation(_gps, "GPGSV", 5);
        gpsCustom _azimuth(_gps, "GPGSV", 6);
        gpsCustom _snr(_gps, "GPGSV", 7);

        gpsCustom _cog(_gps, "GPRMC", 8);
        gpsCustom _timestamp(_gps, "GPRMC", 1);

        for (;; ) {
                unsigned long start = millis();
                do
                {
                        while (Serial2.available())
                                _gps.encode(Serial2.read());
                } while (millis() - start < 100);

                positionBox.latitude = _gps.location.lat();
                positionBox.longitude = _gps.location.lng();
                positionBox.cog = _cog.value();

                gpsInfoBox.mode = _mode.value();
                gpsInfoBox.fixtype = _fixtype.value();
                gpsInfoBox.pdop = _pdop.value();
                gpsInfoBox.hdop = _gps.hdop.value();
                gpsInfoBox.vdop = _vdop.value();
                gpsInfoBox.numbers_of_gpgsv_messages = _nummessages.value();
                gpsInfoBox.index_number_of_gpgsv_message = _messagenum.value();
                gpsInfoBox.numbers_of_sats_inview = _numsatsinview.value();
                gpsInfoBox.prn = _prn.value();
                gpsInfoBox.snr = _snr.value();
                gpsInfoBox.elevation = _elevation.value();
                gpsInfoBox.azimuth = _azimuth.value();
                gpsInfoBox.timestamp = _timestamp.value();
                gpsInfoBox.number_of_satellites = _gps.satellites.value();
        }
}
