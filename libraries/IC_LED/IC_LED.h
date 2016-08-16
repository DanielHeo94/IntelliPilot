//
//  IC_LED.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#ifndef _IC_LED_H_
#define _IC_LED_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#define DEFAULT_LED_INDICATOR_PIN 43

class IC_LED {
public:
								int pin;

								IC_LED();

								void begin(int val = DEFAULT_LED_INDICATOR_PIN);

								void idleIndicator();
								void manualControlIndicator();
								void guidedControlIndicator();
								void autoControlIndicator();

private:
};

#endif
