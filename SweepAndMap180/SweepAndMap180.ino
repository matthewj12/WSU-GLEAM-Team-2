#include <Servo.h>
#include <LIDARLite.h>
#include <Wire.h>

// https://learn.sparkfun.com/tutorials/lidar-lite-v3-hookup-guide/all

const int servo_range = 180;
const int pan_intvl  = (int) (servo_range / 20);
const int tilt_intvl = (int) (servo_range / 20);

void sweepAndMap(Servo& pan_servo, Servo& tilt_servo) {
    pan_servo.write(0);
    delay(200);
    
    for (int pan_pos = 0; pan_pos <= 180; pan_pos += pan_intvl) {
        pan_servo.write(pan_pos);
        delay(20);
        tilt_servo.write(0);
        delay(200);
        
        for (int tilt_pos = 0; tilt_pos <= 180; tilt_pos += tilt_intvl) {
            tilt_servo.write(tilt_pos);
            delay(20);
            
            float dist = lidar.distance(true);

            Serial.print(dist);
            Serial.print(", ");
            Serial.print(pan_pos);
            Serial.print(", ");
            Serial.print(tilt_pos);
            Serial.println();
        }
    }
}

Servo pan_servo, tilt_servo;
LIDARLite lidar;

void setup() {
    Serial.begin(9600);
    pan_servo.attach(3);
    tilt_servo.attach(5);

    lidar.begin(0, true);
    lidar.configure(0);

    sweepAndMap(pan_servo, tilt_servo);
}

void loop() {
    pan_servo.write(0);
    tilt_servo.write(0);
}
