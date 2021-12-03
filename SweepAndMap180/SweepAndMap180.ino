#include <Servo.h>
#include <Vector.h>

class Point3D;
class SpatialMap;
float getDist();
float degreesToRadians(float p_deg_val);
void sweepAndMap(Servo& servo, SpatialMap& spatial_map);

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

class SpatialMap {
private:
    Point3D m_storage_array[30];
    Vector<Point3D> m_point_list;
public:
    SpatialMap() {
        m_point_list.setStorage(m_storage_array);
    }

    void addPoint(Point3D& point) {
        m_point_list.push_back(point);
    }

    void printAllPoints() {
        for (Point3D point : m_point_list) {
            Serial.print(point.getX());
			Serial.print(", ");
			Serial.print(point.getY());
			Serial.print(", ");
			Serial.print(point.getZ());
			Serial.println();
        }
    }
};

float getDist() {
    return 10.5;
}

float degreesToRadians(float p_deg_val) {
    return p_deg_val * 0.0174533;
}

void sweepAndMap(Servo& pan_servo, Servo& tilt_servo, char* points_ptr, int& points_length) {
    pan_servo.write(0);
    delay(200);
    
    for (int pan_pos = 0; pan_pos <= 180; pan_pos += 10) {
        tilt_servo.write(0);
        delay(200);
        
        pan_servo.write(pan_pos);
        delay(20);
        
        for (int tilt_pos = 0; tilt_pos <= 180; tilt_pos += 10) {
            tilt_servo.write(tilt_pos);
            delay(20);
            
            float dist = getDist();
            float pan_pos_rad = degreesToRadians(pan_pos);
            float tilt_pos_rad = degreesToRadians(tilt_pos);
            float xy_dist = cos(tilt_pos_rad) / dist; // xy component of dist
            
//          Point3D* point = new Point3D(cos(pan_pos_rad) / xy_dist,  sin(pan_pos_rad) / xy_dist, sin(tilt_pos_rad) / dist);
//    		spatial_map.addPoint(*point);

            char* insert_here = points_ptr + points_length;
            *insert_here = (int) (cos(pan_pos_rad) / xy_dist);
            insert_here++;
            *insert_here = (int) (sin(pan_pos_rad) / xy_dist);
            insert_here++;
            *insert_here = (int) (sin(tilt_pos_rad) / dist);
            insert_here++;

//            points_length += 3;
            
            Serial.print(pan_pos);
            Serial.print(", ");
            Serial.println(tilt_pos);
        }
    }
}

void printPoints(char* points_ptr, int points_length) {
    for (char* it = points_ptr; it != points_ptr + points_length; ) {
        Serial.print(*it);
        Serial.print(", ");
        it++;

        Serial.print(*it);
        Serial.print(", ");
        it++;

        Serial.print(*it);
        Serial.print(", ");
        it++;
    }
}

Servo pan_servo, tilt_servo;
    
//SpatialMap spatial_map = SpatialMap();

void setup() {
    Serial.begin(9600);
    pan_servo.attach(3);
    tilt_servo.attach(5);

    char points_ptr[300];
    int points_length = 0;

    sweepAndMap(pan_servo, tilt_servo, points_ptr, points_length);
//	spatial_map.printAllPoints();
    printPoints(points_ptr, points_length);
}

void loop() {
    pan_servo.write(0);
    tilt_servo.write(0);
//    sweepAndMap(pan_servo, spatial_map);
}
