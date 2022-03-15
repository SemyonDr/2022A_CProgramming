#include "Parsing.h"

/* Returns 1 if character c is a number
   and 0 if it is not. */
int IsDigit(char c) {
    if (c >=48 && c <= 57)
        return 1;
    else
        return 0;
}



/* Returns 1 if line is a number
   and 0 if it is not. */
int IsNumber(char* s) {
    int pos = 0;

    /* Checking for leading sign. */
    if (s[pos] == '+' || s[pos] == '-') {
        if (s[pos] == '\0')
            return 0;
        else
            pos++;
    }

    /* Checking rest of the number. */
    while (s[pos] != '\0') {
        if (s[pos] <= 48 || s[pos] >=57)
            return 0;
        pos++;
    }
    return 1;
}



/* Checks if string s contains only
   letter and number characters.
   Arguments:
    s   -- Line to check.
   Returns:
    0   -- If s contains a character that is not a number, or letter.
    1   -- All characters of s are numbers and letters. */
int IsAz09(char* s) {
    int i=0; /* Iterator */

    while (s[i] != '\0') {
        if (s[i] < 48)
            return 0;
        if (s[i] > 57 && s[i] < 65)   
            return 0;
        if (s[i] > 90 && s[i] < 97)    
            return 0;
        if (s[i] > 122)
            return 0;
        i++;
    }

    return 1;
}



/* Checks if character c is a blank character.
   Blank characters are ' ' '\t' '\n'.
   Arguments:
    c   -- Character to check.
   Returns:
    0   -- c is not a blank character.
    1   -- c is a blank character. */
int IsBlankChar(char c) {
    if (c == ' ' || c == '\t' || c=='\n')
        return 1;
    return 0;
}



/* Skips blank characters (space and tab) in
   given string starting from position pos.
   Returns position of first non-blank character met.
   Arguments:
    s   -- String (null terminated)
    pos -- Pointer to position in string from which to start to skip 
           characters.
   Returns:
    pos will be set to position where first non-blank character is met.*/
void SkipBlank(char* s, int* pos) {
    while (IsBlankChar(s[*pos]) && s[*pos] != '\0') {
        (*pos)++;
    }
}



/* Skips consequitive commas (poissibly separated by blanks) after given position
   and advances position to first non-comma character.
   Returns number of commas skipped.
   Arguments:
    line    -- Instruction line.
    pos     -- Position of character after command name.
   */
int SkipCommas(char* line, int* pos) {
    int num = 0; /* Number of commas. */

    SkipBlank(line, pos);
    while (line[*pos] == ',') {
        num++;      /* Counting comma. */
        (*pos)++;   /* Moving position to the next char. */
        SkipBlank(line, pos);
    }

    return num;
}



/* Checks if string is a reserved assembly
   word and returns its type.
   Arguments:
    s   -- String to check (null-terminated)
   Returns:
    0   -- s not a reserved word.
    1   -- s is an instruction name.
    2   -- s is a register name.
    3   -- s is a keyword.
*/
int IsReservedWord(char* s) {
    int i; /* Iterator */
    char rx[3] = "r0"; /* Register name <10 */
    char rxx[4] = "r10"; /* Register name >= 10 */
    char* instructions[16]; /* Array of instruction names */
    char* keywords[9];  /* Array of assembly keywords */

    /* Listing instructions names */
    instructions[0] = "mov";
    instructions[1] = "cmp";
    instructions[2] = "add";
    instructions[3] = "sub";

    instructions[4] = "lea";
    instructions[5] = "clr";
    instructions[6] = "not";
    instructions[7] = "inc";

    instructions[8] = "dec";
    instructions[9] = "jmp";
    instructions[10] = "bne";
    instructions[11] = "jsr";

    instructions[12] = "red";
    instructions[13] = "prn";
    instructions[14] = "rts";
    instructions[15] = "stop";
    
    /* Checking if s is an instruction name. */
    for (i=0; i<16; i++) {
        if (CompareStrings(s, instructions[i]))
            return 1;
    }

    /* Checking if s is an register name. */
    for (i = 0; i<10; i++) {
        rx[1] = (48 + i);
        if (CompareStrings(s, rx))
            return 2;
        if (i <=5) {
            rxx[2] = 48 + i;
            if (CompareStrings(s, rxx))
                return 2;
        }
    }    

    /* Listing keywords. */
    keywords[0] = "macro";
    keywords[1] = "endm";
    keywords[2] = "data";
    keywords[3] = ".data";

    keywords[4] = "string";
    keywords[5] = ".string";
    keywords[6] = "extern";
    keywords[7] = ".extern";

    keywords[8] = "entry";
    keywords[9] = ".entry";

    /* Checking if s is a keyword */
    for (i=0; i<9; i++) {
        if (CompareStrings(s, keywords[i]))
            return 3;
    }

    /* If not a reserved word */
    return 0;
}



/* Gets the word from given line starting from given
   position and advances position counter. Considers that word ends
   if new line, termination, space, tab or any of "end" characters are met.
   Skips leading blank characters.
   Arguments:
    line    -- Source line (null-terminated)
    pos     -- Initial position in line.
    word    -- Buffer string to hold word.
    maxLen  -- Maximum length of a word to get (without termination character)
    end     -- String containing additional characters that considered word end.
               Can be NULL.
   Returns:
    Directly returns word from the line, or NULL if nothing was taken.
    word    -- Word from line.
    pos     -- Will be set to first character after the word.
*/
char* GetNextWord(char* line, int* pos, char* word, int maxLen, char* end) {
    int c = 1; /* Flag that shows if copying of the word should continue. */
    int word_pos = 0; /* Iterator for resulting word. */

    /* Checking if buffer has size or exists */
    if (word == NULL || maxLen == 0)
        return NULL;

    /* Skipping leading blanks */
    SkipBlank(line, pos);

    /* Checking if line has ended after leading blanks. */
    if (line[*pos] == '\0')
        return NULL;

    /* Copying word characters and searching for the end. */
    while (c) {
        /* Iterator for "end" characters */
        int end_iter = 0; 

        /* Copying character */
        word[word_pos] = line[*pos];

        /* Advancing iterators */
        word_pos++;
        (*pos)++;

        /* Checking if max word length was read */
        if (word_pos == maxLen)
            c = 0;

        /* Checking if blank character is met */
        if (c && IsBlankChar(line[*pos]))
            c = 0;

        /* Checking if line has ended */
        if (c && (line[*pos] == '\0'))
            c = 0;

        /* Checking if any of "end" symbols are met */
        if (c && end != NULL) {
            while (end[end_iter] != '\0') {
                if (line[*pos] == end[end_iter])
                    c = 0;
                end_iter++;
            }
        }
    }

    /* Adding termination */
    word[word_pos] = '\0';

    return word;
}



/* Gets instruction type according by instruction name.
   Argument:
    ins     -- String containing istruction name.
   Returns:
    Instruction number according to InstructionsEnum.
    -1 if instruction name not recognized. */
int GetInstructionType(char* ins) {
    if (CompareStrings(ins, "mov"))
        return ins_mov;
    if (CompareStrings(ins, "cmp"))
        return ins_cmp;
    if (CompareStrings(ins, "add"))
        return ins_add;
    if (CompareStrings(ins, "sub"))
        return ins_sub;

    if (CompareStrings(ins, "lea"))
        return ins_lea;
    if (CompareStrings(ins, "clr"))
        return ins_clr;
    if (CompareStrings(ins, "not"))
        return ins_not;
    if (CompareStrings(ins, "inc"))
        return ins_inc;

    if (CompareStrings(ins, "dec"))
        return ins_dec;
    if (CompareStrings(ins, "jmp"))
        return ins_jmp;
    if (CompareStrings(ins, "bne"))
        return ins_bne;
    if (CompareStrings(ins, "jsr"))
        return ins_jsr;

    if (CompareStrings(ins, "red"))
        return ins_red;
    if (CompareStrings(ins, "prn"))
        return ins_prn;
    if (CompareStrings(ins, "rts"))
        return ins_rts;
    if (CompareStrings(ins, "stop"))
        return ins_stop;

    return -1;
}



/* Tries to get label before assembly statement.
   Label considered to be first word in line if it ends with ':' character.
   If label is found moves line position to character after ':'.
   If label wasn't found leaves position as it was.
   Arguments:
    line    -- Statement line (null-terminated).
    pos     -- Position in line. Assumed to be beggining of the line when function is called.
    label   -- Buffer to hold resulting label string.
    maxLen  -- Maximum length to read (without termination character).
   Returns:
    If label is found it will be written to label buffer and pointer will be returned.
    If label isn't found function will return NULL. */
char* TryGetLabel(char* line, int* pos, char* label, int maxLen) {
    int _pos = 0; /* Position in file internal for this function. */
    char* res;  /* Result of reading a word. */

    /* Getting first word in line and considering : end of the word. */
    res = GetNextWord(line, &_pos, label, MAX_STATEMENT_LEN+2, ":");

    /* Checking if first word was a label. */
    if (res != NULL
        && line[_pos] == ':' 
        && (IsBlankChar(line[_pos+1]) || line[_pos+1] == '\0')) {
        *pos = _pos+1; /* Moving position to character after ':'. */
        return label;
    }
    else
        /* Position wasn't moved. */
        return NULL;
}



/* Gets next argument string starting from specified position in line.
   Allocates result on heap. Advances position to character after argument.
   Arguments:
    line    -- Instruction line.
    pos     -- Position in instruction line after which next argument should be taken.
   Returns:
    Argument string allocated on heap, or NULL if only blank symbols were after given position.  */
char* GetNextArg(char* line, int* pos) {
    char* arg;  /* String to hold the argument */
    char* res;  /* Result of getting the argument. */

    /* Allocating result string. */
    arg = (char*)malloc(sizeof(char)*(MAX_STATEMENT_LEN+2));
    if (arg == NULL) {
        perror("Failed to allocate memory.");
        exit(1);
    }

    /* Getting next argument considering ',' end of the word. */
    res = GetNextWord(line, pos, arg, MAX_STATEMENT_LEN+1, ",");

    /* If nothing was taken. */
    if (res == NULL) {
        free(arg);
        return NULL;
    }
    printf("DEBUG: \t\tGot argument [%s]. Now pos is %d.\n", arg, *pos);
    return arg;
}



/* Gets arguments from given line as strings.
   Advances line position to line termination character.
   Checks for comma errors.
   Allocates argument strings on heap.
   Arguments:
    line    -- Instruction line
    pos     -- Position in line after instruction or directive name.
    errors  -- List of errors.
    lineNum -- Number of given line in expanded source code.
    slr     -- Source line reference.
   Returns:
    List of stings of arguments. Empty list will be returned if no arguments.*/
List* GetRawArgs(char* line, int* pos, Errors* errors) {
    List* args; /* List for holding arguments. */
    char* arg; /* Variable for storing an argument. */
    int num_commas; /* Variable for storing number of commas. */

    /* Initializing the list. */
    args = CreateList();

    /* Skipping possible illegal commas after command name
       and before arguments. */
    num_commas = SkipCommas(line, pos);
    /* Checking if illegal commas were present. */
    if (num_commas>0)
        /* Adding error and continuing. */
        AddError(errors, ErrCmm_Before, line, NULL);

    /* Getting arguments in a loop.*/
    while ((arg = GetNextArg(line, pos)) != NULL) {
        /* Adding argument to the list. */
        ListAdd(args, arg);
        
        /* Checking for comma errors between arguments. */
        num_commas = SkipCommas(line, pos);
        /* If arguments wasn't last in line. */
        if (line[*pos] != '\0') {
            if (num_commas > 1) 
                AddError(errors, ErrCmm_Multiple, line, NULL);
            if (num_commas == 0)
                AddError(errors, ErrCmm_Missing, line, NULL);
        } /* If argument was the last one. */
        else {
            if (num_commas != 0)
                AddError(errors, ErrCmm_After, line, NULL);
        }
    }

    printf("DEBUG: \t\tGetArgs: Got %d arguments.\n", args->count);

    return args;
}



/* Parses a string that is a number. 
   Assumes that string represents a number.
   Arguments:
    s   -- Number in a string form.
   Returns:
    Number as int.
    0 if failed.  */
int ParseNumber(char* s) {
    char sign = '+'; /* Character for storing sign of the number. */
    int pos = 0; /* Position in digits string. */
    int num = 0; /* Resulting number. */
    int multiplier = 1; /* Represents 10^n. */
    char* digits = s;   /* Line without sign character. */

    /* If string is empty, or doesn't exists. */
    if (s[0] == '\0' || s == NULL)
        return 0;

    /* Checking if line begins with a sign. */
    if (s[0] == '+' || s[0] == '-') {
        digits = &(s[1]);
        sign = s[0];
    }

    /* Moving position to the last digit in the digits string. */
    while(digits[pos+1] != '\0')
        pos++;

    /* Going from end of the string to beginning converting
       characters to digits and multiplying them by 10^n. */
    while (pos != -1) {
        num += (digits[pos]-48)*multiplier;
        pos--;
        multiplier *= 10;
    }

    /* Applying sign. */
    if (sign == '-')
        num = num*(-1);

    return num;
}



/* Parses a register name (rx, rxx)
   and returns number of a register.
   Arguments:
    s   -- String containing register name.
   Returns:
    Number of a register as int.
    -1 if parsing failed. */
int ParseRegisterName(char* s) {
    /* Checking if first character is 'r'. */
    if (s[0] != 'r')
        return -1;

    /* Checking if 'r' isn't only character. */
    if (s[1] == '\0')
        return -1;

    /* If string is rx where x is a number. */
    if (s[2] == '\0' && IsDigit(s[1]))
        return s[1] - 48; /* Returning x. */

    /* If string is rxx where xx is a number. */
    if (s[3] == '\0' && IsDigit(s[1]) && IsDigit(s[2])) {
        int num = (s[1]-48)*10 + (s[2]-48); /* Coverting characters to int. */
        if (num < 16) /* If number is valid register number */
            return num; /* Returning xx. */
    }
        
    /* If anything else it's not a register name. */
    return -1;
}



/* Tries to get indexer part as string from label argument.
   (rxx from argument label[rxx]).
   Moves position to character after the closing bracket.
   Allocates resulting string on heap.
   Arguments:
    arg     -- Label argument.
    pos     -- Position after label name.
    failed  -- Pointer to result indicator set to 1 if parsing errors encountered.
               If indexer not found left as it was.
   Returns:
    Indexer content as sting (if arg is label[r0] "r0" will be returned).
 */
char* GetIndexer(char* arg, int* pos, int* failed, Errors* errors) {
    char* indexer; /* Variable for holding indexer. */
    int i =0; /* Indexer string iterator. */
    int start = 0; /* Starting position of indexer name. */
    int len = -1; /* Length of indexer name. */

    /* Searching for opening bracket '['. */
    while (arg[*pos] != '[' && arg[*pos] != '\0')
        (*pos)++;

    /* If opening bracket not present there is no indexer. */
    if (arg[*pos] == '\0')
        return NULL;

    /* Setting name start character. */
    start = (*pos) + 1;

    /* Counting name length. */
    while (arg[*pos] != ']' && arg[*pos] != '\0') {
        (*pos)++;
        len++;
    }

    /* If there is no name after opening bracket. */
    if (len == 0) {
        AddError(errors, ErrArg_MissingIndex, arg, NULL);
        *failed = 1;
        return NULL;
    }

    /* If ']' found advancing position to next character. */
    if (arg[*pos] == ']') {
        (*pos)++;
        /* Checking for extra text after closing bracket. */
        if (arg[*pos] != '\n' && arg[*pos] != '\0') {
            AddError(errors, ErrArg_Extra, arg, NULL);
            /* Not considered failure, but error will be registered. */
        }
    }
    else /* If there is no closing bracket, but name is present. */ {
        AddError(errors, ErrArg_MissingBracket, arg, NULL);
        *failed = 1;
    }
    
    /* Allocating indexer string.*/
    indexer = (char*)malloc(sizeof(char)*(len+1));
    if (indexer == NULL) {
        perror("Failed to allocate memory.");
        exit(1);
    }

    /* Copying indexer */
    while (i<len) {
        indexer[i] = arg[start+i];
        i++;
    }
    /* Adding termination character. */
    indexer[i] = '\0';

    return indexer;
}



/* Writes into provided structure, returns NULL if failed, or pointer to structure.
   Parses label argument of instruction. (label, or label[r0] for example).
   Arguments:
    arg     -- Label argument as string.
    parg    -- Pointer for returning result.
    errors  -- List of errors.
   Returns:
    Pointer to parsed argument structure if succeeded.
    NULL if parsing failed. */
InsArg* ParseLabelArgument(char* arg, InsArg* parg, Errors* errors) {
    int failed = 0; /* Flag that shows if errors were found while parsing label.
                        Used for accumulating error codes before returning NULL from function. */
    int pos = 0; /* Char position in arg. */
    char* indexer; /* Variable for holding indexer part if present (content of [rx] brackets). */

    /* Copying label until end of argument, or [ ].
        i.e. copying label name without indexer part. */
    while (arg[pos] != '\0' && pos<MAX_LABEL_LEN && arg[pos] != '[') {
        (parg->label)[pos] = arg[pos];
        pos++;
    }
    /* Adding line termination character. */
    (parg->label)[pos] = '\0';

    /* Checking if label name is valid. */
    if (!IsAz09(parg->label) || IsDigit(parg->label[0])) {
        AddError(errors, ErrArg_InvalidLabel, arg, NULL);
        failed = 1;
    }

    /* Checking if label is less than 31 character. */
    if (pos == 31 && arg[pos] != '[' && arg[pos] != '\0') {
        AddError(errors, ErrArg_LongSymbol, arg, NULL);
        failed = 1;
    }

    /* Trying to get indexer. */
    if ((indexer = GetIndexer(arg, &pos, &failed, errors)) != NULL) {
        /* If indexer is present trying to parse it. */
        parg->val = ParseRegisterName(indexer);
        if (parg->val == -1) { /* If failed to parse. */
            AddError(errors, ErrArg_InvalidIndex, arg, NULL);
            failed = 1;
        }
        /* Freeing memory. */
        free(indexer);
        /* If index was found and parsed addressing mode is direct index. */
        parg->amode = am_index;
    }
    else {
        /* If no index present addressing mode is direct. */
        parg->amode = am_direct;
    }

    /* If errors were found while parsing label argument returning NULL. */
    if (failed) {
        free(parg);
        return NULL;
    }
    else
    /* If label argument parsed succesfully. */
        return parg;
}



/*Parses instruction argument.
  Arguments:
   arg      -- String containing the argument.
   errors   -- Errors list.
  Returns:
   Structure that describes the argument.
   NULL if failed. */
InsArg* ParseInsArg(char* arg, Errors* errors) {
    InsArg* parg;   /* Parsed argument. */

    /* Allocating argument structure. */
    parg = (InsArg*)malloc(sizeof(InsArg));

    /* Checking if argument is direct number. */
    if (arg[0] == '#') {
        if (arg[1] != '\0' && IsNumber(&(arg[1]))) {
            parg->amode = am_immediate; /* Setting adressing mode to direct. */
            parg->val = ParseNumber(&(arg[1])); /* Parsing number (without a #) */
            return parg;
        }
        else {
            /* If arg starts with # it should be a number. */
            AddError(errors, ErrArg_NotANumber, arg, NULL);
            free(parg);
            return NULL;
        }
    }

    /* Checking if argument is a register if it starts with 'r'. */
    if (arg[0] == 'r') {
        /* Trying to parse register name. */
        parg->val = ParseRegisterName(arg);
        if (parg->val != -1) { /* Parsed successfully. */
            parg->amode = am_rdirect; /* Setting adressing mode to register direct. */
            return parg;
        }
    }

    /* Now if argument isn't # number, or rxx register it is a label possibly with index. */ 
    return ParseLabelArgument(arg, parg, errors);
}

char* GetSymbolDirectiveArgument(char* line, int* pos, char arg[MAX_LABEL_LEN+1], Errors* errors) {
    int i = 0; /* Iterator. */
    char word[MAX_STATEMENT_LEN+2]; /* Buffer wor storing word from line. */
    char* res;  /* Result of getting the word. */

    /* Getting argument word. */
    res = GetNextWord(line, pos, word, MAX_STATEMENT_LEN+1, NULL);

    /* Checking if word is present. */
    if (res == NULL) {
        AddError(errors, ErrDir_NoArgument, line, NULL);
        return NULL;
    }

    /* Checking if word not exeeds MAX_LABEL_LEN. */
    if (StringLen(word) > 31) {
        AddError(errors, ErrArg_LongSymbol, line, NULL);
        return NULL;
    }

    /* Checking if word is AZ09 and starts with a letter. */
    if (!IsAz09(word) || IsDigit(word[0])) {
        AddError(errors, ErrArg_InvalidLabel, line, NULL);
        return NULL;
    }

    /* Copying the word to output buffer. */
    while (word[i] != '\0') {
        arg[i] = word[i];
        i++;
    }
    arg[i] = '\0'; /* Adding termination sign */

    /* Checking for extra text after argument. */
    res = GetNextWord(line, pos, word, MAX_STATEMENT_LEN+1, NULL);
    if (res != NULL) {
        /* Argument will be returned, but error will be saved. */
        AddError(errors, ErrDit_Extra, line, NULL);
    }

    return arg;   
}

/* Parses list of .data directive numeric arguments given as strings.
   Returns dynamic array containing integer value.
   Arguments:
    line    -- Line with .data directive.
    rawArgs -- Arguments as strings.
    errors  -- Errors list.
   Returns:
    Dynamic array containing integer values of corresponding arguments.
    NULL if parsing failed. */
DynArr* ParseDataArgs(char* line, List* rawArgs, List* errors) {
    DynArr* pargs; /* Array of parsed arguments values. */
    ListNode* cur; /* List iterator. */

    /* Creating result array. */
    pargs = CreateDArrayInt(8);

    /* If no arguments in the list. */
    if (rawArgs->count == 0) {
        AddError(errors, ErrDir_NoArgument, line, NULL);
        FreeDynArr(pargs);
        return NULL;
    }

    /* Parsing arguments in a loop. */
    cur = rawArgs->head; /* Initializing iterator. */
    while (cur != NULL) {
        /* Checking if raw argument is a number and parsing it. */
        if (IsNumber(cur->data))
            AddDynArr(pargs, ParseNumber(cur->data));
        else {
            /* Invalid argument encountered. */
            AddError(errors, ErrDt_DtInvalidArg, line, cur->data);
            FreeDynArr(pargs);
            return NULL;
        }
        cur = cur->next; /* Advancing iterator. */
    }

    return pargs;
}

/* Takes line with pos after ".string"  directive keyword.
   Parses .string argument - Checks if quotation marks are correct and 
   returns string inside them.
   Arguments:
    line    -- Directive line. For error messages.
    arg     -- .string argument with quotation marks.   
    errors  -- Errors list.    
   Returns:
    Parsed string argument. 
*/
char* ParseStringArgument(char* line, char* arg, Errors* errors) {
    int start; /* Position of the first character inside "". */
    int len = 0; /* Length of string inside "". */
    int pos = 0; /* Position in arg string. */
    int ipos = 0; /* Position inside of string between "". */
    char* str; /* Parsed string. */

    /* Checking if first character is ". */
    if (arg[pos] != '"') {
        AddError(errors, ErrDt_StrInvalidArg, line, arg);
        return NULL;
    }

    /* Advancing position by 1 and marking start. */
    pos++;
    start = pos;

    /* Searching for closing " and counting length.*/
    while (arg[pos] != '"' && arg[pos] != '\0') {
        pos++;
        len++;
    }
    
    /* If string wasn't closed. */
    if (arg[pos] == '\0') {
        AddError(errors, ErrDt_StrMissingClosing, line, arg);
        return NULL;
    }

    /* Checking for extra text. */
    pos++; /* Moving to closing ". */
    if (!IsBlank(arg[pos]) && arg[pos] != '\0') {
        AddError(errors, ErrDt_StrExtra, line, NULL);
        return NULL;
    }

    /* Allocating result string. */
    str = (char*)malloc(sizeof(char)*(len+1));
    if (str == NULL) {
        perror("Failed to allocate memory.");
        exit(1);
    }

    /* Copying string content. */
    for (ipos=0; ipos<len; ipos++)
        str[ipos] = line[start+ipos];

    /* Adding termination character. */
    str[ipos] = '\0';

    return str;
}