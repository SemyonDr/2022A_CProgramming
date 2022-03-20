;This file contains code with directives errors

;Missing arguments
STR: .string
DATA: .data
.entry
.extern

;Invalid arguments
	.string 100
	.data a, b, c

;Not closed text
.string "text

;Extra after closing text
.string "text"extra

;Unknown directive
DIR: .dir args