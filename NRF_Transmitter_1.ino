#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

// Define the digital inputs
#define jB1 1  
#define jB2 0  



RF24 radio(9, 10);   //  (CE, CSN)
const byte address[6] = "00001"; // Address


struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;

};

Data_Package data; //Create a variable with the above structure


void setup() {

  Serial.begin(9600);
// Define the radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);

  // Activate the Arduino internal pull-up resistors
  pinMode(jB1, INPUT_PULLUP);
  pinMode(jB2, INPUT_PULLUP);


//  default values 0 to 255.
  data.j1PotX = 127; 
  data.j1PotY = 127;
  data.j1Button = 1;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j2Button = 1;

}





void loop() {
  // analog inputs
  data.j1PotX = map(analogRead(A1), 0, 1023, 0, 255); // analog read value into a BYTE value from 0 to 255
  data.j1PotY = map(analogRead(A0), 0, 1023, 0, 255);

  // digital inputs
  data.j1Button = digitalRead(jB1);
  data.j2Button = digitalRead(jB2);

  // Send to receiver
  radio.write(&data, sizeof(Data_Package));
}











