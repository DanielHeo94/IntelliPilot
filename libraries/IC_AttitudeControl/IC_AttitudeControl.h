//
//  IC_AttitudeControl.h
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include <FreeRTOS_ARM.h>

#include <IC_PID/IC_PID.h>

#include <vars/vars_attitude.h>
#include <vars/vars_motors.h>
#include <vars/vars_pid.h>

#include <gains.h>


class IC_AttitudeControl {
public:
								IC_AttitudeControl(double*, double*, double*, double*, PidError_t*, Attitude_t*, ServoOutput_t*);

								void Compute();
};
