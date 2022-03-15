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

/* Return info about given instruction. */
InsInfo GetInstructionInfo(int code) {
    InsInfo info;

    switch (code)
    {
    case ins_mov:
        info.ins = ins_mov;
        info.opcode = 0;
        info.funct = -1;
        info.amodes_source = 1+2+4+8; /* 0,1,2,3 */
        info.amodes_dest = 2+4+8; /* 1,2,3 */
        break;
    case ins_cmp:
        info.ins = ins_cmp;
        info.opcode = 1;
        info.funct = -1;
        info.amodes_source = 1+2+4+8; /* 0,1,2,3 */
        info.amodes_dest = 1+2+4+8; /* 0,1,2,3 */
        break;
    case ins_add:
        info.ins = ins_add;
        info.opcode = 2;
        info.funct = 10;
        info.amodes_source = 1+2+4+8; /* 0,1,2,3 */
        info.amodes_dest = 2+4+8; /* 1,2,3 */
        break;
    case ins_sub:
        info.ins = ins_sub;
        info.opcode = 2;
        info.funct = 11;
        info.amodes_source = 1+2+4+8; /* 0,1,2,3 */
        info.amodes_dest = 2+4+8; /* 1,2,3 */
        break;

    case ins_lea:
        info.ins = ins_lea;
        info.opcode = 4;
        info.funct = -1;
        info.amodes_source = 2+4; /* 1,2 */
        info.amodes_dest = 2+4+8; /* 1,2,3 */
        break;
    case ins_clr:
        info.ins = ins_clr;
        info.opcode = 5;
        info.funct = 10;
        info.amodes_source = 0; /* No source operand. */
        info.amodes_dest = 2+4+8; /* 1,2,3 */
        break;  
    case ins_not:
        info.ins = ins_not;
        info.opcode = 5;
        info.funct = 11;
        info.amodes_source = 0; /* No source operand. */
        info.amodes_dest = 2+4+8; /* 1,2,3 */
        break;  
    case ins_inc:
        info.ins = ins_inc;
        info.opcode = 5;
        info.funct = 12;
        info.amodes_source = 0; /* No source operand. */
        info.amodes_dest = 2+4+8; /* 1,2,3 */
        break;  

    case ins_dec:
        info.ins = ins_dec;
        info.opcode = 5;
        info.funct = 13;
        info.amodes_source = 0; /* No source operand. */
        info.amodes_dest = 2+4+8; /* 1,2,3 */
        break;  
    case ins_jmp:
        info.ins = ins_jmp;
        info.opcode = 9;
        info.funct = 10;
        info.amodes_source = 0; /* No source operand. */
        info.amodes_dest = 2+4; /* 1,2 */
        break;  
    case ins_bne:
        info.ins = ins_bne;
        info.opcode = 9;
        info.funct = 11;
        info.amodes_source = 0; /* No source operand. */
        info.amodes_dest = 2+4; /* 1,2 */
        break;  
    case ins_jsr:
        info.ins = ins_jsr;
        info.opcode = 9;
        info.funct = 12;
        info.amodes_source = 0; /* No source operand. */
        info.amodes_dest = 2+4; /* 1,2 */
        break;  

    case ins_red:
        info.ins = ins_red;
        info.opcode = 12;
        info.funct = -1;
        info.amodes_source = 0; /* No source operand. */
        info.amodes_dest = 2+4+8; /* 1,2,3 */
        break;      
    case ins_prn:
        info.ins = ins_prn;
        info.opcode = 13;
        info.funct = -1;
        info.amodes_source = 0; /* No source operand. */
        info.amodes_dest = 1+2+4+8; /* 0,1,2,3 */
        break;      
    case ins_rts:
        info.ins = ins_rts;
        info.opcode = 14;
        info.funct = -1;
        info.amodes_source = 0; /* No source operand. */
        info.amodes_dest = 0; /* No destination operand. */
        break;      
    case ins_stop:
        info.ins = ins_stop;
        info.opcode = 15;
        info.funct = -1;
        info.amodes_source = 0; /* No source operand. */
        info.amodes_dest = 0; /* No destination operand. */
        break;      
    
    default:
        info.ins = -1;
        info.opcode = -1;
        info.funct = -1;
        info.amodes_source = -1;
        info.amodes_dest = -1;
        return info;
    }

    return info;
}

/* DEBUG 
   FOR PRINTING INSTRUCTION NAME BY CODE
   Returns instruction name by given code. */
char* GetInsNameByCode(int insCode) {
    if (insCode <0 || insCode > 15)
        return CopyStringToHeap("NaN");

    char* insnames[16];
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

#endif