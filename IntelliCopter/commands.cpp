//
//  commands.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

void System::Setup::commands() {
        radio.begin();
}

void System::Publish::commands(void *arg) {

        for(;; ) {

                __status.temporary = radio.getCommands(__commands.raw);

  #if (DEBUG_RADIO_COMMAND_ON == 1)
                Serial.print(__commands.raw[0]);
                Serial.print("\t");
                Serial.print(__commands.raw[1]);
                Serial.print("\t");
                Serial.print(__commands.raw[2]);
                Serial.print("\t");
                Serial.println(__commands.raw[3]);
  #endif

                if ((__status.temporary - __status.temporary_last) != 0) copter.commands_processing(__status.temporary);
                __status.temporary_last = __status.temporary;
        }
}

void System::commands_processing(int event) {

        switch (event)
        {
        case STATE_PREFLIGHT_ARMED:
                __status.flight_mode = MAV_MODE_PREFLIGHT;
                break;

        case STATE_PREFLIGHT_DISARMED:
                __status.flight_mode = MAV_MODE_PREFLIGHT;
                vTaskResume(task_flight_control_pre_flight);
                vTaskSuspend(task_flight_control_manual);
                break;

        case STATE_MANUAL_ARMED:
                __status.flight_mode = MAV_MODE_MANUAL_ARMED;
                vTaskSuspend(task_flight_control_pre_flight);
                vTaskResume(task_flight_control_manual);
                break;

        case STATE_HOLD_ARMED:
                __status.flight_mode = MAV_MODE_GUIDED_ARMED;
                vTaskResume(task_flight_control_pre_flight);
                vTaskSuspend(task_flight_control_manual);
                break;

        case STATE_AUTO_ARMED:
                __status.flight_mode = MAV_MODE_AUTO_ARMED;
                vTaskResume(task_flight_control_pre_flight);
                vTaskSuspend(task_flight_control_manual);
                break;

        default:
                break;

        }
}
