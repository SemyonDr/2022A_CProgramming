#include "DataContainers.h"

/* Frees memory occupied by instruction structure. 
   Removes argument structures if they are present
   and instruction itself.*/
void FreeIns(Ins* ins) {
   if (ins != NULL) {
      /* Removing argument structures. */
      if (ins->source != NULL)
         free(ins->source);
      if (ins->dest != NULL)
         free(ins->dest);
      /* Removing instruction structure. */
      free(ins);
   }
}

/* Tells if binary value describing addressing modes
   in instruction info has specific mode.
   Arguments:
    amodes          -- Source, or destination adressing modes value.
    adressing mode  -- Adressing mode according to AdressingModesEnum.
   Returns:
    1   -- If modes has specific adressing mode.
    0   -- Otherwise. */
int HasMode(int amodes, int adressingMode) {
    /* This function explained by example. */
    /* For example adressing mode is 2 and amodes are 0110=6*/
    int i; /* Iterator. */
    int mode_bin = 1; /* Binary representation of a mode. Starts as 0001 */
    mode_bin = mode_bin << adressingMode; /* Making binary shift. Now mode_bin is 0100. */
    /* Checking if mode is in amodes. */
    return amodes & mode_bin; /* 0110 & 0100 = 0100 > 1 = true*/
}

/* Checks if attribute "code" is set for symbol.
   Arguments:
    attributes -- attributes number from Symbol structure.
   Returns:
    0 -- attribute is set.
    1 -- Attribute not set. */
int IsCode(Symbol* smb) {
   return smb->attributes & 8;
}

/* Checks if attribute "data" is set for symbol.
   Arguments:
    attributes -- attributes number from Symbol structure.
   Returns:
    0 -- attribute is set.
    1 -- Attribute not set. */
int IsData(Symbol* smb) {
   return smb->attributes & 4;
}

/* Checks if attribute "extern" is set for symbol.
   Arguments:
    attributes -- attributes number from Symbol structure.
   Returns:
    0 -- attribute is set.
    1 -- Attribute not set. */
int IsExtern(Symbol* smb) {
   return smb->attributes & 2;
}

/* Checks if attribute "entry" is set for symbol.
   Arguments:
    attributes -- attributes number from Symbol structure.
   Returns:
    0 -- attribute is set.
    1 -- Attribute not set. */
int IsEntry(Symbol* smb) {
   return smb->attributes & 1;
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


/* Add symbol to symbols table. */
void AddSymbol(List* symbols, Symbol* new_smb, Errors* errors) {
    ListNode* cur = symbols->head; /* List iterator. */

    /* Searching if symbol already in the table. */
    while (cur != NULL) {
        /* Taking current iteration symbol from table. */
        Symbol* cur_smb = (Symbol*)(cur->data); 
        /* If symbol with the same name found */
        if (CompareStrings(cur_smb->name, new_smb->name)) {
            /* If symbol has attribute .extern */
            if (IsExtern(cur_smb)) {
                /* Cannot be re-defined as entry. */
                if (IsEntry(new_smb)) {
                    AddError(errors, ErrSmb_EntryExtern, new_smb->name, NULL);
                    return;
                }
                /* Cannot be re-defined as code, or data, or another extern (which will be completely identical definition)*/
                AddError(errors, ErrSmb_NameIdentical, new_smb->name, NULL);
                return;
            }
            /* If existing symbol has attributes code or data
               new symbol can only has attribute .entry, in every other context it will re-definition. */
            if ((IsCode(cur_smb) || IsData(cur_smb)) && !IsEntry(new_smb)) {
                AddError(errors, ErrSmb_NameIdentical, new_smb->name, NULL);
                return;
            }
            /* If existing symbol has attribute .entry */
            if (IsEntry(cur_smb)) {
                /* New symbol can't be. extern. */
                if (IsExtern(new_smb)) {
                    AddError(errors, ErrSmb_EntryExtern, new_smb->name, NULL);
                    return;
                }
                /* New symbol can't be also .entry (identical definition). */
                if (IsEntry(new_smb)) {
                    AddError(errors, ErrSmb_NameIdentical, new_smb->name, NULL);
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