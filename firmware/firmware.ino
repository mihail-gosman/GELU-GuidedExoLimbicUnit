#include <Stepper.h>
#include <Servo.h>

// Define constants for command types
const char CONTINUE = 0x00;
const char STOP = 0x0F;
const char SERVO_POS = 0X20;
const char SERVO1_POS = 0x21;
const char SERVO2_POS = 0x22;

// Define start and end of text characters
const char SOT = '<';  // Start of text
const char EOT = '>';  // End of text

// Define buffer and array sizes
const int BUFFER_SIZE = 16;
const int MAX_ARG = 8;
const int SERVO_DELAY = 50;
const int STEPS_PER_REVOLUTION = 2038;   // Defines the number of steps per rotation
const int SERVO_PIN[3] = {11, 10, 9};  // The pins used to control the 3 servos
const int STEPPER_PIN[4] = {12, 8, 7, 4}; // IN1 IN2 IN3 IN4

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
  unsigned long deltaTime;
} servo[3];

// Data structure for LED
struct led {
  int state = 0;
  unsigned long deltaTime;
};

Stepper  myStepper = Stepper(STEPS_PER_REVOLUTION, STEPPER_PIN[0], STEPPER_PIN[1], STEPPER_PIN[2], STEPPER_PIN[3]);

// Global instance of the Command struct
Command command;

// Global instance for the built in LED
  led ledIndicator;

// Global variables
int stop = 0;
int error = 0;

// Setup function //
void setup() {
  Serial.begin(9600);
  Serial.println("The program is running...");
  // Attach servos to pins
  initializeServos();  
  // Set the led indicator
  pinMode(LED_BUILTIN, OUTPUT);
}

// Main loop   //
void loop() {
  // Get and execute commands if no errors
  error = getCommand();
  if (error) {
    errorHandler(error);
  } else {
    error = executeCommand();
  }

  // Led indicator
  ledUsage();

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
              parseCommand(buffer);

              isCommand = 0;
            } else {
              if (argIndex < MAX_ARG) {
                command.arg[argIndex] = atoi(buffer);
                argIndex += 1;
              } else {
                return 2; // Too many arguments error
              }
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

// Function to parse the command **
int parseCommand(char * buffer) {
  if (!strcmp(buffer, "STOP")) {
    command.type = STOP;
  } else if (!strcmp(buffer, "CONTINUE")) {
    command.type = CONTINUE;
  } else if (!strcmp(buffer, "SERVO1_POS")) {
    command.type = SERVO1_POS;
  } else if (!strcmp(buffer, "SERVO2_POS")) {
    command.type = SERVO2_POS;
  } else if (!strcmp(buffer, "SERVO_POS")) {
    command.type = SERVO2_POS;
  } else {
    return 1;  // Unknown command error
  }
}

// Function to execute the received command  **
int executeCommand() {
  if (command.type == STOP) {
    stop = 1;
  } else if (command.type == CONTINUE) {
    stop = 0;
  } else if (command.type == SERVO1_POS) {
    servo[0].inPosition = command.arg[0];
  } else if (command.type == SERVO2_POS) {
    servo[1].inPosition = command.arg[0];
  } else if (command.type == SERVO_POS) {
    servo[0].inPosition = command.arg[0];
    servo[1].inPosition = command.arg[1];
  }

  return 0;
}

// Function to update servo positions
int update() {
  for (int i = 0; i < 3; i++) {
    if (servo[i].position != servo[i].inPosition) {
      if (millis() - servo[i].deltaTime >= SERVO_DELAY) {
        if (servo[i].position > servo[i].inPosition) {
          servo[i].position -= 1;
        } else {
          servo[i].position += 1;
        }
        servo[i].deltaTime = millis();
      }
    }
    servo[i].servomotor.write(servo[i].position);
  }
  return 0;
}

void initializeServos() {
  for (int i = 0; i < 3; i++) {
      servo[i].servomotor.attach(SERVO_PIN[i]);
    }
}

// Function to handle errors
void errorHandler(int errorCode) {
  switch (errorCode) {
    case 1:
      Serial.println("Error: Unknown command");
      break;
    case 2:
      Serial.println("Error: Too many arguments");
      break;
    // Add more cases for additional error scenarios as needed
    default:
      Serial.println("Error: Unknown error");
  }
}

void ledUsage() {
  if (ledIndicator.state == 0) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}