//
//  control_pre_flight.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

void System::Control::control_pre_flight(void *arg) {

        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xWakePeriod = 1000 / portTICK_PERIOD_MS;

        for (;; ) {
                motors.stop();

                vTaskDelayUntil(&xLastWakeTime, xWakePeriod);
        }
}
