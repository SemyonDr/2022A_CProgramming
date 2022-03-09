/* This program takes from user input of undefined length and
tries to parse it as exactly N*N integer numbers separated by blank characters.
If parsing is succesfull parsed values are saved in MSQUARE two dimensional integer array
and form matrix with dimensions of N*N.
Then this matrix is printed on the screen.
Then checking is performed to determine if given matrix forms magic square.
Result of this check is printed on the screen.

Central functions are:
GetInput - recieves input from stdin
ParseInput - tries to parse recieved input and fill MSQUARE array.
PrintSquare - prints recieved matrix of numbers
CheckMagicSquare - Checks if matrix is a magic square

INPUT structure is used to recieve user input.
CreateInputBuffer is used for initialization of INPUT
ExpandInputBuffer is used to reallocate buffer memory of INPUT and increase its size
ReleaseInputBuffer frees memory allocated for INPUT
*/

#include <stdio.h>
#include <stdlib.h>
/* This is the dimensions of the magic square */
#define N 5


/* Defining type alias for two dimensional array
   that represents square of numbers. */
typedef int MSQUARE[N][N];


/* This structure holds raw input from the user */
typedef struct {
    char* buffer; /* Array that holds input */
    int buf_len; /* Length of buffer array */
    int buf_len_step; /* Length of each step of buffer length increase */
    int input_len; /* Length of the input stored in the buffer */
} INPUT;


/* Returns on screen length in characters of given integer num.
   Calculates length by counting how many times num is divided by ascending
   powers of 10 before 0 is returned.
   Used in PrintSquare.
   Arguments:
        num -- Number length of which is calculated.
   Returns:
        Length of num in screen characters. */
int GetNumberLength(int num) {
    int numlen = 0; /* Length. */
    int digit = 1;  /* Currently checked digit. */
    /* Special case if number is 0*/
    if (num == 0)
        return 1;
    /* Dividing number by digit until it is less than digit. */
    while (num/digit != 0) {
        numlen++;
        digit = digit*10;
    }
    return numlen;
}


/* Prints given matrix MSQUARE on the screen.
   Every column have width of longest number in that column and
   numbers are aligned to the right.
   Function finds maximum number length for every column and
   prints spaces before numbers to achieve aligment.
   Arguments:
        sq  -- Square to be printed.
   */
void PrintSquare(MSQUARE sq) {
    
    int row; /* Row iterator*/
    int col; /* Column iterator */
    int maxlen[N]; /* Array that holds maximum length of a character for each column. */
    /* Finding maximum length (in characters) of a number in each column*/
    /* Initializing maxlen array with zeroes. */
    for (col =0; col<N; col++)
        maxlen[col] = 0;
    /* Going trough each number in each column and finding maximum length. */
    for (col = 0; col<N; col++) {
        for (row = 0; row<N; row++) {
            int numlen;
            numlen = GetNumberLength(sq[row][col]);
            /* If number is bigger than maximum it is now maximum. */
            if (maxlen[col] < numlen)
                maxlen[col] = numlen;
        }
    }

    /* Printing the numbers. */
    for (row =0; row<N; row++) {
        for (col=0; col<N; col++) {
            /*Adding spaces before number so total length will be maxlen[col]. */
            int numlen;
            numlen = GetNumberLength(sq[row][col]);
            while (numlen<maxlen[col]) {
                numlen++;
                printf(" ");
            }
            /* Printing number*/
            printf("%d", sq[row][col]);

            /* Printing space after number or moving to a new line*/
            if (col<N-1) 
                printf(" ");
            else
                printf("\n");
        }
    }
}/* PrintSquare*/


/*  Creates and initializes new input structure.
    Sets length and allocates memory for the buffer.
    Initial size of the buffer assumes that user enters N^2 numbers
    no bigger than 3 digits each, separated by one character.
    On each reallocation this initial size added one more time to the total length.
    Return:   
        New input structure. */
INPUT CreateInputBuffer() {
    INPUT in;
    in.input_len = 0;
    in.buf_len_step =  3*N*N + N*N - 1;
    in.buf_len = in.buf_len_step;
    in.buffer = (char*)malloc(in.buf_len_step*sizeof(char));
    /* Checking if memory allocated succesfully */
    if (in.buffer == NULL) {
        printf("\nFailed to allocate memory for input buffer.\n");
        exit(-1);
    }
    return in;
}


/* Increases size of the input buffer by one size step
   by calling realloc function.
   Step length is defined in INPUT.buf_len_step.
   Arguments:
        in  -- Pointer to INPUT structure
   */
void ExpandInputBuffer(INPUT* in) {
    char* newBuffer; /* pointer to a new buffer. */
    int new_length; /* New input buffer size. */
    new_length = in->buf_len + in->buf_len_step;
    newBuffer = (char*)realloc(in->buffer, sizeof(char)*new_length);
    /* Checking if memory reallocated succesfully */
    if (newBuffer == NULL) {
        printf("\nFailed to allocate memory for input buffer.\n");
        exit(-1);
    }
    /* Setting new pointer */
    in->buffer = newBuffer;
    /* Setting new length */
    in->buf_len = new_length;
}

/* Frees memory allocated for input buffer.
   Arguments:
        in  -- Input structure. */
void ReleaseInputBuffer(INPUT in) {
    free(in.buffer);
}


/* This function takes input from stdin and writes it in input buffer.
   Input will be taken until EOF state is reached on the stream.
   Creates new INPUT structure using creation function.
   Calls buffer expansion function if buffer limit is reached.
   Returns:   
        INPUT structure. */
INPUT GetInput() {
    INPUT in;  /* Input structure. */
    char next; /* Next character read from stdin */
    in = CreateInputBuffer();
    /* Reading from stdin until EOF is reached */
    while ((next = getc(stdin)) != EOF) {
        /* Checking if input buffer isn't full */
        if (in.input_len == in.buf_len) 
            ExpandInputBuffer(&in);
        
        /* Writing read character into input buffer */
        in.buffer[in.input_len] = next;
        in.input_len++;
    }
    return in;
}


/* Determines and returns type of a ASCII character.
   This function is a part of input parsing.
    Arguments:
        c   -- Character
    Returns:
        0   -- c is blank character (' ', '\t', '\n')
        1   -- c is digit character (0-9).
        2   -- c is of other type of character. */
int GetCharType(char c) {
    if (c == ' ' || c == '\t' || c == '\n')
        return 0;
    if (c>=48 && c<=57) 
        return 1;

    return 2;
}


/* Parses character sequence from INPUT structure buffer to integer number.
   This function is a part of input parsing.
   Arguments:
        in      -- User input structure.
        start   -- Position in input buffer where number starts.
        len     -- Length of a number text in characters.
   Returns:
        Parsed integer number. */
int ParseNumber(INPUT in, int start, int len) {
    int i; /* Iterator*/
    int d; /* Current digit*/
    int num = 0; /* resulting number*/
    /* Going trough number from left to right*/
    for (i = 0; i < len; i++) {
        int j; /* Iterator*/
        /* Converting char digit to int*/
        d = in.buffer[start+i] - 48;
        /* Multiplying digit by 10^(len-1-i)*/
        for (j=0; j<len-1-i; j++) {
            d = d*10;
        }
        /* Adding to result */
        num += d;
    }
    return num;
}


/* Parses input from user and fills magic square.
   Iterater trough INPUT.buffer array and:
    - skips blank characters
    - when encounters digit sequence uses ParseNumber to convert it to integer
      and writes it to the appropriate position in square
    - when encounters not blank, or digit character returns error
   Counts how many numbers are read from input and returns error if it is incorrect.
   Arguments:
        in  -- Input from user.
        sq  -- Square to fill.
    Returns:
        0   -- Parsed succesfully.
        1   -- Not a number encountered in input.
        2   -- Not enough numbers in input.
        3   -- Too many numbers in input. */
int ParseInput(INPUT in, MSQUARE* sq) {
    int i; /* Iterator */
    int count = 0; /* Number of numbers parsed. */

    /* Iterating trough input array */
    for (i = 0; i <in.input_len; i++) {
        int type;
        /* Determening the type of i character. */
        type = GetCharType(in.buffer[i]);

        /* If character is not digit or blank return error.*/
        if (type == 2)
            return 1;

        /* If a new digit sequence (number) encountered. */
        if (type == 1) {    
            int num = 0; /* Number value.*/
            int num_start = 0;  /* Position in buffer where number starts. */
            int num_len = 0; /* Number length in chars. */
            /* If square is already full returning error.*/
            if (count+1 >N*N)
                return 3;

            /* Searching end of the number.
               Number ends before input end or if non-digit character is next.
               After search input iterator will be set to last digit of a number.*/
            num_start = i;
            num_len = 1;
            while (i < in.input_len-1 && GetCharType(in.buffer[i+1]) == 1) {
                    i++;
                    num_len++;
            }
            /* Turning number characters into int. */
            num = ParseNumber(in, num_start, num_len);
            
            /* Writing number to square array*/
            count++;
            (*sq)[(count-1)/N][(count-1)%N] = num;
        }
    } /* End of iterations. */

    /* Checking if square is filled*/
    if (count < N*N) 
        return 2;

    return 0;
}/* ParseInput() */


/* Checks if given matrix of numbers is a magic square.
   Arguments:
        sq  -- Square to check.
   Return:
        -1  -- Square is not a magic square.
               Magic number if square is magic.*/
int CheckMagicSquare(MSQUARE sq) {
    int mnum = 0; /* Magic number.*/
    int row; /* Row iterator. */
    int col; /* Column iterator*/

    /*Calculating reference magic number
      by adding up first row numbers.*/
    for (col = 0; col<N; col++)
        mnum += sq[0][col];

    /* Checking if each row sums up to magic number.*/
    for (row=1; row<N; row++) {
        int sum =0;
        for (col=0; col<N; col++) 
            sum += sq[row][col];
        if (sum != mnum)
            return -1;
    }

    /* Checking if each column sums up to magic number*/
    for (col=0; col<N; col++) {
        int sum = 0;
        for (row=0; row<N;row++)
            sum += sq[row][col];
        if (sum != mnum)
            return -1;
    }

    /* Checking if \ diagonal sums up to magic number. */
    {
        int sum = 0;
        for (row = 0; row<N; row++)
            sum += sq[row][row];
        if (sum != mnum)
            return -1;
    }

    /* Checking if / diagonal sums up to magic number. */
    {
        int sum = 0;
        for (row = 0; row<N; row++)
            sum += sq[row][N-row-1];
        if (sum != mnum)
            return -1;
    }

    return mnum;
}


/* Main function prints input invintation, gets input, tries to parse it,
   and depending on the result prints parsing error, or parsed square.
   Then it checks if square is magic and prints result.*/
int main() {
    INPUT in; /* INPUT structure for storing user input.*/
    MSQUARE sq; /* Square that is filled when parsing. */
    int magic_number; /* Magic number of a square. */
    int parsing_result; /* Variable to hold result of parsing. */

    /* Printing input invintation for user.*/
    printf("This program will decide if given sequence of numbers forms a magic square %dx%d.\n", N, N);
    printf("Please enter %d numbers, separated by blank characters (space, tab, new line).\n", N*N);
    printf("Order of entering numbers for square is left to right, top to bottom.\n");
    printf("To finish entering press Ctrl+D:\n");
    /* Getting input. */
    in = GetInput();
    
    /* Parsing input. */
    parsing_result = ParseInput(in, &sq);
    /* Freeng memory used by input buffer. */
    ReleaseInputBuffer(in);
    /* Acting on parsing result.*/
    switch (parsing_result) {
    case 0:
        /* If parsing is successful*/
        printf("\nMagic square you have entered is:\n");
        /* Printing square. */
        PrintSquare(sq);
        break;
    case 1:
        printf("\nInvalid input: Not a number.\n");
        exit(1);
        break;
    case 2:
        printf("\nInvalid input: Not enough numbers.\n");
        exit(1);
        break;
    case 3:
        printf("\nInvalid input: Too many numbers.\n");
        exit(1);
        break;
    default:
        break;
    }

    /* Checking if square is magic square. */
    magic_number = CheckMagicSquare(sq);
    /* Printing check result. */
    if (magic_number == -1)
        printf("This square is not magic square.\n");
    else
        printf("This square is magic square. Magic number is %d.\n", magic_number);

    return 0;
}   