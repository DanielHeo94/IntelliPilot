//
//  control_manual.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

void System::Control::controlManual(void *arg) {

        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xWakePeriod = FREQUENCY_TASK_FC_MANUAL_CONTROL;

        IC_AttitudeControl attitude_control(&(subscribe.commands()->controlManual[0]),
                                            &(subscribe.commands()->controlManual[1]),
                                            &(subscribe.commands()->controlManual[2]),
                                            &(subscribe.commands()->controlManual[3]),
                                            subscribe.pidError(),
                                            subscribe.attitude(),
                                            subscribe.servoOutput());

        for(;; ) {

                attitude_control.Compute();

                motors.rotate(subscribe.servoOutput()->a,
                              subscribe.servoOutput()->b,
                              subscribe.servoOutput()->c,
                              subscribe.servoOutput()->d);

                vTaskDelayUntil(&xLastWakeTime, xWakePeriod);
        }
}
