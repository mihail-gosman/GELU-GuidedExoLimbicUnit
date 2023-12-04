#include <Servo.h>

const char EOkF = '\n';
const int MAX_INPUT = 32;

void execute();

char input[MAX_INPUT];
unsigned int index = 0;
char inByte;
int execute = 0;


struct Servos{
  Servo servo;
  int pos = 0;
} servomotor[2];
 

void setup(){
  Serial.begin(9600);
  Serial.println("Program is running...");
  for(int i=0; i<3; i++)
    servomotor[i].servo.attach(11-i);
}


void loop() {
  if (Serial.available()>0) {
    inByte = Serial.read();
    if (inByte == '\n') {
      Serial.println(input);
    }
    else {
      input[index] = inByte;
    }
    index += 1;
  }
}


void execute(){
  
}