//
//  control_manual.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

void System::Control::control_manual(void *arg) {

        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xWakePeriod = FREQUENCY_TASK_FC_MANUAL_CONTROL;

        IC_AttitudeControl attitude_control(&(subscribe.commands()->manual[0]), &(subscribe.commands()->manual[1]), &(subscribe.commands()->manual[2]), &(subscribe.commands()->manual[3]), subscribe.pid_error(), subscribe.attitude(), subscribe.servo_output());

        for(;; ) {

                attitude_control.Compute();

                motors.rotate(subscribe.servo_output()->a, subscribe.servo_output()->b, subscribe.servo_output()->c, subscribe.servo_output()->d);

                vTaskDelayUntil(&xLastWakeTime, xWakePeriod);
        }
}
