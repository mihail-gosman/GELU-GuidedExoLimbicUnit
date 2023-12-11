#include <Stepper.h>
#include <Servo.h>


const char CONTINUE = 0x00;  //
const char STOP =  0x0F;      // 
const char SERVO1_POS = 0x21; 

const char SOT =  '>';//0x02;       //  start of text
const char EOT =  '\n';//0x03;       //  end of text


const int BUFFER_SIZE = 16;
const int MAX_ARG = 8;                  
const int SERVO_PIN[3] = {11, 10, 9};   //  The pins used to controll the 3 servos


int getCommand();    
int executeCommand();


struct Command{
  char type;
  int arg[MAX_ARG];
};

// 
struct myServo{
  Servo servomotor;
  int position;
  int inPosition;
  long deltaTime;
} servo[3];


Command command;


int stop = 0;
int error = 0;


void setup(){
  Serial.begin(9600);
  Serial.println("The program is running...");

  for(int i = 0; i < 3; i++){
    servo[i].servomotor.attach(SERVO_PIN[i]);
  }
}


void loop(){
  error = getCommand();
  if (!error) {
    error = executeCommand();
  }
  Serial.println(servo[0].inPosition);
  if (!stop) {
    ;
  }
}


int getCommand(){
  static char inByte;
  static char buffer[BUFFER_SIZE];
  static int bufferIndex = 0;
  static int argIndex = 0;
  static int isCommand = 1;
 
  if (Serial.available() > 0) {
    inByte = Serial.read();
    //

    if (inByte == SOT) {
      do {
        if (Serial.available() > 0) {
          inByte = Serial.read();
          
          if(inByte == ' ' || inByte == EOT) {
            buffer[bufferIndex] = '\0';
            
            if (isCommand) {
              if (!strcmp(buffer, "STOP")) {
                command.type = STOP;
              } else if (!strcmp(buffer, "CONTINUE")) {
                command.type = CONTINUE;
              } else if (!strcmp(buffer, "SERVO1_POS")) {
                command.type = SERVO1_POS;
              }

              isCommand = 0;
            } else {
              command.arg[argIndex] = atoi(buffer);
              argIndex += 1;
            }

            bufferIndex = 0;
          } else {
            buffer[bufferIndex] = inByte;
            bufferIndex += 1;
          }
        }
        
      } while (inByte != EOT);

      argIndex = 0;
      isCommand = 1;
    }
  }
  return 0;
}


int executeCommand(){
  if (command.type == STOP) {
    stop = 1;
  } else if (command.type == CONTINUE) {
    stop = 0;
  } else if (command.type == SERVO1_POS) {
    servo[0].inPosition = command.arg[0];
  }

  return 0;
}

