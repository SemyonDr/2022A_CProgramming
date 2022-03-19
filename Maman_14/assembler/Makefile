# Setting C compiler
CC = gcc

# Parameters for C compiler:
# -Wall -- enable all warnings
# -ansi	-- defines that ANSI 89 standard is used
# -pedantic	-- forces to comform to chosen standard
CFLAGS = -Wall -ansi -pedantic

# Target, that should be used to compile whole program
# Executes commands on specified targets
all: compile

# Compile executable
# $(CC) - use GCC (defined above)
# $(CFLAGS) - c compiler flags (defined above)
# con.c -- file to be compiled
# -o ./assembler -- resulting executable
compile:
	$(CC) Definitions.c MyString.c Data.c DataContainers.c Symbols.c Errors.c Parsing.c Preprocessor.c  Binary.c Output.c assembler.c $(CFLAGS) $(CFLAGS) -o ./assembler