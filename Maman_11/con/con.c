#include <stdio.h>
/*Maximum length of input string*/
#define MAX_LEN 80

/*Returns the type of the character "c":
    0 - uppercase letters, 
    1 - lowercase letters, 
    2 - numbers, 
    3 - symbols */
int GetCharType(char c) {
    /*Uppercase*/
    if ((c >= 65) && (c <=90))
        return 0;
    /*Lowercase*/
    if (c >= 97 && c <= 122)
        return 1;
    /*Numbers*/
    if (c >= 48 && c <=57)
        return 2;
    
    /*If it's something else, it's a symbol*/
    return 3;
}

/*Contracts sequence s1 and writes contracted version to s2.
Interates through input array, marking beggining and length of
every ascending sequence of characters. When sequence ends contracted version is written
to the output array*/
void contract(char s1[], char s2[]) {
    int out_pos = 0; /*Index to write the contracted sequence to in the output array*/
    int seq_len = 0; /*Length of the current sequence*/
    int seq_start = 0; /*Index of the beginning of the current sequence in the input array*/
    int stop; /*Indicates if sequence ended (see cycle)*/

    /*Iterate through input string*/
    int i;
    for (i = 0; s1[i] != '\0'; i++) {
        seq_len++;
        /*Checking if the next symbol don't belong to the sequence*/
        stop = 0;  /*Assuming it does belong.*/
        /*1) If it's of different type.*/
        if (GetCharType(s1[i+1]) != GetCharType(s1[i]))
            stop = 1;
        /*2) If it's not a next symbol of that type.*/
        if (!stop && s1[i+1] != s1[i] + 1)
            stop = 1; 
        /*3) If current character is a symbol. Symbols do not form sequences and every symbol is it's own sequence.*/
        if (!stop && GetCharType(s1[i]) == 3)
            stop = 1;
        /*4) If array has ended.*/
        if (!stop && s1[i+1] == '\0')
            stop = 1;
            

        /*If sequence is ended writing contracted version to output array in appropriate form (1, 2, or 3 symbols)*/
        if (stop) {
            if (seq_len >= 3) {
                s2[out_pos] = s1[seq_start];
                s2[out_pos+1] = '-';
                s2[out_pos+2] = s1[seq_start+seq_len - 1];
                out_pos += 3;
                seq_len = 0;
                seq_start = i+1;
                }
            if (seq_len == 2) {
                s2[out_pos] = s1[seq_start];
                s2[out_pos+1] = s1[seq_start + 1];
                out_pos += 2;
                seq_len = 0;
                seq_start = i+1;
            }
            if (seq_len == 1) {
                s2[out_pos] = s1[seq_start];
                out_pos++;
                seq_len = 0;
                seq_start = i+1;
            }
        }
    }

    /*Adding end of the string symbol to the output*/
    s2[out_pos] = '\0';
}

/*This program contracts given sequence following these rules:
  Every sub-sequence of ascending numbers, lowercase letters and uppercase letters
  will be shortened to First-Last when length of the sub-sequence is 3 and more,
  to FirstLast when the length is 2, or left as it is if it's a single symbol.
  Every other symbol will be left as it is.
  Main function first takes input from the user, contracts it, and then
  prints result.*/
int main() {
    /*Taking input from the user*/
    /*Array to hold the input. 80 symbols maximum as given in the problem + 1 extra symbol for end of the string.*/
    char s1[MAX_LEN+1];
    /*Array to hold the output*/
    char s2[MAX_LEN+1];
    /*Input invintation for the user*/
    printf("Please enter the string to contract:\n");
    /*Saving user input in s1*/
    fgets(s1, MAX_LEN+1, stdin);
    printf("Input : %s\n", s1);
    /*Contracting the string*/
    contract(s1, s2);
    /*Printing the result*/
    printf("Output: %s\n", s2);

    return 0;
}

