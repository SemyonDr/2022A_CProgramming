#ifndef BINARY_H
    #define BINARY_H

#include <stdio.h>
#include "MyString.h"
#include "Definitions.h"
#include "Data.h"
#include "DataContainers.h"
#include "Errors.h"
#include "Parsing.h"

/* Determines type of the directive:
   string, data, or extern/entry. 
   Assumes that first word of the line after label starts with '.'.
   Advances position to first blank character after directive name.
   Arguments:
    line    -- Line containing statement. 
    pos     -- Position in line pointing to '.' before directive name.
   Returns:
    Directive type according to DirectivesEnum.
    Returns -1 if statement not recognized. */
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

/* Translates given instruction structure to binary words
   and writes them into code binary segment.
   Creates label references if found and adds them to list.
   Arguments:
    ins          -- Instruction structure.
    code         -- Code binary segment.
    references   -- List of label arguments (label references).
    lineNum      -- Number of line in expanded source file where instruction originates. */
void InstructionToBinary(Ins* ins, BinarySegment* code, List* references, int lineNum);

/* Translates given statement of any kind (instruction, or directive)
   to binary words and writes them to appropriate binary segment.
   Also produces structure that describes label before the line if it is present.
   Arguments:
    line        -- String that contains statement.
    references  -- List of label arguments (label references).
    code        -- Code binary segment.
    data        -- Data binary segment.
    errors      -- Errors list.
   Returns:
    If statement opened with a label symbol is created with appropriate address and attribute fields.
    If line not contained opening label returns NULL (not considere a failure). */
Symbol* StatementToBinary(char *line, List *unresolved, BinarySegment *code, BinarySegment *data, Errors *errors);

/* Reads expanded source file and produces binary segments with unresolved label arguments.
   Also produces symbols table and list of label references.
   After this step it is neccessary only to resolve label references.
   Arguments:
    fileName    -- Name of source file without extension.
    code        -- Code binary segment.
    data        -- Data binary segment.
    symbols     -- Symbols table.
    references  -- List of references to labels as instruction arguments.
    errors      -- Errors list. */
void ProduceInitialBinary(char* fileName, BinarySegment* code, BinarySegment* data, List* symbols, List* references, Errors* errors);

/* Resolves label references in binary code segment.
   Arguments:
    code        -- Code binary segment.
    symbols     -- Symbols table.
    references  -- List of label references.
    errors      -- Errors list. */
void ResolveReferences(BinarySegment* code, List* symbols, List* references, Errors* errors);
#endif