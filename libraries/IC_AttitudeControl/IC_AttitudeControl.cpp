//
//  IC_AttitudeControl.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "IC_AttitudeControl.h"

IC_AttitudeControl::IC_AttitudeControl(double* target_yaw, double* target_pitch, double* target_roll, double* throttle_in,PID_error* error, Attitude* sample, Servo_output* output) {

        _target_yaw = target_yaw;
        _target_pitch = target_pitch;
        _target_roll = target_roll;
        _throttle_in = throttle_in;

        _error = error;
        _sample = sample;
        _output = output;

        IC_PID yaw_speed_control(&((_sample->gyro).dmp.degrees[0]), &((_error->yaw).speed), _target_yaw, YAW_P_GAIN, YAW_I_GAIN, YAW_D_GAIN, DIRECT);
        IC_PID pitch_angle_control(&((_sample->ypr).dmp.degrees[1]), &((_error->pitch).angle), _target_pitch, PITCH_OUTER_P_GAIN, PITCH_OUTER_I_GAIN, PITCH_OUTER_D_GAIN, REVERSE);
        IC_PID pitch_speed_control(&((_sample->gyro).dmp.degrees[1]), &((_error->pitch).speed), &((_error->pitch).angle), PITCH_INNER_P_GAIN, PITCH_INNER_I_GAIN, PITCH_INNER_D_GAIN, REVERSE);
        IC_PID roll_angle_control(&((_sample->ypr).dmp.degrees[2]), &((_error->roll).angle), _target_roll, ROLL_OUTER_P_GAIN, ROLL_OUTER_I_GAIN, ROLL_OUTER_D_GAIN, DIRECT);
        IC_PID roll_speed_control(&((_sample->gyro).dmp.degrees[2]), &((_error->roll).speed), &((_error->roll).angle), ROLL_INNER_P_GAIN, ROLL_INNER_I_GAIN, ROLL_INNER_D_GAIN, DIRECT);

        yaw_speed_control.SetMode(AUTOMATIC);
        pitch_angle_control.SetMode(AUTOMATIC);
        pitch_speed_control.SetMode(AUTOMATIC);
        roll_angle_control.SetMode(AUTOMATIC);
        roll_speed_control.SetMode(AUTOMATIC);

        yaw_speed_control.SetOutputLimits(-300, 300);
        pitch_angle_control.SetOutputLimits(-1000, 1000);
        pitch_speed_control.SetOutputLimits(-500, 500);
        roll_angle_control.SetOutputLimits(-1000, 1000);
        roll_speed_control.SetOutputLimits(-500, 500);
}

void IC_AttitudeControl::Compute() {

        yaw_speed_control.Compute();
        pitch_angle_control.Compute();
        pitch_speed_control.Compute();
        roll_angle_control.Compute();
        roll_speed_control.Compute();

        _output->a = (-(_error->pitch).speed + (_error->roll).speed) * 0.5 - (_error->yaw).speed + *_throttle_in;
        _output->b = (-(_error->pitch).speed - (_error->roll).speed) * 0.5 + (_error->yaw).speed + *_throttle_in;
        _output->c = ((_error->pitch).speed - (_error->roll).speed) * 0.5 - (_error->yaw).speed + *_throttle_in;
        _output->d = ((_error->pitch).speed + (_error->roll).speed) * 0.5 + (_error->yaw).speed + *_throttle_in;
}
