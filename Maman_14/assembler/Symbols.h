#ifndef SYMBOLS_H
    #define SYMBOLS_H

#include <stdlib.h>
#include <stdio.h>
#include "MyString.h"
#include "Data.h"
#include "Errors.h"

/* Structure that represents element of symbols table. */
typedef struct Symbol {
   char name[32];    /* Symbol label. */
   int adress;       /* Address represented by label (decimal) */
   int attributes;   /* Binary 4-bit value that represent symbol attributes:
                        [8]code-[4]data-[2]extern-[1]entry
                        For example 0101 - symbol has data and entry attributes. */
} Symbol;

/* Data about label argument of instruction - referenced label.
   Saved while instruction are parsed and used to resolve 
   argument later using symbols table. */
typedef struct LabelReference {
   char name[32];    /* Label name. */
   int address;      /* Instruction or data counter where label content is referenced. */
   int origin;       /* Number of line in original source code (not expanded) where label referenced. */
} LabelReference;

/* Checks if attribute "code" is set for symbol.
   Arguments:
    attributes -- attributes number from Symbol structure.
   Returns:
    0 -- attribute is set.
    1 -- Attribute not set. */
int IsCode(Symbol* smb);

/* Checks if attribute "data" is set for symbol.
   Arguments:
    attributes -- attributes number from Symbol structure.
   Returns:
    0 -- attribute is set.
    1 -- Attribute not set. */
int IsData(Symbol* smb);

/* Checks if attribute "extern" is set for symbol.
   Arguments:
    attributes -- attributes number from Symbol structure.
   Returns:
    0 -- attribute is set.
    1 -- Attribute not set. */
int IsExtern(Symbol* smb);

/* Checks if attribute "entry" is set for symbol.
   Arguments:
    attributes -- attributes number from Symbol structure.
   Returns:
    0 -- attribute is set.
    1 -- Attribute not set. */
int IsEntry(Symbol* smb);

/* Allocates new symbol structure.
   Makes new copy of label string, only MAX_LABEL_LEN first characters will be copied.
   Arguments:
    label       -- Label name string.
    address     -- Address of instruction where symbol declared.
    attribute   -- Attribute of the symbol in declaration line according to SymbolAttributesEnum.
   Returns:
    New Symbol structure. */ 
Symbol* CreateSymbol(char* label, int address, int attribute);

/* Adds symbol to symbols table.
   Arguments:
    symbols    -- Symbols table
    new_smb    -- Symbol to add.
    errors     -- Errors list.*/
void AddSymbol(List* symbols, Symbol* new_smb, Errors* errors);

/* Searches symbol in symbols table by given name.
   Arguments:
    symbols    -- Symbols table.
    label      -- Name of the label to search.
   Returns:
    Symbol with given name.
    NULL if symbol not found.  */
Symbol* FindSymbolByName(List* symbols, char* label);

/* Allocates new LabelReference structure
   and fills it with provided parameters.
   Arguments:
    label      -- Label name.
    address    -- Address of data word where label value should be substituted.
    origin     -- Number of line where label referenced as argument.
   Returns:
    LabelReference structure allocated on heap. */
LabelReference* CreateLabelReference(char* label, int address, int origin);

/* Validates symbols table.
   Checks for last possible error (others checked when symbol is added)
    - if symbols marked as entry not marked also as code or data.
   Arguments:
    symbols    -- Symbols table.
    errors     -- Errors list. */
void ValidateSymbolsTable(List* symbols, Errors* errors);

#endif