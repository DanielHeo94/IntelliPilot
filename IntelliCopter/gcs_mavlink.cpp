//
//  gcs_mavlink.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

void System::Communicate::gcs_mavlink(void *arg) {

        for(;; ) {
                mavlink_msg_heartbeat_pack(SYSTEM_ID, COM_ID, &_heartbeat_msg, TYPE, AUTOPILOT_TYPE, (subscribe.status())->flight_mode, CUSTOM_MODE, SYSTEM_STATE);
                mavlink_msg_attitude_pack(SYSTEM_ID, COM_ID, &_attitude_msg, 0, (subscribe.attitude())->ypr.dmp.radians[2], (subscribe.attitude())->ypr.dmp.radians[1], (subscribe.attitude())->ypr.dmp.radians[0], (subscribe.attitude())->gyro.dmp.degrees[0], (subscribe.attitude())->gyro.dmp.degrees[1], (subscribe.attitude())->gyro.dmp.degrees[2]);
                mavlink_msg_gps_raw_int_pack(SYSTEM_ID, COM_ID, &_gps_pos_msg, 0, 2, (subscribe.position())->latitude * pow(10, 7), (subscribe.position())->longitude * pow(10, 7), (subscribe.altitude())->gps * 1000, (subscribe.gps_info())->hdop, UINT16_MAX, (subscribe.position())->speed * 100, (int)((subscribe.position())->cog) * 100, (subscribe.gps_info())->number_of_satellites);
                //mavlink_msg_gps_status_pack(SYSTEM_ID, COM_ID, &_gps_stat_msg, numsatsinview, prn, 1, elevation, (255 * azimuth) / 360, snr);
                //mavlink_msg_battery_status_pack(SYSTEM_ID, COM_ID, &_bat_stat_msg, BATTERY_ID, 0, 0, INT16_MAX, 0, -1, -1, -1, batteryPercent);

                _heartbeat_len = mavlink_msg_to_send_buffer(_heartbeat_buf, &_heartbeat_msg);
                _attitude_len = mavlink_msg_to_send_buffer(_attitude_buf, &_attitude_msg);
                _gps_pos_len = mavlink_msg_to_send_buffer(_gps_pos_buf, &_gps_pos_msg);
                //_gps_stat_len = mavlink_msg_to_send_buffer(_gps_stat_buf, &_gps_stat_msg);
                //_bat_stat_len = mavlink_msg_to_send_buffer(_bat_stat_buf, &_bat_stat_msg);

    #if (COMMUNICATE_GCS_WIRE == 1)
                Serial.write(_heartbeat_buf, _heartbeat_len);
                Serial.write(_attitude_buf, _attitude_len);
                Serial.write(_gps_pos_buf, _gps_pos_len);
                //Serial.write(_gps_stat_buf, _gps_stat_len);
                //Serial.write(_bat_stat_buf, _bat_stat_len);
    #endif

    #if (COMMUNICATE_GCS_WIRELESS == 1)
                Serial3.write(_heartbeat_buf, _heartbeat_len);
                Serial3.write(_attitude_buf, _attitude_len);
                Serial3.write(_gps_pos_buf, _gps_pos_len);
                //Serial3.write(_gps_stat_buf, _gps_stat_len);
                //Serial3.write(_bat_stat_buf, _bat_stat_len);
    #endif

                vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
        }
}
