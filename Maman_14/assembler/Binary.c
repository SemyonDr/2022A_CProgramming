#include "Binary.h"

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
int GetDirectiveType(char *line, int *pos)
{
    char name[MAX_STATEMENT_LEN + 2]; /* Buffer for holding word from line. */
    char *res;                        /* Result of getting the word. */

    /* Reading first word (after label). */
    res = GetNextWord(line, pos, name, MAX_STATEMENT_LEN + 1, ",");
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



/* Writes .data line arguments to data binary segment.
   Arguments:
    dataArgs    -- Parsed numeric values of .data arguments.
    data        -- Data binary segment. */
void DataToBinary(DynArr *dataArgs, BinarySegment *data)
{
    int i = 0;   /* Iterator. */
    int are = 4; /* Absolute addressing mode 100 = 4*/

    /* Writing .data argument values to binary data segment.*/
    for (i = 0; i < dataArgs->count; i++)
    {
        int word = 0;                  /* Binary word. */
        int isNegative = 0;            /* Flag that shows if value is negative. */
        int one = 1;                   /* Number one. */
        int bit = 0;                   /* Bit iterator. */
        int value = dataArgs->data[i]; /* Argument value. */
        /* Truncating value to 16 bits by copying into word only 16 least significant bits of the value. */
        /* If value is negative its positive compliment found before truncating. */
        if (value < 0)
        {
            value = ((-1) * value);
            isNegative = 1;
        }
        /* Copying first 15 bits to word. */
        for (bit = 0; bit < 15; bit++)
            word += (one << bit) & value; /* This operation will add to word value of individual bit on position "bit". */
        /* Setting sign bit. */
        if (isNegative) {
            word = (-1)*word;
            word += one << 16; /* Setting bit #15 */
        }    
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
void StringToBinary(char *arg, BinarySegment *data)
{
    int i=0;       /* Iterator. */
    int are = 4; /* Absolute mode ARE = 100 = 4. */
    int word;    /* Binary word. */

    /* Writing individual letters to data segment. */
    while (arg[i] != '\0')
    {
        word = arg[i];
        word += are << 16;
        AddBinary(data, word);
        i++;
    }

    /* Adding closing 0 word. */
    word = 0 + (are<<16);
    AddBinary(data, word);
}


/* Translates given instruction structure to binary words
   and writes them into code binary segment.
   Creates label references if found and adds them to list.
   Arguments:
    ins          -- Instruction structure.
    code         -- Code binary segment.
    references   -- List of label arguments (label references).
    lineNum      -- Number of line in expanded source file where instruction originates.
   Algorithm:
    Breaks down each word to elements (opcode, addressing mode, etc), determines element value
    and uses binary shifts to combine elements into binary word.
    Uses InsInfo to get instruction description and uses it alongside with ins structure
    to produce elements. */
void InstructionToBinary(Ins* ins, BinarySegment* code, List* references, int lineNum) {
    InsInfo info;      /* Info about instruction. */
    int word;          /* Number representing machine word. */
    int are;           /* Number that represent adressing mode for word. */
    int opcode;        /* Number that represent opcode. */
    int funct;         /* Number that represent funct.*/
    int src = 0;       /* Number that represent source register. */
    int src_mode = 0;  /* Number that represent source register addressing mode. */
    int dest = 0;      /* Number that represent destination register. */
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
    if (info.funct >=10 && info.funct <= 13)
        funct = info.funct;
    else   
        funct = 0;
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
    /* If function has source argument. */
    if (info.amodes_source != 0) {
        /* If source mode is immediate writing value word. */
        if (src_mode == am_immediate) {
            word = ins->source->val + (are << 16);
            AddBinary(code, word);
        }
        /* If source mode is direct, or indexed modes leave 2 blank words to resolve data+offset later. */
        if (src_mode == am_direct || src_mode == am_index) {
            /* Adding label to list of unresolved references. */
            LabelReference* ref = CreateLabelReference(ins->source->label, NextSegmentAddress(code), lineNum);
            ListAdd(references, ref);
            /* Adding binary words. */
            AddBinary(code, 0);
            AddBinary(code, 0);
        }
    }

    /* If destination is in immedeate mode writing value word. */
    if (dest_mode == am_immediate)
    {
        /* ARE is still absolute */
        word = ins->dest->val + (are << 16);
        AddBinary(code, word);
    }
    /* If destination is in direct, or indexed modes leave 2 blank words to resolve base+offset later. */
    if (dest_mode == am_direct || dest_mode == am_index) {
        /* Adding label to list of unresolved references. */
        LabelReference* ref = CreateLabelReference(ins->dest->label, NextSegmentAddress(code), lineNum);
        ListAdd(references, ref);
        /* Adding binary words. */
        AddBinary(code, 0);
        AddBinary(code, 0);
    }
}



/* Translates given statement of any kind (instruction, or directive)
   to binary words and writes them to appropriate binary segment.
   Also produces structure that describes label before the line if it is present.
   Arguments:
    line        -- String that contains statement.
    references  -- List of label arguments (label references).
    code        -- Code binary segment.
    data        -- Data binary segment.
    errors      -- Errors list.
   Returns:
    If statement opened with a label symbol is created with appropriate address and attribute fields.
    If line not contained opening label returns NULL (not considere a failure). 
   Algorithm:
    Tries to get label (first word that ends with :). 
    After that position in line is set to first characer after label. Or before first word if label is not present.
    All further parsing is performed on a line with label removed by setting position.
    First word should be a command name.
    If it starts with a dot then checked if it is a directive (.data .string .entry .extern). If it is not parsing considered failed.
    If it is a directive appropriate argument getting and parsing functions are called.
    After getting and parsing arguments appropriate ToBinary functions are called for .data and .string and Symbol created and returned
    for .entry and .extern.
    If command name does not begin with a dot ParseInstructionLine is called and Ins structure produced. Then InstructionToBinary 
    called to translate and write Ins structure to binary code. */
Symbol* StatementToBinary(char* line, List* references, BinarySegment* code, BinarySegment* data, Errors* errors) {
    int pos = 0;                       /* Position in line. */
    char label[MAX_STATEMENT_LEN + 2]; /* Buffer for holding label. */
    char* lptr;                        /* Variable for holding result of getting the label.*/

    /* Trying to get the label. */
    lptr = TryGetLabel(line, &pos, label, MAX_STATEMENT_LEN + 1);

    /* Checking length of a label. */
    if (lptr != NULL && StringLen(label) > MAX_LABEL_LEN) {
        /* If label is too long pretending it does not exist and adding error. */
        lptr = NULL;
        AddError(errors, ErrSmb_TooLong, line, label);
    }

    /* Skipping blanks. We should arrive to a name of a command. */
    SkipBlank(line, &pos);

    /* If command starts with a dot checking if statement is a directive. */
    if (line[pos] == '.')
    {
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
            List* raw_data_args;                         /* Raw .data arguments (strings). */
            DynArr* data_args;                           /* parsed .data arguments (values). */
            int data_counter = NextSegmentAddress(data); /* Adress of this data block. */

            /* Getting raw arguments. */
            raw_data_args = GetRawArgs(line, &pos, errors);
            /* Checking if arguments are present. */
            if (raw_data_args->count == 0)
            {
                AddError(errors, ErrDt_DtNoArgument, line, NULL);
                FreeListAndData(raw_data_args);
                return NULL;
            }
            /* Parsing arguments. */
            data_args = ParseDataArgs(line, raw_data_args, errors);
            /* Checking if parsing failed. */
            if (data_args == NULL) {
                FreeListAndData(raw_data_args);
                return NULL;
            }
            /* Adding .data arguments to data segment. */
            DataToBinary(data_args, data);
            /* If line opened with label returning the symbol. */
            if (lptr != NULL)
                return CreateSymbol(label, data_counter, att_data);
            else
                return NULL;
        }

        /* If directive is .string. */
        if (dir_type == dir_string)
        {
            List *raw_string_args;                       /* List of raw (string) arguments for .string.*/
            char *arg;                                   /* Parsed argument (content of ""). */
            int data_counter = NextSegmentAddress(data); /* Adress of this data block. */
            /* Getting raw arguments of .string directive. */
            raw_string_args = GetRawArgs(line, &pos, errors);
            /* Checking if arguments are present. */
            if (raw_string_args->count == 0)
            {
                AddError(errors, ErrDt_StrNoArgument, line, NULL);
                FreeListAndData(raw_string_args);
                return NULL;
            }
            /* Checking if more that one argument given. */
            if (raw_string_args->count > 1)
            {
                AddError(errors, ErrDt_StrExtra, line, NULL);
            }
            /* Parsing the argument. */
            arg = ParseStringArgument(line, raw_string_args->head->data, errors);
            /* If parsing failed. */
            if (arg == NULL)
            {
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
        if (dir_type == dir_entry || dir_type == dir_extern)
        {
            char arg[MAX_LABEL_LEN + 1]; /* Buffer for holding directive argument. */
            char *res;                   /* Result of taking directive argument. */
            res = GetSymbolDirectiveArgument(line, &pos, arg, errors);
            /* Not succeeded in taking the argument. */
            if (res == NULL)
                return NULL;
            /* If line opened with a label print warning and ignore it. */
            if (lptr != NULL) {
                char *linecp = CopyStringToHeap(line); /* Making line copy. */
                RemoveLeadingBlanks(linecp);           /* Preparing line copy for printing. */
                ReplaceNewLine(linecp, '\0');
                printf("Warning: Line %d: \"%s\" <- Label before .entry or .extern will be ignored.\n", errors->cur_line_num, linecp);
                free(linecp);
            }
            /* Creating appropriate symbol structure. */
            if (dir_type == dir_entry)
                return CreateSymbol(arg, 0, att_entry);
            else /* dir_type == dir_extern*/
                return CreateSymbol(arg, 0, att_extern);
        }
    } /* If name starts with a dot section end. */

    /* If command name doesn't start with a dot it is considered to be an instruction name.
       Instruction parsing function is called then. */
    {
        Ins *ins;                                   /* Pointer to parsed instruction structure. */
        int ins_counter = NextSegmentAddress(code); /* Saving address where this instructions block starts. */
        /* Trying to parse the instruction. */
        ins = ParseInstructionLine(line, &pos, errors);
        /* Checking if instruction parsing succeeded. */
        if (ins == NULL)
            return NULL;
        /* Writing parsed structure to code binary segment. */
        InstructionToBinary(ins, code, references, (errors->slr->data)[errors->cur_line_num]);
        /* If label existed creating the symbol. */
        if (lptr != NULL)
            return CreateSymbol(label, ins_counter, att_code);
        else
            return NULL;
    }
}



/* Reads expanded source file and produces binary segments with unresolved label arguments.
   Also produces symbols table and list of label references.
   After this step it is neccessary only to resolve label references.
   Arguments:
    fileName    -- Name of source file without extension.
    code        -- Code binary segment.
    data        -- Data binary segment.
    symbols     -- Symbols table.
    references  -- List of references to labels as instruction arguments.
    errors      -- Errors list.
   Algorithm:
    Opens expanded source file.
    Reads statements from file and uses StatementToBinary to translate them into binary words,
    extract symbols and register label references. Adds symbols to symbols table.
    After code and data segments are constructed sets initial addres of data segment to be next address after code segment.
    Initial binary contains data segment in full and in code segment everything is ready, except for base+offset 
    data words which set to 0 and should be resolved using LabelReference and symbols table. */
void ProduceInitialBinary(char* fileName, BinarySegment* code, BinarySegment* data, List* symbols, List* references, Errors* errors) {
    int lineNum = 1; /* Current line number. */
    FILE* source;    /* Handler of expanded source file. */
    char* fullFname; /* Name of the file with extension. */
    int fullNameLen; /* Length of the full file name (not counting termination character). */
    char line[MAX_STATEMENT_LEN+2]; /* Buffer for holding line from source file. */

    /* Opening the file. */
    fullNameLen = StringLen(fileName) + 3;
    fullFname = (char*)malloc(sizeof(char)*(fullNameLen+1));
    if (fullFname == NULL) { 
        perror("Failed to allocate memory.\n"); 
        exit(1); }
    /* Getting full file name. */
    AppendExtension(fileName, "am", fullFname, fullNameLen);
    source = fopen(fullFname, "r"); /* Opening target file for reading */
    /* Check. */
    if (source == NULL) { 
        perror("Failed to open file.\n"); 
        exit(2); 
    }

    /* Reading file line by line and creating binary representation. */
    while (fgets(line, MAX_STATEMENT_LEN+1, source) != NULL) {
        Symbol* smb; /* Line label info. */
        /* Changing current line for errors. */
        ChangeErrCurLine(errors, lineNum);
        /* Processing current statement. */
        smb = StatementToBinary(line, references, code, data, errors);
        /* If line strats with a label adding it to the symbols table. */
        if (smb != NULL)
            AddSymbol(symbols, smb, errors);
        /* Adviancing iterator. */
        lineNum++;    
    }

    fclose(source);

    /* Moving data segment to address after instructions segment. */
    data->base = NextSegmentAddress(code);

    /* Moving data symbols addresses to new data base. */
    {
        ListNode* cur = symbols->head;
        while (cur != NULL) {
            Symbol* smb = cur->data;
            if (IsData(smb))
                smb->adress += data->base;
            cur = cur->next;
        }
    }
}



/* Resolves label references in binary code segment.
   Arguments:
    code        -- Code binary segment.
    symbols     -- Symbols table.
    references  -- List of label references.
    errors      -- Errors list.
   Algorithm:
    For each label reference saved in references table searches
    symbol by same name in symbols table and substitutes
    binary words in code segment pointed by reference structure
    with base+offset address stored in symbols table.
    If symbol marked as extern 0 is written. */
void ResolveReferences(BinarySegment* code, List* symbols, List* references, Errors* errors) {
    /* Going trough references list. */
    ListNode* cur = references->head; /* References iterator. */

    while (cur != NULL) {
        Symbol* smb;
        LabelReference* ref = cur->data;

        /* Searching for symbol in symbols table. */
        smb = FindSymbolByName(symbols, ref->name);

        /* If symbol found resolving reference. */
        if (smb != NULL) {
            int are; /* ARE part of binary word. */
            int word; /* Binary word. */
            
            /* Setting ARE mode. */
            if (IsExtern(smb)) {
                /* If Symbol is .extern setting ARE to External = 001 = 1 */
                are = 1;        
                /* Writing 2 empty data words. */
                word = are <<16;
                SetBinary(code, ref->address, word);
                SetBinary(code, ref->address+1, word);
            }        
            else {
                BOAddress bo; /* Variable for storing address. */
                /* If symbol is code or data setting ARE to Relocatable = 010 = 2 */
                are = 2;

                /* Calculating base and offset of the memory address. */
                bo = AddressToBO(smb->adress);

                /* Writitng base data word. */
                word = (are << 16) + bo.base;
                SetBinary(code, ref->address, word);
                /* Writing offset data word */
                word = (are<<16)+bo.offset;
                SetBinary(code, ref->address+1, word);
            }
        }
        else { /* If symbol not found. */
            AddErrorManual(errors, ref->origin, ErrSmb_NotFound, ref->name, NULL);
        }
        /* Advancing iterator */
        cur = cur->next;
    }
}