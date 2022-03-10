#ifndef DATA_H
    #define DATA_H

/* Structure that describes info about macro.*/
typedef struct MacroInfo {
    char* name;  /* Macro name */
    long body_pos; /* Index position in file where macro body starts. */
    int body_line_num; /* Number of line in source file where macro body starts. */
    int num_lines; /* Length of macro body definition in lines (excluding name line and endm line) */    
} MacroInfo;

/* Enumeration of processor Instructions. */
enum InstructionsEnum {
   ins_mov, ins_cmp, ins_add, ins_sub,
   ins_lea, ins_clr, ins_not, ins_inc,
   ins_dec, ins_jmp, ins_bne, ins_jsr,
   ins_red, ins_prn, ins_rts, ins_stop
};
 
/* Enumeration of adressing modes for instruction argument. */
enum AdressingModesEnum {
   am_immediate,  /* Immediate */
   am_direct,     /* Direct */
   am_index,      /* Index */
   am_rdirect     /* Register direct */
};

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


/* Defines node of linked list.
   Data is stored as a pointer.
*/
typedef struct ListNode {
    void* data;
    struct ListNode* next;
} ListNode;


/* Defines Linked List structure.
   Data is stored as pointers and can be of any type. */
typedef struct List {
    struct ListNode* head;
    int count;
} List;

/* Dynamic array of integer type.
   Size of this array can be expanded by calling 
   a function. Size is expanded in steps.
   Steps are defined when creaing this structure.
   Elements can be added one after another with 
   a function. In this case array is expanded automatically.
   When user creates this array he should decide if he will
   add elements directly to ->data field and use function to expand
   capacity, or use function to add elements one after another in
   which case array will be expanded automatically.
    */
typedef struct {
   int* data;  /* Array holding data. */
   int size; /* Size of data array. */
   int step; /* Expansion step in cells. */
   int count; /* Number of added elements. */
} DArrayInt;

/* Creates new linked list.
   Returns:
    Empty linked list.
 */
List* CreateList();

/* Adds node containing given data to a list.
   If given list does not exists (NULL) does nothing.
   Arguments:
    list    -- list to add to.
    data    -- pointer to data of new node.
 */
void ListAdd(List* list, void* data);

/* Creates dynamic array of integer type.
   Arguments:
    step    -- Expansion step in cells.
   Returns:
    New dynamic array of size */
DArrayInt* CreateDArrayInt(int step);

/* Expands dynamic integer array by one step.
   Arguments:
    dArray  -- Dynamic array for expansion. */
void ExpandDArrayInt(DArrayInt* dArray);

/* Writes data to dynamic array at position ->count
   and increases this counter.
   Arguments:
    dArray  -- Dynamic array.
    data    -- New data element. */
void DArrayIntAdd(DArrayInt* dArray, int data);

/* Frees memory allocated for dynamic array.
   Removes data and structure.
   Arguments:
    dArray  -- Dynamic array to free. */
void DArrayIntFree(DArrayInt* dArray);

/* Checks if attribute "code" is set.
   Arguments:
    attributes -- attributes number from Symbol structure.
   Returns:
    0 -- attribute is set.
    1 -- Attribute not set. */
int IsCode(int attributes);

/* Checks if attribute "data" is set.
   Arguments:
    attributes -- attributes number from Symbol structure.
   Returns:
    0 -- attribute is set.
    1 -- Attribute not set. */
int IsData(int attributes);

/* Checks if attribute "extern" is set.
   Arguments:
    attributes -- attributes number from Symbol structure.
   Returns:
    0 -- attribute is set.
    1 -- Attribute not set. */
int IsExtern(int attributes);

/* Checks if attribute "entry" is set.
   Arguments:
    attributes -- attributes number from Symbol structure.
   Returns:
    0 -- attribute is set.
    1 -- Attribute not set. */
int IsEntry(int attributes);



#endif