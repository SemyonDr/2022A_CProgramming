#ifndef BINARY_H
    #define BINARY_H

#include <stdio.h>
#include "Definitions.h"
#include "DataContainers.h"
#include "MyString.h"
#include "Data.h"
#include "Symbols.h"
#include "Errors.h"
#include "Parsing.h"

typedef struct ParsedCode {
    List* symbols;          /* Symbols table. */
    BinarySegment* code;
    BinarySegment* data;
} ParsedCode;

/* Parses instruction line and produces Ins structure allocated on heap.
   Structure contains istruction code and structures that describe arguments.
   Catches parsing and arguments errors.
   Moves position to end of the line.
   Assumes that provided arguments are correct and does not check them.
   Arguments:
    line    -- Instruction line from source code.
    pos     -- Position in line after label and before instruction name.
    errors  -- Errors list.
   Returns:
    Pointer to allocated instruction structure. NULL if parsing failed. */
Ins* ParseInstructionLine(char* line, int* pos, Errors* errors);

/* Determines type of the directive:
   string, data, or extern/entry. 
   Assumes that first word of the line after label starts with '.'.
   Advances position to first blank character after directive name.
   Arguments:
    line    -- Line containing statement. 
    pos     -- Position in line pointing to '.' before directive name.
   Returns:
    Directive type according to DirectivesEnum.
    Returns -1 if statement not recognized) */
int GetDirectiveType(char* line, int* pos);

/* Writes .data line arguments to data binary segment.
   Arguments:
    dataArgs    -- Parsed numeric values of .data arguments.
    data        -- Data binary segment. */
void DataToBinary(DynArr* dataArgs, BinarySegment* data);

/* Writes .string line argument to data binary segment.
   Arguments:
    line    -- Line containing .string directive.
    pos     -- Position in line after word .string and before arguments. 
    data    -- Data binary segment.
    lineNum -- Number of line in expanded source code.
    errors  -- List of errors.
    slr     -- Source line reference.
 */
void StringToBinary(char* arg, BinarySegment* data);

void InstructionToBinary(Ins* ins, BinarySegment* code);

#endif