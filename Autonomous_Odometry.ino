#include <ps5Controller.h>
#include "2_AUTO.h"
#include "1_Odometry.h"

unsigned long programTime;

void setup() {
  Serial.begin(921600);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);

  pinMode(encoderR1, INPUT_PULLUP);
  pinMode(encoderR2, INPUT_PULLUP);
  pinMode(encoderL1, INPUT_PULLUP);
  pinMode(encoderL2, INPUT_PULLUP);

  attachInterrupt(encoderR1, countR1, RISING);
  attachInterrupt(encoderR2, countR2, RISING);
  attachInterrupt(encoderL1, countL1, RISING);
  attachInterrupt(encoderL2, countL2, RISING);

  ps5.begin("48:18:8D:EC:69:F7"); //MAC Address PS5 Controller
  Serial.println("STARTING...");
}

void loop() {
  if (ps5.isConnected()) {
    odometry.updateButtons();
    Serial.println("CONNECT!!!");
    if (auto_state) {
      automation.runToGoalPoint();
    }
    if (square_state) {
      automation.loopSquare();
    }
    if (killswitch) {
      odometry.moveRobot(0,0);
    }
    else {
      odometry.moveRobot(0,0);
    }
  } else {
    odometry.moveRobot(0,0);
    if (millis() - programTime > 200) {
      Serial.println("Not connected...");
      programTime = millis();
    }
  }
}
