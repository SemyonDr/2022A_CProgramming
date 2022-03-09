#ifndef MYCOMP_H
    #define MYCOMP_H    

#include "complex.h"
#include "parsing.h"
#include "execution.h"
#define CMD_MAX_LEN 256

/* Removes new line character from given 
   string and replaces it with line termination character.
   Arguments:
    s   - String. */
void RemoveNewLine(char* s);

/* Creates array of 6 complex variables A-F
   and initializes values to zeroes.
   Returns:
    Array of 6 complex variables. */
complex* InitializeVariables();

#endif /* MYCOMP_H */