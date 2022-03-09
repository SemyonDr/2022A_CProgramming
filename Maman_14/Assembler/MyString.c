#include "MyString.h"

/* Returns length of null-terminated string s.
   Argumetns:
    s   -- string
   Returns:
    length of s without null-terminating character.*/
int StringLen(char* s) {
    int len = 0;
    while (s[len] != '\0')
        len++;
    return len;
}

/* Append string s2 after string s1 and returns
   resulting string of maximum length res_len.
   Arguments:
    s1      -- First string
    s2      -- Second string
    result  -- Buffer to hold concatenated string
    res_len -- Maximum length of result string in chars not including null-termination. 
*/
void ConcatenateStrings(char* s1, char* s2, char* result, int res_len) {
    int pos = 0; /* string iterator */
    int s1_len = 0; /* Length of first string */

    /* Checking if buffer string is not null, or has length. */
    if (result == 0 || res_len == 0)
        return;

    /* Copying the first string. */
    while (s1[pos] != '\0' && res_len != 0) {
        result[pos] = s1[pos];
        pos++;
        res_len--;
    }

    s1_len = pos;
    pos = 0;
    while (s2[pos] != '\0' && res_len != 0) {
        result[s1_len+pos] = s2[pos];
        pos++;
        res_len--;
    }
    /* Adding termination */
    result[s1_len+pos]='\0';
}

/* Compares two strings and returns value that
   indicates if they are identical. 
   Arguments:
    s1  -- First string (null-terminated)
    s2  -- Second string (null-terminated)
   Returns:
    0   -- Strings are not identical.
    1   -- Strings are identical. */
int CompareStrings(char* s1, char* s2) {
    int pos = 0; /* String iterator */

    if (s1 == NULL || s2 == NULL) {
        if (s1 == s2)
            return 1;
        else
            return 0;
    }

    /* Comparing until one of the strings ends or difference is found. */
    while (s1[pos] != '\0' && s2[pos] != '\0') {
        if (s1[pos] != s2[pos])
            return 0;
        pos++;
    }

    /* Checking if both strings ended. */
    if (s1[pos] != s2[pos])
        return 0;

    return 1;
}

/* Allocates copy of string s on heap.
   Arguments:
    s   -- String to copy.
   Returns:
    Copy of string s allocated on heap. */
char* CopyStringToHeap(char* s) {
    int pos = 0; /* Position in string. */
    int len; /* String length (without termination).*/
    char* hs; /* String on heap. */
    len = StringLen(s);

    /* Allocating the string. */
    hs = (char*)malloc(sizeof(char)*(len+1));
    if (hs == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    /* Copying the string. */
    while (s[pos] != '\0') {
        hs[pos] = s[pos];
        pos++;
    }

    /* Adding termination character. */
    hs[pos] = '\0';

    return hs;
}

/* Append file extension to file name.
   User should provide "fullFname" buffer of appropriate size.
   Arguments:
    fname       -- File name without extension.
    ex          -- Extension without a dot. 
    fullFname   -- Resulting file name with extension.
    fullLen     -- Length of resulting file name with extension (not counting termination character).
*/
void AppendExtension(char* fname, char* ex, char* fullFname, int fullLen) {
    int i=0; /* Iterator */
    int len = 0; /* Length of a name with a dot but without extension. */

    /* Copying file name. */
    while (fname[i] != '\0' && i<fullLen) {
        fullFname[i] = fname[i];
        i++;
    }

    /* Adding dot. */
    if (i<fullLen) {
        fullFname[i] = '.';
        i++;
    }

    len = i;

    /* Copying extension. */
    i = 0;
    while (ex[i] != '\0' && (len+i)<fullLen) {
        fullFname[len+i] = ex[i];
        i++;
    }

    /* Adding termination character. */
    if (i<=fullLen) {
        fullFname[len+i] = '\0';
    }
}

/* Removes new line characters in line
   by replacing them with c character.
   Arguments:
    line    -- Null-terminated line.
    c       -- Character to replace new line with.
   Returns:
    Modifies given line. */
void ReplaceNewLine(char* line, char c) {
    int pos = 0;
    while (line[pos] != '\0') {
        if (line[pos] == '\n')
            line[pos] = c;
        pos++;
    }
}

/* Modifies line by removing leading
   blank characters (' ' '\t' '\n'). 
   Arguments:
    line    -- Null-terminated line.
   Returns:
    Modifies given line. */
void RemoveLeadingBlanks(char* line) {
    int pos = 0; /* Iterator. */
    int txt_start = 0; /* Position of first symbol after leading blanks */

    /* Searching for text start position */
    /* (Parsing.h:SkipBlanks is not used to avoid circular dependency.) */
    while (line[txt_start] == ' ' || line[txt_start] == '\t' || line[txt_start] == '\n')
        txt_start++;
    
    /* Copying text over blank characters */
    if (txt_start != 0) {
        while (line[txt_start+pos] != '\0') {
            line[pos] = line[txt_start+pos];
            pos++;
        }
        /* Adding termination character */
        line[pos] = '\0';
    }
}