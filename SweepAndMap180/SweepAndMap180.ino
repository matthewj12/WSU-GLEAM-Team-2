#include <Servo.h>
#include <LIDARLite.h>
#include <Wire.h>
#include <Vector.h>

// https://learn.sparkfun.com/tutorials/lidar-lite-v3-hookup-guide/all

class Point3D {
private:
    float m_x, m_y, m_z;
public:
	Point3D() {
	    m_x = 0;
        m_y = 0;
        m_z = 0;
    }
    
    Point3D(float p_x, float p_y, float p_z) {
        m_x = p_x;
        m_y = p_y;
        m_z = p_z;
    }
    
    float getX() { return m_x; }
    float getY() { return m_y; }
    float getZ() { return m_z; }

    void setX(float p_x) { m_x = p_x; }
    void setY(float p_y) { m_y = p_y; }
    void setZ(float p_z) { m_z = p_z; }
    
};

float degreesToRadians(float p_deg_val) {
    return p_deg_val * 0.0174533;
}

void sweepAndMap(Servo& pan_servo, Servo& tilt_servo) {
    pan_servo.write(0);
    delay(200);
    
    for (int pan_pos = 0; pan_pos <= 180; pan_pos += 10) {
        pan_servo.write(pan_pos);
        delay(20);
        tilt_servo.write(0);
        delay(200);
        
        for (int tilt_pos = 0; tilt_pos <= 90; tilt_pos += 10) {
            tilt_servo.write(tilt_pos);
            delay(20);
            
            float dist = lidar.distance(true);
            float pan_pos_rad = degreesToRadians(pan_pos);
            float tilt_pos_rad = degreesToRadians(tilt_pos);
            float xy_dist = cos(tilt_pos_rad) * dist; // xy component of dist

            Serial.print(cos(pan_pos_rad) * xy_dist);
            Serial.print(", ");
            Serial.print(sin(pan_pos_rad) * xy_dist);
            Serial.print(", ");
            Serial.print(sin(tilt_pos_rad) * dist);
            Serial.println();
        }
    }
}
Servo pan_servo, tilt_servo;
LIDARLite lidar;
int cal_cnt = 0;

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
