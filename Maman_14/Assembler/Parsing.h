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

/* Skips consequitive commas (poissibly separated by blanks) after given position
   and advances position to first non-comma character.
   Returns number of commas skipped.
   Arguments:
    line    -- Instruction line.
    pos     -- Position of character after command name.
   */
int SkipCommas(char* line, int* pos);

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

/* Gets instruction type according by instruction name.
   Argument:
    ins     -- String containing istruction name.
   Returns:
    Instruction number according to InstructionsEnum.
    -1 if instruction name not recognized. */
int GetInstructionType(char* ins);

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

/* Gets next argument string starting from specified position in line.
   Allocates result on heap. Advances position to character after argument.
   Arguments:
    line    -- Instruction line.
    pos     -- Position in instruction line after which next argument should be taken.
   Returns:
    Argument string allocated on heap, or NULL if only blank symbols were after given position.  */
char* GetNextArg(char* line, int* pos);

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

/* Parses a string that is a number. 
   Assumes that string represents a number.
   Arguments:
    s   -- Number in a string form.
   Returns:
    Number as int.
    0 if failed.  */
int ParseNumber(char* s);

/* Parses a register name (rx, rxx)
   and returns number of a register.
   Arguments:
    s   -- String containing register name.
   Returns:
    Number of a register as int.
    -1 if parsing failed. */
int ParseRegisterName(char* s);

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
char* GetIndexer(char* arg, int* pos, int* failed, Errors* errors);

/* Parses label argument of instruction. (label, or label[r0] for example).
   Arguments:
    arg     -- Label argument as string.
    parg    -- Pointer for returning result.
    errors  -- List of errors.
   Returns:
    Pointer to parsed argument structure if succeeded.
    NULL if parsing failed. */
InsArg* ParseLabelArgument(char* arg, InsArg* parg, Errors* errors);

/*Parses instruction argument.
  Arguments:
   arg      -- String containing the argument.
   errors   -- Errors list.
  Returns:
   Structure that describes the argument.
   NULL if failed. */
InsArg* ParseInsArg(char* arg, Errors* errors);


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