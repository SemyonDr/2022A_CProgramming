#include "Errors.h"

/* Changes error list current line for adding errors.
   Arguments:
    errors  -- Errors list.
    val     -- Number of current line in expanded source file where errors are checked.*/
void ChangeErrCurLine(Errors* errors, int val) {
    if (val >= 0) {
        errors->cur_line_num = val;
    }
}


/* Adds new source line reference to slr array in errors structure.
   Arguments:
    errors      -- List of errors.
    exLineNum   -- Number of line in expanded source file. */
void AddLineReference(Errors* errors, int exLineNum) {
    AddDynArr(errors->slr, exLineNum);
}


/* Creates new errors array structure. */
Errors* CreateErrors() {
    /* Allocating the structure. */
    Errors* errors = (Errors*)malloc(sizeof(Errors));
    if (errors == NULL) {
        perror("Failed to allocate memory.");
        exit(1);
    }

    /* Allocating data array. */
    errors->list = (Error*)malloc(sizeof(Error)*ERR_STEP);
    if (errors->list == NULL) {
        perror("Failed to allocate memory.");
        exit(1);
    }

    /* Creating source line reference. */
    errors->slr = CreateDynArr(32);
    AddDynArr(errors->slr, -1);

    /* Setting other fields. */
    errors->cur_line_num = 0;
    errors->count = 0;
    errors->capacity = ERR_STEP;

    return errors;
}

/* Adds new error to the errors list.
   Uses cur_line_num field of errors structure to determine 
   number of line in source file.
   Arguments:
    errors      -- Errors list.
    errCode     -- Error code according to ErrorsEnum.
    source      -- Source line or argument (string) where error is found (can be NULL). 
    info        -- Optional additional info (can be NULL).*/
void AddError(Errors* errors, int errCode, char* source, char* info) {
    int lineNum = errors->slr->data[errors->cur_line_num];
    AddErrorManual(errors, lineNum, errCode, source, info);
}


/* Adds new error to the errors list.
   Uses provided source file line number.
   Arguments:
    errors      -- Errors list.
    lineNum     -- Number of line in original (not expanded) source file where error found.
    errCode     -- Error code according to ErrorsEnum.
    source      -- Source line or argument (string) where error is found (can be NULL). 
    info        -- Optional additional info (can be NULL).*/
void AddErrorManual(Errors* errors, int lineNum, int errCode, char* source, char* info) {
    Error* new;  /* Pointer to new error structure. */
    int spos = 0; /* Position in source line. */
    int cpos = 0; /* Position in copied line. */

    /* Checking if array should be expanded. */
    if (errors->count == errors->capacity) {
        Error* res; /* Result of array reallocation. */
        int new_cap = (errors->capacity) + ERR_STEP; /* New errors array capacity */
        /* Reallocating array. */
        res = (Error*)realloc(errors->list, sizeof(Error)*new_cap);
        if (res == NULL) {
            perror("Failed to allocate memory.");
            exit(1);
        }
        if (res != errors->list) {
            errors->list = res;
        }
        /* Setting new capacity. */
        errors->capacity = new_cap;
    }

    /* Getting pointer to new error for shortening names in code. */
    new = &(errors->list[errors->count]);

    /* Writing error code */
    new->error_code = errCode;
    /* Setting line number. */
    new->source_line_num = lineNum;

    if (source != NULL) {
        /* Copying source line. 
        Leading blank characters and new line characters are not copied.*/
        while (source[spos] == ' ' || source[spos] == '\t' || source[spos] == '\n')
            spos++;
        while (source[spos] != '\0' && spos < MAX_STATEMENT_LEN+1) {
            if (source[spos] != '\n')
                new->source[cpos] = source[spos];
            cpos++;
            spos++;
        }
        new->source[cpos] = '\0';
    }
    else
        new->source[0] = '\0';
    if (info != NULL) {
        /* Copying additional info.
        Leading blank characters and new line characters are not copied.*/
        spos = 0;
        cpos = 0;
        while (info[spos] == ' ' || info[spos] == '\t' || info[spos] == '\n')
            spos++;
        while (info[spos] != '\0' && spos < MAX_INFO_LEN) {
            if (info[spos] != '\n')
                new->info[cpos] = info[spos];
            spos++;
            cpos++;
        }
        new->info[cpos] = '\0';
    }
    else
        new->info[0] = '\0';

    /* Increasing errors count */
    (errors->count)++;    
}


/* Prints individual error message to stdout.
   Arguments:
    er  -- Error to print.
   Algorithm:
    Uses switch to print error description 
    according to error code. */
void PrintError(Error* er) {
    /* Printing line number. If line number is 0, or negative it will not be printed. */
    if (er->source_line_num > 0)
        printf(" Line %d: ", er->source_line_num);
    /* Printing error source */
    if (er->source[0] != '\0')
        printf("\"%s\" <- ", er->source);

    /* Printing error explanation */
    switch (er->error_code)
    {
    case ErrMacro_NameNumber:
        printf("Macro name can't begin with a number.");
        break;

    case ErrMacro_NameIllegal:
        printf("Macro name can contain only letter and number characters.");
        break;

    case ErrMacro_NameNotDefined:
        printf("Macro name is not defined.");
        break;

    case ErrMacro_NameReserved:
        if (er->info != NULL)
            printf("Illegal macro name, \"%s\" is a reserved word.", er->info);
        else
            printf("Macro name can't be a reserved word.");
        break;

    case ErrMacro_NameIdentical:
        if (er->info[0] != '\0')
            printf("Macro name %s already defined.", er->info);
        else
            printf("Macro name already defined.");
        break;

    case ErrMacro_ExtraDef:
        printf("Extra text after macro definition.");
        break;

    case ErrMacro_ExtraDefEnd:
        printf("Extra text after macro closing tag.");
        break;

    case ErrMacro_ExtraCall:
        printf("Extra text after macro call.");
        break;

    case ErrMacro_Nested:
        printf("Nested macro definitions are forbidden.");
        break;

    case ErrStm_Empty:
        printf("Statement is empty.");
        break;

    case ErrStm_NotRecognized:
        if (er->info[0] != '\0')
            printf("Unknown command \"%s\".", er->info);
        else
            printf("Unknown command.");
        break;

    case ErrCmm_Before:
        printf("Illegal comma(s) before arguments. ");
        break;

    case ErrCmm_Multiple:
        printf("Multiple commas between arguments.");
        break;

    case ErrCmm_Missing:
        printf("Missing comma between arguments.");
        break;

    case ErrCmm_After:
        printf("Illegal comma(s) after arguments.");
        break;

    case ErrArg_NotANumber:
        printf("Number was expected.");
        break;

    case ErrArg_InvalidLabel:
        printf("Invalid symbol as argument.");
        break;

    case ErrArg_LongSymbol:
        printf("Label is too long. Max label length is %d.", MAX_LABEL_LEN);
        break;

    case ErrArg_MissingIndex:
        printf("Index not specified.");
        break;    

    case ErrArg_MissingBracket:    
        printf("Closing ] bracket is missing..");
        break;

    case ErrArg_InvalidIndex:
        printf("Expected register name as indexer (r0-r15).");
        break;

    case ErrArg_Extra:
        printf("Text after indexer not allowed.");
        break;

    case ErrIns_MissingArg:
        printf("Missing argument for instruction.");
        break;

    case ErrIns_ExtraArg:
        printf("Too many arguments.");
        break;

    case ErrIns_InvalidSrcAmode:
        printf("Unsupported source adressing mode.");
        break;
    
    case ErrIns_InvalidDestAmode:
        printf("Unsupported destination adressing mode.");
        break;

    case ErrDt_StrNoArgument:
        printf("No argument provided for .string directive.");
        break;

    case ErrDt_StrInvalidArg:
        printf("Expected \"string\" as argument.");
        break;

    case ErrDt_StrMissingClosing:
        printf("Closing \" is missing in argument [ %s ].", er->info);
        break;

    case ErrDt_StrExtra:
        printf("Extra text after argument.");
        break;

    case ErrDt_DtNoArgument:
        printf("Expected argument for .data directive.");
        break;

    case ErrDt_DtInvalidArg:
        if (er->info[0] != '\0')
            printf("[%s] - Expected number argument", er->info);
        else    
            printf("Expected number argument.");
        break;

    case ErrDir_NotRecognized:
        printf("Directive not recognized.");
        break;

    case ErrDir_NoArgument:
        printf("Expected label argument.");
        break;

    case ErrSmb_TooLong:
        printf("Label is too long. Maximum %d characters allowed.", MAX_LABEL_LEN);
        break;

    case ErrSmb_NameIdentical:
        printf("Label already defined.");
        break;

    case ErrSmb_EntryExtern:
        printf("Label cannot be defined as .entry and .extern simultaniously.");
        break;

    case ErrSmb_NotFound:
        printf("Failed to resolve symbol argument. Label not found");
        break;

    case ErrSmb_EntryUndefined:
        printf("Symbol marked as entry does not have definition.");
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
void PrintErrorsList(Errors* errors) {
    int i; /* Iterator */
    if (errors->count > 0)
        printf("Found %d errors:\n", errors->count);
    for (i = 0; i < errors->count; i++) {
        Error* er = &(errors->list[i]);
        PrintError(er);
    }
        
}