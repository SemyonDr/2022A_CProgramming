#ifndef DATACONTAINERS_H
    #define DATACONTAINERS_H

#include <stdlib.h>
#include <stdio.h>
#include "MyString.h"
#include "Data.h"

/* Structure that describes info about macro.*/
typedef struct MacroInfo {
    char* name;  /* Macro name */
    long body_pos; /* Index position in file where macro body starts. */
    int body_line_num; /* Number of line in source file where macro body starts. */
    int num_lines; /* Length of macro body definition in lines (excluding name line and endm line) */    
} MacroInfo;


/* Structure that represents instruction argument.
   Can represent any kind of argument - number, label, label with index, register.
    */
typedef struct InsArg {
   int amode;        /* Adressing mode according to AdressingModes enum. */
   int val;          /* Number in immediate mode, or register number in register direct or direct index modes. */
   char label[32];   /* If argument is a label label is saved in this field (direct and direct index modes). */
} InsArg;


/* Structure that represents an instruction. */
typedef struct Ins {
   int ins;          /* Instruction according to InstructionsEnum */
   InsArg* source;   /* Source argument. */
   InsArg* dest;     /* Destination argument. */
} Ins;

/* Structure that represents memory address in base+offset format. */
typedef struct BOAddress {
   int base;
   int offset;
} BOAddress;

/* Frees memory occupied by instruction. 
   Removes argument structures if they are present
   and instruction itself.*/
void FreeIns(Ins* ins);

/* Tells if binary value describing addressing modes
   in instruction info has specific mode.
   Arguments:
    amodes          -- Source, or destination adressing modes value.
    adressing mode  -- Adressing mode according to AdressingModesEnum.
   Returns:
    1   -- If modes has specific adressing mode.
    0   -- Otherwise. */
int HasMode(int amodes, int adressingMode);

/* Converts given memory address to base+offset format.
   base is part of address that is divisible by 16
   and offset is a remainer. */
BOAddress AddressToBO(int address);

#endif