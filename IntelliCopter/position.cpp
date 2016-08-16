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

                __position.latitude = _gps.location.lat();
                __position.longitude = _gps.location.lng();
                __position.cog = _cog.value();

                __gps_info.mode = _mode.value();
                __gps_info.fixtype = _fixtype.value();
                __gps_info.pdop = _pdop.value();
                __gps_info.hdop = _gps.hdop.value();
                __gps_info.vdop = _vdop.value();
                __gps_info.numbers_of_gpgsv_messages = _nummessages.value();
                __gps_info.index_number_of_gpgsv_message = _messagenum.value();
                __gps_info.numbers_of_sats_inview = _numsatsinview.value();
                __gps_info.prn = _prn.value();
                __gps_info.snr = _snr.value();
                __gps_info.elevation = _elevation.value();
                __gps_info.azimuth = _azimuth.value();
                __gps_info.timestamp = _timestamp.value();
                __gps_info.number_of_satellites = _gps.satellites.value();
        }
}
