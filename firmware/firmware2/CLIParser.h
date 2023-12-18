#ifndef CLIPARSER_H_INCLUDED
#define CLIPARSER_H_INCLUDED

#define MAX_ARGS 5  // Maximum number of arguments allowed
#define MAX_ARG_LENGTH 16  // Maximum length of each argument
#define MAX_INPUT_LENGTH (MAX_ARGS * (MAX_ARG_LENGTH + 1))  // Maximum length of the entire input string

#include <stdio.h>

// Definition of the ParsedCommand structure
typedef struct
{
    char command[MAX_ARG_LENGTH];        // Holds the command string
    char args[MAX_ARGS][MAX_ARG_LENGTH]; // Holds the arguments as an array of strings
    int numArgs;                         // Holds the number of arguments
} ParsedCommand;

// Definition of the CommandEntry structure
typedef struct
{
    char command[MAX_ARG_LENGTH];          // Holds the command string
    void (*handler)(const ParsedCommand*); // Function pointer to the command handler
} CommandEntry;

// Function to parse input and fill the ParsedCommand structure
int parseInput(const char*, ParsedCommand*);

// Function to find and execute the command
int executeCommand(const ParsedCommand*, CommandEntry*, size_t);

#endif // CLIPARSER_H_INCLUDED
