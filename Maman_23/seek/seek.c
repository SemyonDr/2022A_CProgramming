/* Program description:
    Program prints symbol number n and its ASCII code from every provided file.
    Number and list of files are given as arguments to program execution.
   Program operations:
    Program tries to open each file from provided list of file names and 
    tries to read n'th symbol from it. Then it prints the result.
   Modules:
    Whole program is contained in seek.c (and corresponding header) 
    and there is no modules to it.
   Algorithm:
    Checks number of arguments passed to program.
    Checks if first (number) argument is valid.
    If arguments are not valid execution stopped.
    Tries to open each file from arguments in a loop.
    If file is opened tries to take character with a provided number from file.
    If character exists prints it and its ASCII code.
    On each step if operation failed prints appropriate error message.
   Input:
    Only input is command line parameters for program execution.
    Constist of positive integer number, followed by list of files.
   Output:
    Line containing file name, character and its ASCII code.
    Or error messages.*/

#include "seek.h"

/* Takes string that represents non-negative integer number and returns the number.
   Arguments:
    s   - String representation of non-negative integer number (null-terminated, without sign).
   Returns:
    Integer number if parsing is successfull.
    -1 if parsing failed.
   Algorithm:
    1) Checks if provided string contains only digits
    2) Checks if string if not of 0 length
    3) Creates number by parsing every digit character and 
       adding it to appropriate place in a number.
   */
int ParseNumberArgument(char* s) {
    int result = 0; /* Parsed value. */
    int len = 0; /* Length of the string without termination symbol. */
    int i; /* Iterator. */

    /* Checking if string represents integer number and simultaniously getting its length. */
    while (s[len] != '\0') {
        if (s[len] <= 47 || s[len] >= 58)
            return -1;
        len++;
    }

    /* Checking if string contains anything. */
    if (len == 0)
        return -1;

    /* Calculating number by parsing digits and adding them to result. */
    for (i=0; i< len; i++) {
        int j; /* Iterator */
        int digit_val; /* Value of the digit. */
        int digit_pos = 10; /* Number digit should be multiplied on to place it at correct position in result. (10^i) */

        /* Getting digit value */
        digit_val = (int)s[len-i-1] - 48;

        /* Calculating 10^i */
        if (i==0)
            digit_pos = 1;
        else {
            for (j=0; j<i-1; j++) {
                digit_pos = digit_pos*10;
            }
        }

        /* Adding digit to the result */
        result += digit_val*digit_pos;
    }

    return result;
}


/* Returns ASCII number of a character. */
int CharToASCIICode(char c) {
    return (int)c;
}


/* Returns character number n from file that pointed by file pointer fp.
   Assumes that file pointer is not NULL and does not check it.
   Assumes that n is positive integer and does not check it.
   Arguments:
    fp      - File pointer.
    n       - Number of a character to read. (Number of the first character is 1)
   Returns:
    char    - Character at position n-1. 
              -1 if end of file encountered before n'th symbol
   Algorithm:
    Uses fseek to set file pointer to position n-1 and then reads character with getc.
    Returns -1 if fseek failed.
*/
char GetCharAt(FILE* fp, int n) {
    int seek_res; /* Result of fseek. */

    /* Setting position in the file to n-1 character. */
    seek_res = fseek(fp, n-1, SEEK_SET);

    /* Checking if position set succesfully. */
    if (seek_res != 0)
        /* Assumes that failure to set position is exclusively due to eof encounter. */
        return -1;

    /* Returning character at position n-1. */
    return getc(fp);
}


/* Function that prints formatted message containing file name,
   character number char_num in this file, and its ASCII CODE.
   Assumes that arguments are valid.
   Arguments:
    fileName    - String that contains file name, null terminated.
    char_num    - Number of printed character.
    c           - Character to print
   Algorithm:
    Determines if character can be printed and prints a message.
*/
void PrintASCII(char* fileName, int char_num, char c) {
    /* Printing file name */
    printf("%s: ", fileName); 
    /* Printing original character */
    if (c < 31 || c == 127)
        printf("Character number %d is not printable. ", char_num); 
    else 
        printf("Character number %d is '%c'. ", char_num, c);
    
    /* Printing ASCII code. */
    printf("ASCII code is %d.\n", CharToASCIICode(c));
}

/* Main function.
   Checks arguments validity,
   then goes trough provided list of files in a loop
   and tries to open them and print character with a provided number.
*/
int main(int argc, char** argv) {
    int i; /* Iterator. */
    int n; /* Number of the character, that should be printed. */
    /* Checking number of arguments. */
    if (argc <3) {
        printf("Error: Not enough arguments.\n");
        exit(1);
    }

    /* Parsing number argument. */
    n = ParseNumberArgument(argv[1]);
    if (n == -1 || n == 0) {
        printf("Error: First argument is not a valid number.\n");
        exit(1);
    }

    /* Trying to print character numbers from files. */
    for (i=2; i<argc; i++) {
        FILE* fp;
        /* Opening file. */
        fp = fopen(argv[i], "r");
        if (fp != NULL) {
            /* File opened successfully. */
            char c;
            c = GetCharAt(fp, n);   /* Getting character */
            fclose(fp); /* Closing file. */
            if (c == -1)
                printf("File \"%s\" does not contain %d symbols\n", argv[i], n);
            else {
                PrintASCII(argv[i], n, c); /* Printing ASCII number */
            }
        }
        else
            printf("Failed to open file \"%s\"\n", argv[i]);
    }

    return 0;
}