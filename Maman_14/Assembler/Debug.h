#ifndef MYDEBUG_H
    #define MYDEBUG_H

/* DEBUG 
   FOR PRINTING INSTRUCTION NAME BY CODE
   Returns instruction name by given code. */
char* GetInsNameByCode(int insCode) {
    if (insCode <0 || insCode > 15)
        return "NaN";

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