//
//  battery_indicator.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

void System::Communicate::battery_indicator(void *arg) {

        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xWakePeriod = 60000 / portTICK_PERIOD_MS;

        double batteryVoltageLast;

        for (;; ) {

                int _analogread = analogRead(A0);
                __battery.voltages = _analogread * (3.3 / 1023.0);
                __battery.remain.percents = (__battery.voltages / MAX_BATTERY_VOLTAGE) * 100;
                __battery.remain.minutes = __battery.voltages / (batteryVoltageLast - __battery.voltages); //(batteryVoltageLast - batteryVoltage);
                batteryVoltageLast = __battery.voltages;

                #if (DEBUG_BATTERY == 1)
                Serial.print("bat vol per:\t");
                Serial.print(__battery.voltages);
                Serial.print("\t");
                Serial.println(__battery.remain.percents);
                #endif

                vTaskDelayUntil(&xLastWakeTime, xWakePeriod);
        }
}
