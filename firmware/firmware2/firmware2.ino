#include <Stepper.h>
#include "CLIParser.h"
#include "servoCtl.h"

struct Servomotor Servomotors[3];

CommandEntry {
  {}
} Dict[];


uint8_t error;
uint8_t pause = 1;

void setup() {
  Serial.begin(9600);
  Serial.println(F("System Initializing..."));
  Serial.println(F("Version 2.0"));

  error = servoInit(Servomotors);
  if (error) {
    ; // error
  } else {
    ; // init success
  }
}

void loop() {

  if (!pause) {
    ;
  }

}
