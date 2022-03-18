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

Symbol* FindSymbolByName(List* symbols, char* label) {
   ListNode* cur = symbols->head; /* Symbols iterator. */
   /* Iterating trough symbols list. */
   while (cur != NULL) {
      Symbol* cur_smb = cur->data;
      /* Comparing symbol name. */
      if (CompareStrings(label,cur_smb->name)) {
         return cur_smb;
      }
      cur = cur->next;
   }
   /* If nothing found. */
   return NULL;
}

LabelReference* CreateLabelArgument(char* label, int address, int origin) {
   int i = 0; /* Iterator. */
   LabelReference* la;
   /* Allocating the structure. */
   la = (LabelReference*)malloc(sizeof(LabelReference));
   if (la == NULL) {
      perror("Failed to allocate memory.");
      exit(1);
   }
   /* Setting address and origin. */
   la->address = address;
   la->origin = origin;
   /* Copying label name. */
   while (label[i] != '\0') {
      la->name[i] = label[i];
      i++;
   }
   la->name[i] = '\0';
   return la;
}


void CheckEntries(List* symbols, Errors* errors) {
   ListNode* cur;
   cur = symbols->head;
   while (cur != NULL) {
      Symbol* smb = cur->data;
      if (IsEntry(smb)) {
         if (!IsCode(smb) && !IsData(smb))
            AddErrorManual(errors, 0, ErrSmb_EntryUndefined, smb->name, NULL);
      }
      cur = cur->next;
   }
}