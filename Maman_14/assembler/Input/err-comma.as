;In this file code contains comma errors between arguments
;And also invalid statements

DATA1: .data,, 1,  -3 ,4
DATA2: .data 2 -4, 5

CODE1: add DATA1[r5], r10,,
CODE2: add DATA2[r10] DATA1

CODE3: foobar foo bar
CODE4:

DATA3: .dttt 5, 6, 18