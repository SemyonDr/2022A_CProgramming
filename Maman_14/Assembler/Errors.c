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
        /* For readability leading blanks are removed and
           new line characters replaced with spaces. */
        RemoveLeadingBlanks(er->info);
        ReplaceNewLine(er->info, '\0');
    }
    else
        er->info = NULL;

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
    /*Printing line number */
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
        break;

    case ErrCmm_Before:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Illegal comma(s) before arguments. ");
        break;

    case ErrCmm_Multiple:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Multiple commas between arguments.");
        break;

    case ErrCmm_Missing:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Missing comma between arguments.");
        break;

    case ErrCmm_After:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Illegal comma(s) after arguments.");
        break;

    case ErrArg_NotANumber:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Number was expected.");
        break;

    case ErrArg_InvalidLabel:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Invalid symbol as argument.");
        break;

    case ErrArg_LongSymbol:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Label is too long.");
        break;

    case ErrArg_MissingIndex:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Index not specified.");
        break;    

    case ErrArg_MissingBracket:    
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Closing bracket is missing (']').");
        break;

    case ErrArg_InvalidIndex:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Expected register name as indexer (r0-r15).");
        break;

    case ErrArg_Extra:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Text after indexer not allowed.");
        break;

    case ErrIns_MissingArg:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Missing argument for instruction.");
        break;

    case ErrIns_ExtraArg:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Too many arguments.");
        break;

    case ErrIns_InvalidSrcAmode:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Unsupported source adressing mode.");
        break;
    
    case ErrIns_InvalidDestAmode:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Unsupported destination adressing mode.");
        break;

    case ErrDt_StrNoArgument:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("No argument provided for .string directive.");
        break;

    case ErrDt_StrInvalidArg:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Expected \"string\" as argument.");
        break;

    case ErrDt_StrMissingClosing:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Closing \" is missing.");
        break;

    case ErrDt_StrExtra:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Extra text after argument.");
        break;

    case ErrDt_DtNoArgument:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Expected argument for .data directive.");
        break;

    case ErrDir_NotRecognized:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Directive not recognized.");
        break;

    case ErrSmb_TooLong:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Label is too long. Maximum %d characters allowed.", MAX_LABEL_LEN);
        break;

    case ErrSmb_NameIdentical:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Label already defined.");
        break;

    case ErrSmb_EntryExtern:
        if (er->info != NULL)
            printf("\"%s\" <- ", er->info);
        printf("Label cannot be defined as .entry and .extern simultaniously.");
        break;

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
    printf("DEBUG: Printing [%d] errors:\n", errors->count);
    if (errors->count != 0) {
        while (cur != NULL) {
            er = (Error*)(cur->data); /* Casting for readability. */
            PrintError(er); /* Printing error. */
            cur = cur->next;
        }
    }
}