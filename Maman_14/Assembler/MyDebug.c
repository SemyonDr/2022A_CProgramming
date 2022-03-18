#include "MyDebug.h"

/* DEBUG 
   FOR PRINTING INSTRUCTION NAME BY CODE
   Returns instruction name by given code. */
char* GetInsNameByCode(int insCode) {
    char* insnames[16];

    if (insCode <0 || insCode > 15)
        return "NaN";

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
    
    return insnames[insCode];
}

void PrintSLR(DynArr* slr) {
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


/* Prints first 20 bits of a binary number. */
void PrintBinary(int val) {
    int one = 1;
    int pos;
    int trim;
    int mask = 65535;
    /* Printing binary value. */
    for (pos = 19; pos >= 0; pos--) {
        if ((val & (one<<pos)) != 0)
            printf("1");
        else 
            printf("0");
        if ((pos)%4 == 0)
            printf(" ");
    }
    /* Printing decimal value (16 bits) */
    trim = val&mask;
    printf("\t%d", trim);
    printf("\n");
}

void PrintBinarySegment(BinarySegment* bin) {
    int i;
    printf("Printing binary segment, %d words:\n", bin->counter);
    for (i=0; i<bin->counter; i++) {
        printf("%4d -- ", bin->base+i);
        PrintBinary(GetBinary(bin, bin->base+i));
    }
}

void PrintSymbol(Symbol* smb) {
    if (smb != NULL) {
        printf("Symbol [ %s ]:\n", smb->name);
        printf("Address: %d\n", smb->adress);
        printf("Attributes: ");
        if (IsCode(smb))
            printf("code ");
        if (IsData(smb))
            printf("data ");
        if (IsEntry(smb))
            printf(".entry ");
        if (IsExtern(smb))
            printf(".extern ");
        printf("\n");
    }
    else
        printf("Symbol is NULL\n");
}

void PrintSymbolsTable(List* symbols) {
    ListNode* cur;
    if (symbols == NULL)
        return;
    if (symbols->count == 0) {
        printf("Symbols table is empty.\n");
        return;
    }

    printf("Name        Add     Att\n");
    cur = symbols->head;
    while (cur != NULL) {
        Symbol* smb = cur->data;
        printf("%-12s", smb->name);
        printf("%-4d    ", smb->adress);
        if (IsCode(smb))
            printf("code ");
        if (IsData(smb))
            printf("data ");
        if (IsEntry(smb))
            printf(".entry ");
        if (IsExtern(smb))
            printf(".extern ");
        printf("\n");
        cur = cur->next;
    }
        

}


void PrintReferencesList(List* unresolved) {
    if (unresolved != NULL) {
        ListNode* cur = unresolved->head;
        printf("Unresolved arguments:\n");
        printf("line name        address\n");
        while (cur != NULL) {
            LabelReference* ref = cur->data;
            printf("%-4d %-12s%d\n", ref->origin, ref->name, ref->address);
            cur = cur->next;
        }
    }
}