//
//  gcs_mavlink.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Being modified by Minchan Kim since 2016. 8. 30.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"
#include "vars/vars_waypoints.h"

void System::Setup::gcs_mavlink() {

        Serial3.begin(COMMUNICATE_GCS_WIRELESS_BAUDRATE);
        Serial3.flush();
}

void System::Communicate::transferMsgToGcs(void *arg) {

        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xWakePeriod = FREQUENCY_TASK_COMM_GCS;

        for(;; ) {
                mavlink_msg_heartbeat_pack(SYSTEM_ID, COM_ID, &_heartbeat_msg, TYPE, AUTOPILOT_TYPE, (subscribe.status())->flightMode, CUSTOM_MODE, SYSTEM_STATE);
                mavlink_msg_attitude_pack(SYSTEM_ID, COM_ID, &_attitude_msg, 0,
                                          (subscribe.attitude())->ypr.dmp.radians[2],
                                          (subscribe.attitude())->ypr.dmp.radians[1],
                                          (subscribe.attitude())->ypr.dmp.radians[0],
                                          (subscribe.attitude())->gyro.dmp.radians[0],
                                          (subscribe.attitude())->gyro.dmp.radians[1],
                                          (subscribe.attitude())->gyro.dmp.radians[2]);
                mavlink_msg_gps_raw_int_pack(SYSTEM_ID, COM_ID, &_gps_pos_msg, 0, 2,
                                             (subscribe.position())->latitude * pow(10, 7),
                                             (subscribe.position())->longitude * pow(10, 7),
                                             (subscribe.altitude())->gps * 1000,
                                             (subscribe.gpsInfo())->hdop, UINT16_MAX,
                                             (subscribe.position())->speed * 100,
                                             (int)((subscribe.position())->cog) * 100,
                                             (subscribe.gpsInfo())->number_of_satellites);
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

                vTaskDelayUntil(&xLastWakeTime, xWakePeriod);;
        }
}

void System::Communicate::receiveMsgFromGcs(void* arg) {

        Process P;
        mavlink_message_t msg;
        mavlink_status_t msg_status;

        for(;; ) {
                while (Serial3.available())
                {
                        if (mavlink_parse_char(MAVLINK_COMM_0, (char)Serial3.read(), &msg, &msg_status))
                        {
                                switch (msg.msgid)
                                {
                                case MAVLINK_MSG_ID_COMMAND_INT:
                                        P.processCommandInt(msg);
                                        break;

                                case MAVLINK_MSG_ID_MISSION_COUNT:
                                        P.processMissionCount(msg);
                                        break;

                                case MAVLINK_MSG_ID_MISSION_ITEM_INT:
                                        P.processMissionItemInt(msg);
                                        break;

                                case MAVLINK_MSG_ID_MISSION_REQUEST_LIST:
                                        P.processMissionRequestList(msg);
                                        break;

                                case MAVLINK_MSG_ID_MISSION_REQUEST_INT:
                                        P.processMissionRequestInt(msg);
                                        break;

                                case MAVLINK_MSG_ID_MISSION_ACK:
                                        P.processMissionAck(msg);
                                        break;

                                case MAVLINK_MSG_ID_MISSION_CLEAR_ALL:
                                        P.processMissionClearAll(msg);
                                        break;

                                default:
                                        break;
                                }
                        }
                }
        }
}
