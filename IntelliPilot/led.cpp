// 
// 
// 

#include "led.h"

#include <FreeRTOS_ARM.h>

led::led() { }

void led::begin(int val) {

	pin = val;

	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
}

void led::idleIndicator() {

	digitalWrite(pin, HIGH);
	vTaskDelay((1000L * configTICK_RATE_HZ) / 1000L);
	digitalWrite(pin, LOW);
	vTaskDelay((1000L * configTICK_RATE_HZ) / 1000L);
}

void led::manualControlIndicator() {

	digitalWrite(pin, HIGH);
	vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
	digitalWrite(pin, LOW);
	vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
}

void led::guidedControlIndicator() {

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

void led::autoControlIndicator() {

	digitalWrite(pin, HIGH);
	vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
	digitalWrite(pin, LOW);
	vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
	digitalWrite(pin, HIGH);
	vTaskDelay((1000L * configTICK_RATE_HZ) / 1000L);
	digitalWrite(pin, LOW);
	vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
}