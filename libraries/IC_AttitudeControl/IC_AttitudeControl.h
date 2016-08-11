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


class IC_AttitudeControl {
public:
  IC_AttitudeControl(double*, double*, double*, double*, PID_error*, Attitude*, Servo_output*);

  void Compute();

private:

  double* _target_yaw;
  double* _target_pitch;
  double* _target_roll;
  double* _throttle_in;

  PID_error* _error;
  Attitude* _sample;
  Servo_output* _output;
};
