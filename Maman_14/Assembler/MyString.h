#ifndef MYSTRING_H
    #define MYSTRING_H

#include <stdlib.h>
#include <stdio.h>
#include "Definitions.h"

/* Returns length of null-terminated string s.
   Argumetns:
    s   -- string
   Returns:
    length of s without null-terminating character.*/
int StringLen(char* s);

/* Append string s2 after string s1 and returns
   resulting string of maximum length res_len.
   Arguments:
    s1      -- First string
    s2      -- Second string
    result  -- Buffer to hold concatenated string
    res_len -- Length of result string in chars not including null-termination. 
*/
void ConcatenateStrings(char* s1, char* s2, char* result, int res_len);

/* Compares two strings and returns value that
   indicates if they are identical. 
   Arguments:
    s1  -- First string (null-terminated)
    s2  -- Second string (null-terminated)
   Returns:
    0   -- Strings are not identical.
    1   -- Strings are identical. */
int CompareStrings(char* s1, char* s2);

/* Allocates copy of string s on heap.
   Arguments:
    s   -- String to copy.
   Returns:
    Copy of string s allocated on heap. */
char* CopyStringToHeap(char* s);

/* Append file extension to file name.
   User should provide "fullFname" buffer of appropriate size.
   Arguments:
    fname       -- File name without extension.
    ex          -- Extension without a dot. 
    fullFname   -- Resulting file name with extension.
    fullLen     -- Length of resulting file name with extension (not counting termination character).
*/
void AppendExtension(char* fname, char* ex, char* fullFname, int fullLen);

/* Removes new line characters in line
   by replacing them with c character.
   Arguments:
    line    -- Null-terminated line.
    c       -- Character to replace new line with.
   Returns:
    Modifies given line. */
void ReplaceNewLine(char* line, char c);

/* Modifies line by removing leading
   blank characters (' ' '\t' '\n'). 
   Arguments:
    line    -- Null-terminated line.
   Returns:
    Modifies given line. */
void RemoveLeadingBlanks(char* line);
#endif