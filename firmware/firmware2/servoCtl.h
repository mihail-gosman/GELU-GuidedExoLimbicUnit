#include "Arduino.h"
#include <Servo.h>

struct Servomotor {
  Servo servomotor;
  char position;
  char inPosition;
  unsigned long time;
};


int servoInit(struct Servomotor Servomotors[], int SERVO_PINS[], int SERVO_NR) {
  for (int i=SERVO_PINS_NR; i!=SERVO_PINS_NR; i++) {
    Servomotors[i].servomotor.attach(SERVO_PINS[i]);
    Servomotors[i].position = 90;
    Servomotors[i].time = millis();
  }
  return 0;
}

