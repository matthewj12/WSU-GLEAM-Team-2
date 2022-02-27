#include <Servo.h>
#include <LIDARLite.h>
#include <Wire.h>

// https://learn.sparkfun.com/tutorials/lidar-lite-v3-hookup-guide/all

Servo pan_servo, tilt_servo;
int pan_pos = 0;
int tilt_pos = 0;
// Is 15 in servo example sketch
int delay_for_one_deg = 30;
LIDARLite lidar;

const int tilt_0_offset = 5;
const int pan_0_offset = 0;

// 0 degrees tilt is parallel with ground, +90 is straight up
// 0 degrees pan is straight forward, +90 is left (if we are looking from behind when at 0)
const int pan_min = -45;
const int pan_max = 45;
const int tilt_min = 0;
const int tilt_max = 45;

int convertToServoPan(int pan_angle) {
    return (90 + pan_0_offset) + pan_angle;
}
int convertToServoTilt(int tilt_angle) {
    return (90 + tilt_0_offset) - tilt_angle;
}

void resetServoPos() {
    pan_servo.write(convertToServoPan(0));
    tilt_servo.write(convertToServoTilt(90));
    delay(300);
}

int cnt = 0;

void sweepAndMap(int pan_steps=46, int tilt_steps=23) {
    int pan_intvl  = (int) ((pan_max - pan_min) / (pan_steps - 1));
    int tilt_intvl = (int) ((tilt_max - tilt_min) / (tilt_steps - 1));

    resetServoPos();
    
    for (int pan_pos = pan_min; pan_pos <= pan_max; pan_pos += pan_intvl) {
        pan_servo.write(convertToServoPan(pan_pos));
        tilt_servo.write(convertToServoTilt(tilt_min));
        delay(300);
        
        for (int tilt_pos = tilt_min; tilt_pos <= tilt_max; tilt_pos += tilt_intvl) {
            tilt_servo.write(convertToServoTilt(tilt_pos));
            delay(tilt_intvl * delay_for_one_deg);

            int dist;

            if (cnt == 0) { // with bias corrections
                dist = lidar.distance();
            }
            else { // without bias corection
                dist = lidar.distance(false);
            }
            cnt++;
            cnt = cnt % 100;

           Serial.print(dist);
           Serial.print(", ");
           Serial.print(pan_pos);
           Serial.print(", ");
           Serial.print(tilt_pos);
           Serial.println();
        }
        Serial.println();
    }

    resetServoPos();
}

void setup() {
    lidar.begin(0, true);
    lidar.configure(0);

    Serial.begin(9600);
    
    pan_servo.attach(3);
    tilt_servo.attach(9);

    resetServoPos();
}

void acceptSerialInputControlServo(String in) {
    // Sweep using default pan and tilt steps
    if (in.length() == 1 && in.equals("s")) {
        sweepAndMap();
    }
    else {
        String cmd = in.substring(0, 2);
        // Set pan position
        if (cmd.equals("p ")) {
            int pan_pos = in.substring(2, in.length()).toInt();
            Serial.print("Setting pan position to ");
            pan_servo.write(convertToServoPan(pan_pos));
            Serial.print(pan_pos);
            Serial.println("...");
            delay(300);
        }
        // Set tilt position
        else if (cmd.equals("t ")) {
            int tilt_pos = in.substring(2, in.length()).toInt();
            Serial.print("Setting tilt position to ");
            tilt_servo.write(convertToServoTilt(tilt_pos));
            Serial.print(tilt_pos);
            Serial.println("...");
            delay(300);
        }
        // Sweep using custom pan and tilt steps
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
    Serial.println("Enter command: ");
    while (Serial.available() == 0) {
        delay(50);
    }
    String serial_in = Serial.readString();
    // Remove the newline character
    serial_in.remove(serial_in.length()-1);
    
	acceptSerialInputControlServo(serial_in);
}
