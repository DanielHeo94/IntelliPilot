//
//  vars_gps.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _VARS_GPS_H_
#define _VARS_GPS_H_

typedef struct GpsInfo_t {
        const char *timestamp;
        const char *mode;
        const char *pdop;
        const char *vdop;
        const char *fixtype;
        const char *numbers_of_gpgsv_messages;
        const char *index_number_of_gpgsv_message;
        const char *numbers_of_sats_inview;
        const char *prn;
        const char *snr;
        const char *elevation;
        const char *azimuth;

        int number_of_satellites;
        int32_t hdop;

        unsigned long age, date, chars;
        unsigned short sentences, failed;
} GpsInfo_t;

static GpsInfo_t gpsInfoBox;

#endif
