#ifndef OUTPUT_H
    #define OUTPUT_H

#include <stdio.h>
#include "MyString.h"
#include "Data.h"
#include "DataContainers.h"
#include "Symbols.h"

/* Converts integer values from 0-15 range to 
   corresponding hex digit as character.
   Arguments:
    val     -- Integer value.
   Returns:
    Character that represents hex digit corresponding to that value.
    If val is out of range 0-15 ' ' (blank) is returned. */
char IntToHexDigit(int val);

/* Converts 20 bit binary word to text representation
   of that word in "special" base. 
   Most significant bits in val past bit 19 are not
   expected and will be ignored.
   Arguments:
    val     -- 20 bit binary value.
    word    -- Buffer for writing string representation of val in "special" base. */
void BinaryToSpecial(int val, char word[15]);

/* Creates and fills .ob object file.
   Writes code and binary segments in "special" base to .ob file.
   Assumes that binary segment arguments are correct.
   Arguments:
    fileName    -- Name of the file without extension.
    code        -- Code binary segment.
    data        -- Data binary segment.
*/
void WriteBinaryToObject(char* fileName, BinarySegment* code, BinarySegment* data);

/* Creates and fills entries .ent file.
   Assumes that arguments are correct.
   Arguments:
    fileName    -- Source file name without extension.
    symbols     -- Symbols table. */
void WriteEntries(char* fileName, List* symbols);

/* Writes external symbols info to .ext file. 
   Arguments:
    fileName    -- Name of source file without extension.
    symbols     -- Symbols table.
    references  -- List of symbol references in arguments. */
void WriteExterns(char* fileName, List* symbols, List* references);
#endif