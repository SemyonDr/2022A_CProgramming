#ifndef BINARY_H
    #define BINARY_H

#include <stdio.h>
#include "MyString.h"
#include "Data.h"
#include "Errors.h"
#include "Parsing.h"

typedef struct ParsedCode {
    List* instructions;     /* List of parsed instructions in order. */
    List* data;             /* List of parsed data lines in order. */
    List* symbols;          /* Symbols table. */
} ParsedCode;

char* GetInsNameByCode(int insCode);

Ins* ParseInstructionLine(char* line, List* errors, DArrayInt* slr, int lineNum);

#endif