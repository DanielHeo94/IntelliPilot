//
//  status.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

//  MAV_MODE_PREFLIGHT=0,  System is not ready to fly, booting, calibrating, etc. No flag is set. |
//  MAV_MODE_MANUAL_DISARMED=64,  System is allowed to be active, under manual (RC) control, no stabilization |
//  MAV_MODE_TEST_DISARMED=66,  UNDEFINED mode. This solely depends on the autopilot - use with caution, intended for developers only. |
//  MAV_MODE_STABILIZE_DISARMED=80,  System is allowed to be active, under assisted RC control. |
//  MAV_MODE_GUIDED_DISARMED=88,  System is allowed to be active, under autonomous control, manual setpoint |
//  MAV_MODE_AUTO_DISARMED=92,  System is allowed to be active, under autonomous control and navigation (the trajectory is decided onboard and not pre-programmed by MISSIONs) |
//  MAV_MODE_MANUAL_ARMED=192,  System is allowed to be active, under manual (RC) control, no stabilization |
//  MAV_MODE_TEST_ARMED=194,  UNDEFINED mode. This solely depends on the autopilot - use with caution, intended for developers only. |
//  MAV_MODE_STABILIZE_ARMED=208,  System is allowed to be active, under assisted RC control. |
//  MAV_MODE_GUIDED_ARMED=216,  System is allowed to be active, under autonomous control, manual setpoint |
//  MAV_MODE_AUTO_ARMED=220,  System is allowed to be active, under autonomous control and navigation (the trajectory is decided onboard and not pre-programmed by MISSIONs) |

#include "System.h"

bool bArmingProcedureEnalbed, bDisarmingProcedureEnabled = false;
unsigned long tArmingProcedureStart, tDisarmingProcedureStart;

void System::Setup::status() {
        statusBox.flightReady = false;
        statusBox.flightMode = MAV_MODE_PREFLIGHT;
}

void System::Publish::status(void *arg) {

        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xWakePeriod = 10;

        for(;; ) {
                static bool temp = false;

                if (subscribe.commands()->pulseWidth[0] <= (RC_CH1_HIGH + 20) &&
                    subscribe.commands()->pulseWidth[3] >= (RC_CH4_LOW - 20) &&
                    !bArmingProcedureEnalbed) {
                        bArmingProcedureEnalbed = true;
                        tArmingProcedureStart = millis();
                } else if (subscribe.commands()->pulseWidth[0] >= (RC_CH1_LOW - 20) &&
                           subscribe.commands()->pulseWidth[3] >= (RC_CH4_LOW - 20) &&
                           !bDisarmingProcedureEnabled) {
                        bDisarmingProcedureEnabled = true;
                        tDisarmingProcedureStart = millis();
                } else {
                        bArmingProcedureEnalbed = false;
                        bDisarmingProcedureEnabled = false;
                }

                if (!subscribe.status()->flightReady &&
                    bArmingProcedureEnalbed &&
                    (millis() - tArmingProcedureStart) >= 3000) {
                        if (subscribe.commands()->pulseWidth[4] == RC_CH5_LOW) {
                                statusBox.flightReady = true;
                                bArmingProcedureEnalbed = false;
                        } else {
                                // HACK: Alert error message when user tries arming without locating CH5 on manual mode.
                        }
                } else if (subscribe.status()->flightMode == MAV_MODE_MANUAL_ARMED &&
                           bDisarmingProcedureEnabled &&
                           (millis() - tDisarmingProcedureStart) >= 3000) {
                        statusBox.flightReady = false;
                        bDisarmingProcedureEnabled = false;
                }

                if (temp == false && subscribe.status()->flightReady == true) {
                        if(subscribe.status()->flightMode != MAV_MODE_MANUAL_ARMED //&&
                           /*subscribe.commands()->pulseWidth[4] == RC_CH5_LOW*/) {
                                statusBox.flightMode = MAV_MODE_MANUAL_ARMED;
                                vTaskSuspend(controlPreFlightHandler);
                                vTaskResume(controlManualHandler);
                        } else if (subscribe.commands()->pulseWidth[4] == RC_CH5_MID) {

                        } else if (subscribe.commands()->pulseWidth[4] == RC_CH5_HIGH) {

                        }
                        temp = subscribe.status()->flightReady;
                } else if (temp == true && subscribe.status()->flightReady == false) {
                        statusBox.flightMode = MAV_MODE_PREFLIGHT;

                        vTaskSuspend(controlManualHandler);
                        vTaskResume(controlPreFlightHandler);

                        temp = subscribe.status()->flightReady;
                }
                vTaskDelayUntil(&xLastWakeTime, xWakePeriod);
        }
}
