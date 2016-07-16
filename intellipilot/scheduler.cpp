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

extern void* idleHandle;
extern void* manualControlHandle;
extern void* posHoldControlHandle;

scheduler::scheduler() {}

void scheduler::taskCreate() {
    
    Serial.println("Creating tasks");
    
    portBASE_TYPE sa1, sa2, sa3, sa4, sa5;
    portBASE_TYPE c1, c2;
    portBASE_TYPE cp1;
    portBASE_TYPE fc1, fc2, fc3, fc4, fc5;
    portBASE_TYPE l1;

	extern SemaphoreHandle_t sem;

	sem = xSemaphoreCreateCounting(1, 0);
    
    Serial.print("\t\tgetEulerAnglesGyroThread..."); sa1 = xTaskCreate(_tasks.getEulerAnglesGyroThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL); Serial.println("\t\tSuccess.");
    Serial.print("\t\tgetAccAltThread...");          sa2 = xTaskCreate(_tasks.getAccAltThread, NULL, configMINIMAL_STACK_SIZE * 3, NULL, 1, NULL);      Serial.println("\t\t\tSuccess.");
    //Serial.print("\t\tgetBaroThread...");          sa3 = xTaskCreate(_tasks.getBaroThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);            Serial.println("\t\t\tSuccess.");
    //Serial.print("\t\tgetCompThread...");          sa4 = xTaskCreate(_tasks.getCompThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);            Serial.println("\t\t\tSuccess.");
    Serial.print("\t\tgetPosThread...");             sa5 = xTaskCreate(_tasks.getPosThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);             Serial.println("\t\t\t\tSuccess.");
    
    Serial.print("\t\tgetCommandsThread...");        c1 = xTaskCreate(_tasks.getCommandsThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);         Serial.println("\t\t\tSuccess.");
	Serial.print("\t\tcommGcsThread...");			 c2 = xTaskCreate(_tasks.commGcsThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);				Serial.println("\t\tSuccess.");

	fc2 = xTaskCreate(_tasks.manualControlThread, NULL, 300, NULL, 1, &manualControlHandle);

    if (sa1 != pdPASS
        //|| sa2 != pdPASS
        //|| sa3 != pdPASS
        //|| sa4 != pdPASS
        || sa5 != pdPASS
        || c1  != pdPASS
		|| c2  != pdPASS
		|| fc2  != pdPASS
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

