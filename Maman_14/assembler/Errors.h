#ifndef ERRORS_H
    #define ERRORS_H

#define ERR_STEP 32 /* Step of expansion of errors dynamic array. */
#define MAX_INFO_LEN 31 /* Maximum length of additional info of error (without termination). */

#include <stdlib.h>
#include <stdio.h>
#include "Definitions.h"
#include "Data.h"


/* Enumeration of errors found in assembly source file. */
enum ErrorsEnum {
    /* Macro name errors. */
    ErrMacro_NameNumber,        /* Macro name starts with a number. */
    ErrMacro_NameIllegal,       /* Macro name contains illegal characters. */
    ErrMacro_NameNotDefined,    /* Name of the macro not defined after macro keyword. */
    ErrMacro_NameReserved,      /* Name is a reserved assembly word. */
    ErrMacro_NameIdentical,     /* Macro name defined more than once. */
    /* Other macro errors. */
    ErrMacro_ExtraDef,           /* Extra text after macro definition */
    ErrMacro_ExtraDefEnd,        /* Extra text after endm */
    ErrMacro_ExtraCall,          /* Extra text after macro name in macro call line*/
    ErrMacro_Nested,             /* Macro defined inside macro. */
    /* Statement errors. */
    ErrStm_Empty,                /* Statement is empty line. */
    ErrStm_NotRecognized,        /* Statement not recognized. */
    /* Comma errors. */
    ErrCmm_Before,               /* Comma before arguments. */
    ErrCmm_Multiple,             /* Multiple commas between arguments. */
    ErrCmm_Missing,              /* Missing comma between arguments. */
    ErrCmm_After,                /* Comma after arguments. */
    /* Instruction label arguments errors. */
    ErrArg_NotANumber,           /* Expected a number after #. */
    ErrArg_InvalidLabel,         /* Symbol in argument is not a valid label. */   
    ErrArg_LongSymbol,           /* Symbol in arguments is too long for a label. */
    ErrArg_MissingIndex,         /* In label with index [ opened, but index not provided. */
    ErrArg_MissingBracket,       /* In label with index closing bracket is missing. */
    ErrArg_InvalidIndex,         /* In label with index index is not a valid register name. */
    ErrArg_Extra,                /* Text after indexer brackets [rx]. */
    /* Instruction argument errors. */
    ErrIns_MissingArg,           /* Missing argument in instruction. */
    ErrIns_ExtraArg,             /* Too many arguments. */
    ErrIns_InvalidSrcAmode,      /* Source adressing mode does not exists for instucrion. */
    ErrIns_InvalidDestAmode,     /* Destination adressing mode does not exists for instruction. */ 
    /* String argument errors. */
    ErrDt_StrNoArgument,         /* No argument provided for string. */
    ErrDt_StrInvalidArg,         /* .string argument is not a string. */
    ErrDt_StrMissingClosing,     /* Closing " is missing in string. */
    ErrDt_StrExtra,              /* Extra text after string argument. */
    ErrDt_DtNoArgument,          /* No argument for .data directive. */
    ErrDt_DtInvalidArg,          /* .data arguments is not a number. */  
    /* Directive errors. */
    ErrDir_NotRecognized,        /* Directive not recognized. */
    ErrDir_NoArgument,           /* .entry or .extern without an argument. */
    ErrDit_Extra,                /* Extra text after .entry or .extern argument. */
    /* Symbol errors. */
    ErrSmb_TooLong,              /* Label is too long*/
    ErrSmb_NameIdentical,        /* Found symbol that is already defined. */   
    ErrSmb_EntryExtern,          /* Symbol declared both as entry and extern. */
    ErrSmb_NotFound,             /* Label argument not found in symbols table. */
    ErrSmb_EntryUndefined        /* Symbol marked as entry but no definition (code or data) provided. */
};


/* Structure that describes error in source file. */
typedef struct Error {
    int source_line_num;    /* Number of line in original (not expanded) source file where error is found. */
    int error_code;         /* Error code according to ErrorsEnum. */
    char source[MAX_STATEMENT_LEN+2]; /* Error source - line or argument. */
    char info[MAX_INFO_LEN+1];        /* Additional error info. */
} Error;


/* Dynamic array that represents list of errors.
   Errors should be added by using AddError().
   cur_line_num will be used when adding errors
   when iterating lines it should be changed for every line
   by calling ChangeErrCurLine(). */
typedef struct Errors {
    Error* list; /* Array that holds errors. */
    int count;   /* Number of elements in errors array. */
    /* Source line reference. Allows to find source file line number
       using number of line in expanded source file.
       Indexes correspond to original file line numbers and values correspond to
       expanded file line numbers. Element by index [0] is -1 because line
       numbering starts with 1. */
    DynArr* slr;
    int cur_line_num; /* Current line number in expanded file. Will be used to get source_line_num for added errors. */
    int capacity; /* Current capacity of this dynamic array. */
} Errors;

/* Changes error list current line for adding errors.
   Arguments:
    errors  -- Errors list.
    val     -- Number of current line in expanded source file where errors are checked.*/
void ChangeErrCurLine(Errors* errors, int val);

/* Adds new source line reference to slr array in errors structure.
   Arguments:
    errors      -- List of errors.
    exLineNum   -- Number of line in expanded source file. */
void AddLineReference(Errors* errors, int exLineNum);

/* Creates new errors array structure. */
Errors* CreateErrors();

/* Adds new error to the errors list.
   Uses cur_line_num field of errors structure to determine 
   number of line in source file.
   Arguments:
    errors      -- Errors list.
    errCode     -- Error code according to ErrorsEnum.
    source      -- Source line or argument (string) where error is found (can be NULL). 
    info        -- Optional additional info (can be NULL).*/
void AddError(Errors* errors, int errCode, char* source, char* info);

/* Adds new error to the errors list.
   Uses provided source file line number.
   Arguments:
    errors      -- Errors list.
    lineNum     -- Number of line in original (not expanded) source file where error found.
    errCode     -- Error code according to ErrorsEnum.
    source      -- Source line or argument (string) where error is found (can be NULL). 
    info        -- Optional additional info (can be NULL).*/
void AddErrorManual(Errors* errors, int lineNum, int errCode, char* source, char* info);

/* Sorts errors list by number of error source line.
   Uses insertion sort algorithm.
   Arguments:
    errors  -- Errors list.
    */
void SortErrors(Errors* errors);

/* Prints individual error message to stdout.
   Arguments:
    er  -- Error to print. */
void PrintError(Error* er);

/* Prints errors in list to stdout in order.
   If list is empty nothing will be printed.
   Assumes tha errors is not NULL.
   Arguments:
    errors  -- errors list. */
void PrintErrorsList(Errors* errors);

/* Frees memory occupied by errors list.
   Deallocates structures used by errors and errors structure itself.
   Arguments:
    errors  -- Errors list.*/
void FreeErrors(Errors* errors);

#endif