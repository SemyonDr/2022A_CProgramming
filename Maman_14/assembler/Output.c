#include "Output.h"

/* Converts integer values from 0-15 range to 
   corresponding hex digit as character.
   Arguments:
    val     -- Integer value.
   Returns:
    Character that represents hex digit corresponding to that value.
    If val is out of range 0-15 ' ' (blank) is returned. */
char IntToHexDigit(int val) {
    if (val < 16 && val >= 0) {
        /* If value can be represented by decimal digit. */
        if (val < 10)
            return val+48; /* ASCII number of corresponding decimal digit character. */
        else
            /* If value should be represented by hex digit. */
            return val-10+97; /* ASCII number of corresponding letter character. */
    }
    else
        /* Value out of range. */
        return ' ';
}

/* Converts 20 bit binary word to text representation
   of that word in "special" base. 
   Most significant bits in val past bit 19 are not
   expected and will be ignored.
   Arguments:
    val     -- 20 bit binary value.
    word    -- Buffer for writing string representation of val in "special" base. */
void BinaryToSpecial(int val, char word[15]) {
    int i; /* Iterator. */
    int mask = 15; /* Mask that used to get first 4 bits of a binary number 15 = 0000 0000 0000 1111 */
    char gname = 'A'; /* Name of a 4 bit group in "special" base. */
    char gval; /* Hex digit value character of 4 bit group in "special" base. */
    /* Iterating trough 5 4 bit groups A B C D E */
    for (i = 0; i<5; i++, gname++) {
        /* Getting 4 bit group value. */
        /* Example: 
           if val = 1010 1101 0010 0100, i = 2.
           val >> (4-i)*4 = 0000 0000 1010 1101
           (val >> (4-i)*4) & mask = 0000 0000 0000 1101 = 13 = 'd'
        */
        gval = IntToHexDigit((val >> ((4-i)*4)) & mask);

        /*Writing group to result buffer. */
        word[i*3] = gname;
        word[(i*3)+1] = gval;
        word[(i*3)+2] = '-';
    }
    /* Adding termination character. */
    word[14] = '\0';
}

/* Creates and fills .ob object file.
   Writes code and binary segments in "special" base to .ob file.
   Assumes that binary segment arguments are correct.
   Arguments:
    fileName    -- Name of the file without extension.
    code        -- Code binary segment.
    data        -- Data binary segment.
*/
void WriteBinaryToObject(char* fileName, BinarySegment* code, BinarySegment* data) {
    FILE* object;    /* Handler of object file. */
    char* fullFname; /* Name of the file with extension. */
    int fullNameLen; /* Length of the full file name (not counting termination character). */
    int i; /* Iterator. */

    /* Opening the file. */
    fullNameLen = StringLen(fileName) + 3;
    fullFname = (char*)malloc(sizeof(char)*(fullNameLen+1));
    if (fullFname == NULL) { 
        perror("Failed to allocate memory.\n"); 
        exit(1); }
    /* Getting full file name. */
    AppendExtension(fileName, "ob", fullFname, fullNameLen);
    object = fopen(fullFname, "w"); /* Opening object file for writing */
    /* Check. */
    if (object == NULL) { 
        perror("Failed to open file.\n"); 
        exit(2); 
    }
    /* Freeing file name string. */
    free(fullFname);

    /* Writing object file header. */
    fprintf(object, "%d %d\n", code->counter, data->counter);

    /* Writing binary words of the code segment. */
    for (i = 0; i < code->counter; i++) {
        int adr;    /* Word address in code segment. */
        int word_val; /* Value of the word. */
        char word_sp[15]; /* Word value in "special" base. */

        /* Setting address. */
        adr = code->base+i;
        /* Getting word value by address. */
        word_val = GetBinary(code, adr);
        /* Getting word in "special" base. */ 
        BinaryToSpecial(word_val, word_sp);

        /* Writing word address to file. */
        fprintf(object, "%04d ", adr);
        /* Writing word in "special" base. */
        fprintf(object, "%s", word_sp);
        
        /* Writing new line character if word is not last, or if it is last, but data segment expected after. */
        if (i != code->counter-1)
            fputc('\n', object);
        else {
            if (data->counter != 0)
                fputc('\n', object);
        }
    }

    /* Writing binary words of the data segment. */
    for (i = 0; i< data->counter; i++) {
        int adr;    /* Word address in data segment. */
        int word_val; /* Value of the word. */
        char word_sp[15]; /* Word value in "special" base. */

        /* Setting address. */
        adr = data->base+i;
        /* Getting word value by address. */
        word_val = GetBinary(data, adr);
        /* Getting word in "special" base. */ 
        BinaryToSpecial(word_val, word_sp);

        /* Writing word address to file. */
        fprintf(object, "%04d ", adr);
        /* Writing word in "special" base. */
        fprintf(object, "%s", word_sp);

        /* If word is not last adding new line character. */
        if (i != data->counter - 1) 
            fputc('\n', object);
    }

    /* Closing the file. */
    fclose(object);
}

/* Creates and fills entries .ent file.
   Assumes that arguments are correct.
   Arguments:
    fileName    -- Source file name without extension.
    symbols     -- Symbols table. */
void WriteEntries(char* fileName, List* symbols) {
    FILE* ent;       /* Handler of entries file. */
    char* fullFname; /* Name of the file with extension. */
    int fullNameLen; /* Length of the full file name (not counting termination character). */
    ListNode* cur;   /* List iterator.*/
    int num = 0;    /* Number of entry symbols in symbols table. */

    /* Opening the file. */
    fullNameLen = StringLen(fileName) + 4;
    fullFname = (char*)malloc(sizeof(char)*(fullNameLen+1));
    if (fullFname == NULL) { 
        perror("Failed to allocate memory.\n"); 
        exit(1); }
    /* Getting full file name. */
    AppendExtension(fileName, "ent", fullFname, fullNameLen);
    ent = fopen(fullFname, "w"); /* Opening entries file for writing */
    /* Check. */
    if (ent == NULL) { 
        perror("Failed to open file.\n"); 
        exit(2); 
    }
    /* Freeing file name string. */
    free(fullFname);

    /* Iterating trough symbols table and counting entries. */
    cur = symbols->head;
    while (cur != NULL) {
        if (IsEntry(cur->data))
            num++;
        cur = cur->next;
    }

    /* Iterating trough symbols table and writing entries to file. */
    cur = symbols->head;
    while (cur != NULL) {
        /* Getting symbol. */
        Symbol* smb = cur->data;
        /* Checking if symbol is entry. */
        if (IsEntry(smb)) {
            /* Converting symbol address to base+offset format. */
            BOAddress bo = AddressToBO(smb->adress);

            /* Writing entry name. */
            fprintf(ent, "%s,", smb->name);
            /* Writing base+offset address. */
            fprintf(ent, "%d,%d", bo.base, bo.offset);
            /* Counting entry as written. */
            num--;
            /* If entry not the last one adding new line character. */
            if (num != 0)
                fputc('\n', ent);

        }
        cur = cur->next;
    }   

    /* Closing the file */
    fclose(ent);
}

/* Writes external symbols info to .ext file. 
   Arguments:
    fileName    -- Name of source file without extension.
    symbols     -- Symbols table.
    references  -- List of symbol references in arguments. */
void WriteExterns(char* fileName, List* symbols, List* references) {
    FILE* ext;       /* Handler of externals file. */
    char* fullFname; /* Name of the file with extension. */
    int fullNameLen; /* Length of the full file name (not counting termination character). */
    ListNode* cur;   /* List iterator.*/
    int num = 0;    /* Number of references to externs. */

    /* Opening the file. */
    fullNameLen = StringLen(fileName) + 4;
    fullFname = (char*)malloc(sizeof(char)*(fullNameLen+1));
    if (fullFname == NULL) { 
        perror("Failed to allocate memory.\n"); 
        exit(1); }
    /* Getting full file name. */
    AppendExtension(fileName, "ext", fullFname, fullNameLen);
    ext = fopen(fullFname, "w"); /* Opening externs file for writing */
    /* Check. */
    if (ext == NULL) { 
        perror("Failed to open file.\n"); 
        exit(2); 
    }
    /* Freeing file name string. */
    free(fullFname);  

    /* Iteterating trough references table and checking if referenced symbol
       has attribute extern. Counting extern references. */
    cur = references->head;
    while (cur != NULL) {
        Symbol* smb;
        /* Getting current symbol reference. */
        LabelReference* ref = cur->data;
        /* Searching it in symbols table. */
        smb = FindSymbolByName(symbols, ref->name);
        /* Checking if symbol is marked extern. */
        if (IsExtern(smb))
            num++;
        /* Advancing iterator. */
        cur = cur->next;
    }

    /* Iterating trough references list and writitng symbols that have attribute extern. */
    cur = references->head;
    while (cur != NULL) {
        Symbol* smb;
        /* Getting current symbol reference. */
        LabelReference* ref = cur->data;
        /* Searching it in symbols table. */
        smb = FindSymbolByName(symbols, ref->name);
        /* If symbol marked extern writing info to file. */
        if (IsExtern(smb)) {
            /* Writing base line. */
            fprintf(ext, "%s BASE %d\n", ref->name, ref->address);
            /* Writing offset line */
            fprintf(ext, "%s OFFSET %d", ref->name, ref->address+1);
            /* Counting external reference as written. */
            num--;
            /* If reference is not the last one adding empty line. */
            if (num != 0)
                fprintf(ext, "\n\n");
        }
        /* Advancing iterator. */
        cur = cur->next;
    }

    fclose(ext);
}