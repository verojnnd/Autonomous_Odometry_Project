#ifndef _AUTO_LIBRARY_H
#define _AUTO_LIBRARY_H

#include "1_Odometry.h"

/*
  from the formula and calculation, it is obtained that 1 cm of robot movement requires 120 counters
  so, we can just determine needed distance in centimeter
*/

unsigned long program_start_time;
static bool flag = false;
static bool flag_run = false; 
int rotateSpeed = 50;
int normalSpeed = 60;

//rotation distance, when the right wheel moves forward and the left wheel moves backwards along half the circumference, or vice versa
//0.5 x phi x Diameter = 0.5 phi x 30 = 94.2477
float distanceRotate = 94.25;

class Auto {
  public:
  int state = 0;
  void turn(int newState) {
    if (abs(right_distance) <= distanceRotate) {
      Serial.println("turn");
    } else {
      state = newState;
      right_distance = 0;
      program_start_time = 0;
    }
  }
  void moveTurnRight(int newState) {
    if (millis() - program_start_time >= 100) {
      state = newState;
      Serial.println("move turn right");
      odometry.moveRobot(-rotateSpeed, rotateSpeed);
    }
  }
  void moveTurnLeft(int newState) {
    if (millis() - program_start_time >= 100) {
      state = newState;
      Serial.println("move turn left");
      odometry.moveRobot(rotateSpeed, -rotateSpeed);
    }
  }
  void moveForward(int newState){
    if (millis() - program_start_time >= 100) {
      state = newState;
      Serial.println("move forward");
      odometry.moveRobot(normalSpeed, normalSpeed);
    }
  }
  void moveBackward(int newState){
    if (millis() - program_start_time >= 100) {
      state = newState;
      Serial.println("move backward");
      odometry.moveRobot(-normalSpeed, -normalSpeed);
    }
  }
  void forward(int newState, int needed_distance) {
    if (right_distance <= needed_distance) {
      Serial.println("forward");
    } else {
      state = newState;
      right_distance = 0;
      program_start_time = 0;
    }
  }
  void backward(int newState, int needed_distance) {
    if (abs(right_distance) <= needed_distance) {
      Serial.println("backward");
    } else {
      state = newState;
      right_distance = 0;
      program_start_time = 0;
    }
  }
  void stop(int newState, int needed_distance){
    if (right_distance <= needed_distance) {
      Serial.println("STOP");
    } else {
      odometry.moveRobot(0,0);
      state = newState;
      right_distance = 0;
      program_start_time = 0;
    }
  }
  void loopSquare() {
    odometry.serialPrint();
    switch (state) {
      case 0:
        if (!flag) {
          right_distance = 0;
          state = 1; program_start_time = millis();
          flag = true;
        } break;

      case 1:
        moveForward(2); break;
        
      case 2:
        forward(3, 50); break; //forward 50 cm
      
      case 3:
        moveTurnRight(4); break;

      case 4:
        turn(1); break;
    }
  }

  void runToGoalPoint(){
    odometry.serialPrint();
    switch (state) {
      case 0:
        if (!flag_run) {
          state = 1;
          right_distance = 0;
          Serial.print(program_start_time); Serial.print(" | ");
          flag_run = true;
        }break;
        
      case 1:
        moveForward(2); break;
        
      case 2:
        forward(3, 60); break; //forward 60cm
      
      case 3:
        moveTurnRight(4); break;

      case 4:
        turn(5); break;
      
      case 5:
        moveForward(6); break;

      case 6:
        forward(7, 150); break; //forward 150cm

      case 7:
        moveTurnLeft(8); break;

      case 8:
        turn(9); break;
      
      case 9:
        moveForward(10); break;
      
      case 10:
        forward(11, 35); break; //forward 35cm

      case 11:
        moveTurnLeft(12); break;

      case 12:
        turn(13); break;
      
      case 13:
        moveForward(14); break;

      case 14:
        forward(15, 200); break; //forward 200cm

      case 15:
        moveTurnRight(16); break;

      case 16:
        turn(17); break;
      
      case 17:
        moveForward(18); break;
        
      case 18:
        stop(0, 87); break; //stop after forward 87cm

    }
  }
};

Auto automation;

#endif