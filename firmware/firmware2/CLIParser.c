#include "CLIParser.h"

// Function to parse input and fill the ParsedCommand structure
int parseInput(const char *input, ParsedCommand *parsedCommand)
{
    int inputIndex = 0;
    int commandIndex = 0;
    int argIndex = 0;

    // Parse the command
    while (input[inputIndex] != ' ' && input[inputIndex] != '\0')
    {
        parsedCommand->command[commandIndex++] = input[inputIndex++];
    }
    parsedCommand->command[commandIndex] = '\0';

    // Parse the arguments
    while (input[inputIndex] != '\0')
    {
        // Skip spaces
        while (input[inputIndex] == ' ')
        {
            inputIndex++;
        }

        // Parse the argument
        commandIndex = 0;
        while (input[inputIndex] != ' ' && input[inputIndex] != '\0')
        {
            parsedCommand->args[argIndex][commandIndex++] = input[inputIndex++];
        }
        parsedCommand->args[argIndex][commandIndex] = '\0';
        argIndex++;

        // Check if the maximum number of arguments is reached
        if (argIndex >= MAX_ARGS)
        {
            return 1; // Return 1 to indicate an error (too many arguments)
        }
    }

    parsedCommand->numArgs = argIndex;

    return 0; // Return 0 to indicate successful parsing
}

// Function to find and execute the command
int executeCommand(const ParsedCommand *parsedCommand, CommandEntry *commandDictionary, size_t dictionarySize)
{
    for (size_t i = 0; i < dictionarySize; ++i)
    {
        if (strcmp(parsedCommand->command, commandDictionary[i].command) == 0)
        {
            commandDictionary[i].handler(parsedCommand);
            return 0; // Return 0 to indicate successful command execution
        }
    }

    return 1; // Return 1 to indicate that the command was not found
}
