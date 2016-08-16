//
//  IC_LED.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "IC_LED.h"

#include <FreeRTOS_ARM.h>

IC_LED::IC_LED() {
}

void IC_LED::begin(int val) {

								pin = val;

								pinMode(pin, OUTPUT);
								digitalWrite(pin, HIGH);
}

void IC_LED::idleIndicator() {

								digitalWrite(pin, HIGH);
								vTaskDelay((1000L * configTICK_RATE_HZ) / 1000L);
								digitalWrite(pin, LOW);
								vTaskDelay((1000L * configTICK_RATE_HZ) / 1000L);
}

void IC_LED::manualControlIndicator() {

								digitalWrite(pin, HIGH);
								vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
								digitalWrite(pin, LOW);
								vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
}

void IC_LED::guidedControlIndicator() {

								digitalWrite(pin, HIGH);
								vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
								digitalWrite(pin, LOW);
								vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
								digitalWrite(pin, HIGH);
								vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
								digitalWrite(pin, LOW);
								vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
								digitalWrite(pin, HIGH);
								vTaskDelay((1000L * configTICK_RATE_HZ) / 1000L);
								digitalWrite(pin, LOW);
								vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
}

void IC_LED::autoControlIndicator() {

								digitalWrite(pin, HIGH);
								vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
								digitalWrite(pin, LOW);
								vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
								digitalWrite(pin, HIGH);
								vTaskDelay((1000L * configTICK_RATE_HZ) / 1000L);
								digitalWrite(pin, LOW);
								vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
}
