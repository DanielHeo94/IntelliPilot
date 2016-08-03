//
//  scheduler.cpp
//  intellipilot
//
//  Created by 허진수 on 2016. 7. 12..
//  Copyright © 2016년 dronix.kr. All rights reserved.
//

#include "scheduler.h"
#include "tasks.h"

#include <FreeRTOS_ARM.h>

tasks _tasks;

extern TaskHandle_t idleHandle;
extern TaskHandle_t manualControlHandle;
extern TaskHandle_t posHoldControlHandle;

scheduler::scheduler() {}

void scheduler::taskCreate() {
    
    Serial.println("Creating tasks");
    
    portBASE_TYPE sa1, sa2, sa3, sa4, sa5, sa6;
    portBASE_TYPE c1, c2;
    portBASE_TYPE cp1;
    portBASE_TYPE fc1, fc2, fc3, fc4, fc5;
    portBASE_TYPE l1;
	portBASE_TYPE b1;
    
    Serial.print("\t\tgetAttitudeThread...");		 sa1 = xTaskCreate(_tasks.getAttitudeThread, NULL, configMINIMAL_STACK_SIZE + 300, NULL, 1, NULL);	   Serial.println("\t\tSuccess.");
	//Serial.print("\t\tgetGyroThread...");			 sa2 = xTaskCreate(_tasks.getGyroThread, NULL, configMINIMAL_STACK_SIZE + 300, NULL, 1, NULL);		   Serial.println("\t\tSuccess.");
	//Serial.print("\t\tgetAccAltThread...");        sa3 = xTaskCreate(_tasks.getAccAltThread, NULL, configMINIMAL_STACK_SIZE + 200, NULL, 1, NULL);	   Serial.println("\t\t\tSuccess.");
    //Serial.print("\t\tgetBaroThread...");          sa4 = xTaskCreate(_tasks.getBaroThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);               Serial.println("\t\t\tSuccess.");
    //Serial.print("\t\tgetCompThread...");          sa5 = xTaskCreate(_tasks.getCompThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);               Serial.println("\t\t\tSuccess.");
    Serial.print("\t\tgetPosThread...");             sa6 = xTaskCreate(_tasks.getPosThread, NULL, configMINIMAL_STACK_SIZE + 300, NULL, 1, NULL);          Serial.println("\t\t\t\tSuccess.");
    
    Serial.print("\t\tgetCommandsThread...");        c1 = xTaskCreate(_tasks.getCommandsThread, NULL, configMINIMAL_STACK_SIZE + 500, NULL, 1, NULL);       Serial.println("\t\t\tSuccess.");
	Serial.print("\t\tcommGcsThread...");			 c2 = xTaskCreate(_tasks.commGcsThread, NULL, configMINIMAL_STACK_SIZE + 800, NULL, 1, NULL);			Serial.println("\t\t\tSuccess.");

	fc1 = xTaskCreate(_tasks.idle, "idleTask", configMINIMAL_STACK_SIZE, NULL, 1, &idleHandle);
	fc2 = xTaskCreate(_tasks.manualControlThread, "manualControlTask", configMINIMAL_STACK_SIZE + 1000, NULL, 1, &manualControlHandle);

	Serial.print("\t\tledIndicateThread...");		 l1 = xTaskCreate(_tasks.ledIndicateThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);				Serial.println("\t\t\tSuccess.");
	Serial.print("\t\tbatteryCheckThread...");	 b1 = xTaskCreate(_tasks.batteryCheckThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);			Serial.println("\t\tSuccess.");

    if (sa1 != pdPASS
        //|| sa2 != pdPASS
        //|| sa3 != pdPASS
        //|| sa4 != pdPASS
        //|| sa5 != pdPASS
		|| sa6 != pdPASS
        || c1  != pdPASS
		|| c2  != pdPASS
		|| fc1 != pdPASS 
		|| fc2 != pdPASS
		|| l1  != pdPASS
		|| b1  != pdPASS
        ) {
        Serial.println(F("Creation problem"));
        while(1);
    }
}

void scheduler::taskStart() {
    
    // Start scheduler
    Serial.println("Start scheduler.");
	
	vTaskSuspend(manualControlHandle);
    vTaskStartScheduler();

    Serial.println("Insufficient RAM");
    while(1);
}

