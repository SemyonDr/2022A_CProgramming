;In this file code contains macro errors
	;Macro name starts with a number
	macro 1s
		add DATA[r1], r5
	endm
	inc r5
	1s
	;macro name contains illegal characters
	macro m#1
		add DATA[r1], r5
	endm
	m#1
	
	;Macro name not defined
	macro
		add DATA[r1], r5
	endm
	
	;Macro name is reserved
	macro add
		add DATA[r1], r5
	endm
	
	;Macro defined twice
	;Extra text after name and endm
	macro xx extra
		add DATA[r1], r5
	endm
	macro xx
		add DATA[r1], r5
	endm extra
	xx
	;Extra text after macro call
	xx extra
	
	;Nested macro definition
	macro yy
		macro zz
		endm
	endm
DATA: .data 100, 200