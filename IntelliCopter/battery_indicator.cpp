//
//  battery_indicator.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

void System::Publish::battery(void *arg) {

        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xWakePeriod = 60000 / portTICK_PERIOD_MS;

        double batteryVoltageLast;

        for (;; ) {

                int _analogread = analogRead(A0);
                batteryBox.voltages = _analogread * (3.3 / 1023.0);
                batteryBox.remain.percents = (batteryBox.voltages / MAX_BATTERY_VOLTAGE) * 100;
                batteryBox.remain.minutes = batteryBox.voltages / (batteryVoltageLast - batteryBox.voltages); //(batteryVoltageLast - batteryVoltage);
                batteryVoltageLast = batteryBox.voltages;

                #if (DEBUG_BATTERY == 1)
                Serial.print("bat vol per:\t");
                Serial.print(batteryBox.voltages);
                Serial.print("\t");
                Serial.println(batteryBox.remain.percents);
                #endif

                vTaskDelayUntil(&xLastWakeTime, xWakePeriod);
        }
}
