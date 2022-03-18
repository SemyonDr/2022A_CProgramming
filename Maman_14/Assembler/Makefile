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

def:
	$(CC) Definitions.c $(CFLAGS) -c -o ./bin/object/def.o

string:
	$(CC) MyString.c $(CFLAGS) -c -o ./bin/object/mystring.o

data:
	$(CC) Data.c $(CFLAGS) -c -o ./bin/object/data.o

containers:
	$(CC) DataContainers.c $(CFLAGS) -c -o ./bin/object/containers.o

err:
	$(CC) Errors.c $(CFLAGS) -c -o ./bin/object/errors.o

parsing:
	$(CC) Parsing.c $(CFLAGS) -c -o ./bin/object/parsing.o

pp:
	$(CC) Preprocessor.c $(CFLAGS) -c -o ./bin/object/preprocessor.o

# Compiles Data.c file into object
binary:
	$(CC) Binary.c $(CFLAGS) -c -o ./bin/object/binary.o



# Compile executable
# $(CC) - use GCC (defined above)
# $(CFLAGS) - c compiler flags (defined above)
# con.c -- file to be compiled
# -o ./bin/assembler -- resulting executable
compile:
	$(CC) Definitions.c MyString.c Data.c DataContainers.c Errors.c Parsing.c Preprocessor.c  Binary.c assembler.c $(CFLAGS) -o ./bin/assembler

debug:
	$(CC) Definitions.c MyString.c Data.c DataContainers.c Errors.c Parsing.c MyDebug.c Preprocessor.c  Binary.c assembler.c $(CFLAGS) -o ./bin/assembler_dbg