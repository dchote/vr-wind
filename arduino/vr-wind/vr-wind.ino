/*
  vr-wind
  By Daniel Chote 2018
  https://github.com/dchote/vr-wind
*/

#include <elapsedMillis.h>

// Pins
int leftFanPin = 9;
int rightFanPin = 10;

int fanAdjustPin = 0;

// state
elapsedMillis timeElapsed;
int fanValueTimeout = 3000; // 3 second timeout

int fanAdjustValue = 0;

int minFanSpeed = 0;
int maxFanSpeed = 0;

int leftFanValue = 0;
int rightFanValue = 0;

int leftFanSpeed = 0;
int rightFanSpeed = 0;

void setup() {
  Serial.begin(9600);
  Serial.println();
  
  delay(500);
  
  Serial.println("vr-wind");
  timeElapsed = 0;
  
  pinMode(leftFanPin, OUTPUT);
  pinMode(rightFanPin, OUTPUT);
  
  // set PWM mode
  TCCR1B = TCCR1B & B11111000 | B00000001;
}

void loop() {
  fanAdjustValue = map(analogRead(fanAdjustPin), 0, 1023, 0, 255);
  
  if (fanAdjustValue < 10) {
    // turn off fans
    minFanSpeed = 0;
    maxFanSpeed = 0;
  } else {
    if (fanAdjustValue < 240) {
      // allow fan to not spin if not needed
      minFanSpeed = 0;
    } else {
      // low fan idle speed
      minFanSpeed = 20;
    }
    // scale the adjustment
    maxFanSpeed = 255 - map(fanAdjustValue, 0, 255, 215, 0);
  }
  
  if (leftFanValue < minFanSpeed) {
    leftFanValue = minFanSpeed;
  }
  
  if (rightFanValue < minFanSpeed) {
    rightFanValue = minFanSpeed;
  }
  
  int serialTimeout = 0;
  while (Serial.available() > 0) {
    serialTimeout++;
    
    leftFanValue = Serial.parseInt();
    Serial.read(); // seperator
    rightFanValue = Serial.parseInt();
    
    if (Serial.read() == '\n') {
      timeElapsed = 0;
      /*
      Serial.print("Recieved: ");
      Serial.print(leftFanValue);
      Serial.print(" ");
      Serial.print(rightFanValue);
      Serial.print(" Min: ");
      Serial.print(minFanSpeed);
      Serial.print(" Max: ");
      Serial.print(maxFanSpeed);
      Serial.println();
      */
      break;
    }
    
    if (serialTimeout > 100) break;
  }
  
  if ((leftFanValue > 0 || rightFanValue > 0) && timeElapsed > fanValueTimeout) {
    //Serial.println("timeout");
    leftFanValue = 0;
    rightFanValue = 0;
    timeElapsed = 0;
  }
  
  leftFanSpeed = map(leftFanValue, 0, 255, 0, maxFanSpeed);
  rightFanSpeed = map(rightFanValue, 0, 255, 0, maxFanSpeed);
  
  analogWrite(leftFanPin, leftFanSpeed);
  analogWrite(rightFanPin, rightFanSpeed);
  
  // debug print current values
  /*
  Serial.print("Left Speed: ");
  Serial.print(leftFanSpeed);
  Serial.print(" Right Speed: ");
  Serial.print(rightFanSpeed);
  Serial.println();
  delay(500);
  */
}