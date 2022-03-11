#include "Binary.h"

/* DEBUG 
   FOR PRINTING INSTRUCTION NAME BY CODE
   Returns instruction name by given code. */
char* GetInsNameByCode(int insCode) {
    if (insCode <0 || insCode > 15)
        return CopyStringToHeap("NaN");

    char* insnames[16];
    insnames[0] = "mov";
    insnames[1] = "cmp";
    insnames[2] = "add";
    insnames[3] = "sub";

    insnames[4] = "lea";
    insnames[5] = "clr";
    insnames[6] = "not";
    insnames[7] = "inc";

    insnames[8] = "dec";
    insnames[9] = "jmp";
    insnames[10] = "bne";
    insnames[11] = "jsr";

    insnames[12] = "red";
    insnames[13] = "prn";
    insnames[14] = "rts";
    insnames[15] = "stop";
    
    return CopyStringToHeap(insnames[insCode]);
}

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

/* Return info about given instruction. */
InsInfo* GetInstructionInfo(int code) {
    InsInfo* info;

    info = (InsInfo*)malloc(sizeof(InsInfo));
    if(info == NULL) {
        perror("Failed to allocate memory.");
        exit(1);
    }

    switch (code)
    {
    case ins_mov:
        info->ins = ins_mov;
        info->opcode = 0;
        info->funct = -1;
        info->amodes_source = 1+2+4+8; /* 0,1,2,3 */
        info->amodes_dest = 2+4+8; /* 1,2,3 */
        break;
    case ins_cmp:
        info->ins = ins_cmp;
        info->opcode = 1;
        info->funct = -1;
        info->amodes_source = 1+2+4+8; /* 0,1,2,3 */
        info->amodes_dest = 1+2+4+8; /* 0,1,2,3 */
        break;
    case ins_add:
        info->ins = ins_add;
        info->opcode = 2;
        info->funct = 10;
        info->amodes_source = 1+2+4+8; /* 0,1,2,3 */
        info->amodes_dest = 2+4+8; /* 1,2,3 */
        break;
    case ins_sub:
        info->ins = ins_sub;
        info->opcode = 2;
        info->funct = 11;
        info->amodes_source = 1+2+4+8; /* 0,1,2,3 */
        info->amodes_dest = 2+4+8; /* 1,2,3 */
        break;

    case ins_lea:
        info->ins = ins_lea;
        info->opcode = 4;
        info->funct = -1;
        info->amodes_source = 2+4; /* 1,2 */
        info->amodes_dest = 2+4+8; /* 1,2,3 */
        break;
    case ins_clr:
        info->ins = ins_clr;
        info->opcode = 5;
        info->funct = 10;
        info->amodes_source = 0; /* No source operand. */
        info->amodes_dest = 2+4+8; /* 1,2,3 */
        break;  
    case ins_not:
        info->ins = ins_not;
        info->opcode = 5;
        info->funct = 11;
        info->amodes_source = 0; /* No source operand. */
        info->amodes_dest = 2+4+8; /* 1,2,3 */
        break;  
    case ins_inc:
        info->ins = ins_inc;
        info->opcode = 5;
        info->funct = 12;
        info->amodes_source = 0; /* No source operand. */
        info->amodes_dest = 2+4+8; /* 1,2,3 */
        break;  

    case ins_dec:
        info->ins = ins_dec;
        info->opcode = 5;
        info->funct = 13;
        info->amodes_source = 0; /* No source operand. */
        info->amodes_dest = 2+4+8; /* 1,2,3 */
        break;  
    case ins_jmp:
        info->ins = ins_jmp;
        info->opcode = 9;
        info->funct = 10;
        info->amodes_source = 0; /* No source operand. */
        info->amodes_dest = 2+4; /* 1,2 */
        break;  
    case ins_bne:
        info->ins = ins_bne;
        info->opcode = 9;
        info->funct = 11;
        info->amodes_source = 0; /* No source operand. */
        info->amodes_dest = 2+4; /* 1,2 */
        break;  
    case ins_jsr:
        info->ins = ins_jsr;
        info->opcode = 9;
        info->funct = 12;
        info->amodes_source = 0; /* No source operand. */
        info->amodes_dest = 2+4; /* 1,2 */
        break;  

    case ins_red:
        info->ins = ins_red;
        info->opcode = 12;
        info->funct = -1;
        info->amodes_source = 0; /* No source operand. */
        info->amodes_dest = 2+4+8; /* 1,2,3 */
        break;      
    case ins_prn:
        info->ins = ins_prn;
        info->opcode = 13;
        info->funct = -1;
        info->amodes_source = 0; /* No source operand. */
        info->amodes_dest = 1+2+4+8; /* 0,1,2,3 */
        break;      
    case ins_rts:
        info->ins = ins_rts;
        info->opcode = 14;
        info->funct = -1;
        info->amodes_source = 0; /* No source operand. */
        info->amodes_dest = 0; /* No destination operand. */
        break;      
    case ins_stop:
        info->ins = ins_stop;
        info->opcode = 15;
        info->funct = -1;
        info->amodes_source = 0; /* No source operand. */
        info->amodes_dest = 0; /* No destination operand. */
        break;      
    
    default:
        free(info);
        return NULL;
    }

    return info;
}

int HasMode(int amodes, int adressingMode) {
    /* This function explained by example. */
    /* For example adressing mode is 2 and amodes are 0110=6*/
    int i; /* Iterator. */
    int mode_bin = 1; /* Binary representation of a mode. Starts as 0001 */
    mode_bin = mode_bin << adressingMode; /* Making binary shift. Now mode_bin is 0100. */
    /* Checking if mode is in amodes. */
    return amodes & mode_bin; /* 0110 & 0100 = 0100 > 1 = true*/
}


/* DONT FORGET TO ADD REMOVING (FREE) OF ARGS ON POINTS OF FAILURE AND AT THE END!!!1111
   ALSO SHOULD REMOVE INSTRUCTION INFO */
Ins* ParseInstructionLine(char* line, List* errors, DArrayInt* slr, int lineNum) {
    int pos = 0; /* Position in line. */
    char word[MAX_STATEMENT_LEN+2]; /* Buffer for holding word from line. */
    char* res;  /* Result of getting the word. */
    Ins* ins;   /* Parsed instruction structure. */
    List* args; /* Instruction arguments as strings. */
    InsInfo* insinfo;   /* Info about instruction. */
    int num_args; /* Number of arguments for instruction. */

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

    ins->source = NULL;
    ins->dest = NULL;

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

    /* Getting instruction info. */
    insinfo = GetInstructionInfo(ins->ins);

    printf("DEBUG: \t Instruction info is:\n");
    printf("DEBUG: \t\t Name is %s\n", GetInsNameByCode(insinfo->ins));
    printf("DEBUG: \t\t opcode is %d\n", insinfo->opcode);
    printf("DEBUG: \t\t funct is %d\n", insinfo->funct);
    printf("DEBUG: \t\t source amodes are %d\n", insinfo->amodes_source);
    printf("DEBUG: \t\t dest amodes are %d\n", insinfo->amodes_dest);

    /* Checking number of arguments that should be in instruction. */
    if (insinfo->amodes_dest == 0)
        num_args = 0;   /* If there is no destination argument there is no source argument. */
    else {
        if (insinfo->amodes_source == 0)
            num_args = 1; /* Only destination argument. */
        else
            num_args = 2; /* Destination and source arguments. */
    }

    /* Checking number of arguments that were extracted from the line. */
    /* Missing arguments. */
    if (args->count < num_args) {
        AddError(errors, slr->data[lineNum], ErrIns_MissingArg, line);
            free(ins);
            return NULL;
    }
    
    /* Too many arguments. */
    if (args->count > num_args) 
        /* Too many arguments. Appropriate number of arguments will
           be parsed, but error will be saved. */
        AddError(errors, slr->data[lineNum], ErrIns_ExtraArg, line);


    /* If instruction has 2 arguments: */
    if (num_args == 2) {
        /* Parsing arguments. */
        ins->source = ParseInsArg(args->head->data, errors, lineNum, slr);
        ins->dest = ParseInsArg(args->head->next->data, errors, lineNum, slr);
        /* If parsing arguments failed. */
        if (ins->source == NULL || ins->dest == NULL) {
            free(ins);
            return NULL;
        }
        /* Checking if entered modes are available for instuction arguments. */
        if (!HasMode(insinfo->amodes_source, ins->source->amode)) {
            AddError(errors, slr->data[lineNum], ErrIns_InvalidSrcAmode, line);
            free(ins);
            return NULL;
        }
        if (!HasMode(insinfo->amodes_dest, ins->dest->amode)) {
            AddError(errors, slr->data[lineNum], ErrIns_InvalidDestAmode, line);
            free(ins);
            return NULL;
        }
    }
    
    /* If instruction has 1 argument it is always a destination argument. */
    if (num_args == 1) {
        /* Parsing arguments. */
        ins->dest = ParseInsArg(args->head->data, errors, lineNum, slr);
        /* If parsing arguments failed. */
        if (ins->dest == NULL) {
            free(ins);
            return NULL;
        }
        /* Checking if entered modes are available for destination argument. */
        if (!HasMode(insinfo->amodes_dest, ins->dest->amode)) {
            AddError(errors, slr->data[lineNum], ErrIns_InvalidDestAmode, line);
            free(ins);
            return NULL;
        }
    }

    return ins;

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