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

/* Gets instruction type according by instruction name.
   Argument:
    ins     -- String containing istruction name.
   Returns:
    Instruction number according to InstructionsEnum.
    -1 if instruction name not recognized. */
int GetInstructionType(char* ins) {
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

/* Takes line with pos after ".string"  directive keyword.*/
char* ParseStringDirectiveArgument(char* line, int* pos, List* errors, int lineNum, DArrayInt* slr ) {
    int start; /* Position of the first character inside "". */
    int len; /* Length of string inside "". */
    int spos; /* Position inside of string between "". */
    char* str; /* Parsed string. */

    /* Skipping blanks. */
    SkipBlank(line, pos);

    /* Checking if line ended without an argument. */
    if (line[*pos] == '\0') {
        AddError(errors, slr->data[lineNum], ErrDt_StrNoArgument, line);
    }

    /* Checking if first character is ". */
    if (line[*pos] != '"') {
        AddError(errors, slr->data[lineNum], ErrDt_StrInvalidArg, line);
        return NULL;
    }

    /* Advancing position by 1 and marking start. */
    (*pos)++;
    start = *pos;

    /* Searching for closing " and counting length.*/
    while (line[*pos] != '"' && line[*pos] != '\0') {
        (*pos)++;
        len++;
    }
    
    /* If string wasn't closed. */
    if (line[*pos] == '\0') {
        AddError(errors, slr->data[lineNum], ErrDt_StrMissingClosing, line);
        return NULL;
    }

    /* Checking for extra text. Parsing will continue, but error will be saved. */
    (*pos)++;
    SkipBlank(line, pos);
    if (!IsBlank(line[*pos]) && line[*pos] != '\0') {
        AddError(errors, slr->data[lineNum], ErrDt_StrExtra, line);
    }

    /* Allocating result string. */
    str = (char*)malloc(sizeof(char)*(len+1));
    if (str == NULL) {
        perror("Failed to allocate memory.");
        exit(1);
    }

    /* Copying string content. */
    for (spos=0; spos<len; spos++)
        str[spos] = line[start+spos];

    /* Adding termination character. */
    str[spos] = '\0';

    return str;
}

DArrayInt* ParseDataDirectiveArgs(char* line, int* pos, List* errors, int lineNum, DArrayInt* slr) {
    List* args;
    DArrayInt* pargs;
    ListNode* cur; /* List iterator. */

    pargs = CreateDArrayInt(8);

    /* Getting arguments. */
    args = GetArgs(line, &pos, errors, lineNum, slr);

    /* No arguments. */
    if (args->count == 0) {
        AddError(errors, slr->data[lineNum], ErrDt_DtNoArgument, line);
        return NULL;
    }

    /* Parsing arguments */
    cur = args->head;
    while (cur != NULL) {
        if (IsNumber(cur->data))
            DArrayIntAdd(pargs, ParseNumber(cur->data));
        else {
            DArrayIntFree(pargs);
            /* TODO!!! FREE ARGS ARRAY  !!!*/
            return NULL;
        }
        cur = cur->next;
    }

    return pargs;
}


/* Determines type of the directive:
   string, data, or extern/entry. 
   Assumes that first word of the line after label starts with '.'.
   Advances position to first blank character after directive name.
   Arguments:
    line    -- Line containing statement. 
    pos     -- Position in line pointing to '.' before directive name.
   Returns:
    Directive type according to DirectivesEnum.
    Returns -1 if statement not recognized) */
int GetDirectiveType(char* line, int* pos) {
    char* name[MAX_STATEMENT_LEN+2]; /* Buffer for holding word from line. */
    char* res; /* Result of getting the word. */

    /* Reading first word (after label). */
    res = GetNextWord(line, pos, name, MAX_STATEMENT_LEN+1, NULL);
    /* If there is no word. */
    if (res == NULL)
        return -1;

    /* Checking directive names. */
    if (CompareStrings(name, ".data"))
        return dir_data;
    if (CompareStrings(name, ".string"))
        return dir_string;
    if (CompareStrings(name, ".entry"))
        return dir_entry;
    if (CompareStrings(name, ".extern"))
        return dir_extern;

    return -1;
}

void AddSymbol(List* symbols, Symbol* new_smb, int lineNum, List* errors, DArrayInt* slr) {
    ListNode* cur = symbols->head; /* List iterator. */

    /* Searching if symbol already in the table. */
    while (cur != NULL) {
        /* Taking current iteration symbol from table. */
        Symbol* cur_smb = cur->data; 
        /* If symbol with the same name found */
        if (CompareStrings(cur_smb->name, new_smb->name)) {
            /* If symbol has attribute .extern */
            if (IsExtern(cur_smb)) {
                /* Cannot be re-defined as entry. */
                if (IsEntry(new_smb)) {
                    AddError(errors, slr->data[lineNum], ErrSmb_EntryExtern, new_smb->name);
                    return;
                }
                /* Cannot be re-defined as code, or data, or another extern (which will be completely identical definition)*/
                AddError(errors, slr->data[lineNum], ErrSmb_NameIdentical, new_smb->name);
                return;
            }
            /* If existing symbol has attributes code or data
               new symbol can only has attribute .entry, in every other context it will re-definition. */
            if ((IsCode(cur_smb) || IsData(cur_smb)) && !IsEntry(new_smb)) {
                AddError(errors, slr->data[lineNum], ErrSmb_NameIdentical, new_smb->name);
                return;
            }
            /* If existing symbol has attribute .entry */
            if (IsEntry(cur_smb)) {
                /* New symbol can't be. extern. */
                if (IsExtern(new_smb)) {
                    AddError(errors, slr->data[lineNum], ErrSmb_EntryExtern, new_smb->name);
                    return;
                }
                /* New symbol can't be also .entry (identical definition). */
                if (IsEntry(new_smb)) {
                    AddError(errors, slr->data[lineNum], ErrSmb_NameIdentical, new_smb->name);
                    return;
                }
            }
            /* In any other case (combinations code||data+entry, or entry+code||data) adding 
               new attribute to existing symbol and deallocating new symbol as it is already in table. */ 
            /* Adding attribute using binary OR operation. Example: if existing attribute is 1000 and new is 0001 result is 1001. */
            cur_smb->attributes = cur_smb->attributes | new_smb->attributes; 
            free(new_smb);
            return;
        }
        /* Advancing iterator. */
        cur = cur->next;
    }

    /* If symbol does not exist in table yet adding it. */
    ListAdd(symbols, new_smb);
}

/* Allocates new symbol structure.
   Makes new copy of label string, only MAX_LABEL_LEN first characters will be copied.
   Arguments:
    label       -- Label name string.
    address     -- Address of instruction where symbol declared.
    attribute   -- Attribute of the symbol in declaration line according to SymbolAttributesEnum.
   Returns:
    New Symbol structure. */ 
Symbol* CreateSymbol(char* label, int address, int attribute) {
    int pos = 0; /* Label string iterator. */
    int one = 1; /* Binary number one.*/
    /* Allocating structure. */
    Symbol* smb = (Symbol*)malloc(sizeof(Symbol));
    if (smb == NULL) {
        perror("Failed to allocate memory.");
        exit(1);
    }
    /* Setting attribute with binary shift. */
    smb->attributes = one << attribute; 

    /* Copying label name. */
    while (label[pos] != '\0' && pos<32) {
        (smb->name)[pos] = label[pos];
        pos++;
    }
    /* Adding termination character. */
    (smb->name)[pos] = '\0';

    /* Setting address. */
    smb->adress = address;

    return smb;
}

/*  */
void StatementToBinary(char* line, List* symbols, BinarySegment* code, BinarySegment* data, int lineNum, List* errors, DArrayInt* slr) {
    int pos = 0; /* Position in line. */
    char label[MAX_STATEMENT_LEN+2];  /* Buffer for holding label. */
    int lptr; /* Variable for holding result of getting the label.*/

    /* Trying to get the label. */
    lptr = TryGetLabel(line, &pos, label, MAX_STATEMENT_LEN+1);

    /* Checking length of a label. */
    if (lptr != NULL && StringLen(label)>MAX_LABEL_LEN) {
        /* If label is too long pretending it does not exist and adding error. */
        lptr = NULL;
        AddError(errors, slr->data[lineNum], ErrSmb_TooLong, label);
    }

    /* Skipping blanks. We should arrive to a name of a command. */
    SkipBlank(line, pos);

    /* If command starts with a dot checking if statement is a directive. */
    if (line[pos] == '.') {
        int dir_type; /* Variable for holding directive type. */
        /* Trying to get directive type. pos will be advanced to first char after the name.*/
        dir_type = GetDirectiveType(line, pos);
        /* Checking if directive is recognized. */
        if (dir_type == -1) {
            AddError(errors, slr->data[lineNum], ErrDir_NotRecognized, line);
            /* Instruction name cannot start with a dot, so if directive is not recognized
               line is not an instruction either and cannot be parsed.*/
            return;
        }
        /* If directive is .data. */
        if (dir_type == dir_data) {
            /* Getting data arguments (values). */
            DArrayInt* values = ParseDataDirectiveArgs(line, &pos, errors, lineNum, slr);
            /* If failed to parse args, or they do not exist. */
            if (values == NULL)
                return;
            /* If line opened with label trying to save it to symbols table. */
            if (lptr != NULL) {
                Symbol* smb = CreateSymbol(label, NextSegmentAddress(data), att_data);
                AddSymbol(symbols, smb, lineNum, errors, slr);
            }
            /* Writing .data argument values to binary data segment.*/
            
                
        }
    }

}