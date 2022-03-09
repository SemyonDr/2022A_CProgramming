#include "Errors.h"

/* Adds new error to the errors list.
   Info string will be copied to heap.
   Arguments:
    errors  -- List of errors.
    line    -- Number of line of source file where error is found.
    errCode -- Numeric error code according to ErrorsEnum 
    info    -- String of additional info about error. Usually
               source code line, or some name. */
void AddError(List* errors, int line, int errCode, char* info) {
    /* Creating new error structure. */
    Error* er = (Error*)malloc(sizeof(Error));
    if (er == NULL) {
        perror("Failed to allocate memory.\n");
        exit(1);
    }
    /* Setting error data. */
    er->error_code= errCode;
    er->source_line = line;
    if (info != NULL) {
        /* Info string allocated on heap */
        er->info = CopyStringToHeap(info);
        /* For readability Leading blanks are removed and
           new line characters replaced with spaces. */
        RemoveLeadingBlanks(er->info);
        ReplaceNewLine(er->info, ' ');
    }
    /* Adding error to the list. */
    ListAdd(errors, er);
}

/* Prints individual error message to stdout.
   Arguments:
    er  -- Error to print.
   Algorithm:
    Uses switch to print error description 
    according to error code. */
void PrintError(Error* er) {
    /*Printing line */
    printf(" Line %d: ", er->source_line);

    /* Printing error explanation */
    switch (er->error_code)
    {
    case ErrMacro_NameNumber:
        if (er->info != NULL) 
            printf("\"%s\" <- ", er->info);
        printf("Macro name can't begin with a number.");
        break;

    case ErrMacro_NameIllegal:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Macro name can contain only letter and number characters.");
        break;

    case ErrMacro_NameNotDefined:
        printf("Macro name is not defined.");
        break;

    case ErrMacro_NameReserved:
        if (er->info != NULL)
            printf("\"%s\" <- Illegal macro name, \"%s\" is a reserved word.", er->info, er->info);
        else
            printf("Macro name can't be a reserved word.");
        break;

    case ErrMacro_NameIdentical:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Macro name already defined.");
        break;

    case ErrMacro_ExtraDef:
        if (er->info != NULL) 
            printf("\"%s\" <- ", er->info);
        printf("Extra text after macro definition.");
        break;

    case ErrMacro_ExtraDefEnd:
        if (er->info != NULL) 
            printf("\"%s\" <- ", er->info);
        printf("Extra text after macro closing tag.");
        break;

    case ErrMacro_ExtraCall:
        if (er->info != NULL) 
            printf("\"%s\" <- ", er->info);
        printf("Extra text after macro call.");
        break;

    case ErrMacro_Nested:
        printf("Nested macro definitions are forbidden.");
        break;

    case ErrStm_Empty:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Statement is empty.");
        break;

    case ErrStm_NotRecognized:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Unknown command.");

    

    default:
        break;
    }

    printf("\n");
}

/* Prints errors in list to stdout in order.
   If list is empty nothing will be printed.
   Assumes tha errors is not NULL.
   Arguments:
    errors  -- errors list. */
void PrintErrorsList(List* errors) {
    Error* er;
    ListNode* cur;
    cur = errors->head;
    if (errors->count != 0) {
        while (cur != NULL) {
            er = (Error*)(cur->data);
            PrintError(er);
            cur = cur->next;
        }
    }
}