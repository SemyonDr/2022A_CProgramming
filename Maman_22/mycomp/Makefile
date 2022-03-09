# Setting C compiler
CC = gcc

# Parameters for C compiler:
# -Wall -- enable all warnings
# -ansi	-- defines that ANSI 89 standard is used
# -pedantic	-- forces to comform to chosen standard
# -lm	-- links standard math library
CFLAGS = -Wall -ansi -pedantic -lm


# Target, that should be used to compile whole program
# Executes commands on specified targets
all: compile


# Compile executable
# $(CC) - use GCC (defined above)
# $(CFLAGS) - c compiler flags (defined above)
# con.c -- file to be compiled
# -o con -- resulting executable
compile:
# Using this command (complinig c code into executable)
	$(CC) complex.c parsing.c execution.c mycomp.c $(CFLAGS) -o mycomp