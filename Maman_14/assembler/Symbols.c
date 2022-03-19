#include "Symbols.h"

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



/* Adds symbol to symbols table.
   Arguments:
    symbols    -- Symbols table
    new_smb    -- Symbol to add.
    errors     -- Errors list.
   Algorithm:
    If symbol not found in the table it is added immediately.
    If symbol is already in the table attributes checked:
    Symbol can have only following two attribute pairs:
    code-entry and data-entry. Every other combination of existing and new 
    attributes produces error without adding new symbol.
    If pair is allowed new attribute added to existing attribute.
    If entry existed and new symbol is code or data symbol address rewritten. */
void AddSymbol(List* symbols, Symbol* new_smb, Errors* errors)
{
    ListNode* cur = symbols->head; /* List iterator. */

    /* Searching if symbol already in the table. */
    while (cur != NULL)
    {
        /* Taking current iteration symbol from table. */
        Symbol* cur_smb = (Symbol*)(cur->data);
        /* If symbol with the same name found */
        if (CompareStrings(cur_smb->name, new_smb->name))
        {
            /* If symbol has attribute .extern */
            if (IsExtern(cur_smb))
            {
                /* Cannot be re-defined as entry. */
                if (IsEntry(new_smb))
                {
                    AddError(errors, ErrSmb_EntryExtern, new_smb->name, NULL);
                    return;
                }
                /* Cannot be re-defined as code, or data, or another extern (which will be completely identical definition)*/
                AddError(errors, ErrSmb_NameIdentical, new_smb->name, NULL);
                return;
            }
            /* If existing symbol has attributes code or data
               new symbol can only has attribute .entry, in every other context it will re-definition. */
            if ((IsCode(cur_smb) || IsData(cur_smb)) && !IsEntry(new_smb))
            {
                AddError(errors, ErrSmb_NameIdentical, new_smb->name, NULL);
                return;
            }
            /* If existing symbol has attribute .entry */
            if (IsEntry(cur_smb))
            {
                /* New symbol can't be. extern. */
                if (IsExtern(new_smb))
                {
                    AddError(errors, ErrSmb_EntryExtern, new_smb->name, NULL);
                    return;
                }
                /* New symbol can't be also .entry (identical definition). */
                if (IsEntry(new_smb))
                {
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



/* Searches symbol in symbols table by given name.
   Arguments:
    symbols    -- Symbols table.
    label      -- Name of the label to search.
   Returns:
    Symbol with given name.
    NULL if symbol not found.  */
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



/* Allocates new LabelReference structure
   and fills it with provided parameters.
   Arguments:
    label      -- Label name.
    address    -- Address of data word where label value should be substituted.
    origin     -- Number of line where label referenced as argument.
   Returns:
    LabelReference structure allocated on heap. 
    */
LabelReference* CreateLabelReference(char* label, int address, int origin) {
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



/* Validates symbols table.
   Checks for last possible error (others checked when symbol is added)
    - if symbols marked as entry not marked also as code or data.
   Arguments:
    symbols    -- Symbols table.
    errors     -- Errors list. */
void ValidateSymbolsTable(List* symbols, Errors* errors) {
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