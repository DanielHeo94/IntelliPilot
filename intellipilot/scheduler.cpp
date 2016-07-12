//
//  scheduler.cpp
//  intellipilot
//
//  Created by 허진수 on 2016. 7. 12..
//  Copyright © 2016년 dronix.kr. All rights reserved.
//

#include "scheduler.h"
#include "tasks.h"

#include "src/FreeRTOS_ARM.h"

tasks _tasks;

scheduler::scheduler() {
    
    portBASE_TYPE sa1, sa2, sa3, sa4, sa5;
    portBASE_TYPE c1, c2;
    portBASE_TYPE cp1;
    portBASE_TYPE fc1, fc2, fc3, fc4, fc5;
    portBASE_TYPE l1;
    
    sa1 = xTaskCreate(_tasks.getEulerAnglesGyroThread, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    sa2 = xTaskCreate(_tasks.getHeightThread, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    sa3 = xTaskCreate(_tasks.getBaroThread, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    sa4 = xTaskCreate(_tasks.getCompThread, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    sa5 = xTaskCreate(_tasks.getGpsThread, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    
    if (sem== NULL ||
            s1 != pdPASS ||
            s2 != pdPASS ||
            s3 != pdPASS ||
            s4 != pdPASS ||
            s5 != pdPASS) {
        Serial.println(F("Creation problem"));
        while(1);
    }
}

void scheduler::taskStart() {
    
    // Start scheduler
    vTaskStartScheduler();
    Serial.println("Insufficient RAM");
    while(1);
}

