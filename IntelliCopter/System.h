//
//  System.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//
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

#include <IC_GPS/IC_GPS.h>
#include <IC_GPS/parser.h>

#include <IC_InertialSensor/BMP085.h>
#include <IC_InertialSensor/EBIMU9DOFV2.h>
#include <IC_InertialSensor/HMC5883L.h>
#include <IC_InertialSensor/LIDARLiteV2.h>
#include <IC_InertialSensor/MPU6050_6Axis_MotionApps20.h>

#include <IC_LED/IC_LED.h>
#include <IC_Math/IC_Math.h>
#include <IC_Motors/IC_Motors.h>
#include <IC_Radio/IC_Radio.h>
#include <IC_State/IC_State.h>

#include <GCS_MAVLink/mavlink/common/mavlink.h>
#include <GCS_MAVLink/mavlink/vars.h>

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

#include "scheduler.h"

#include "config.h"
#include "params.h"

IC_GPS gps_setup;
gps _gps;
IC_LED led;
IC_Math math;
IC_Motors motors;
IC_Radio radio;

BMP085 bmp085;
EBIMU9DOFV2 ebimu9dofv2;
HMC5883L hmc5883l;
LIDARLiteV2 lidarlitev2;
MPU6050 mpu6050;

class System {
public:
        System();

        void config();
        void start();

        void task_create();
        void scheduler_start();

        void commands_processing(int event);

        class Setup {
public:
                Setup();

                void commands();
                void attitude();
                void altitude();
                void position();
                void bearing();
        };

        class Publish {
public:
                Publish();

                static void commands(void *arg);
                static void attitude(void *arg);
                static void altitude(void *arg);
                static void position(void *arg);
                static void bearing(void *arg);
        };

        class Subscribe {
public:
                Subscribe();

                Altitude* altitude() {
                        return &__altitude;
                }
                Attitude* attitude() {
                        return &__attitude;
                }
                Barometer* barometer() {
                        return &__barometer;
                }
                Battery* battery() {
                        return &__battery;
                }
                Bearing* bearing() {
                        return &__bearing;
                }
                Commands* commands() {
                        return &__commands;
                }
                GPS_info* gps_info() {
                        return &__gps_info;
                }
                Servo_output* servo_output() {
                        return &__servo_output;
                }
                PID_error* pid_error() {
                        return &__pid_error;
                }
                Position* position() {
                        return &__position;
                }
                Status* status() {
                        return &__status;
                }

        };

        class Communicate {
public:
                Communicate();

                static void gcs_mavlink(void *arg);
                static void led_indicator(void *arg);
                static void battery_indicator(void *arg);
        };

        class Control {
public:
                Control();

                static void control_pre_flight(void *arg);
                static void control_manual(void *arg);
        };

private:
};

System copter;
System::Setup _setup;
System::Publish publish;
System::Subscribe subscribe;
System::Communicate communicate;
System::Control control;

#endif
