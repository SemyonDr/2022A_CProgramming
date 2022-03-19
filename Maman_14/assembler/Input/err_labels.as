;This file contains label errors

;Label is too long
LAAAAAAAAAAAAAAAAAAAAAAAAAAAABEL: add r1, r2

;Label defined twice
DOUBLE: sub r1, r2
DOUBLE: .data 1, 2

;Label defined as entry and extern
.entry LABEL
.extern LABEL

;Label not found
add missing, DOUBLE

;Entry not defined
.entry ENTRY