#include <Stepper.h>
#include "CLIParser.h"
#include "servoCtl.h"

unsigned const int SERVOS_NR = 3;
unsigned const int SERVO_PINS[SERVOS_NR] = {11, 10, 9};
unsigned const int BUFFER_SIZE = 16;


struct Servomotor Servomotors[3];


uint8_t error;
uint8_t pause = 1;

char input[BUFFER_SIZE];

void setup() {
  Serial.begin(9600);
  Serial.println(F("System Initializing..."));
  Serial.println(F("Version 2.0"));

  error = servoInit(Servomotors, SERVO_PINS, SERVOS_NR);
  if (error) {
    ; // error
  } else {
    ; // init success
  }
}

void loop() {
  getInput();

  if (!pause) {
    ;
  }

}
