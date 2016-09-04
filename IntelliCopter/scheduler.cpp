//
//  scheduler.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

void System::createTasks() {

        Serial.println("Creating tasks");

    #if (TASK_GET_ATTITUDE == 1)
        Serial.print("\t\tTASK_GET_ATTITUDE");
        portBASE_TYPE pPubAttitude = xTaskCreate(publish.attitude, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        Serial.println("\t\tSuccess.");
    #endif

    #if (TASK_GET_ALTITUDE == 1)
        Serial.print("\t\tTASK_GET_ALTITUDE");
        portBASE_TYPE pPubAltitude = xTaskCreate(publish.altitude, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        Serial.println("\t\t\tSuccess.");
    #endif

    #if (TASK_GET_BEARING == 1)
        Serial.print("\t\tTASK_GET_BEARING");
        portBASE_TYPE pPubBearing = xTaskCreate(publish.bearing, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        Serial.println("\t\t\tSuccess.");
    #endif

    #if (TASK_GET_POSITION == 1)
        Serial.print("\t\tTASK_GET_POSITION");
        portBASE_TYPE pPubPosition = xTaskCreate(publish.position, NULL, configMINIMAL_STACK_SIZE + 300, NULL, 1, NULL);
        Serial.println("\t\t\t\tSuccess.");
    #endif

    #if (TASK_GET_COMMANDS == 1)
        Serial.print("\t\tTASK_GET_COMMANDS");
        portBASE_TYPE pPubCommands = xTaskCreate(publish.commands, NULL, configMINIMAL_STACK_SIZE + 500, NULL, 1, NULL);
        Serial.println("\t\t\tSuccess.");
    #endif

    #if (TASK_GET_STATUS == 1)
        Serial.print("\t\tTASK_GET_STATUS");
        portBASE_TYPE pPubStatus = xTaskCreate(publish.status, NULL, configMINIMAL_STACK_SIZE + 128, NULL, 1, NULL);
        Serial.println("\t\t\tSuccess.");
    #endif

    #if (TASK_BATTERY_CHECK == 1)
        Serial.print("\t\tbatteryCheckThread...");
        portBASE_TYPE pPubBattery = xTaskCreate(publish.battery, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        Serial.println("\t\tSuccess.");
    #endif

    #if (TASK_COMM_GCS == 1)
        Serial.print("\t\tcommGcsThread...");
        portBASE_TYPE pComTransferMsgToGcs = xTaskCreate(communicate.transferMsgToGcs, NULL, configMINIMAL_STACK_SIZE + 512, NULL, 1, NULL);
        Serial.println("\t\t\tSuccess.");
    #endif

    #if (TASK_FC_IDLE == 1)
        portBASE_TYPE pConConPreFlight = xTaskCreate(control.controlPreFlight, "control_pre_flight", configMINIMAL_STACK_SIZE, NULL, 1, &controlPreFlightHandler);
    #endif

    #if (TASK_FC_MANUAL_CONTROL == 1)
        portBASE_TYPE pConConManual = xTaskCreate(control.controlManual, "control_manual", configMINIMAL_STACK_SIZE + 1000, NULL, 1, &controlManualHandler);
    #endif

    #if (TASK_LED_INDICATOR == 1)
        Serial.print("\t\tledIndicateThread...");
        portBASE_TYPE pComShowLedIndication = xTaskCreate(communicate.showLedIndication, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        Serial.println("\t\t\tSuccess.");
    #endif

        if (
                false
        #if (TASK_GET_ATTITUDE == 1)
                || pPubAttitude != pdPASS
        #endif
        #if (TASK_GET_ALTITUDE == 1)
                || pPubAltitude != pdPASS
        #endif
        #if (TASK_GET_BEARING == 1)
                || pPubBearing != pdPASS
        #endif
        #if (TASK_GET_POSITION == 1)
                || pPubPosition != pdPASS
        #endif
        #if (TASK_GET_COMMANDS == 1)
                || pPubCommands != pdPASS
        #endif
        #if (TASK_COMM_GCS == 1)
                || pComTransferMsgToGcs != pdPASS
        #endif
        #if (TASK_FC_IDLE == 1)
                || pConConPreFlight != pdPASS
        #endif
        #if (TASK_FC_MANUAL_CONTROL == 1)
                || pConConManual != pdPASS
        #endif
        #if (TASK_LED_INDICATOR == 1)
                || pComShowLedIndication != pdPASS
        #endif
        #if (TASK_BATTERY_CHECK == 1)
                || pPubBattery != pdPASS
        #endif
                ) {
                Serial.println(F("Creation problem"));
                while(1) ;
        }
}

void System::startScheduler() {

        // Start scheduler
        Serial.println("Start scheduler.");

        vTaskSuspend(controlManualHandler);
        vTaskStartScheduler();

        Serial.println("Insufficient RAM");
        while(1) ;
}
