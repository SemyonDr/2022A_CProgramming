#ifndef PARSING_H
    #define PARSING_H

/* List of possible command parsing errors */
enum ParsingErrors {
    NoError,
    MissingParameter,
    MissingComma,
    MultipleCommas,
    IllegalComma,
    UndefinedCommand,
    ExtraText,
    InvalidVariable,
    InvalidNumber
};

/* Enumeration of all commands. */
enum CommandTypes {
    unknown_cmd,
    read_comp_cmd,
    print_comp_cmd,
    add_comp_cmd,
    sub_comp_cmd,
    mult_comp_real_cmd,
    mult_comp_img_cmd,
    mult_comp_comp_cmd,
    abs_comp_cmd,
    stop_cmd
};

/* Enumeration of parameter types.
   Used for parsing parameters lists. */
enum ParamTypes {
    letter_param,
    number_param
};

/* Structure representing parameters for a command */
typedef struct {
    int var_1;  /* First complex variable name index */
    int var_2;  /* Second complex variable name index*/
    double real_1; /* First real value */
    double real_2; /* Second real value */
} CmdParams;


/* Frees memory occupied by array of pointers of given size.
   Frees memory for each pointer and for array itself.
   Arguments:
    array   - Array of pointers.
    size    - Size of array. */
void FreePointersArray(void** array, int size);

/* Compares two null terminated strings and returns 
   value indicating if strings are equal.
   Arguments:
    s1  - First string (null-terminated)
    s2  - Second string (null-terminated)
   Returns:
    0   - Strings are not equal.
    1   - Strings are equal. */
int CompareStrings(char* s1, char* s2);

/* Skips tab and space symbols in given line 
   starting from given position and advances 
   position in the line to where first non blank
   character encountered.
   Arguments:
    line    - Line where spaces will be skipped.
    pos     - Pointer to initial position starting from which spaces should be skipped. */
void SkipSpaces(char* line, int* pos);

/* Extracts command name string from given command line.
   Arguments:
    line    - Line containing command
    pos     - Pointer to a value that will be set 
              to position of the next symbol after command name. 
   Return:
    Command name in text form (null-terminated string)
    NULL if no command name is found (end of the line, or comma encountered).
    */
char* GetCommandName(char* line, int* pos);

/* Parses given command name string and returns
   corresponding CommandTypes enum value.
   Arguments:
    name    - Command name in string form.
   Returns:
    Command number in CommandTypesEnum if parsing is succesfull.
    0 (Unknown command) indicates that parsing failed. */
int ParseCommandName(char* name);

/* Gets single parameter from command line 
   starting from given position in line
   and advances position to symbol after it.
   Assumes that line is null-terminated.
   Arguments:
    line    - Line containing command.
    pos     - Position in line starting on which parameter should be searched for.
   Returns:
    Parameter text, null-terminated (without leading, or trailing blank symbols)
    NULL if parameter not found (comma, or end of the line found before any text) */
char* GetNextParameter(char* line, int* pos);

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
*/
char** GetParameters(int pNum, char* line, int* error, int* pos);

/* Tries to parse letter parameter A-C from given parameter string p.
   Assumes that p is null-terminated and contains at least one other symbol.
   Arguments:
    p   - Parameter in text form.
   Returns:
    -1  - Parameter is invalid.
    0-5 - Letters A-F */
int ParseParameterLetter(char* p);

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
   */
double ParseParameterReal(char* p, int *isParsed);

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
                8   - Failed to parse number parameter */
CmdParams ParseParameters(int pNum, char** parametersList, int* error, int* types);

#endif /* PARSING_H */