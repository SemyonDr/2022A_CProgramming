#include "Binary.h"



/* Parses instruction line and produces Ins structure allocated on heap.
   Structure contains istruction code and structures that describe arguments.
   Catches parsing and arguments errors.
   Moves position to end of the line.
   Assumes that provided arguments are correct and does not check them.
   Arguments:
    line    -- Instruction line from source code.
    pos     -- Position in line after label and before instruction name.
    errors  -- Errors list.
    slr     -- Source line reference.
    lineNum -- Number of line in expanded source file.
   Returns:
    Pointer to allocated instruction structure. NULL if parsing failed. */
Ins* ParseInstructionLine(char* line, int* pos, List* errors, DArrayInt* slr, int lineNum) {
    char word[MAX_STATEMENT_LEN+2]; /* Buffer for holding word from line. */
    char* res;  /* Result of getting the word. */
    Ins* ins;   /* Parsed instruction structure. */
    List* rawArgs; /* Instruction arguments as strings. */
    InsInfo insinfo;   /* Info about instruction. */
    int num_args; /* Number of arguments required by instruction (not necessarily actually entered number of arguments).
                     Deducted from insinfo. */

    {/* DEBUG PRINT ****************************************************** */
        char* line_copy = CopyStringToHeap(line);
        RemoveLeadingBlanks(line_copy);
        ReplaceNewLine(line_copy, ' ');
        printf("DEBUG: \tBreaking down instruction line [%s]\n", line_copy);
        free(line_copy);
    }/* DEBUG PRINT ****************************************************** */

    /* Getting first word after label (possible instruction name). */
    res = GetNextWord(line, &pos, word, MAX_STATEMENT_LEN+1, ",");
    /* Checking if line is not empty. */
    if (res == NULL) {
        AddError(errors, slr->data[lineNum], ErrStm_Empty, line);
        return NULL;
    }

    /* Allocating instruction structure. */
    ins = (Ins*)malloc(sizeof(Ins));
    if (ins == NULL) {
        perror("Failed to allocate memory.");
        exit(1);
    }
    /* Initializing fields. */
    ins->source = NULL;
    ins->dest = NULL;    

    /* Getting instruction code. */
    ins->ins = GetInstructionType(word);
    /* If instruction is not recognized. */
    if (ins->ins == -1) {
        AddError(errors, slr->data[lineNum], ErrStm_NotRecognized, line);
        FreeIns(ins);
        return NULL;
    }

    {/* DEBUG PRINT ****************************************************** */
        printf("DEBUG: \t\tInstruction name: %s\n", word);
        printf("DEBUG: \t\tInstruction number: %d\n", ins->ins);
    }/* DEBUG PRINT ****************************************************** */

    /* Getting instruction arguments as strings. */
    rawArgs = GetRawArgs(line, &pos, errors, lineNum, slr);

    {/* DEBUG PRINT ****************************************************** */
        int i = 1;
        ListNode* cur = rawArgs->head;
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
    printf("DEBUG: \t\t Name is %s\n", GetInsNameByCode(insinfo.ins));
    printf("DEBUG: \t\t opcode is %d\n", insinfo.opcode);
    printf("DEBUG: \t\t funct is %d\n", insinfo.funct);
    printf("DEBUG: \t\t source amodes are %d\n", insinfo.amodes_source);
    printf("DEBUG: \t\t dest amodes are %d\n", insinfo.amodes_dest);

    /* Deducting number of arguments that should be in instruction. */
    if (insinfo.amodes_dest == 0)
        num_args = 0;   /* If there is no destination argument there is no source argument. */
    else {
        if (insinfo.amodes_source == 0)
            num_args = 1; /* Only destination argument. */
        else
            num_args = 2; /* Destination and source arguments. */
    }

    /* Checking number of arguments that were extracted from the line. */
    /* Missing arguments. */
    if (rawArgs->count < num_args) {
        AddError(errors, slr->data[lineNum], ErrIns_MissingArg, line);
        free(ins); /* Freeing instruction structure. */
        FreeListAndData(rawArgs); /* Freeing arguments list. */
        return NULL;
    }
    
    /* Too many arguments. */
    if (rawArgs->count > num_args) 
        /* Too many arguments. Appropriate number of arguments will
           be parsed, but error will be saved. */
        AddError(errors, slr->data[lineNum], ErrIns_ExtraArg, line);

    /* If instruction has 2 arguments: */
    if (num_args == 2) {
        /* Parsing arguments. */
        ins->source = ParseInsArg(rawArgs->head->data, errors, lineNum, slr);
        ins->dest = ParseInsArg(rawArgs->head->next->data, errors, lineNum, slr);
        /* Freeing raw arguments list. */
        FreeListAndData(rawArgs);
        /* If parsing arguments failed. */
        if (ins->source == NULL || ins->dest == NULL) {
            FreeIns(ins);  /* Removing instruction structure. */
            return NULL;
        }
        /* Checking if entered modes are available for instuction arguments. */
        if (!HasMode(insinfo.amodes_source, ins->source->amode)) {
            AddError(errors, slr->data[lineNum], ErrIns_InvalidSrcAmode, line);
            FreeIns(ins);
            return NULL;
        }
        if (!HasMode(insinfo.amodes_dest, ins->dest->amode)) {
            AddError(errors, slr->data[lineNum], ErrIns_InvalidDestAmode, line);
            FreeIns(ins);
            return NULL;
        }
    }
    
    /* If instruction has 1 argument it is always a destination argument. */
    if (num_args == 1) {
        /* Parsing arguments. */
        ins->dest = ParseInsArg(rawArgs->head->data, errors, lineNum, slr);
        FreeListAndData(rawArgs); /* Freeing string arguments. */
        /* If parsing arguments failed. */
        if (ins->dest == NULL) {
            FreeIns(ins);
            return NULL;
        }
        /* Checking if entered modes are available for destination argument. */
        if (!HasMode(insinfo.amodes_dest, ins->dest->amode)) {
            AddError(errors, slr->data[lineNum], ErrIns_InvalidDestAmode, line);
            FreeIns(ins);
            return NULL;
        }
    }

    return ins;
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
                /* If .entry was in table and new symbol is code or data its address should overwrite .entry address. */
                cur_smb->adress = new_smb->adress;
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

/* Writes .data line arguments to data binary segment.
   Arguments:
    dataArgs    -- Parsed numeric values of .data arguments.
    data        -- Data binary segment. */
void DataToBinary(DynArr* dataArgs, BinarySegment* data) {
    int i = 0; /* Iterator. */
    int are = 4; /* Absolute addressing mode 100 = 4*/

    /* Writing .data argument values to binary data segment.*/
    for (i=0; i < dataArgs->count; i++) {
        int word = 0; /* Binary word. */
        int isNegative = 0; /* Flag that shows if value is negative. */
        int one = 1; /* Number one. */
        int bit = 0; /* Bit iterator. */
        int value = dataArgs->data[i]; /* Argument value. */
        /* Truncating value to 16 bits by copying into word only 16 least significant bits of the value. */
        /* If value is negative its positive compliment found before truncating. */
        if (value < 0) {
            value = ((-1)*value)-1;
            isNegative = 1;
        }
        /* Copying first 15 bits to word. */
        for (bit=0; bit<15; bit++)
            word += (one << bit) && value; /* This operation will add to word value of individual bit on position "bit". */
        /* Setting sign bit. */
        if (isNegative)
            word += one << 16; /* Setting bit #15 */
        /* Adding ARE qualifier. */    
        word += are << 16;
        /* Adding resulting word to binary segment. */
        AddBinary(data, word);
    }
}

/* Writes .string line argument to data binary segment.
   Arguments:
    line    -- Line containing .string directive.
    pos     -- Position in line after word .string and before arguments. 
    data    -- Data binary segment.
    lineNum -- Number of line in expanded source code.
    errors  -- List of errors.
    slr     -- Source line reference.
 */
void StringToBinary(char* arg, BinarySegment* data) {
    int i; /* Iterator. */
    int are = 4; /* Absolute mode ARE = 100 = 4. */
    int word; /* Binary word. */

    /* Writing individual letters to data segment. */    
    while (arg[i] != '\0') {
        word = arg[i];
        word += are << 16;
        AddBinary(data, word);
        i++;
    }

    /* Adding closing 0 word. */
    AddBinary(data, 0);
}


void InstructionToBinary(Ins* ins, BinarySegment* code) {
    InsInfo info; /* Info about instruction. */
    int word; /* Number representing machine word. */
    int are;  /* Number that represent adressing mode for word. */
    int opcode; /* Number that represent opcode. */
    int funct; /* Number that represent funct.*/
    int src = 0;  /* Number that represent source register. */
    int src_mode = 0; /* Number that represent source register addressing mode. */
    int dest = 0; /* Number that represent destination register. */
    int dest_mode = 0; /* Number that represent destination register addressing mode.*/
    /* Typical two-word instruction:
       0[are][opcode]
       0[are][funct][src][src_mode][dest][dest_mode]*/

    /* Getting instruction info. */
    info = GetInstructionInfo(ins->ins);

    /* ***** Encoding opcode word. ***** */
    /* ARE for instruction word is ablosute = 100 = 4*/
    are = 4;
    /* Encoding opcode by shifting 1 to 'opcode' bit. */
    opcode = 1 << info.opcode;
    /* Combining ARE and opcode to word. */
    word = opcode + (are << 16);
    /* Writing word to code segment. */
    AddBinary(code, word);

    /* If instruction does not have arguments we are done. */
    if (info.amodes_dest == 0 && info.amodes_source == 0)
        return;

    /* If instruction has arguments. */

    /* ***** Encoding funct word ***** */
    /* ARE stays absolute, so are=4. */
    /* Encoding funct. */
    funct = info.funct;
    /* If instruction has argument it at least has destination argument. */
    /* Encoding destination addressing mode. */
    dest_mode = ins->dest->amode;
    /* Encoding destination register. */
    if (dest_mode == am_immediate || dest_mode == am_direct)
        /* In direct and immediate modes register not specified. */
        dest = 0;
    else
        /* In indexed and register modes register is saved in value field of argument structure. */
        dest = ins->dest->val;
    /* If there is source argument */
    if (info.amodes_source != 0) {
        /* Encoding source addressing mode. */
        src_mode = ins->source->amode;
        /* Encoding source register. */
        if (src_mode == am_immediate || src_mode == am_direct)
            src = 0;
        else
            src = ins->source->val;
    }
    /* Combining fields to funct word using binary shift. */
    word = dest_mode;
    word += dest << 2;
    word += src_mode << 6;
    word += src << 8;
    word += funct << 12;
    word += are << 16;
    /* Writing word to code segment. */
    AddBinary(code, word);

    /* ***** Encoding data words ***** */
    /* If destination is in immedeate mode writing value word. */
    if (dest_mode == am_immediate) {
        /* ARE is still absolute */
        word = ins->dest->val + (are << 16);
        AddBinary(code, word);
    }
    /* If destination is in direct, or indexed modes leave 2 blank words to resolve base+offset later. */
    if (dest_mode == am_direct || dest_mode == am_index) {
        AddBinary(code, 0);
        AddBinary(code, 0);
    }
    /* If source mode is immediate writing value word. */
    if (src_mode == am_immediate) {
        word = ins->source->val + (are << 16);
        AddBinary(code, word);
    }
    /* If source mode is direct, or indexed modes leave 2 blank words to resolve data+offset later. */
    if (src_mode == am_direct || dest_mode == am_index) {
        AddBinary(code, 0);
        AddBinary(code, 0);
    }
}

/*  */
Symbol* StatementToBinary(char* line, List* unresolved, BinarySegment* code, BinarySegment* data, Errors* errors) {
    int pos = 0; /* Position in line. */
    char label[MAX_STATEMENT_LEN+2];  /* Buffer for holding label. */
    int lptr; /* Variable for holding result of getting the label.*/
    Symbol* smb; /* Structure that holds info about line label for symbols table. */    

    /* Trying to get the label. */
    lptr = TryGetLabel(line, &pos, label, MAX_STATEMENT_LEN+1);

    /* Checking length of a label. */
    if (lptr != NULL && StringLen(label)>MAX_LABEL_LEN) {
        /* If label is too long pretending it does not exist and adding error. */
        lptr = NULL;
        AddError(errors, ErrSmb_TooLong, line, label);
    }

    /* Skipping blanks. We should arrive to a name of a command. */
    SkipBlank(line, &pos);

    /* If command starts with a dot checking if statement is a directive. */
    if (line[pos] == '.') {
        int dir_type; /* Variable for holding directive type. */
        /* Trying to get directive type. pos will be advanced to first char after the name.*/
        dir_type = GetDirectiveType(line, &pos);

        /* Checking if directive is recognized. */
        if (dir_type == -1) {
            AddError(errors, ErrDir_NotRecognized, line, NULL);
            /* Instruction name cannot start with a dot, so if directive is not recognized
               line is not an instruction either and cannot be parsed.*/
            return NULL;
        }

        /* If directive is .data. */
        if (dir_type == dir_data) {
            List* raw_data_args; /* Raw .data arguments (strings). */
            DynArr* data_args; /* parsed .data arguments (values). */
            int data_counter = NextSegmentAddress(data); /* Adress of this data block. */

            /* Getting raw arguments. */
            raw_data_args = GetRawArgs(line, &pos, errors);
            /* Checking if arguments are present. */
            if (data_args->count == 0) {
                AddError(errors, ErrDt_DtNoArgument, line, NULL);
                FreeListAndData(data_args);
                return NULL;
            }
            /* Parsing arguments. */
            data_args = ParseDataArgs(line, raw_data_args, errors);
            /* Adding .data arguments to data segment. */
            DataToBinary(data_args, data);
            /* If line opened with label returning the symbol. */
            if (lptr != NULL)
                return CreateSymbol(label, data_counter, att_data);
            else
                return NULL;
        }

        /* If directive is .string. */
        if (dir_type == dir_string) {
            List* raw_string_args; /* List of raw (string) arguments for .string.*/
            char* arg; /* Parsed argument (content of ""). */
            int data_counter = NextSegmentAddress(data); /* Adress of this data block. */
            /* Getting raw arguments of .string directive. */
            List* raw_string_args = GetRawArgs(line, &pos, errors);
            /* Checking if arguments are present. */
            if (raw_string_args->count == 0) {
                AddError(errors, ErrDt_StrNoArgument, line, NULL);
                FreeListAndData(raw_string_args);
                return NULL;
            }
            /* Checking if more that one argument given. */
            if (raw_string_args->count > 1) {
                AddError(errors, ErrDt_StrExtra, line, NULL);
            }
            /* Parsing the argument. */
            arg = ParseStringArgument(raw_string_args->head->data, errors);
            /* If parsing failed. */
            if (arg == NULL) {
                FreeListAndData(raw_string_args);
                free(arg);
                return NULL;
            }
            /* Adding string data to data segment. */
            StringToBinary(arg, data);
            /* Freeing memory. */
            FreeListAndData(raw_string_args);
            free(arg);

            /* If line opened with label returning the symbol. */
            if (lptr != NULL)
                return CreateSymbol(label, data_counter, att_data);
            else
                return NULL;
        }

        /* If directive is .entry or .extern*/
        if (dir_type == dir_entry || dir_type == dir_extern) {
            char arg[MAX_LABEL_LEN+1]; /* Buffer for holding directive argument. */
            char* res;  /* Result of taking directive argument. */
            res = GetSymbolDirectiveArgument(line, &pos, arg, lineNum, errors, slr);
            /* Not succeeded in taking the argument. */
            if (res == NULL)
                return;
            /* If line opened with a label print warning and ignore it. */
            if (lptr != NULL) {
                char* linecp = CopyStringToHeap(line); /* Making line copy. */
                RemoveLeadingBlanks(linecp);    /* Preparing line copy for printing. */
                ReplaceNewLine(linecp, '\0');
                printf("Warning: Line %d: \"%s\" <- Label before .entry or .extern will be ignored.\n", slr->data[lineNum], linecp);
                free(linecp);
            }
            /* Creating appropriate symbol structure. */
            if (dir_type == dir_entry)
                smb = CreateSymbol(arg, 0, att_entry);
            else /* dir_type == dir_extern*/
                smb = CreateSymbol(arg, 0, att_extern);
            /* Adding symbol to symbols table. */
            AddSymbol(symbols, smb, lineNum, errors, slr);
            return;
        }
    } /* If name starts with a dot section end. */
}