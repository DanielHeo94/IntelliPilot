//
//  scheduler.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

void System::task_create() {

    Serial.println("Creating tasks");

    portBASE_TYPE s1, s2, s3, s4, s5;
    portBASE_TYPE c1, c2;
    portBASE_TYPE cp1;
    portBASE_TYPE fc1, fc2, fc3, fc4, fc5;
    portBASE_TYPE l1;
    portBASE_TYPE b1;

    #if (TASK_GET_ATTITUDE == 1)
    Serial.print("\t\tTASK_GET_ATTITUDE");
    s1 = xTaskCreate(publish.attitude, NULL, configMINIMAL_STACK_SIZE + 300, NULL, 1, NULL);
    Serial.println("\t\tSuccess.");
    #endif

    #if (TASK_GET_ALTITUDE == 1)
    Serial.print("\t\tTASK_GET_ALTITUDE");
    s2 = xTaskCreate(publish.altitude, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    Serial.println("\t\t\tSuccess.");
    #endif

    #if (TASK_GET_BEARING == 1)
    Serial.print("\t\tTASK_GET_BEARING");
    s3 = xTaskCreate(publish.bearing, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    Serial.println("\t\t\tSuccess.");
    #endif

    #if (TASK_GET_POSITION == 1)
    Serial.print("\t\tTASK_GET_POSITION");
    s4 = xTaskCreate(publish.position, NULL, configMINIMAL_STACK_SIZE + 300, NULL, 1, NULL);
    Serial.println("\t\t\t\tSuccess.");
    #endif

    #if (TASK_GET_COMMANDS == 1)
    Serial.print("\t\tTASK_GET_COMMANDS");
    c1 = xTaskCreate(publish.commands, NULL, configMINIMAL_STACK_SIZE + 500, NULL, 1, NULL);
    Serial.println("\t\t\tSuccess.");
    #endif

    #if (TASK_COMM_GCS == 1)
    Serial.print("\t\tcommGcsThread...");
    c2 = xTaskCreate(communicate.gcs_mavlink, NULL, configMINIMAL_STACK_SIZE + 800, NULL, 1, NULL);
    Serial.println("\t\t\tSuccess.");
    #endif

    #if (TASK_FC_IDLE == 1)
    fc1 = xTaskCreate(control.control_pre_flight, "control_pre_flight", configMINIMAL_STACK_SIZE, NULL, 1, &task_flight_control_pre_flight);
    #endif

    #if (TASK_FC_MANUAL_CONTROL == 1)
    fc2 = xTaskCreate(control.control_manual, "control_manual", configMINIMAL_STACK_SIZE + 1000, NULL, 1, &task_flight_control_manual);
    #endif

    #if (TASK_LED_INDICATOR == 1)
    Serial.print("\t\tledIndicateThread...");
    l1 = xTaskCreate(communicate.led_indicator, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    Serial.println("\t\t\tSuccess.");
    #endif

    #if (TASK_BATTERY_CHECK == 1)
    Serial.print("\t\tbatteryCheckThread...");
    b1 = xTaskCreate(communicate.battery_indicator, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    Serial.println("\t\tSuccess.");
    #endif

    if (
        #if (TASK_GET_ATTITUDE == 1)
        s1 != pdPASS
        #endif
        #if (TASK_GET_ALTITUDE == 1)
        || s2 != pdPASS
        #endif
        #if (TASK_GET_BEARING == 1)
        || s3 != pdPASS
        #endif
        #if (TASK_GET_POSITION == 1)
        || s4 != pdPASS
        #endif
        #if (TASK_GET_COMMANDS == 1)
        || c1  != pdPASS
        #endif
        #if (TASK_COMM_GCS == 1)
		    || c2  != pdPASS
        #endif
        #if (TASK_FC_IDLE == 1)
		    || fc1 != pdPASS
        #endif
        #if (TASK_FC_MANUAL_CONTROL == 1)
		    || fc2 != pdPASS
        #endif
        #if (TASK_LED_INDICATOR == 1)
		    || l1  != pdPASS
        #endif
        #if (TASK_BATTERY_CHECK == 1)
		    || b1  != pdPASS
        #endif
        ) {
        Serial.println(F("Creation problem"));
        while(1);
    }
}

void System::scheduler_start() {

    // Start scheduler
    Serial.println("Start scheduler.");

	  vTaskSuspend(task_flight_control_manual);
    vTaskStartScheduler();

    Serial.println("Insufficient RAM");
    while(1);
}
