//
//  esc_calibration.ino
//  IntelliCopter
//
//  Created by Daniel Heo on 2016. 7. 12.
//  Copyright © 2016 http://dronix.kr. All rights reserved.
//

#include <Servo.h>

#define MAX_SIGNAL 1800
#define MIN_SIGNAL 1100
#define TEST_SIGNAL 1300

#define MOTOR_PIN_A 9
#define MOTOR_PIN_B 6
#define MOTOR_PIN_C 5
#define MOTOR_PIN_D 3

Servo a, b, c, d;

void setup() {
  Serial.begin(9600);
  Serial.println("Program begin...");
  Serial.println("This program will calibrate the ESC.");

  a.attach(MOTOR_PIN_A);
  b.attach(MOTOR_PIN_B);
  c.attach(MOTOR_PIN_C);
  d.attach(MOTOR_PIN_D);

  Serial.println("Now writing maximum output.");
  Serial.println("Turn on power source, then wait 2 seconds and press any key.");
  a.writeMicroseconds(MAX_SIGNAL);
  b.writeMicroseconds(MAX_SIGNAL);
  c.writeMicroseconds(MAX_SIGNAL);
  d.writeMicroseconds(MAX_SIGNAL);

  delay(1000);

  // Wait for input
  while (!Serial.available());
  Serial.read();
  Serial.flush();

  // Send min output
  Serial.println("Sending minimum output");
  a.writeMicroseconds(MIN_SIGNAL);
  b.writeMicroseconds(MIN_SIGNAL);
  c.writeMicroseconds(MIN_SIGNAL);
  d.writeMicroseconds(MIN_SIGNAL);

  delay(1000);

  Serial.println("Now, Let's test the motors");
  Serial.println("Plug off the battery, then power off the arduino too");
  Serial.println("Send any char if you're done");

  // Wait for input
  while (!Serial.available());
  Serial.read();
  Serial.flush();

  Serial.println("Good job, plug in the battery back and power the board on");
  Serial.println("Send any char if you're done");

  // Wait for input
  while (!Serial.available());
  Serial.read();
  Serial.flush();

  Serial.println("Arming... Wait for a sec");
  a.writeMicroseconds(MIN_SIGNAL);
  b.writeMicroseconds(MIN_SIGNAL);
  c.writeMicroseconds(MIN_SIGNAL);
  d.writeMicroseconds(MIN_SIGNAL);
  delay(6000);

  Serial.println("Send any char to test the motors");

  // Wait for input
  while (!Serial.available());
  Serial.read();
  Serial.flush();

  a.writeMicroseconds(TEST_SIGNAL);
  b.writeMicroseconds(TEST_SIGNAL);
  c.writeMicroseconds(TEST_SIGNAL);
  d.writeMicroseconds(TEST_SIGNAL);


}

void loop() {

}
