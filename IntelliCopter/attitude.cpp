//
//  attitude.cpp
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include "System.h"
#include <IC_InertialSensor/I2Cdev.h>
#include <IC_InertialSensor/MPU6050_6Axis_MotionApps20.h>

MPU6050 mpu6050;

#define MPU6050_INTERRUPT_PIN 11

bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorInt16 aa;
VectorInt16 aaReal;
VectorInt16 aaWorld;
VectorFloat gravity;
int16_t gyro[3];

volatile bool mpuInterrupt = false;
void dmpDataReady() {
        mpuInterrupt = true;
}

void System::Setup::attitude() {

        Wire1.begin();
        Wire1.setClock(400000);

        Serial.println(F("Initializing MPU6050..."));
        mpu6050.initialize();
        pinMode(MPU6050_INTERRUPT_PIN, INPUT);

        Serial.println(F("Testing device connections..."));
        Serial.println(mpu6050.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

        Serial.println(F("Initializing DMP..."));
        devStatus = mpu6050.dmpInitialize();

        mpu6050.setXGyroOffset(220);
        mpu6050.setYGyroOffset(76);
        mpu6050.setZGyroOffset(-85);
        mpu6050.setZAccelOffset(1788);

        if (devStatus == 0) {
                Serial.println(F("\t\tEnabling DMP..."));
                mpu6050.setDMPEnabled(true);

                Serial.println(F("\t\tEnabling interrupt detection..."));
                attachInterrupt(MPU6050_INTERRUPT_PIN, dmpDataReady, RISING);
                mpuIntStatus = mpu6050.getIntStatus();

                Serial.println(F("\t\tDMP ready! Waiting for first interrupt..."));
                dmpReady = true;

                packetSize = mpu6050.dmpGetFIFOPacketSize();
        } else {
                // ERROR!
                // 1 = initial memory load failed
                // 2 = DMP configuration updates failed
                // (if it's going to break, usually the code will be 1)
                Serial.print(F("DMP Initialization failed (code "));
                Serial.print(devStatus);
                Serial.println(F(")"));
        }
}

void System::Publish::attitude(void *arg) {

        for(;; ) {
                if (!dmpReady) return;

                while (!mpuInterrupt && fifoCount < packetSize) {}

                mpuInterrupt = false;
                mpuIntStatus = mpu6050.getIntStatus();

                fifoCount = mpu6050.getFIFOCount();

                if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
                        // reset so we can continue cleanly
                        mpu6050.resetFIFO();
                        Serial.println(F("FIFO overflow!"));

                        // otherwise, check for DMP data ready interrupt (this should happen frequently)
                } else if (mpuIntStatus & 0x02) {
                        // wait for correct available data length, should be a VERY short wait
                        while (fifoCount < packetSize) fifoCount = mpu6050.getFIFOCount();

                        // read a packet from FIFO
                        mpu6050.getFIFOBytes(fifoBuffer, packetSize);

                        // track FIFO count here in case there is > 1 packet available
                        // (this lets us immediately read more without waiting for an interrupt)
                        fifoCount -= packetSize;

                        mpu6050.dmpGetQuaternion(&q, fifoBuffer);
                        mpu6050.dmpGetGravity(&gravity, &q);
                        mpu6050.dmpGetYawPitchRoll(attitudeBox.ypr.dmp.radians, &q, &gravity);
                        mpu6050.dmpGetEuler(attitudeBox.euler.dmp.radians, &q);
                        mpu6050.dmpGetGyro(gyro, fifoBuffer);

                        attitudeBox.ypr.dmp.degrees[0] = math.radiantodegree(attitudeBox.ypr.dmp.radians[0]);
                        attitudeBox.ypr.dmp.degrees[1] = math.radiantodegree(attitudeBox.ypr.dmp.radians[1]);
                        attitudeBox.ypr.dmp.degrees[2] = math.radiantodegree(attitudeBox.ypr.dmp.radians[2]);

                        attitudeBox.euler.dmp.degrees[0] = math.radiantodegree(attitudeBox.euler.dmp.radians[0]);
                        attitudeBox.euler.dmp.degrees[1] = math.radiantodegree(attitudeBox.euler.dmp.radians[1]);
                        attitudeBox.euler.dmp.degrees[2] = math.radiantodegree(attitudeBox.euler.dmp.radians[2]);

                        attitudeBox.gyro.dmp.degrees[0] = (float) gyro[0];
                        attitudeBox.gyro.dmp.degrees[1] = (float) gyro[1];
                        attitudeBox.gyro.dmp.degrees[2] = (float) gyro[2];

          #if (DEBUG_READABLE_QUATERNION == 1)
                        Serial.print("quat\t");
                        Serial.print(q.w);
                        Serial.print("\t");
                        Serial.print(q.x);
                        Serial.print("\t");
                        Serial.print(q.y);
                        Serial.print("\t");
                        Serial.println(q.z);
          #endif

          #if (DEBUG_READABLE_EULER == 1)
                        Serial.print("euler\t");
                        Serial.print(attitudeBox.euler.dmp.degrees[0]);
                        Serial.print("\t");
                        Serial.print(attitudeBox.euler.dmp.degrees[1]);
                        Serial.print("\t");
                        Serial.println(attitudeBox.euler.dmp.degrees[2]);
          #endif

          #if (DEBUG_READABLE_YAWPITCHROLL == 1)
                        Serial.print("ypr\t");
                        Serial.print(attitudeBox.ypr.dmp.degrees[0]);
                        Serial.print("\t");
                        Serial.print(attitudeBox.ypr.dmp.degrees[1]);
                        Serial.print("\t");
                        Serial.println(attitudeBox.ypr.dmp.degrees[2]);
          #endif


                }
        }
}

IC_Math math;
