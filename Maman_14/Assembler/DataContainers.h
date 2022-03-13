#ifndef DATACONTAINERS_H
    #define DATACONTAINERS_H

/* Structure that describes info about macro.*/
typedef struct MacroInfo {
    char* name;  /* Macro name */
    long body_pos; /* Index position in file where macro body starts. */
    int body_line_num; /* Number of line in source file where macro body starts. */
    int num_lines; /* Length of macro body definition in lines (excluding name line and endm line) */    
} MacroInfo;

/* Structure that represents instruction argument.
   Can represent any kind of argument:
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

/* Structure that represents element of symbols table. */
typedef struct Symbol {
   char name[32];    /* Symbol label. */
   int adress;       /* Address represented by label (decimal) */
   int attributes;   /* [8]code-[4]data-[2]extern-[1]entry*/
} Symbol;

/* Structure for holding pair of label and pointer
   to instuction argument where label is mentioned in code. 
   Together with symbols table allows to resolve label
   arguments. */
typedef struct UnresolvedArgument {
   char name[32];    /* Label name. */
   InsArg* arg;      /* Pointer to argument where label is used. */
} UnresolvedArgument;

/* Represents general info about an instruction
   and its structure. */
typedef struct InsInfo {
   int ins;                /* Instruction number according to InstructionsEnum*/
   int opcode;             /* Instruction opcode */
   int funct;              /* Instruction funct code. (0 if no code)*/
   int amodes_source;      /* [8]immideate-[4]direct-[2]d-index-[1]register*/
   int amodes_dest;
} InsInfo;

/* Tells if binary value describing addressing modes
   in instruction info has specific mode.
   Arguments:
    amodes          -- Source, or destination adressing modes value.
    adressing mode  -- Adressing mode according to AdressingModesEnum.
   Returns:
    1   -- If modes has specific adressing mode.
    0   -- Otherwise. */
int HasMode(int amodes, int adressingMode);

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

#endif