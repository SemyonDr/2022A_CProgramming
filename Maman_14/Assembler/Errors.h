#ifndef ERRORS_H
    #define ERRORS_H

#include <stdlib.h>
#include <stdio.h>
#include "Data.h"
#include "MyString.h"

/* Enumeration of errors found in assembly source file. */
enum ErrorsEnum {
    
    ErrMacro_NameNumber,        /* Macro name starts with a number. */
    ErrMacro_NameIllegal,       /* Macro name contains illegal characters. */
    ErrMacro_NameNotDefined,    /* Name of the macro not defined after macro keyword. */
    ErrMacro_NameReserved,      /* Name is a reserved assembly word. */
    ErrMacro_NameIdentical,     /* Macro name defined more than once. */

    ErrMacro_ExtraDef,           /* Extra text after macro definition */
    ErrMacro_ExtraDefEnd,        /* Extra text after endm */
    ErrMacro_ExtraCall,          /* Extra text after macro name in macro call line*/
    ErrMacro_Nested,             /* Macro defined inside macro. */

    ErrStm_Empty,                /* Statement is empty line. */
    ErrStm_NotRecognized,        /* Statement not recognized. */

    ErrCmm_Before,               /* Comma before arguments. */
    ErrCmm_Multiple,             /* Multiple commas between arguments. */
    ErrCmm_Missing,              /* Missing comma between arguments. */
    ErrCmm_After,                /* Comma after arguments. */

    ErrArg_NotANumber,           /* Expected a number. */
    ErrArg_InvalidLabel          /* Symbol in argument is not a valid label. */   
};

/* Structure that describes error in source file. */
typedef struct Error {
    int source_line;    /* Line in not expanded source file where error is found. */
    int error_code;     /* Error code according to ErrorsEnum. */
    char* info;         /* Additional error info. */
} Error;


/* Adds new error to the errors list.
   Info string will be copied to heap.
   Arguments:
    errors  -- List of errors.
    line    -- Number of line of source file where error is found.
    errCode -- Numeric error code according to ErrorsEnum 
    info    -- String of additional info about error. Usually
               source code line, or some name. */
void AddError(List* errors, int line, int errCode, char* info);

/* Prints individual error message to stdout.
   Arguments:
    er  -- Error to print. */
void PrintError(Error* er);

/* Prints errors in list to stdout in order.
   If list is empty nothing will be printed.
   Assumes tha errors is not NULL.
   Arguments:
    errors  -- errors list. */
void PrintErrorsList(List* errors);

#endif