#ifndef _ODOMETRY_LIBRARY_H
#define _ODOMETRY_LIBRARY_H

#include "2_AUTO.h"

// Motor pin
const int motorR1 = 22;
const int motorR2 = 23;
const int motorL1 = 27;
const int motorL2 = 13;

// Encoder pin
const int encoderR1 = 25;
const int encoderR2 = 26;
const int encoderL1 = 2;
const int encoderL2 = 0;

// Odometry variable
float x = 0;          // x position
float y = 0;          // y position
float theta = 0;      // orientation
float delta_s = 0;    // traveled distance 
float delta_theta = 0;// orientation change

// Encoder's variable
float left_distance, right_distance;
const float PPR = 3600 * PI;
const float WHEEL_RADIUS = 15;  // in centimeter
const float WHEELBASE = 48;     // in centimeter
volatile long counterL = 0;
volatile long counterR = 0;
volatile long last_counterL = 0;
volatile long last_counterR = 0;

unsigned long pushTime = 0;
bool secondLoop = false;
bool auto_state = false;
bool killswitch = false;
bool square_state = false;

// Interrupt encoder function
void IRAM_ATTR countR1(){
  if (digitalRead(encoderR2) == LOW) {
    counterR++;
  } else {
    counterR--;
  }
}

void IRAM_ATTR countR2() {
  if (digitalRead(encoderR1) == LOW) {
    counterR--;
  } else {
    counterR++;
  }
}

void IRAM_ATTR countL1() {
  if (digitalRead(encoderL2) == LOW) {
    counterL++;
  } else {
    counterL--;
  }
}

void IRAM_ATTR countL2() {
  if (digitalRead(encoderL1) == LOW) {
    counterL--;
  } else {
    counterL++;
  }
}

class BaseOdo {
  public:
  void odometry() {
    // Calculate delta_s and delta_theta
    left_distance = 2 * PI * WHEEL_RADIUS * (counterL - last_counterL) / PPR;
    right_distance = 2 * PI * WHEEL_RADIUS * (counterR - last_counterR) / PPR;
    delta_s = (left_distance + right_distance) / 2;
    delta_theta = (right_distance - left_distance) / WHEELBASE;

    // Calculate position and orientation
    x += delta_s * cos(theta + delta_theta / 2);
    y += delta_s * sin(theta + delta_theta / 2);
    theta += delta_theta;
  }
  
  void moveRobot(int speedL, int speedR) {
    if (speedR < 0) {
      analogWrite(motorR1, 0);
      analogWrite(motorR2, abs(speedR));
    } else {
      analogWrite(motorR1, speedR);
      analogWrite(motorR2, 0);
    }

    if (speedL < 0) {
      analogWrite(motorL1, 0);
      analogWrite(motorL2, abs(speedR));
    } else {
      analogWrite(motorL1, speedL);
      analogWrite(motorL2, 0);
    }
  }

  void serialPrint() {
    // Print odometry calculation
    Serial.print("countR: ");Serial.print(counterR);
    Serial.print(" | countL: "); Serial.print(counterL);
    Serial.print(" | distR: "); Serial.print(right_distance);
    Serial.print(" | distL: "); Serial.print(left_distance);
    Serial.print(" | x: "); Serial.print(x);
    Serial.print(" | y: "); Serial.print(y);
    Serial.print(" | delta_s: "); Serial.print(delta_s);
    Serial.print(" | delta_theta: "); Serial.println(delta_theta);
  }

  void updateButtons() {
    if (ps5.Square()) {
      square_state = true;
    }
    if (ps5.Cross()) {
      killswitch = true; auto_state = false; square_state = false;
    }
    if (ps5.Circle()) {
      counterR = 0;
      if (millis() - pushTime >= 1000) {
        // secondLoop is a State to made auto mode only run once
        // need to Reset if wanna run auto mode once again
        secondLoop = true;
        if(secondLoop)
          auto_state = true;
        pushTime = millis();
      }
    }
  }
};

BaseOdo odometry;
#endif