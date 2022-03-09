/* Program description:
    Program performs operations on complex numbers - addition, multiplication (in some variations)
    and taking absolute value.
    Complex numbers are entered by user to available named variables A B C D E F.
    To set and print variables and perform mathematical operations
    user enter commands with parameters.
   Program operations: 
    Program takes user input, determines which command is entered with which 
    parameters and executes it, or prints a message if command, or command parameters
    are not valid.
   Modules:
    -- complex
        Contains definition of complex number structure and mathematical functions
        that can be performed on this number.
    -- parsing
        Contains functions for breaking input line to command name and individual parameters.
        And functions that parse command name and parameters and convert text to values.
        Also contains:
         - Helper functions used for text processing.
         - Enumeration of possible parsing errors.
         - Structure that represents parsed command parameters
    -- execution
        Contains function that uses parsing module to parse command and reports parsing errors.
        Function that prints parsing errors.
        Function that actually calls for mathematical functions to execute parsed command and print results.
        Also contains additional dictionary-like helper functions that provide command-specific
        parameters for parsing.
    -- mycomp
        Central module that contains main function. Initializes program state, takes user input
        and calls for execution of user entered commands.
   Algorithm:
    Program takes input line by line. 
    On each line TryExecute is called.
    TryExecute is central function that uses other functions.
    First it tries to get command name and parse it.
    If command is determined then TryExecute tries to take and parse
    entered parameters according to list of the parameters required by particular command.
    If parameters parsed successfuly they are saved in structure CmdParams.
    Then TryExecute calls Execute function and passes command type and parsed parameters.
    Execute calls to functions from "complex.c" module and performs command operations and prints results.
   Input:
    Input consists of commands with parameters taken line by line.
    Output is results of execution of entered commands and possible commands syntax errors.
    Input is taken until "stop" command is entered by user. Sudden end of input stream
    is considered an error and stops program execution.
    Empty lines are ignored.
   Assumptions:
    Maximum length of every input line is considered to be CMD_MAX_LEN (defined in mycomp.h) including new line and termination symbols.
    Valid command is assumed to have format "command_name param1, param2, param3".


*/
#include <stdlib.h>
#include <stdio.h>
#include "mycomp.h"

/* Removes new line character from given 
   string and replaces it with line termination character.
   Arguments:
    s   - String. */
void RemoveNewLine(char* s) {
    int i;
    i=0;
    while (s[i] != '\0') {
        if (s[i] == '\n') {
            s[i] = '\0';
            return;
        }
        i++;
    }
}

/* Creates array of 6 complex variables A-F
   and initializes values to zeroes.
   Returns:
    Array of 6 complex variables. */
complex* InitializeVariables() {
    complex* variables; /* Variables array */
    int i; /* Iterator */
    complex init; /* Initial value for variable */
    init.Re = 0.0;
    init.Im = 0.0;

    /* Allocating memory */
    variables = (complex*)malloc(sizeof(complex)*6);
    if (variables == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    /* Initializing variables*/
    for (i=0; i<6; i++)
        variables[0] = init;

    return variables;
}


/* Main function. Creates list of variables and takes input lines in a loop
   until stop condition is met. 
   Passes taken lines to execution and prints result. */
int main() {    
    complex* variables; /* Array for holding variables A-F*/
    int stop = 0; /* Flag for deciding if input loop should be stopped */
    char* cmd; /* Buffer for storing user input command. */

    /* Initializing variables */ 
    variables = InitializeVariables();

    /* Allocating memory for input buffer. */
    cmd = (char*)malloc(sizeof(char)*CMD_MAX_LEN);
    if (cmd == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    /* Printing explanation for the user */
    printf("This program performs mathematical operations on complex numbers represented by variables.\n");
    printf("Please enter commands to set and print variable values and perform operations.\n");
    printf("Available variables for storing complex numbers: A B C D E F\n");
    printf("Maximum length of a command line is %d symbols, including new line symbol.\n", CMD_MAX_LEN-1);
    
    /* Taking input from the user until stop condition is met. */
    while (!stop) {
        int pos = 0; /* Variable used for skipping spaces */
        int exec_res = 0; /* Variable for storing command execution result. */

        printf("Enter command: "); /* Invintation for input */

        /* Getting the command. */
        if (fgets(cmd, CMD_MAX_LEN, stdin) == NULL) { 
            printf("\nUnexpected end of input\n");
            exit(1);
        }

        RemoveNewLine(cmd); /* Removing new line character from input */
        printf("%s\n", cmd); /* Printing command echo */

        /* If line is not empty trying to execute command */
        SkipSpaces(cmd, &pos);
        if (cmd[pos] != '\0') {
            /* Trying to execute the command. */
            exec_res = TryExecute(cmd, variables); 

            /* Checking if stop command encountered */
            if (exec_res == -1)
                stop = 1;

            /* Printing parsing error if encountered */    
            PrintParsingError(exec_res);
        }
    }

    /* Freeing memory */
    free(cmd);
    free(variables);

    return 0;
}