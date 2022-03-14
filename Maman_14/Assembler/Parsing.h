#ifndef PARSING_H
    #define PARSING_H

#include <stdlib.h>
#include <stdio.h>
#include "Definitions.h"
#include "DataContainers.h"
#include "Data.h"
#include "MyString.h"
#include "Errors.h"

/* Returns 1 if character c is a number
   and 0 if it is not. */
int IsDigit(char c);

/* Returns 1 if line is a number
   and 0 if it is not. */
int IsNumber(char* s);

/* Checks if string s contains only
   letter and number characters.
   Arguments:
    s   -- Line to check.
   Returns:
    0   -- If s contains a character that is not a number, or letter.
    1   -- All characters of s are numbers and letters. */
int IsAz09(char* s);

/* Checks if character c is a blank character.
   Blank characters are ' ' '\t' '\n'.
   Arguments:
    c   -- Character to check.
   Returns:
    0   -- c is not a blank character.
    1   -- c is a blank character. */
int IsBlankChar(char c);

/* Skips blank characters (space and tab) in
   given string starting from position pos.
   Returns position of first non-blank character met.
   Arguments:
    s   -- String (null terminated)
    pos -- Pointer to position in string from which to start to skip 
           characters.
   Returns:
    pos will be set to position where first non-blank character is met.*/
void SkipBlank(char* s, int* pos);


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
char* GetNextWord(char* line, int* pos, char* word, int maxLen, char* end);

/* Searches word in given line and returns position
   where word starts if it is present. If word is 
   present multiple times first occurence is returned.
   Arguments:
    line    -- line to search word in (null-terminated)
    word    -- word to search (null-terminated)
   Return:
    If word is found position of first word symbol in line returned.
    If word isn't found -1 is returned.  */
int FindWord(char* line, char* word);

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
int IsReservedWord(char* s);

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
char* TryGetLabel(char* line, int* pos, char* label, int maxLen);

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
List* GetRawArgs(char* line, int* pos, Errors* errors);

/* Parses instruction argument.
NULL if failed! */
InsArg* ParseInsArg(char* arg, List* errors, int lineNum, DArrayInt* slr);

int ParseNumber(char* s);

char* GetSymbolDirectiveArgument(char* line, int* pos, char arg[MAX_LABEL_LEN+1], int lineNum, List* errors, DArrayInt* slr);

/* Parses list of .data directive numeric arguments given as strings.
   Returns dynamic array containing integer value.
   Arguments:
    line    -- Line with .data directive.
    rawArgs -- Arguments as strings.
    errors  -- Errors list.
   Returns:
    Dynamic array containing integer values of corresponding arguments.
    NULL if parsing failed. */
DynArr* ParseDataArgs(char* line, List* rawArgs, List* errors);
#endif