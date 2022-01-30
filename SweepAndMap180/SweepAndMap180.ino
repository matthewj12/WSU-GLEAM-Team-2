#include <Servo.h>
#include <LIDARLite.h>
#include <Wire.h>

// https://learn.sparkfun.com/tutorials/lidar-lite-v3-hookup-guide/all

const int pan_servo_min = 0;
const int pan_servo_max = 180;
const int tilt_servo_min = 5;
const int tilt_servo_max = 180;


Servo pan_servo, tilt_servo;
int pan_pos = 0;
int tilt_pos = 0;
// Is 15 in servo example sketch
int delay_for_one_deg = 5;
LIDARLite lidar;

void resetServoPos() {
    pan_servo.write(pan_servo_min);
    tilt_servo.write(tilt_servo_min);
    delay(300);
}

void sweepAndMap(int pan_steps=10, int tilt_steps=5) {
    int pan_intvl  = (int) ((pan_servo_max - pan_servo_min) / (pan_steps - 1));
    int tilt_intvl = (int) ((tilt_servo_max - tilt_servo_min) / (tilt_steps - 1));

    resetServoPos();
    
    for (int pan_pos = pan_servo_min; pan_pos <= 180; pan_pos += pan_intvl) {
        pan_servo.write(pan_pos);
        tilt_servo.write(tilt_servo_min);
        delay(300);
        
        for (int tilt_pos = tilt_servo_min; tilt_pos <= 180; tilt_pos += tilt_intvl) {
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

    resetServoPos();
}

void acceptSerialInputControlServo() {
    Serial.println("Enter command: ");
    while (Serial.available() == 0) {}
    String in = Serial.readString();
    in.remove(in.length()-1); // Remove the newline character

    if (in.length() == 1 && in.equals("s")) {
        sweepAndMap();
    }
    else {
        String cmd = in.substring(0, 2);
        if (cmd.equals("p ")) {
            int pan_pos = in.substring(2, in.length()).toInt();
            Serial.print("Setting pan position to ");
            pan_servo.write(pan_pos);
            Serial.print(pan_pos);
            Serial.println("...");
            delay(300);
        }
        else if (cmd.equals("t ")) {
            int tilt_pos = in.substring(2, in.length()).toInt();
            Serial.print("Setting tilt position to ");
            tilt_servo.write(tilt_pos);
            Serial.print(tilt_pos);
            Serial.println("...");
            delay(300);
        }
        else if (cmd.equals("s ") && in.length() >= 5) {
            String pan_steps_str, tilt_steps_str;
            int i = 2;
            for (i; in[i] != ' '; i++) {
                pan_steps_str += in[i];
            }
            i++;
            for (i; i < in.length(); i++) {
                tilt_steps_str += in[i];
            }

            Serial.println("Sweeping with " + pan_steps_str  + " pan steps and "
                                          + tilt_steps_str + " tilt steps...");
            
            sweepAndMap(pan_steps_str.toInt(), tilt_steps_str.toInt());
        }
        else {
            Serial.println("Invalid input. ");
        }
    }
    
    //while (Serial.available() == 0) {}
    //Serial.readString();
}

void loop() {
	acceptSerialInputControlServo();
}
