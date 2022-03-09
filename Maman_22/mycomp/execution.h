#ifndef EXECUTION_H
    #define EXECUTION_H

#include "complex.h"
#include "parsing.h"

/* This function prints text description
   of given parsing error. 
   Does not print message when error is NoError.
   Arguments:
    execRes - Execution result according to ParsingErrors enum.
   */
void PrintParsingError(int error);

/* Returns number of parameters of given command.
   Assumes that argument is valid command type.
   Arguments:
    cmdType - Command type according to CommandTypes enum.
   Return:
    Number of command parameters. */
int GetNumberOfParameters(int cmdType);

/* Returns array containing types of parameters of given command.
   Arguments:
    cmdType - Command type according to CommandTypes enum.
   Returns:
    Array of parameter types according to ParamTypes enum. */
int* GetParameterTypes(int cmdType);

/* Tries to parse and execute given command line.
   Arguments:
    cmd         - Command line string. Assumed to be null-terminated and with new line character removed.
    variables   - List of variables A-F
   Returns:
    -1          - Stop command encountered.
    0-8         - Parsing error code according to ParsingErrors enum.
                  0 Indicates no errors encountered. */
int TryExecute(char* cmd, complex* variables);

/* Executes given command with given parameters.
   Assumes that arguments are valid.
   Arguments:
    cmd     - Type of command to execute according to CommandTypes enum.
    p       - Structure containing command parameters.
    var     - Array that contains named complex variables A-F. */
void Execute(int cmd, CmdParams p, complex* var);

#endif /* EXECUTION_H*/