#include "assembler.h"

PrintSLR(DArrayInt* slr) {
    int i;
    printf("src\t\ttrg\n");
    for (i =0; i<slr->count; i++) {
        printf("%3d\t\t%3d\n", i, slr->data[i]);
    }
}



void PrintArg(InsArg* arg) {
    printf("DEBUG: \tAdressing mode is ");
    if (arg->amode == 0) {
        printf("[immedeate].\n");
        printf("DEBUG: \tImmediate value is %d\n", arg->val);
    }
    if (arg->amode == 1) {
        printf("[direct].\n");
        printf("DEBUG: \tLabel is [%s]\n", arg->label);
    }
    if (arg->amode == 2) {
        printf("[direct indexed].\n");
        printf("DEBUG: \tLabel is [%s]\n", arg->label);
        printf("DEBUG: \tIndex is [r%d]\n", arg->val);
    }
    if (arg->amode == 3) {
        printf("[register].\n");
        printf("DEBUG: \tRegister is [r%d]\n", arg->val);
    }
}


int main(int argc, char** argv) {
    int x = 0;
    /* Array in which every index is a number of a line in expanded source file
       and value for that index is a number of a corresponding line in original source file.
       In short it allows to find after expansion of macros which line in original file 
       was source for line in expanded file. */
    Ins* ins;
    DArrayInt* source_lines_reference; 
    List* errors;
    /* #-6547 
       tag
       tag[r12]
       r13
    */
    char* line = CopyStringToHeap("label: jmp r17\n");
    char* sline = "label: .string \"texttext\"";

    /* Initializing line reference array */
    source_lines_reference = CreateDArrayInt(32);
    DArrayIntAdd(source_lines_reference, -1); /* Line count start with 1, so cell [0] is filled and not used. */

    PrintSLR(source_lines_reference);

    errors = CreateList();

    /*Preprocess("./Input/Example", source_lines_reference, errors);*/
    printf("DEBUG: macros expanded\n");

    ins = ParseInstructionLine(line, errors, source_lines_reference, 0);

    ReplaceNewLine(line, '\0');

    if (ins != NULL) {
        char* name;
        printf("DEBUG: Parsed instruction line [%s]:\n", line);
        printf("DEBUG: Instruction is %s\n", GetInsNameByCode(ins->ins));
        if (ins->source != NULL) {
            printf("DEBUG: Source argument:\n");
            PrintArg(ins->source);
        }
        if (ins->dest != NULL) {
            printf("DEBUG: Destination argument:\n");
            PrintArg(ins->dest);
        }
    }
    else
        printf("Failed to parse instruction line [%s]\n", line);


    


    PrintErrorsList(errors);

    return 0;
}