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

/* Converts given memory address to base+offset format.
   base is part of address that is divisible by 16
   and offset is a remainer. */
BOAddress AddressToBO(int address) {
   BOAddress bo;

   /* Getting base by dividing by 16 and then multiplying by 16. */
   bo.base = (address/16)*16;
   /* Calculating remainer. */
   bo.offset = address-bo.base;

   return bo;
}