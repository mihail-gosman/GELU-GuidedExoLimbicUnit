#include <Stepper.h>
#include <Servo.h>

// Define constants for command types
const char CONTINUE = 0x00;
const char STOP = 0x0F;
const char SERVO1_POS = 0x21;
const char SERVO2_POS = 0x22;

// Define start and end of text characters
const char SOT = '<';  // Start of text
const char EOT = '>';  // End of text

// Define buffer and array sizes
const int BUFFER_SIZE = 16;
const int MAX_ARG = 8;
const int MAX_TIME = 50;
const int SERVO_PIN[3] = {11, 10, 9};  // The pins used to control the 3 servos

// Data structure to represent a command
struct Command {
  char type;
  int arg[MAX_ARG];
};

// Data structure to represent a servo
struct myServo {
  Servo servomotor;
  int position;
  int inPosition;
  long deltaTime;
} servo[3];

// Global instance of the Command struct
Command command;

// Global variables
int stop = 0;
int error = 0;

// Setup function
void setup() {
  Serial.begin(9600);
  Serial.println("The program is running...");

  // Attach servos to pins
  for (int i = 0; i < 3; i++) {
    servo[i].servomotor.attach(SERVO_PIN[i]);
  }
}

// Main loop
void loop() {
  // Get and execute commands if no errors
  error = getCommand();
  if (!error) {
    error = executeCommand();
  }

  // Print servo position to serial monitor
  Serial.println(servo[0].position);

  // Update servos if not in stop state
  if (!stop) {
    update();
  }
}

// Function to receive a command from serial
int getCommand() {
  static char inByte;
  static char buffer[BUFFER_SIZE];
  static int bufferIndex = 0;
  static int argIndex = 0;
  static int isCommand = 1;

  if (Serial.available() > 0) {
    inByte = Serial.read();

    // Check for start of text character
    if (inByte == SOT) {
      do {
        // Continue reading until end of text character
        if (Serial.available() > 0) {
          inByte = Serial.read();

          // Check for space or end of text character
          if (inByte == ' ' || inByte == EOT) {
            buffer[bufferIndex] = '\0';

            // Process command or argument
            if (isCommand) {
              if (!strcmp(buffer, "STOP")) {
                command.type = STOP;
              } else if (!strcmp(buffer, "CONTINUE")) {
                command.type = CONTINUE;
              } else if (!strcmp(buffer, "SERVO1_POS")) {
                command.type = SERVO1_POS;
              } else if (!strcmp(buffer, "SERVO2_POS")) {
                command.type = SERVO2_POS;
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

// Function to execute the received command
int executeCommand() {
  if (command.type == STOP) {
    stop = 1;
  } else if (command.type == CONTINUE) {
    stop = 0;
  } else if (command.type == SERVO1_POS) {
    servo[0].inPosition = command.arg[0];
  } else if (command.type == SERVO2_POS) {
    servo[1].inPosition = command.arg[0];
  }

  return 0;
}

// Function to update servo positions
int update() {
  for (int i = 0; i < 3; i++) {
    if (servo[i].position != servo[i].inPosition) {
      if (millis() - servo[i].deltaTime >= MAX_TIME) {
        if (servo[i].position > servo[i].inPosition) {
          servo[i].position -= 1;
        } else {
          servo[i].position += 1;
        }
      }
    }
    servo[i].servomotor.write(servo[i].position);
  }
  return 0;
}
