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

/* Searches word in given line and returns position
   where word starts if it is present. If word is 
   present multiple times first occurence is returned.
   Arguments:
    line    -- line to search word in (null-terminated)
    word    -- word to search (null-terminated)
   Return:
    If word is found position of first word symbol in line returned.
    If word isn't found -1 is returned.  */
int FindWord(char* line, char* word) {
    int w_start = 0; /* First position of found word. */
    int line_pos = 0; /* Position in line. */
    int word_pos = 0; /* Position in word when comparing. */

    /* In case word has 0 length. */
    if (word[0] == '\0')
        return 0;

    while (line[line_pos] != '\0') {
        if (line[line_pos] == word[word_pos]) {
            /* If symbols match advancing position in word. */
            if (word_pos == 0)
                w_start = line_pos; /* Remembering where word starts */
            word_pos++;
            
            /* If full word is found returning result. */
            if (word[word_pos] == '\0')
                return w_start;
        }
        else {
            /* If symbol doesn't match resetting position in word. */
            word_pos = 0;
        }        

        line_pos++;
    }

    /* If word is not already found it doesn't exist in the line*/
    return -1;
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

/* 
   Tries to get label before assembly statement.
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

/* Gets arguments from given line. Advances 
   line position to line termination character.
   Checks for comma errors.
   Arguments:
    line    -- Instruction line
    pos     -- Position in line after instruction or directive name.
    errors  -- List of errors.
    lineNum -- Number of given line in expanded source code.
    slr     -- Source line reference.
   Returns:
    List of stings of arguments.  */
List* GetArgs(char* line, int* pos, List* errors, int lineNum, DArrayInt* slr) {
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
        AddError(errors, slr->data[lineNum], ErrCmm_Before, line);

    /* Getting arguments in a loop.*/
    while ((arg = GetNextArg(line, pos)) != NULL) {
        /* Adding argument to the list. */
        ListAdd(args, arg);
        
        /* Checking for comma errors between arguments. */
        num_commas = SkipCommas(line, pos);
        /* If arguments wasn't last in line. */
        if (line[*pos] != '\0') {
            if (num_commas > 1) 
                AddError(errors, slr->data[lineNum], ErrCmm_Multiple, line);
            if (num_commas == 0)
                AddError(errors, slr->data[lineNum], ErrCmm_Missing, line);
        } /* If argument was the last one. */
        else {
            if (num_commas != 0)
                AddError(errors, slr->data[lineNum], ErrCmm_After, line);
        }
    }

    printf("DEBUG: \t\tGetArgs: Got %d arguments.\n", args->count);

    return args;
}

/* Parses a string that is a number. 
   Assumes that string represents a number. */
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

/* -1 if not a register. */
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

/* Tries to get indexer from label argument.
   Moves position to character after closing bracket.
   Allocates resulting string on heap.
   NULL if not found (brackets are empty or not exist).
 */
char* GetIndexer(char* arg, int* pos, int* failed, List* errors, int lineNum, DArrayInt* slr) {
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
        AddError(errors, slr->data[lineNum], ErrArg_MissingIndex, arg);
        *failed = 1;
        return NULL;
    }

    /* If ']' found advancing position to next character. */
    if (arg[*pos] == ']') {
        (*pos)++;
        /* Checking for extra text after closing bracket. */
        if (arg[*pos] != '\n' && arg[*pos] != '\0') {
            AddError(errors, slr->data[lineNum], ErrArg_Extra, arg);
            /* Not considered failure, but error will be registered. */
        }
    }
    else /* If there is no closing bracket, but name is present. */ {
        AddError(errors, slr->data[lineNum], ErrArg_MissingBracket, arg);
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

/* Writes into provided structure, returns NULL if failed, or pointer to structure. */
InsArg* ParseLabelArgument(char* arg, InsArg* parg, List* errors, int lineNum, DArrayInt* slr) {
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
        AddError(errors, slr->data[lineNum], ErrArg_InvalidLabel, arg);
        failed = 1;
    }

    /* Checking if label is less than 31 character. */
    if (pos == 31 && arg[pos] != '[' && arg[pos] != '\0') {
        AddError(errors, slr->data[lineNum], ErrArg_LongSymbol, arg);
        failed = 1;
    }

    /* Trying to get indexer. */
    if ((indexer = GetIndexer(arg, &pos, &failed, errors, lineNum, slr)) != NULL) {
        /* If indexer is present trying to parse it. */
        parg->val = ParseRegisterName(indexer);
        if (parg->val == -1) { /* If failed to parse. */
            AddError(errors, slr->data[lineNum], ErrArg_InvalidIndex, arg);
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

/* Parses instruction argument.
NULL if failed! */
InsArg* ParseInsArg(char* arg, List* errors, int lineNum, DArrayInt* slr) {
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
            AddError(errors, slr->data[lineNum], ErrArg_NotANumber, arg);
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
    return ParseLabelArgument(arg, parg, errors, lineNum, slr);
}

