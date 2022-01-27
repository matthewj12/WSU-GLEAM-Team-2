#include <Servo.h>
#include <LIDARLite.h>
#include <Wire.h>

// https://learn.sparkfun.com/tutorials/lidar-lite-v3-hookup-guide/all

const int pan_servo_zero_offset = 0;
const int tilt_servo_zero_offset = 5;
const int pan_servo_range = 180;
const int tilt_servo_range = 175;
const int pan_steps = 5;
const int tilt_steps = 5;
const int pan_intvl  = (int) (pan_servo_range / pan_steps);
const int tilt_intvl = (int) (tilt_servo_range / tilt_steps);

Servo pan_servo, tilt_servo;
int pan_pos = 0;
int tilt_pos = 0;
// Is 15 in servo example sketch
int delay_for_one_deg = 5;
LIDARLite lidar;

void resetServoPos() {
    pan_servo.write(pan_servo_zero_offset);
    tilt_servo.write(tilt_servo_zero_offset);
    delay(300);
}

void sweepAndMap(Servo& pan_servo, Servo& tilt_servo) {
    resetServoPos();
    
    for (int pan_pos = pan_servo_zero_offset; pan_pos <= 180; pan_pos += pan_intvl) {
        pan_servo.write(pan_pos);
        tilt_servo.write(tilt_servo_zero_offset);
        delay(300);
        
        for (int tilt_pos = tilt_servo_zero_offset; tilt_pos <= 180; tilt_pos += tilt_intvl) {
            tilt_servo.write(tilt_pos);
            delay(tilt_intvl * delay_for_one_deg);
            
//            float dist = lidar.distance(true);

//            Serial.print(dist);
//            Serial.print(", ");
//            Serial.print(pan_pos);
//            Serial.print(", ");
//            Serial.print(tilt_pos);
//            Serial.println();
        }
    }

    resetServoPos();
}

void setup() {
//    lidar.begin(0, true);
//    lidar.configure(0);

    Serial.begin(9600);
    
    pan_servo.attach(3);
    tilt_servo.attach(9);
    
    Serial.println("Starting sweep...");
    Serial.println();
    
    sweepAndMap(pan_servo, tilt_servo);
}

void acceptSerialInputControlServo() {
    Serial.println("Enter pan and tilt positions: ");
    while (Serial.available() == 0) {
      pan_servo.write(pan_pos);
      tilt_servo.write(tilt_pos);    
    }
    pan_pos = Serial.parseInt();
    tilt_pos = Serial.parseInt();
    Serial.print("Pan pos = ");
    Serial.print(pan_pos);
    Serial.print(", Tilt pos = ");
    Serial.print(tilt_pos);
    Serial.println();

    pan_servo.write(pan_pos);
    tilt_servo.write(tilt_pos);
    Serial.println("Moving...");
    delay(300);

    while (Serial.available() == 0) {}
    Serial.readString();
}

void loop() {
	acceptSerialInputControlServo();
}
