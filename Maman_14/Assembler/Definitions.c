#include "Definitions.h"

/* Return info about given instruction.
   Argument:
    code    -- Instruction code according to InstructionsEnum.
   Returns:
    Instruction info structure.  */
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