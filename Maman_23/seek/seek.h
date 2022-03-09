#ifndef SEEK_H
    #define SEEK_H

#include <stdlib.h>
#include <stdio.h>

/* Takes string that represents non-negative integer number and returns the number.
   Arguments:
    s   - String representation of non-negative integer number (null-terminated, without sign).
   Returns:
    Integer number if parsing is successfull.
    -1 if parsing failed.
   */
int ParseNumberArgument(char* s);


/* Returns ASCII number of a character c. */
int CharToASCIICode(char c);


/* Returns character number n from file that pointed by file pointer fp.
   Assumes that file pointer is not NULL and does not check it.
   Assumes that n is positive integer and does not check it.
   Arguments:
    fp      - File pointer.
    n       - Number of a character to read. (Number of the first character is 1)
   Returns:
    char    - Character at position n-1. 
              -1 if end of file encountered before n'th symbol
    */
char GetCharAt(FILE* fp, int n);


/* Function that prints formatted message containing file name,
   character number char_num in this file, and its ASCII CODE.
   Assumes that arguments are valid.
   Arguments:
    fileName    - String that contains file name, null terminated.
    char_num    - Number of printed character.
    c           - Character to print
*/
void PrintASCII(char* fileName, int char_num, char c);

#endif