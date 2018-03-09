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
  TCCR1B = (TCCR1B & 0b11111000) | 0x01;
}

void loop() {
  fanAdjustValue = map(analogRead(fanAdjustPin), 0, 1023, 0, 255);
  
  if (fanAdjustValue < 10) {
    // turn off fans
    minFanSpeed = 0;
    maxFanSpeed = 0;
  } else {
    if (fanAdjustValue < 250) {
      // allow fan to not spin if not needed
      minFanSpeed = 0;
    } else {
      // low fan idle speed
      minFanSpeed = 15;
    }
    
    maxFanSpeed = 255;
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
    leftFanValue = minFanSpeed;
    rightFanValue = minFanSpeed;
    timeElapsed = 0;
  }
  
  // lets scale fan speed in to buckets, this is arbitrary based off what I felt seemed right.
  leftFanSpeed = fanSpeedAdjust(calculateFanSpeed(leftFanValue));
  rightFanSpeed = fanSpeedAdjust(calculateFanSpeed(rightFanValue));
  
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

int calculateFanSpeed(int requestedValue) 
{
  if (requestedValue == 0) {
    return 0;
  } else if (requestedValue <= 50) {
    return map(requestedValue, 0, 50, 10, 18);
  } else if (requestedValue <= 100) {
    return map(requestedValue, 50, 100, 18, 25);
  } else if (requestedValue <= 150) {
    return map(requestedValue, 100, 150, 25, 50);
  } else {
    return map(requestedValue, 150, 255, 50, 255);
  }
}

int fanSpeedAdjust(int requestedSpeed)
{
  float adjustment = (float)fanAdjustValue / 255;
  
  requestedSpeed = round((float)requestedSpeed * adjustment);
  
  if (requestedSpeed < minFanSpeed) {
    requestedSpeed = minFanSpeed;
  }
  
  if (requestedSpeed > maxFanSpeed) {
    requestedSpeed = maxFanSpeed;
  }
  
  return requestedSpeed;
}