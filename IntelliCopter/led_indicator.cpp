//
//  led_indicator.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"

void System::Communicate::led_indicator(void *arg) {

        for (;; ) {
                switch (subscribe.status()->flight_mode)
                {
                case MAV_MODE_PREFLIGHT:
                        led.idleIndicator();
                        break;

                case MAV_MODE_MANUAL_ARMED:
                        led.manualControlIndicator();
                        break;

                case MAV_MODE_GUIDED_ARMED:
                        led.guidedControlIndicator();
                        break;

                case MAV_MODE_AUTO_ARMED:
                        led.autoControlIndicator();
                        break;

                default:
                        break;
                }
        }
}

IC_LED led;
