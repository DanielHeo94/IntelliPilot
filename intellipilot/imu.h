//
//  imu.h
//  intellipilot
//
//  Created by 허진수 on 2016. 7. 12..
//  Copyright © 2016년 dronix.kr. All rights reserved.
//

#ifndef _IMU_H_
#define _IMU_H_

#include <stdio.h>

class imu{
public:
    
    imu();

	void setup();

	void getGyro(double *data);
    
private:
};

#endif /* imu_h */
