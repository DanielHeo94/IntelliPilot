// led.h

#ifndef _LED_h
#define _LED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define DEFAULT_LED_INDICATOR_PIN	43

class led {
public:
	int pin;

	led();

	void begin(int val = DEFAULT_LED_INDICATOR_PIN);

	void idleIndicator();
	void manualControlIndicator();
	void guidedControlIndicator();
	void autoControlIndicator();

private:
};

#endif

