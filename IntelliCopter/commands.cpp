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

        statusBox.flightMode = MAV_MODE_PREFLIGHT;
}

void System::Publish::commands(void *arg) {
        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xWakePeriod = FREQUENCY_TASK_GET_COMMANDS;

        for(;; ) {
                radio.getCommands(commandsBox.pulseWidth);

                #if (DEBUG_RADIO_COMMANDS == 1)
                Serial.print(subscribe.commands()->pulseWidth[0]);
                Serial.print("\t");
                Serial.print(subscribe.commands()->pulseWidth[1]);
                Serial.print("\t");
                Serial.print(subscribe.commands()->pulseWidth[2]);
                Serial.print("\t");
                Serial.println(subscribe.commands()->pulseWidth[3]);
                #endif

                vTaskDelayUntil(&xLastWakeTime, xWakePeriod);
        }
}

IC_Radio radio;
