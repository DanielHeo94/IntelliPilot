//
//  System.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#pragma once

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <FreeRTOS_ARM.h>

#include <stdio.h>

#include <IC_AttitudeControl/IC_AttitudeControl.h>
#include <IC_AttitudeControl/IC_PositionControl.h>

#include <IC_DataStructure/List.h>

#include <IC_GPS/IC_GPS.h>
#include <IC_GPS/parser.h>

#include <IC_InertialSensor/BMP085.h>
#include <IC_InertialSensor/EBIMU9DOFV2.h>
#include <IC_InertialSensor/HMC5883L.h>
#include <IC_InertialSensor/LIDARLiteV2.h>

#include <IC_LED/IC_LED.h>
#include <IC_Math/IC_Math.h>
#include <IC_Motors/IC_Motors.h>
#include <IC_Radio/IC_Radio.h>
#include <IC_State/IC_State.h>

#include <GCS_MAVLink/mavlink/common/mavlink.h>
#include <GCS_MAVLink/mavlink/vars.h>

#include <IC_Storage/DueFlashStorage.h>

#include "vars/vars_altitude.h"
#include "vars/vars_attitude.h"
#include "vars/vars_barometer.h"
#include "vars/vars_battery.h"
#include "vars/vars_bearing.h"
#include "vars/vars_commands.h"
#include "vars/vars_gps.h"
#include "vars/vars_position.h"
#include "vars/vars_motors.h"
#include "vars/vars_pid.h"
#include "vars/vars_status.h"
#include "vars/vars_waypoints.h"

#include "scheduler.h"

#include "config.h"
#include "params.h"

extern IC_GPS gps_setup;
extern gps _gps;
extern IC_LED led;
extern IC_Math math;
extern IC_Motors motors;
extern IC_Radio radio;

extern BMP085 bmp085;
extern EBIMU9DOFV2 ebimu9dofv2;
extern HMC5883L hmc5883l;
extern LIDARLiteV2 lidarlitev2;

class System {
public:
        System();

        void config();
        void start();

        void createTasks();
        void startScheduler();

        class Setup {
public:
                Setup();

                void gcs_mavlink();

                void commands();
                void attitude();
                void altitude();
                void position();
                void bearing();
                void status();
        };

        class Publish {
public:
                Publish();

                static void commands(void *arg);
                static void attitude(void *arg);
                static void altitude(void *arg);
                static void position(void *arg);
                static void bearing(void* arg);
                static void battery(void* arg);
                static void status(void* arg);
        };

        class Subscribe {
public:
                Subscribe();

                Altitude_t* altitude() {
                        return &altitudeBox;
                }
                Attitude_t* attitude() {
                        return &attitudeBox;
                }
                Barometer_t* barometer() {
                        return &barometerBox;
                }
                Battery_t* battery() {
                        return &batteryBox;
                }
                Bearing_t* bearing() {
                        return &bearingBox;
                }
                Commands_t* commands() {
                        return &commandsBox;
                }
                GpsInfo_t* gpsInfo() {
                        return &gpsInfoBox;
                }
                ServoOutput_t* servoOutput() {
                        return &servoOutputBox;
                }
                PidError_t* pidError() {
                        return &pidErrorBox;
                }
                Position_t* position() {
                        return &positionBox;
                }
                Status_t* status() {
                        return &statusBox;
                }

        };

        class Communicate {
public:
                Communicate();

                static void transferMsgToGcs(void* arg);
                static void receiveMsgFromGcs();

                static void showLedIndication(void* arg);
private:
                static bool isTimeoutEnabled;

                static mavlink_mission_count_t missionCount;

                static mavlink_message_t receivedMsg;
                static mavlink_status_t receivedStatus;

                static mavlink_message_t commonMsg;
                static mavlink_message_t protocolMsg;

                uint8_t getParams();
                static void sendMessage(mavlink_message_t &msg);

                void processCommandInt();
                void processMissionCount();
                void processMissionItem();
                void processMissionRequestList();
                void processMissionRequest();
                void processMissionAck();
                void processMissionClearAll();
        };

        class Control {
public:
                Control();

                static void controlPreFlight(void *arg);
                static void controlManual(void *arg);
        };

private:
};

extern System copter;
extern System::Setup _setup;
extern System::Publish publish;
extern System::Subscribe subscribe;
extern System::Communicate communicate;
extern System::Control control;

#endif
