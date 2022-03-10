#include "Binary.h"

int GetInstructionCode(char* ins) {
    if (CompareStrings(ins, "mov"))
        return ins_mov;
    if (CompareStrings(ins, "cmp"))
        return ins_cmp;
    if (CompareStrings(ins, "add"))
        return ins_add;
    if (CompareStrings(ins, "sub"))
        return ins_sub;

    if (CompareStrings(ins, "lea"))
        return ins_lea;
    if (CompareStrings(ins, "clr"))
        return ins_clr;
    if (CompareStrings(ins, "not"))
        return ins_not;
    if (CompareStrings(ins, "inc"))
        return ins_inc;

    if (CompareStrings(ins, "dec"))
        return ins_dec;
    if (CompareStrings(ins, "jmp"))
        return ins_jmp;
    if (CompareStrings(ins, "bne"))
        return ins_bne;
    if (CompareStrings(ins, "jsr"))
        return ins_jsr;

    if (CompareStrings(ins, "red"))
        return ins_red;
    if (CompareStrings(ins, "prn"))
        return ins_prn;
    if (CompareStrings(ins, "rts"))
        return ins_rts;
    if (CompareStrings(ins, "stop"))
        return ins_stop;

    return -1;
}



Ins* ParseInstructionLine(char* line, List* errors, DArrayInt* slr, int lineNum) {
    int pos = 0; /* Position in line. */
    char word[MAX_STATEMENT_LEN+2]; /* Buffer for holding word from line. */
    char* res;  /* Result of getting the word. */
    Ins* ins;   /* Parsed instruction structure. */
    List* args; /* Instruction arguments as strings. */
    InsInfo insinfo;   /* Info about instruction. */

    {/* DEBUG PRINT ****************************************************** */
        char* line_copy = CopyStringToHeap(line);
        RemoveLeadingBlanks(line_copy);
        ReplaceNewLine(line_copy, ' ');
        printf("DEBUG: \tBreaking down instruction line [%s]\n", line_copy);
        free(line_copy);
    }/* DEBUG PRINT ****************************************************** */

    /* Allocating instruction structure. */
    ins = (Ins*)malloc(sizeof(Ins));
    if (ins == NULL) {
        perror("Failed to allocate memory.");
        exit(1);
    }

    /* Skipping label if present. */
    TryGetLabel(line, &pos, word, MAX_STATEMENT_LEN+1);

    /* Getting first word. */
    res = GetNextWord(line, &pos, word, MAX_STATEMENT_LEN+1, ",");

    /* Getting instruction code. Instruction name assumed to be valid name */
    ins->ins = GetInstructionCode(word);

    {/* DEBUG PRINT ****************************************************** */
        printf("DEBUG: \t\tInstruction name: %s\n", word);
        printf("DEBUG: \t\tInstruction number: %d\n", ins->ins);
    }/* DEBUG PRINT ****************************************************** */

    /* Getting instruction arguments. */
    args = GetArgs(line, &pos, errors, lineNum, slr);

    {/* DEBUG PRINT ****************************************************** */
        int i = 1;
        ListNode* cur = args->head;
        while (cur != NULL) {
            printf("DEBUG: \t\t\t%d: %s\n", i, cur->data);
            cur = cur->next;
            i++;
        }
    }/* DEBUG PRINT ****************************************************** */

    printf("DEBUG: \t Instruction line broken down.\n");

    return NULL;

}

/* Assumes that pre-processed file exists.
   Produces 4 tables:
    instructions (structures)
    data (structures)
    external symbols
    entry symbols
   Also catches parsing errors.
  */
void ParseExpanded(char* source_file_name, DArrayInt* slr, ParsedCode* pcode, List* symbolsTable, List* errors) {
    FILE* file; /* Expanded source file handler. */
    char* full_fname; /* Buffer for holding full file name with extension. */
    int full_fname_len; /* Length of filename with extension. */
    char line[MAX_STATEMENT_LEN+2]; /* Buffer for holding line from file. */
    int line_num = 0;   /* Number of line in expanded source file. */
    int icount=100;     /* Instruction counter. */
    int dcount=0;     /* Data counter. */

    /* Opening expanded (preprocessed) file */
    /* Allocating buffer for full file name */
    full_fname_len = StringLen(source_file_name) + 3;
    full_fname = (char*)malloc(sizeof(char)*(full_fname_len+1));
    ConcatenateStrings(source_file_name, ".am", full_fname, full_fname_len);
    file = fopen(full_fname, "r");
    if (file == NULL) {
        perror("Failed to open file.");
        exit(2);
    }

    /* Reading lines from file in a loop. */
    while (fgets(line, MAX_STATEMENT_LEN+1, file) != NULL) {
        int pos = 0; /* Position in line. */
        char label[MAX_LABEL_LEN+1]; /* Buffer for holding statement label. */
        char word[MAX_STATEMENT_LEN+2]; /* Buffer for holding word from line. */
        int wordtype; /* Variable that stores IsReservedWord result. Allows to determine if line is a instruction, or directive, or neither. */
        char* label_res; /* Variable for storing label reading result. */
        char* res;  /* Variable for storing word reading result. */
        

        line_num++; /* Advancing line counter. Lines are numbered starting from 1. */
        
        /* Trying to get label before statement. */
        label_res = TryGetLabel(line, &pos, label, MAX_LABEL_LEN);

        /* Getting next word */
        res = GetNextWord(line, &pos, word, MAX_STATEMENT_LEN+1, NULL);

        /* If there is nothing besides the label adding error and skipping line. */
        /* If there was a label before empty line it will not be added to symbols table. */
        if (res == NULL) {
            if (label != NULL)
                AddError(errors, slr->data[line_num], ErrStm_Empty, line);
            else
                AddError(errors, slr->data[line_num], ErrStm_Empty, NULL);
            continue; /* Skipping the line. */
        }

        /* Checking if first (besides label) word of the line is an reserved word. */
        wordtype = IsReservedWord(word);

        /* If it's not a reserved word. */
        if (wordtype == 0) {
            AddError(errors, slr->data[line_num], ErrStm_NotRecognized, word);
            continue; /* Skipping the line */
        }

        /* Checking if line is an instruction. */
        if (wordtype == 1) {
            /* If label exists adding it to symbols table. */



            /* Parsing instruction */

        }



    }
    

    /* Closing the file. */
    fclose(file);
}

void ProduceBinary(char* source_file_name) {
    /* Parse expanded */    

    /* Make binary code (array of int) */

    /* write binary in requested format */

    /* write tables */
}