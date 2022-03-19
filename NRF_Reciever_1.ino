#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define enA 3  
#define in1 8   
#define in2 7  
#define in3 6   
#define in4 4  
#define enB 5  

RF24 radio(9,10);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

 struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;

};

Data_Package data; //Create a variable with the above structure

int  steering, throttle;
int motorSpeedA = 0;
int motorSpeedB = 0;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  //Serial.begin(9600);  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();
}




void loop() {
 // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone jas a throttle up, if we lose connection it can keep flying away if we dont reset the function
  }

  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }

  // Parse the data from the Joystic 1 to the throttle and steering variables
  throttle = data.j1PotY;
  steering = data.j2PotX;  




 // Throttle used for forward and backward control
  // Joystick values: 0 to 255; down = 0; middle = 127; up = 255
  if (throttle < 110) {
    // Set Motor A backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);


    //throttle = throttle - 110;  // This produces a negative number
    //throttle = throttle * -1;   // Make the number positive

    // Convert the declining throttle readings for going backward from 110 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(throttle, 110, 0, 0, 255);
    motorSpeedB = map(throttle, 110, 0, 0, 255);
  }
  else if (throttle > 140) {
    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // Convert the increasing throttle readings for going forward from 140 to 255 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(throttle, 140, 255, 0, 255);
    motorSpeedB = map(throttle, 140, 255, 0, 255);
  }else {
    // This is Stopped

    motorSpeedA = 0;
    motorSpeedB = 0;
  }

   //Steering 

   if (steering < 110) {
		// Move Left
 
	  int xMapped = map(steering, 110, 0, 0, 255);

		//steering = steering - 460; // This produces a negative number
		//steering = steering * -1;  // Make the number positive
   	//steering = map(steering, 0, 460, 0, 255);
 
	
		motorSpeedA = motorSpeedA - xMapped;
		motorSpeedB = motorSpeedB + xMapped;
 
		// Don't exceed range of 0-255 for motor speeds
 
		if (motorSpeedA < 0)motorSpeedA = 0;
		if (motorSpeedB > 255)motorSpeedB = 255;
  }
  
    if (steering > 140){

		// Move Right
 
		// Map the number to a value of 255 maximum
 
	 int xmapped = map(steering, 140, 255, 0, 255);
			  
 
		motorSpeedA = motorSpeedA + xmapped;
		motorSpeedB = motorSpeedB - xmapped;
 
		// Don't exceed range of 0-255 for motor speeds
 
		if (motorSpeedA > 255)motorSpeedA = 255;
		if (motorSpeedB < 0)motorSpeedB = 0;		  
 
	}
	// Adjust to prevent "buzzing" at very low speed
 
	if (motorSpeedA < 8)motorSpeedA = 0;
	if (motorSpeedB < 8)motorSpeedB = 0;
 
	// Set the motor speeds
 
	analogWrite(enA, motorSpeedA);
	analogWrite(enB, motorSpeedB);

}







void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j1Button = 1;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j2Button = 1;

}