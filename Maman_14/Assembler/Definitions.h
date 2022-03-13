#ifndef DEFINITIONS_H
    #define DEFINITIONS_H

/* Maximum length of a statement in symbols in assembly
source file not including end of the line \n symbol*/
#define MAX_STATEMENT_LEN 80
/* Maximum length of a label without : and termination character. */
#define MAX_LABEL_LEN 31

/* Enumeration of processor Instructions. */
enum InstructionsEnum {
   ins_mov, ins_cmp, ins_add, ins_sub,
   ins_lea, ins_clr, ins_not, ins_inc,
   ins_dec, ins_jmp, ins_bne, ins_jsr,
   ins_red, ins_prn, ins_rts, ins_stop
};

/* Enumeration of assembler directives (except macro directives). */
enum DirectivesEnum {
   dir_data,
   dir_string,
   dir_entry,
   dir_extern
};

/* Enumeration of symbol attributes in symbols table. */
enum SymbolAttributesEnum {
    att_entry,
    att_extern,
    att_data,
    att_code
};

/* Enumeration of adressing modes for instruction argument. */
enum AdressingModesEnum {
   am_immediate,  /* Immediate */
   am_direct,     /* Direct */
   am_index,      /* Index */
   am_rdirect     /* Register direct */
};

#endif