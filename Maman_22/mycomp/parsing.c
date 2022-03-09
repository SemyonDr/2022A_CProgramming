#include <stdlib.h>
#include <stdio.h>
#include "parsing.h"

/* Frees memory occupied by array of pointers of given size.
   Frees memory for each pointer and for array itself.
   Arguments:
    array   - Array of pointers.
    size    - Size of array. */
void FreePointersArray(void** array, int size) {
    int i;
    for (i=0; i<size; i++) {
        free(array[i]);
    }
    free(array);
}


/* Compares two null terminated strings and returns 
   value indicating if strings are equal.
   Arguments:
    s1  - First string (null-terminated)
    s2  - Second string (null-terminated)
   Returns:
    0   - Strings are not equal.
    1   - Strings are equal. */
int CompareStrings(char* s1, char* s2) {
    int i; /* Iterator */
    int res; /* Result */
    res = 1;
    
    /* Comparing symbol by symbol. */
    i=0;
    do {
        if (s1[i] != s2[i])
            res = 0;
        i++;
    } while (s1[i] != '\0' && s2[i] != '\0' && res == 1);

    /* Checking if lines are of equal length.
       If they are equal both should have termination symbol on position i */
    if (s1[i] != s2[i])
        res = 0;
    
    return res;
}


/* Skips tab and space symbols in given line 
   starting from given position and advances 
   position in the line to where first non blank
   character encountered.
   Arguments:
    line    - Line where spaces will be skipped.
    pos     - Pointer to initial position starting from which spaces should be skipped. */
void SkipSpaces(char* line, int* pos) {
    while ((line[*pos] == ' ' || line[*pos] == '\t') && line[*pos] != '\0') {
        (*pos)++;
    }
}


/* Extracts command name string from given command line.
   Arguments:
    line    - Line containing command
    pos     - Pointer to a value that will be set 
              to position of the next symbol after command name. 
   Return:
    Command name in text form (null-terminated string)
    NULL if no command name is found (end of the line, or comma encountered).
   Algorithm:
    Skips leading spaces.
    Remembers start of the command text.
    Searches where command text ends.
    Copies command text to new line.
    Returns new line.
    */
char* GetCommandName(char* line, int* pos) {
    char* name;
    int start =0; /* Start position of a command name after leading spaces */
    *pos = 0;

    /* Skipping leading spaces */
    SkipSpaces(line, pos);

    /* Remembering start of the command. */
    start = *pos;

    /* Searching where command ends.*/
    while (line[*pos] != ' ' && line[*pos] != '\t' && line[*pos] != ',' && line[*pos] != '\0') {
        (*pos)++;
    }

    /* Checking if name length is not 0. */
    if (*pos-start == 0)
        return NULL;

    /* Allocating memory for name text. Including null-termination symbol. */
    name = (char*)malloc(sizeof(char)*(*pos-start) +1);
    if (name == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }
    
    /* Copying name to a new string. */ {
        int i = 0;
        while (i != *pos-start) {
            name[i] = line[start+i];
            i++;
        }
        name[i] = '\0';
    }

    return name;
}


/* Parses given command name string and returns
   corresponding CommandTypes enum value.
   Arguments:
    name    - Command name in string form.
   Returns:
    Command number in CommandTypesEnum if parsing is succesfull.
    0 (Unknown command) indicates that parsing failed.
   Algorith:
    Compares given string with known command texts. */
int ParseCommandName(char* name) {
    if (CompareStrings(name, "read_comp"))
        return read_comp_cmd;

    if (CompareStrings(name, "print_comp"))
        return print_comp_cmd;

    if (CompareStrings(name, "add_comp"))
        return add_comp_cmd;
    
    if (CompareStrings(name, "sub_comp"))
        return sub_comp_cmd;

    if (CompareStrings(name, "mult_comp_real"))
        return mult_comp_real_cmd;

    if (CompareStrings(name, "mult_comp_img"))
        return mult_comp_img_cmd;

    if (CompareStrings(name, "mult_comp_comp"))
        return mult_comp_comp_cmd;

    if (CompareStrings(name, "abs_comp"))
        return abs_comp_cmd;

    if (CompareStrings(name, "stop"))
        return stop_cmd;

    return unknown_cmd;
}


/* Gets single parameter from command line 
   starting from given position in line
   and advances position to symbol after it.
   Assumes that line is null-terminated.
   Arguments:
    line    - Line containing command.
    pos     - Position in line starting on which parameter should be searched for.
   Returns:
    Parameter text, null-terminated (without leading, or trailing blank symbols)
    NULL if parameter not found (comma, or end of the line found before any text) 
   Algorith:
    Skips leading spaces.
    Searches for the end of the parameter text (blank symbol, comma, or end of line).
    Copies parameter text to new string.
    Returns string.  */
char* GetNextParameter(char* line, int* pos) {
    char* param; /* Parameter text. */
    int start; /* Ititial position of parameter text. */
    
    /* Skipping leading blank symbols */
    SkipSpaces(line, pos);

    /* Remembering where command text starts. */
    start = *pos;

    /* Checking if end of the line is reached */
    if (line[*pos] == '\0')
        return NULL;

    /* Searching for the end of the parameter text 
       Searches until space symbol, comma, or end of the line is met.
       Position will be set to symbol after parameter after search. */
    while (line[*pos] != ' ' && line[*pos] != '\t' && line[*pos] != ',' && line[*pos] != '\0') {
        (*pos)++;
    }

    /* Checking if parameter text does not have 0 length.*/
    if (*pos == start)
        return NULL;
    
    /* Allocating memory for parameter text.
       Including line termination symbol. */
    param = (char*)malloc(sizeof(char)*(*pos-start+1));
    if (param == NULL) {
        perror("Failed to allocate memory.");
        exit(1);
    }

    /* Copying parameter text to the new line*/ {
        int i;
        i=0;
        while (i != *pos-start) {
            param[i] = line[start+i];
            i++;
        }
        param[i] = '\0';
    }

    return param;
}


/* Takes line containing command and number of parameters to get
   and returns array containing text of command parameters (null-terminated).
   Writes error code to provided pointer.
   Advances position in the line to symbol after last parameter.
   Uses GetNextParameter() to get individual parameters.
   Arguments:
    pNum    - Number of parameters to get
    line    - Command line (Assumes null-terminated).
    error   - Pointer for returning errors.
    pos     - Position in line of the next symbol after command name.
              Will be set to a next symbol after last parameter.
   Returns:
    Array containing texts of command parameters.
    NULL if errors encountered.
    error codes:
    Error codes are according to ParsingErrors enum.
                0   - No errors
                1   - Not enough parameters
                2   - Missing comma
                3   - Multiple consecutive commas
                4   - Illegal comma
   Algorith:
    Uses GetNextParameter() to extract pNum of individual parameters.
    Checks for invalid commas between and before parameters:
    If tried to get parameter and encountered comma before parameter text
    then returns IllegalComma if parameter was first and Multiple commas for further parameters.
    If no parameter read and end of the line encountered returns MissingParameter.
    Skips spaces after parameter and if it was not last checks if comma before next parameter is present.
    If parameter was not last one and end of the line found returns MissingParameter.
    Determines if number of parameters present is equal to pNum.
*/
char** GetParameters(int pNum, char* line, int* error, int* pos) {
    char** paramList; /* List of found parameters*/
    int i; /* Parameters iterator */
    *error = 0;

    /* If there is 0 parameters to read not reading anything. */
    if (pNum == 0) {
        return NULL;
    }

    /* Allocating memory for the list*/
    paramList = (char**)malloc(sizeof(char*)*pNum);
    if (paramList == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    /* Getting parameters */
    for (i=0; i<pNum; i++){
        paramList[i] = GetNextParameter(line, pos);

        /* Checking for comma errors.
           Error is found if no parameter read and comma encountered*/
        if (paramList[i] == NULL && line[*pos] == ',') {
            if (i==0) { /* If first parameter is missing before comma */
                *error = IllegalComma;
                FreePointersArray((void**)paramList, 0);
                return NULL;
            }
            else { /* If further parameter missing between commas */
                *error = MultipleCommas;
                /*Freeing memory*/               
                FreePointersArray((void**)paramList, i);
                return NULL;
            }
        }

        /* If parameter is not found and no comma present it means that end of the line
           is reached and parameter is missing. */
        if (paramList[i] == NULL) {
            *error = MissingParameter;
            FreePointersArray((void**)paramList, i);
            return NULL;
        }

        /* If parameter is present and not last one skipping spaces and 
        checking if comma before next parameter is present. */
        if (i+1 != pNum) {
            SkipSpaces(line, pos);
            if (line[*pos] == ',')
                (*pos)++; /* Moving position to symbol after comma */
            else {
                /* Checking if only comma is missing, or this is the end of command line*/
                SkipSpaces(line, pos);
                if (line[*pos] == '\0')
                    *error = MissingParameter;
                else
                    *error = MissingComma;
                FreePointersArray((void**)paramList, i+1);
                return NULL;
            }
        }
    }/* End of getting parameters*/

    return paramList;
}


/* Tries to parse letter parameter A-C from given parameter string p.
   Assumes that p is null-terminated and contains at least one other symbol.
   Arguments:
    p   - Parameter in text form.
   Returns:
    -1  - Parameter is invalid.
    0-5 - Letters A-F */
int ParseParameterLetter(char* p) {
    /* Checking if there is only one symbol of text */
    if (p[1] != '\0')
        return -1;

    /* Checking if symbol is one of variables names (A-F)*/
    if (p[0]>64 && p[0]<71 )
        return p[0]-65;
    else 
        return -1;
}

/* Tries to parse real number parameter from given line p.
   Assumes that p is null-terminated.
   Uses library function strtod to convert text to double.
   Arguments:
    p           - Parameter in text form.
    isParsed    - Pointer value that will indicate if parsing is successful.
   Returns:
    Parsed double value on success.
    0.0 if parsing failed.
    isParsed:
     0  - If parsing failed.
     1  - If parsing succeded.
   Algorithm:
    Checks if parameter text is valid real number:
    Skips sign.
    Until end of the text checks if symbol is a number, or a dot.
    If dot found checks if dot already been found before.
    If text is valid uses strtod() library function to convert text to double. */
double ParseParameterReal(char* p, int *isParsed) {
    int i; /* Iterator */
    int dot; /* Flag that shows if dot is encountered. */
    i=0;
    dot = 0;

    /* Skipping possible sign symbol */
    if (p[i] == '+' || p[i] == '-')
        i++;

    /*Checking if symbols are number, or dot.
      If more than one dot found text is invalid. */
    while (p[i] != '\0') {
        /* Checking if symbol is a number. */
        if (p[i] <48 || p[i] > 57) {
            /*If symbol is not a number checking if it is a dot*/
            if (p[i] == '.') {
                /* If it is a dot then checking if dot already encountered */
                if (dot) {
                    *isParsed = 0;
                    return 0.0;
                }
                /* If dot is found for first time then set flag and continue. */
                dot = 1;
                i++;
            }
            /* If symbol is not dot or number then parameter is invalid.*/
            else {
                *isParsed = 0;
                return 0.0;
            }
        }
        i++;
    }

    /* Setting flag */
    *isParsed = 1;

    /* Using strtod to conver text to double */
    return strtod(p, NULL);
}


/* Tries to parse parameters list according to given parameter types.
   Assumes that number of parameters, types and number of parameters in text array
   are always correct.
   Arguments:
    pNum            - Number of parameters.
    parametersList  - Array of null-terminated strings that contain parameters text.
    error           - Pointer for returning errors.
    types           - Array of parameter types.
   Returns:
    Parsed parameters structure. Returns structure even if parsing failed.
    error   - error code according to ParsingErrors enum:
                0   - No errors
                7   - Failed to parse letter parameter
                8   - Failed to parse number parameter 
   Algorithm: 
    Goes simutaneously trough text and type array and tries to parse each parameter
    string according to its parameter type.
    After parsing determines in which field of parameters structure value should be 
    written:
    For letter parameters writes value to first field that have value -1.
    For number parameters uses flag first_real to know if first real field in structure
    is already used. */
CmdParams ParseParameters(int pNum, char** parametersList, int* error, int* types) {
    int i; /* Iterator */
    CmdParams p; /* Resulting parameters structure */
    int var; /* Variable for storing parsed letter value */
    double real; /* Variable for storing parsed number value */
    /* Variable for deciding which field to use after parsing a number.
       0 if first field is not used, and 1 if already used */
    int first_real = 0; 

    /* Initializing parameters structure */
    p.var_1 = -1;
    p.var_2 = -1;
    p.real_1 = 0.0;
    p.real_2 = 0.0;

    /* Parsing parameters */
    for (i=0; i<pNum; i++) {
        /* Letter parameter */
        if (types[i] == letter_param) {
            var = ParseParameterLetter(parametersList[i]);
            if (var == -1) {
                *error = InvalidVariable;
                return p;
            }
            /* Deciding which field of structure to use*/
            if (p.var_1 == -1)
                p.var_1 = var; 
            else
                p.var_2 = var;
        }

        /* Number parameter */
        if (types[i] == number_param) {
            int isParsedReal = 0; /* Variable for returning number parsing result. */
            real = ParseParameterReal(parametersList[i], &isParsedReal);
            if (!isParsedReal) {
                *error = InvalidNumber;
                return p;
            }

            /* Deciding which field of structure to use*/
            if (!first_real) {
                first_real = 1;
                p.real_1 = real; }
            else
                p.real_2 = real;
        }
    }

    *error = 0;
    return p;
}