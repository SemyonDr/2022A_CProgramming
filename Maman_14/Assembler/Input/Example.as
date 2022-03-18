;file ps.as
.entry LIST
.extern W
clr DATA
CODE: prn #16
		add CODE, W
DATA: .data -1,5,5,3
LIST: .string "empty"