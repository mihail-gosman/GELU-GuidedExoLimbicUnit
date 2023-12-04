#include <Servo.h>

const char EOFn = '\n';

const char COMMAND = 1;
const char NUMBER = 2;

const char PAUSE = 3;

const int MAX_TOKENS = 32;
const int MAX_TOKEN_TYPE = 16;
const int MAX_TOKEN_VALUE = 16;
const int MAX_BUFFER = 16;
const int fast = 0;

void getInput();
void executeInput();


char buffer[MAX_BUFFER];
unsigned int bufferIndex = 0;
unsigned int tokensIndex = 0;
char inByte;
int activeInput = 0;
int pause = 0;


struct Token {
  char type;
  int value_int;
  char value_char;
} tokens[MAX_TOKENS];


Token tokenBuffer;
int buffIndex;
int isFirst = 1;
/*
struct Servos{
  Servo servo;
  int pos = 0;
} servomotor[2];
*/

void setup(){
  Serial.begin(9600);
  Serial.println("Program is running...");
//  for(int i=0; i<3; i++)
  //  servomotor[i].servo.attach(11-i);
}


void loop() {
  getInput();

  if(!pause)
  ;
}


void getInput()
{
  if (Serial.available()>0) {
    inByte = Serial.read();
    
    if (inByte == ' ' || inByte == '\n') {
      buffer[bufferIndex] = '\0';

      if(tokenBuffer.type == COMMAND)
      {
        if(!strcmp(buffer, "PAUSE"))
          tokenBuffer.value_char = PAUSE;
      }
      else 
      {
        tokenBuffer.value_int = atoi(buffer);
      }
      tokens[tokensIndex] = tokenBuffer;
      tokenBuffer.type = 0;
      tokenBuffer.value_char = 0;
      tokenBuffer.value_int = 0;
      tokensIndex += 1;
      isFirst = 1;
      bufferIndex = 0;
      if(inByte == '\n')
      {
        tokens[tokensIndex] = tokenBuffer;
        tokensIndex = 0;
        execute();
      }
    }
    else 
    {
      if(isFirst)
      {
        if(isalpha(inByte))
        {
          tokenBuffer.type = COMMAND;  
        }
        else 
        {
          tokenBuffer.type = NUMBER;
        }
        isFirst = 0;
      }
      buffer[bufferIndex] = inByte;
      bufferIndex += 1;
    }
    
  }
  
}


void execute(){
  for(int i=0; i<MAX_TOKENS; i++)
  if(tokens[i].type != 0)
    Serial.println((int)tokens[i].type);

  Serial.println('\n');
}