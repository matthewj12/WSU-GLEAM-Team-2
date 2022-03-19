#define enA 5
#define in1 2
#define in2 4

#define enB 6
#define in3 7
#define in4 8


int motorSpeedA = 0;
int motorSpeedB = 0;
                               // uses only one joystick to controll the car so we can use the other for the servo                           
int xAxis = A0;
int yAxis = A1;

int xInitPos = 512;
int yInitPos = 512;

void setup() {

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(enA, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(enB, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}



void loop() {

  xInitPos = analogRead(xAxis);
  yInitPos = analogRead(yAxis);

  if (yInitPos < 460) {
    // This is Backward

    // Set Motor A backward

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // Set Motor B backward

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    //Determine Motor Speeds

    // As we are going backwards we need to reverse readings

    yInitPos = yInitPos - 460;  // This produces a negative number
    yInitPos = yInitPos * -1;   // Make the number positive

    motorSpeedA = map(yInitPos, 0, 460, 0, 255);
    motorSpeedB = map(yInitPos, 0, 460, 0, 255);
  }

  else if (yInitPos > 564) {
    // This is Forward

    // Set Motor A forward

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // Set Motor B forward

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    //Determine Motor Speeds

    motorSpeedA = map(yInitPos, 564, 1023, 0, 255);
    motorSpeedB = map(yInitPos, 564, 1023, 0, 255);

  } else {
    // This is Stopped

    motorSpeedA = 0;
    motorSpeedB = 0;
  }

   // Steering 

  if (xInitPos < 460)
	{
		// Move Left
 
		// As we are going left we need to reverse readings
 
		xInitPos = xInitPos - 460; // This produces a negative number
		xInitPos = xInitPos * -1;  // Make the number positive
 
		// Map the number to a value of 255 maximum
 
		xInitPos = map(xInitPos, 0, 460, 0, 255);
			  
 
		motorSpeedA = motorSpeedA - xInitPos;
		motorSpeedB = motorSpeedB + xInitPos;
 
		// Don't exceed range of 0-255 for motor speeds
 
		if (motorSpeedA < 0)motorSpeedA = 0;
		if (motorSpeedB > 255)motorSpeedB = 255;
 
	}

  else if (xInitPos > 564)
	{
		// Move Right
 
		// Map the number to a value of 255 maximum
 
		xInitPos = map(xInitPos, 564, 1023, 0, 255);
			  
 
		motorSpeedA = motorSpeedA + xInitPos;
		motorSpeedB = motorSpeedB - xInitPos;
 
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



