#include <stdio.h>
/*Maximum length of an input string (symbols+\n). Assumed to be 100.*/
#define MAX_LEN 100

/*Global variable showing that reading position currently inside a comment*/
int inComment;
/*Array used in checking balance of a line. 
Contains opening brackets found while checking line.
Worst case is when every symbol in a line is an opening bracket.
Global because there might be lines broken by a comment.*/
char brackets[MAX_LEN];
/*Shows position of last added bracket in the brackets array.
  Value -1 means that there is no brackets in the array.*/
int brackets_index;

/*Checks if the string is a single { or } bracket with blank symbols.
Returns:
    '0' - not single
    '{' or '}' - if single bracket
*/
char CheckForSingleBracket(char s[]) {
    char bracket = '0';
    
    /*Iterating trough string*/
    int i = 0;
    while (s[i] != '\0') {
        /*Checking if bracket found*/
        if (s[i] == '{' || s[i] == '}') {
            /*If one of the brackets already encountered it's not single*/
            if (bracket != '0')
                return '0';
            else
            /*Remembering which bracket was found*/
                bracket = s[i];
        }
        else {
            /*Checking if character is not blank*/
            if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
            /*string isn't a single bracket*/
               return '0';
        }
        i++;
    }

    return bracket;
}



/*Checks if type of the opening bracket matches type of the closing bracket.
Return:
    0 - brackets don't match;
    1 - brackets match;
*/
int MatchBrackets(char opening, char closing) {
    if (opening == '(' && closing == ')')
        return 1;
    if (opening == '{' && closing == '}')
        return 1;
    if (opening == '[' && closing == ']')
        return 1;
    
    return 0;
}

/*Checks if line is balanced.
  Goes trough line symbol by symbol, skips text and comments
  and uses brackets array and index to check for brackets balance.
Return:
    0 - not balanced
    1 - balanced
    */
int CheckLineBalance(char s[]) {
    /*Cycle iterator.*/
    int i = 0;
    /*If line starts while already in comment,
    then when the comment ends there will be continuation
    of previous line, broken by comment, so brackets array state should be preserved.
    If line doesn't start already in comment brackets array should be reset.*/
    if (!inComment)
        brackets_index = -1;

    /*If line started while in comment searching where comment ends and skipping.*/
    if (inComment) {
        while (s[i+1] != '\0' && inComment) {
            if (s[i] == '*' && s[i+1] == '/') {
                i = i+1;
                inComment = 0;
            }
            i++;
        }
    }

    /*Iterate trought string characters*/
    for (; s[i] != '\0'; i++) {

        /*Checking if comment is starting*/
        if (s[i] == '/' && s[i+1] == '*') {
            inComment = 1;
            /*Searching for the end of the comment*/
            while (s[i+1] != '\0' && inComment) {
                /*If end is found skipping to the symbol after it*/
                if (s[i] == '*' && s[i+1] == '/') {
                    inComment = 0;
                    i = i+1;
                }           
                i++;     
            }
            /*If comment end isn't found and the string ended return that
              string is valid and note that inComment now set to 1.*/
            if (s[i] == '\0')
                return 1;
        }

        /*Checking if text is starting*/
        if (s[i] == '"') {
            /*Searching end of the text and skipping to the symbol after that.
              Assumed that text will always be closed at the same line.*/
            do {
                i++;
            } while (s[i] != '"');
            i++;
        }

        /*By this point we should be at the symbol that's not in text, or in comment*/

        /*Checking if the symbol is opening bracket*/
        if (s[i] == '(' || s[i] == '{' || s[i] == '[') {
            /*Adding it to the array*/
            brackets_index++;
            brackets[brackets_index] = s[i];
        }

        /*Checking if symbol is closing bracket*/
        if (s[i] == ')' || s[i] == '}' || s[i] == ']') {
            /*Checking if there were opening brackets*/
            if (brackets_index == -1)
                return 0;
            else {
                /*Checking if last opened bracket matches closing bracket*/
                if (!MatchBrackets(brackets[brackets_index], s[i]))
                    return 0;
                else {
                /*If last opened bracket matches closing bracket it is "removed" from the array*/
                    brackets_index--;
                }
            }
        }        
    }/*End of line iterations*/

    /*If all opened brackets are closed array should be "empty"*/
    if (brackets_index == -1)
        return 1;
    else
        return 0;
}/*End of CheckLineBalance*/



/*Checks bracket balance of the code from "input" stream (keyboard, or file)
  and prints after each line of code (except single { or }) if the brackets are balanced.
  Prints if code as a whole is balanced.
  Line broken by multi-line comments to multiple lines considered to be one line.
  Gets lines from input, checks them for balance, using CheckLineBalance(char*) and prints results
  for each line and for the whole input.
  Return:
    0 - Input is unbalanced
    1 - Input is balanced
  */
int CheckBalance(FILE* input) {
    /*Shows if whole input is balanced.*/
    int balanced = 1;
    /*Single brackets counter*/
    int sBrackets = 0;
    /*String to hold input line. Assumed length no more than MAX_LEN symbols + end of line symbol*/
    char str[MAX_LEN + 1];
    /*If input is from file this is line number for each line.*/
    int str_num = 1;

    /*Print invintation for input.*/
    printf("Enter code line by line.To stop entering code press ctrl+d\n");
    
    /*Getting input line by line*/
    while (fgets(str, MAX_LEN+1, input)) {
        /*Variable to hold result of single bracket check*/
        char sbr;

        /*Print string and string number*/
        /*Checking if the string contains new line character and adding it if it doesn't*/
        int newLine = 0;
        int i = 0;
        while (str[i] != '\0') {
            if (str[i] == '\n')
                newLine = 1;
            i++; 
        }
        if (!newLine) {
            str[i] = '\n';
            str[i+1] = '\0';
        }
        printf("%3d: ", str_num);
        printf("%s", str);
        str_num++;


        /*Checking if line is a single bracket*/
        sbr = CheckForSingleBracket(str);
        if (sbr != '0' && !inComment) {
            if (sbr == '{')
                sBrackets++;
            if (sbr == '}')
                sBrackets--;
        }
        else {
            /*Checking if line is balanced*/
            int bal;
            bal = CheckLineBalance(str);
            if (!inComment) {
                if (bal)
                    printf("#Line is balanced.#\n");
                else {
                    printf("#Line is unbalanced.#\n");
                    balanced = 0;
                }
            }
        }

    }/*Input loop*/

    printf("#End of input#\n");

    if (sBrackets != 0)
        balanced = 0;
    
    return balanced;
}


/*This program will take c-like code from the user and will check line by line
  balance of the brackets in the code. It will print after each code line if it is balanced, except
  for comments line, single '{', or '}' line, and when line broken by comment in two lines.
  After input is ended total evaluation of brackets balance for the input will be printed,
  including for single brackets ({ and }).
  Line broken by multiple comments to multiple lines will be evaluated after line ends.
  */
int main() {
    /*Initializing global variables.*/
    inComment = 0;
    brackets_index = -1;

    /*Printing explanation of the program*/
    printf("This program analizes program code and checks if brackets in each line\nare balanced. Prints summary result for whole program.\n");

    if (CheckBalance(stdin))
        printf("#Code is balanced.#\n");
    else
        printf("#Code is unbalanced.#\n");

    return 0;
}