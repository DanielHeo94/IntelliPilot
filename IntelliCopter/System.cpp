//
//  System.cpp
//  intellipilot
//
//  Created by 허진수 on 2016. 7. 12..
//  Copyright © 2016년 dronix.kr. All rights reserved.
//

#include "System.h"

System::System() {
}
System::Setup::Setup() {
}
System::Publish::Publish() {
}
System::Subscribe::Subscribe() {
}
System::Communicate::Communicate() {
}
System::Control::Control() {
}

void System::config() {

        Serial.begin(115200);
        Serial.flush();

        Serial.println(" ______   ______   ______   ______   ______   ______   ______   ______   ______ ");
        Serial.println("/_____/  /_____/  /_____/  /_____/  /_____/  /_____/  /_____/  /_____/  /_____/ ");
        Serial.println("._. .___        __         .__  .__  .____________.__.__          __    ._.     ");
        Serial.println("| | |   | _____/  |_  ____ |  | |  | |__\______   \__|  |   _____/  |_  | |     ");
        Serial.println("|_| |   |/    \   __\/ __ \|  | |  | |  ||     ___/  |  |  /  _ \   __\ |_|     ");
        Serial.println("|-| |   |   |  \  | \  ___/|  |_|  |_|  ||    |   |  |  |_(  <_> )  |   |-|     ");
        Serial.println("| | |___|___|  /__|  \___  >____/____/__||____|   |__|____/\____/|__|   | |     ");
        Serial.println("|_|          \/          \/                                             |_|     ");
        Serial.println(" ______   ______   ______   ______   ______   ______   ______   ______   ______ ");
        Serial.println("/_____/  /_____/  /_____/  /_____/  /_____/  /_____/  /_____/  /_____/  /_____/ ");

        Serial.println("System configuration start.");

    #if (TASK_GET_ATTITUDE == 1)
        _setup.attitude();
    #endif

    #if (TASK_GET_BEARING == 1)
        _setup.bearing();
    #endif

    #if (TASK_GET_ALTITUDE == 1)
        _setup.altitude();
    #endif

    #if (TASK_GET_POSITION == 1)
        _setup.position();
    #endif

    #if (TASK_GET_COMMANDS == 1)
        _setup.commands();
    #endif

        motors.start();

        Serial.println("System configuration complete.");

        copter.task_create();
}

void System::start() {
        copter.scheduler_start();
}

System copter;
System::Setup _setup;
System::Publish publish;
System::Subscribe subscribe;
System::Communicate communicate;
System::Control control;

IC_Motors motors;
