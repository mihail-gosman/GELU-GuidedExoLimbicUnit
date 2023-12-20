#include "Arduino.h"
#include <Servo.h>

struct Servomotor {
  Servo servomotor;
  uint8_t position;
  uint8_t inPosition;
  unsigned long time;
};


int servoInit(struct Servomotor Servomotors[]) {
  for (int i=11; i!=9; i--) {
    Servomotors[i].servomotor.attach(i);
    Servomotors[i].position = 90;
    Servomotors[i].time = millis();
  }
  return 0;
}

