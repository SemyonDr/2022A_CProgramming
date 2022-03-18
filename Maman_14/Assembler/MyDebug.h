#ifndef MYDEBUG_H
    #define MYDEBUG_H

#include <stdio.h>
#include "Data.h"
#include "DataContainers.h"

/* DEBUG 
   FOR PRINTING INSTRUCTION NAME BY CODE
   Returns instruction name by given code. */
char* GetInsNameByCode(int insCode);

/* DEBUG: Prints source line reference*/
void PrintSLR(DynArr* slr);

/* DEBUG: Prints InsArg structure content. */
void PrintArg(InsArg* arg);

/* Prints first 20 bits of a binary number. */
void PrintBinary(int val);

void PrintBinarySegment(BinarySegment* bin);

void PrintSymbol(Symbol* smb);

void PrintSymbolsTable(List* symbols);

void PrintReferencesList(List* unresolved);

#endif