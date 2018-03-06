/*
	vr-wind
	By Daniel Chote 2018
  https://github.com/dchote/vr-wind
*/

// Pins
int leftFanPin = 5;
int rightFanPin = 6;

int maxFanSpeedPin = 0;


// serial data
int delimiter = 59; // means: ; 
char data[80]; 


// state
int maxFanSpeed = 0;
int leftFanSpeed = 0;
int rightFanSpeed = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(leftFanPin, OUTPUT);
  pinMode(rightFanPin, OUTPUT);
}

void loop() {
  maxFanSpeed = map(analogRead(maxFanSpeedPin), 0, 1023, 0, 255);
  
  analogWrite(leftFanPin, maxFanSpeed);
  analogWrite(rightFanPin, maxFanSpeed);
  
  Serial.println(maxFanSpeed);
  
  delay(500);
}