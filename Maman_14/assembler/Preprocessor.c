#include "Preprocessor.h"

/* Searches macro in macros list by name.
   Arguments:
    macros  -- List of macros.
    name    -- Name of a macro to find.
   Returns:
    MacroInfo with provided name.
    NULL if not found.*/
MacroInfo* FindMacroByName(List* macros, char* name) {
    ListNode* cur; /* List iterator */

    /* Checking if list is empty. */
    if (macros->count == 0)
        return NULL;

    /* Comparing macros names. */
    cur = macros->head;
    while (cur != NULL) {
        if (CompareStrings(name, ((MacroInfo*)cur->data)->name))
            return (MacroInfo*)(cur->data);
        cur = cur->next;    
    }

    return NULL;
}



/* Checks if given line consists of blank characters (' ', '\t', '\n').
   Arguments:
    line    -- Line to check (null-terminated).
   Returns:
    0       -- If line is not blank.
    1       -- If line is blank.*/
int IsLineBlank(char* line) {
    int pos = 0;
    SkipBlank(line, &pos);
    if (line[pos] == '\0')
        return 1;
    else
        return 0;
}



/* Checks if given line is a comment (First non-blank character is ';').
   Arguments:
    line    -- Line to check (null-terminated).
   Returns:
    0       -- Line is not a comment.
    1       -- Line is a comment. */
int IsLineComment(char* line) {
    int pos = 0;
    SkipBlank(line, &pos);
    if (line[pos] == ';')
        return 1;
    else
        return 0;
}



/* Checks if given line is a macro declaration.
   (String with first word "macro").
   Arguments:
    line    -- Line to check (null-terminated).
   Returns:
    0       -- Line is not a macro declaration.
    1       -- Line is a macro declaration. */
int IsLineMacroDef(char* line) {
    int pos = 0;
    char word[6]; /* Buffer for holding the first word of line. */

    /* Skipping leading blank characters. */
    SkipBlank(line, &pos);
    /* Getting first word of the line */
    GetNextWord(line, &pos, word, 5, NULL);
    
    /* Checking if this word is "macro" */
    if (CompareStrings(word, "macro")) {
        /* Checking if next character is blank or termination. */
        if (IsBlankChar(line[pos]) || line[pos] == '\0')
            return 1;
        else
            return 0;    
    }

    return 0;
}



/* Checks if line is an end line of macro definition.
   (First word of line is "endm").
   Arguments:
    line    -- Line to check.
   Returns:
    0       -- Line is not a macro definition end.
    1       -- Line is a macro definition end. */
int IsLineMacroDefEnd(char* line) {
    int pos = 0;
    char word[5]; /* Buffer for holding the first word of the line. */

    /* Getting first word of the line */
    GetNextWord(line, &pos, word, 4, NULL);

    /* Checking if this word is "endm" */
    if (CompareStrings(word, "endm")) {
        /* Checking if next character is blank or termination. */
        if (IsBlankChar(line[pos]) || line[pos] == '\0')
            return 1;
        else
            return 0;    
    }

    return 0;
}



/* Checks if given line is a macro call (macro name).
   Arguments:
    line    -- Source file line (terminated).
    macros  -- List of macros.
   Returns:
    0   -- If line is not a macro call.
    1   -- If line is a macro call. */
int IsLineMacroCall(char* line, List* macros) {
    int pos = 0; /* Position in line. */
    char word[MAX_STATEMENT_LEN+2]; /* Buffer for storing words from line. */
    char* res; /* Word reading result. */

    /* If there is macro with a name equal to first word of the line
       line is considered to be macro call. */
    res = GetNextWord(line, &pos, word, MAX_STATEMENT_LEN+1, NULL);
    if (res != NULL) {
        /* Checking if macro with this name exists. */
        if (FindMacroByName(macros, word) != NULL)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}



/* Gets macro name from macro definition line.
   Checks for macro definition line errors.
   Returned name will be allocated on heap.
   Assumes that line is macro definition line (first word is "macro")
   and does not check for that.
   Arguments:
    line        -- Macro definition line.
    defLineNum  -- Number of macro definition line in source file.
    errors      -- List of errors.
   Returns:
    Macro name string, null-terminated.
    NULL if no name is found, or it is illegal.
   Algorithm:
    Uses GetNextWord to read word after "macro" and considers it a macro name.
    Copies name to heap.
    Checks for following errors:
        If name exists ("macro" is not only word in line).
        If there is text after name.
        If name is a reserved word.
        If name starts with a number.
        If name contains only numbers and letters.
    If errors found, except for extra text, NULL is returned. */
char* GetMacroName(char* line, int defLineNum, Errors* errors) {
    int pos = 0; /* Position in line */
    /* Buffer for reading words from definition line.
       +2 for temination and new line characters. */
    char word[MAX_STATEMENT_LEN + 2];
    char* name; /* Macro name */
    
    /* Skipping leading blanks. */
    SkipBlank(line, &pos);

    /* Skipping "macro" keyword. */
    pos += 5;

    /* Getting macro name */
    if (GetNextWord(line, &pos, word, MAX_STATEMENT_LEN+1, NULL) != NULL)
        name = CopyStringToHeap(word);
    else { /* Name not defined */
        AddErrorManual(errors, defLineNum, ErrMacro_NameNotDefined, line, NULL);
        return NULL;
    }

    /* Checking for extra symbols in macro definition */
    /* Extra symbols will be ignored, but error will be displayed. */
    if (GetNextWord(line, &pos, word, MAX_STATEMENT_LEN+1, NULL) != NULL)
        AddErrorManual(errors, defLineNum, ErrMacro_ExtraDef, line, NULL);

    /* Checking if name is reserved assembly word */
    if (IsReservedWord(name)) {
        AddErrorManual(errors, defLineNum, ErrMacro_NameReserved, line, name);
        free(name);
        return NULL;
    }

    /* Checking if name starts with a number. */
    if (IsDigit(name[0])) {
        AddErrorManual(errors, defLineNum, ErrMacro_NameNumber, line, NULL);
        free(name);
        return NULL;
    }

    /* Checking if name contains only numbers and letters */
    if (!IsAz09(name)) {  
        AddErrorManual(errors, defLineNum, ErrMacro_NameIllegal, line, NULL);
        free(name);
        return NULL;
    }

    return name;
}



/* Gets macro info from source file.
   Arguments:
    source      -- Pointer to source file handler.
    num_lines   -- Variable for returning number of macro lines.
    def_line    -- Line (string) where macro name is defined.
    defLineNum  -- Number of line in source file where macro name is defined.
    errors      -- Errors list.
   Returns:
    Directly returns new MacroInfo structure. NULL will be returned if errors encountered.
    Writes number of lines in macro body to num_lines pointer even if getting info failed.
   Algorithm:
    - Allocates info structure.
    - Uses GetMacroName to get name of the macro from definition line. If name not found it is noted.
    - Saves defLineNum to info structure.
    - Uses ftell to save macro body position to info. Since this function is called only from
      Preprocess(...) reading loop when it will be called macro definition line was read 
      and source file position is exactly where macro body starts.
    - Reads macro lines in a loop from source file and counts them including internal comment and blank lines.  
    - Uses open_tags counter to find line last macro closing line (if there were nested macros).
      While reading lines checks for nested macros and registers error if found.
      When macro closing line is reached checks it for extra text.
      If name was not found, or nested macros found deallocates info structure and returns NULL.
      If errors with macro were only extra text after opening and closing lines then macro info is considered
      correct and returned.
      Since from project description it is assumed that any opened macro definition always be closed
      number of lines in macro body counted and written to num_lines pointer even if macro definition
      was incorrect. Number of lines is returned so source file line counter migth be correctly advanced
      even if macro is incorrect.
*/
MacroInfo* GetMacroInfo(FILE** source, int* num_lines, char* defLine, int defLineNum, Errors* errors) {
    MacroInfo* info; /* Pointer for storing macro info. */
    char line[MAX_STATEMENT_LEN+2]; /* Buffer for holding line read from source file. */
    int pos = 0; /* Line iterator. */
    char word[MAX_STATEMENT_LEN+2]; /* Buffer for holding word read from line. */
    int open_tags = 1; /* Counter of opened macro tags.*/
    int failed = 0; /* Flag that shows if errors were encountered. */

    /* Allocating info structure */
    info = (MacroInfo*)malloc(sizeof(MacroInfo));
    if (info == NULL) {
        perror("Failed to allocate memory.\n");
        exit(1); 
    }

    /* Getting macro name */
    info->name = GetMacroName(defLine, defLineNum, errors);
    if (info->name == NULL)
        failed = 1;

    /* Saving first macro body line number. */
    info->body_line_num = defLineNum+1;

    /* Saving macro body start position in file. */
    info->body_pos = ftell(*source);

    /* Searching where macro ends and counting body lines. */
    /* Macro is considered closed when open_tags counter reaches 0
       in another words when all nested macro definitions will be closed.
       Every macro definition line will increase counter and
       every endm line will decrease it. */
       *num_lines = 0;
    while (open_tags>0) {
        /* Counting line. */
        (*num_lines)++;
        /* Reading line. */
        fgets(line, MAX_STATEMENT_LEN+2, *source);

        /* Checking for nested macro definitions. */
        if (IsLineMacroDef(line)) {
            AddErrorManual(errors, defLineNum+(*num_lines), ErrMacro_Nested, NULL, NULL);
            open_tags++;
            failed = 1;
        }    

        /* Checking for definition end tag. */
        if (IsLineMacroDefEnd(line))
            open_tags--;
    }

    /* Uncounting final closing tag line. */
    (*num_lines)--; 

    /* After cycle we arrive to macro closing line "endm". It is 
       assumed that it will always appear. */
    /* Checking if closing tag line contains extra code. */
    /* Extra text will be ignored, but error will be displayed. */
    SkipBlank(line, &pos); /* Skipping blanks */
    pos += 4; /* Skipping "endm" */
    if (GetNextWord(line, &pos, word, MAX_STATEMENT_LEN+1, NULL) != NULL)
        AddErrorManual(errors, defLineNum+*num_lines+1, ErrMacro_ExtraDefEnd, line, NULL);

    /* Writing number of line to info. */
    info->num_lines = *num_lines;

    /* Checking for failure flag. */
    if (failed) {
        /* Deallocating info structure. */
        if (info->name != NULL)
            free(info->name);
        free(info);
        return NULL;
    }
    else
        return info;
}



/* Registers macro definition in list of macros.
   Will set source file position to the first character
   after macro closing tag line.
   Gives no indication if macro was failed to register.
   Arguments:
    source      -- Pointer to source file handler.
    macros      -- Macros list.
    def_line    -- Line (string) where macro name is defined.
    defLineNum  -- Number of line in source file where macro name is defined.
    errors      -- Errors list.
   Returns:
    Number of line in source file after macro closing tag.
   Algorithm:
    Uses GetMacroInfo to read macro info.
    If macro info acquired successfuly and macro with same name is not in the list
    adds macro info to the macros list.
    Uses returned lines value from GetMacroInfo to return number of line after macro.
    Assumes that arguments are correct and does not check them. */
int RegisterMacroInfo(FILE** source, List* macros, char* defLine, int defLineNum, Errors* errors) {
    MacroInfo* info = NULL; /* Variable to store macro info. */
    int num_lines; /* Number of lines in macro body not counting open/close tags. */

    info = GetMacroInfo(source, &num_lines, defLine, defLineNum, errors);
    /* After GetMacroInfo call file position will be set to the first character
       in line after macro endm line.*/

    /* If macro is read successfully */
    if (info != NULL) {
        /* Checking if macro with this name already registered
            and adding it to the list. */
        if (FindMacroByName(macros, info->name) == NULL)
            ListAdd(macros, info);
        else  /* If macro already exists. */
            AddErrorManual(errors, defLineNum, ErrMacro_NameIdentical, defLine, info->name);
    }
    
    return defLineNum+num_lines+2;
}



/* Expands macro by name defined in callLine.
   Copies macro body lines from source file to current position
   in target (expanded) file.
   Arguments:
    source      -- Pointer to source file hanler.
    target      -- Pointer to target (expanded) file handler.
    srcPos      -- Current position in source file (position after macro call line)
    callLine    -- Line of macro call (first word is a macro name)
    callLineNum -- Number of a call line in source file.
    macros      -- List of registered macros.
    errors      -- List of errors.
   Algorithm:
    - Gets macro name.
    - Uses FindMacroByName to acquire appropriate macro info structure. Since
      this function is called only if IsMacroCallLine returned true
      macro info necceserily will be found.
    - Uses fseek and macro info to place source file position to start of macro body.
    - Copies info->num_lines from source file to target file. By doing that it copies 
      macro body to expanded file.
    - Uses srcPos and fseek to return source file position back to line after macro call line.
    Checks if there were text after macro name in call line. Text will be ignored and macro expanded,
    but error will be registered.
    Assumes that provided arguments are correct and does not check them. */
void ExpandMacro(FILE** source, FILE** target, long srcPos, char* callLine, int callLineNum, List* macros, Errors* errors) {
    int i; /* Line terator */
    MacroInfo* minfo; /* Variable for storing found macro info. */
    int pos =0; /* Position in line. */
    char mline[MAX_STATEMENT_LEN+2]; /* Buffer for holding macro body line. */
    char word[MAX_STATEMENT_LEN+2]; /* Buffer for storing word from line. */

    /* Reading macro name. */
    GetNextWord(callLine, &pos, word, MAX_STATEMENT_LEN+1, NULL);

    /* Getting macro info. */
    minfo = FindMacroByName(macros, word);

    /* Checking if there is text after macro name. */
    if (GetNextWord(callLine, &pos, word, MAX_STATEMENT_LEN+1, NULL) != NULL)
        AddErrorManual(errors, callLineNum, ErrMacro_ExtraCall, callLine, NULL);

    /* Copying macro body lines to target file. */

    /* Setting source file position to beginning of macro body. */
    fseek(*source, minfo->body_pos, SEEK_SET);

    /* Copying macro lines from source file to target file. */
    for (i=0; i<(minfo->num_lines); i++) {
        /* Reading macro body line */
        fgets(mline, MAX_STATEMENT_LEN+1, *source);

        /* Checking if line should be copied (not blank, or comment). */
        if (!IsLineBlank(mline) && !IsLineComment(mline)) {
            /* Writing line to target file */
            fputs(mline, *target);
            /* Saving reference to source line number*/
            AddLineReference(errors, (minfo->body_line_num)+i);
        }
    }

    /* Returning source file stream to initial position (line after macro call line). */
    fseek(*source, srcPos, SEEK_SET);

    return;
}



/* Frees memory occupied by macros list.
   Removes macro info objects, 
   list nodes and list structure itself.
   Arguments:
    macros  -- Macros list. */
void FreeMacrosList(List* macros) {
    ListNode* cur;  /* List iterator */
    ListNode* prev; /* Previous node pointer */

    cur = macros->head;
    while (cur != NULL) {
        MacroInfo* info = (MacroInfo*)(cur->data);
        free(info->name);   /* Freeing macro name string. */
        free(info);         /* Freeing macro info structure. */
        prev = cur;         /* Saving current list node pointer. */
        cur = cur->next;    /* Advancing iterator. */
        free(prev);         /* Removing current node. */
    }
    /* Removing list object. */
    free(macros);
}



/* Executes pre-processing step on assembly source code file:
   Removes comments and blank lines and expands macros.
   Arguments:
    sourceFileName      -- Name of source file without extension.
    errors              -- List of errors.
   Returns:
    Source line reference.
   Algorithm:
    Creates macro info list.
    Using AppendExtension combines file name with appropriate extensions.
    Opens source file for reading and target (expanded) file for writing.
    Reads source file line by line and uses functions from Preprocessor.h
    to determine line type:
     - If empty or comment line will not be copied to target.
     - If line is macro definition RegisterMacroInfo will be called.
     - If line is a macro call (existing macro name) lines of macro body will be copied to target file.
     - If line is something else it will be copied to target as it is.
     SourceLineReference is used to write down order of source line numbers copied to target.
     RegisterMacroInfo and Expand macro will check for errors of macro definition and calls and
     errors will be saved to the errors list.
     Assumes that provided arguments are correct and does not check them. */
void Preprocess(char* sourceFileName, Errors* errors) {
    List* macros; /* List of all found macros. */
    FILE* source; /* Source file handler. */
    FILE* target; /* Expanded file handler. */
    char* fullFname; /* Buffer for holding full file name with extension. */
    int fullNameLen; /* Length of full file name with extension not counting termination character. */
    char line[MAX_STATEMENT_LEN+2]; /* Buffer for holding line read from source file. */
    int line_num = 0; /* Number of line that is currently read from source file. (first line to be read will be 1) */

    /* Initializing the list */
    macros = CreateList();

    /* Opening files. */
    fullNameLen = StringLen(sourceFileName) + 3;
    fullFname = (char*)malloc(sizeof(char)*(fullNameLen+1));
    if (fullFname == NULL) { 
        perror("Failed to allocate memory.\n"); 
        exit(1); }
    /* Source file. */
    AppendExtension(sourceFileName, "as", fullFname, fullNameLen);
    source = fopen(fullFname, "r"); /* Opening source file for reading. */
    /* Target file. */
    AppendExtension(sourceFileName, "am", fullFname, fullNameLen);
    target = fopen(fullFname, "w"); /* Opening target file for writing*/
    /* Check. */
    if (source == NULL || target == NULL) { 
        perror("Failed to open file.\n"); 
        exit(2); 
    }

    /* Reading source file line by line. */
    while (fgets(line, MAX_STATEMENT_LEN+2, source) != NULL) {
        line_num++;

        /* Checking if line is empty */
        if (IsLineBlank(line))
            continue;   /* Not copying. */
        
        /* Checking if line is a comment */
        if (IsLineComment(line))
            continue; /* Not copying. */

        /* Checking if line is a macro definition */
        if (IsLineMacroDef(line)) {
            /* Getting info about macro. */
            /* After this call file position will be set to line after macro 
               closing tag. So, we count skipped lines so line_num will be number
               of line where closing endm is found. */
            line_num = RegisterMacroInfo(&source, macros, line, line_num, errors) - 1;

            continue; /* Not copying this line and any of macro definition lines. */
        }

        /* Checking if line is a macro call. */
        if (IsLineMacroCall(line, macros)) {
            /* Expanding macro. */
            ExpandMacro(&source, &target, ftell(source), line, line_num, macros, errors);
            continue; /* Not copying this line*/
        }

        /* If line is not blank, not a comment, not a macro definition
           and not a macro call we copy it as it is. */
        fputs(line, target);
        /* Saving reference to source file number. */
        AddLineReference(errors, line_num);

    } /* File reading cycle end */

    /* Closing files. */
    fclose(source);
    fclose(target);

    /* Freeing memory. */
    FreeMacrosList(macros);
}