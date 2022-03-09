#include <stdio.h>
#include "Preprocessor.h"
#include "Errors.h"
#include "Data.h"
#include "Preprocessor.h"


int main(int argc, char** argv) {


    /* Array in which every index is a number of a line in expanded source file
       and value for that index is a number of a corresponding line in original source file.
       In short it allows to find after expansion of macros which line in original file 
       was source for line in expanded file. */
    DArrayInt* source_lines_reference; 
    List* errors;
    
    /* Initializing line reference array */
    source_lines_reference = CreateDArrayInt(32);
    DArrayIntAdd(source_lines_reference, -1); /* Line count start with 1, so cell [0] is filled and not used. */

    errors = CreateList();

    Preprocess("./Input/Example", source_lines_reference, errors);
    printf("DEBUG: macros expanded\n");
    PrintErrorsList(errors);

    

    

    return 0;
}