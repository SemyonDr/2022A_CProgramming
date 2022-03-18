#include "assembler.h"

void ResolveReferences(BinarySegment* code, List* symbols, List* references, Errors* errors) {
    /* Going trough references list. */
    ListNode* cur = references->head; /* References iterator. */
    printf("DEBUG: Resolving references:\n");
    while (cur != NULL) {
        Symbol* smb;
        LabelReference* ref = cur->data;
        printf("DEBUG: \tLabel [ %s ] ",ref->name);
        /* Searching for symbol in symbols table. */
        smb = FindSymbolByName(symbols, ref->name);
        /* If symbol found resolving reference. */
        if (smb != NULL) {
            int base; /* Base part of an address. */
            int offset; /* Offset part of an address. */
            int are; /* ARE part of binary word. */
            int word; /* Binary word. */
            printf(" found.\n");
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
                /* If symbol is code or data setting ARE to Relocatable = 010 = 2 */
                are = 2;

                /* Calculating base and offset of the memory address. */
                /* Getting part of address that is divisible by 16 by binary shifting address back and forth. */
                base = (smb->adress >> 4) << 4; 
                offset = smb->adress - base; 
                /* Writitng base data word. */
                word = (are << 16) + base;
                SetBinary(code, ref->address, word);
                /* Writing offset data word */
                word = (are<<16)+offset;
                SetBinary(code, ref->address+1, word);
            }
        }
        else { /* If symbol not found. */
            printf(" not found.\n");
            AddErrorManual(errors, ref->origin, ErrSmb_NotFound, ref->name, NULL);
        }
        /* Advancing iterator */
        cur = cur->next;
    }
}

void MakeAssembly(char* fileName) {
    int lineNum = 1;
    FILE* source; /* Handler of expanded source file. */
    char* fullFname; /* Name of the file with extension. */
    int fullNameLen; /* Length of the full file name (not counting termination character). */
    char line[MAX_STATEMENT_LEN+2]; /* Buffer for holding line from source file. */
    BinarySegment* code; /* Binary data representing code segment as int numbers. */
    BinarySegment* data; /* Binary data representing data segment as int numbers. */
    Errors* errors; /* Errors list. */
    List* symbols;  /* Symbols table. Contains symbol (label) definitions data. */
    List* references; /* List of unresolved label arguments. Reference is use of label as instruction argument. */

    /* Initialization. */
    code = CreateBinary();
    code->base = 100; /* Setting code segment start address.*/
    data = CreateBinary();
    errors = CreateErrors();
    symbols = CreateList();
    references = CreateList();

    /* Preprocessing the file. */
    Preprocess(fileName, errors);

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
        Symbol* smb;
        ChangeErrCurLine(errors, lineNum);
        smb = ProcessStatement(line, references, code, data, errors);
        if (smb != NULL)
            AddSymbol(symbols, smb, errors);
        lineNum++;    
    }
    PrintBinarySegment(code);
    PrintBinarySegment(data);
    PrintReferencesList(references);
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
    CheckEntries(symbols, errors);
    ResolveReferences(code, symbols, references, errors);
    PrintSymbolsTable(symbols); 
    PrintBinarySegment(code);
    PrintBinarySegment(data);

    PrintErrorsList(errors);
}

int main(int argc, char **argv)
{
    /* Running assembler for every file name passed as argument. */
    int argn; /* Argument number. */
    for (argn = 1; argn<argc; argn++) {
        MakeAssembly(argv[argn]);
    }
    return 0;
}