#include <stdlib.h>
#include <stdio.h>
#include "execution.h"

/* This function prints text description
   of given parsing error. 
   Does not print message when error is NoError.
   Uses switch construction to determine text to print.
   Arguments:
    execRes - Execution result according to ParsingErrors enum.
   */
void PrintParsingError(int error) {
    switch (error) {
    case UndefinedCommand:
        printf("Undefined command name\n");
        break;
    case MissingParameter:
        printf("Missing parameter\n");
        break;
    case MissingComma:
        printf("Missing comma\n");
        break;
    case MultipleCommas:
        printf("Multiple consecutive commas\n");
        break;
    case IllegalComma:
        printf("Illegal comma\n");
        break;
    case ExtraText:
        printf("Extraneous text after end of command\n");
        break;
    case InvalidVariable:
        printf("Undefined complex variable\n");
        break;
    case InvalidNumber:
        printf("Invalid parameter - not a number\n");
    default:
        break;
    }
}

/* Returns number of parameters of given command.
   Assumes that argument is valid command type.
   Arguments:
    cmdType - Command type according to CommandTypes enum.
   Return:
    Number of command parameters. */
int GetNumberOfParameters(int cmdType) {
    if (cmdType == stop_cmd)
        return 0;
    if (cmdType == print_comp_cmd || cmdType == abs_comp_cmd)
        return 1;
    if (cmdType == add_comp_cmd || cmdType == sub_comp_cmd || cmdType == mult_comp_real_cmd || cmdType == mult_comp_img_cmd || cmdType == mult_comp_comp_cmd)
        return 2;
    if (cmdType == read_comp_cmd)
        return 3;
    
    return 0;
}

/* Returns array containing types of parameters of given command.
   Arguments:
    cmdType - Command type according to CommandTypes enum.
   Returns:
    Array of parameter types according to ParamTypes enum. */
int* GetParameterTypes(int cmdType) {
    int pNum; /* Number of parameters */
    int* pTypes; /* Array of types */

    /* If no parameters returning null. */
    if (cmdType == stop_cmd)
        return NULL;

    /* Getting number of parameters */
    pNum = GetNumberOfParameters(cmdType);

    /* Allocating memory */
    pTypes = (int*)malloc(sizeof(int)*pNum);
    if (pTypes == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    /* Writing types */
    if (cmdType == read_comp_cmd) {
        pTypes[0] = letter_param;
        pTypes[1] = number_param;
        pTypes[2] = number_param;
        return pTypes;
    }
    if (cmdType == print_comp_cmd || cmdType == abs_comp_cmd) {
        pTypes[0] = letter_param;
        return pTypes;
    }
    if (cmdType == add_comp_cmd || cmdType == sub_comp_cmd || cmdType == mult_comp_comp_cmd) {
        pTypes[0] = letter_param;
        pTypes[1] = letter_param;
        return pTypes;
    }
    if (cmdType == mult_comp_real_cmd || cmdType == mult_comp_img_cmd) {
        pTypes[0] = letter_param;
        pTypes[1] = number_param;
        return pTypes;
    }

    return pTypes;
}

/* Tries to parse and execute given command line.
   Arguments:
    cmd         - Command line string. Assumed to be null-terminated and with new line character removed.
    variables   - List of variables A-F
   Returns:
    -1          - Stop command encountered.
    0-8         - Parsing error code according to ParsingErrors enum.
                  0 Indicates no errors encountered.
    Algorithm:
     Uses parsing functions to extract command name and parse it.
     Then determines number and types of parameters associated with command type.
     Extracts parameters text and parses individual parameters.
     If everything passed parsing succesfully calls to Execute() and executes the command. */
int TryExecute(char* cmd, complex* variables) {
    char* name; /* Extracted command name */
    int cmdType; /* Command type according to CommandTypes enum */
    char** params_str; /* Array of command parameters */
    CmdParams params;
    int pNum; /* Number of command parameters */
    int* pTypes; /* Array of parameter types */
    int pError; /* Error value when getting parameters */
    int pos; /* Position value in command line */

    /* Getting command type */
    name = GetCommandName(cmd, &pos);
    cmdType = ParseCommandName(name);
    free(name); /* Freeing memory */

    /* Checking if command type is valid */
    if (cmdType == -1 || cmdType == unknown_cmd) {
        return UndefinedCommand;
    }

    /* Checking if stop command encnountered */
    if (cmdType == stop_cmd) {
        /* Checking for extra text after command */
        if (cmd[pos] != '\0')
            return ExtraText;
        return -1;
    }
        
    /* Getting parameters array in text form. */
    pNum = GetNumberOfParameters(cmdType);
    params_str = GetParameters(pNum, cmd, &pError, &pos);

    /* Checking parameters extraction errors */
    if (pError > 0)
        return pError;    

    /* Checking for extra text after parameters */
    if (cmd[pos] != '\0')
        return ExtraText;

    /* Parsing parameters */
    pTypes = GetParameterTypes(cmdType);
    params = ParseParameters(pNum, params_str, &pError, pTypes);
    /* Freeing memory*/
    free(pTypes);
    FreePointersArray((void**)params_str, pNum);

    /* Checking parameter parsing errors*/
    if (pError > 0)
        return pError;

    /* Executing command */
    Execute(cmdType, params, variables);

    return NoError;
}


/* Executes given command with given parameters.
   Assumes that arguments are valid.
   Arguments:
    cmd     - Type of command to execute according to CommandTypes enum.
    p       - Structure containing command parameters.
    var     - Array that contains named complex variables A-F.
    Algorithm:
     Uses switch construction to determine which actions to take to execute command.
     Uses complex number mathematical functions from "complex" module to perform
     operations, then uses PrintComplex to print results. */
void Execute(int cmd, CmdParams p, complex* var) {
    switch (cmd) {
        case print_comp_cmd: {
            PrintComplex(var[p.var_1]);
            return;
        }
        case read_comp_cmd: {
            var[p.var_1].Re = p.real_1;
            var[p.var_1].Im = p.real_2;
            return;
        }
        case add_comp_cmd: {
            complex sum;
            sum = AddComplex(var[p.var_1], var[p.var_2]);
            PrintComplex(sum);
            return;
        }
        case sub_comp_cmd: {
            complex diff;
            complex negative;
            complex minus_one = { -1.0, 0.0 };
            negative = MultiplyComplex(var[p.var_2], minus_one);
            diff = AddComplex(var[p.var_1], negative);
            PrintComplex(diff);
            return;
        }
        case mult_comp_real_cmd: {
            complex res;
            complex real;
            real.Re = p.real_1;
            real.Im = 0.0;
            res = MultiplyComplex(var[p.var_1], real);
            PrintComplex(res);
            return;
        }
        case mult_comp_img_cmd: {
            complex res;
            complex imag;
            imag.Re = 0.0;
            imag.Im = p.real_1;
            res = MultiplyComplex(var[p.var_1], imag);
            PrintComplex(res);
            return;
        }
        case mult_comp_comp_cmd: {
            complex res;
            res = MultiplyComplex(var[p.var_1], var[p.var_2]);
            PrintComplex(res);
            return;
        }
        case abs_comp_cmd: {
            complex res;
            res = AbsoluteComplex(var[p.var_1]);
            PrintComplex(res);
            return;
        }
    }
}