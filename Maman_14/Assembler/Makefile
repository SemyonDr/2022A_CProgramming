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

# Compiles Data.c file into object
data:
	$(CC) Data.c $(CFLAGS) -c -o ./bin/object/data.o

parsing:
	$(CC) Parsing.c $(CFLAGS) -c -o ./bin/object/parsing.o

file:
	$(CC) File.c $(CFLAGS) -c -o ./bin/object/file.o

err:
	$(CC) Errors.c $(CFLAGS) -c -o ./bin/object/errors.o

pp:
	$(CC) Preprocessor.c $(CFLAGS) -c -o ./bin/object/preprocessor.o

string:
	$(CC) MyString.c $(CFLAGS) -c -o ./bin/object/mystring.o


# Compile executable
# $(CC) - use GCC (defined above)
# $(CFLAGS) - c compiler flags (defined above)
# con.c -- file to be compiled
# -o ./bin/assembler -- resulting executable
compile:
	$(CC) Definitions.h Data.c MyString.c Parsing.c Errors.c Preprocessor.c Binary.c assembler.c $(CFLAGS) -o ./bin/assembler