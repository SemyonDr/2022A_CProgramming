#include "Parsing.h"

/* Returns 1 if character c is a number
   and 0 if it is not. */
int IsNumber(char c) {
    if (c >=48 && c <= 57)
        return 1;
    else
        return 0;
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

/* Tries to get label before assembly statement.
   Label considered to be first word in line if it ends with ':' character.
   If label is found moves line position to character after ':'.
   If label wasn't found leaves position as it was.
   Arguments:
    line    -- Statement line (null-terminated).
    pos     -- Position in line. Assumed to be beggining of the line when function is called.
    label   -- Buffer to hold label string.
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
    if (res != NULL && line[_pos] == ':' && (IsBlankChar(line[_pos+1]) || line[_pos+1] == '\0')) {
        *pos = _pos+1; /* Moving position to character after ':'. */
        return label;
    }
    else
        return NULL;
}