;In this file code contains instruction arguments errors
;Valid line
CODE: add r1, r2

;Expected a number
add #nn, r5

;Invalid label
jmp la!bel

;Label argument is too long
jmp laaaaaaaaaaaaaaaaaaaaaaaaaaaabel

;Missing index
jmp label[

;Missing ]
jmp label[r10

;Extra text after index
jmp label[r10]extra

;Invalid index
jmp label[not_reg]

;Another invalid index
jmp label[r20]

;Too many arguments
add r1, r2, r3

;Invalid source mode
lea r1, r2

;Invalid destination mode
lea CODE, #5