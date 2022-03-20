/* Program description:
    This program produces binary object files from pseudo 
    assembly language source files. Also produced files that describe symbols
    for linking object files but linker is not presented in this project.
   Program operations:
    Program takes file names as arguments, then for each file expands
    macros, reads source code line by line, parses it and translates to binary
    machine code. If no errors were encountered on any stage object and symbols 
    files are written. If errors encountered a list or errors is printed.
   Modules:
    -- Definitions
        Contains definitions of assembly language -- enums of commands and addressing
        modes. Also contains structure that describes language defined information 
        about given instruction and a function that produces this information
        structure.
    -- MyString
        Collection of small functions for working with strings.
    -- Data
        Contains definitions and functions related to collection data structures --
        linked list, dynamic integer array and binary segment dynamic array.
    -- DataContainers
        Othert non-collection data structures that are used in the application
        for storing information and descriptions.
    -- Symbols
        Structures and functions related to symbols table and label
        references list.
    -- Errors
        Data structures and functions used for storing and printing errors.
        List of possible errors also defined here.
    -- Parsing
        Collection of functions and sub-functions that break down and parse raw text to
        data structures.
    -- Preprocessor
        All logic related to pre-processing stage - macro definitions registration
        and macro expansion.
    -- Binary
        Functions that use parsing to read source code and then tranlate it
        to binary machine words.
    -- Output
        Functions for writing binary instruction and symbols to resulting files.
    -- assembler
        Main function.
   Algorithm:
    On top level applicaion is working with key data structures - binary segments for code and data
    and lists of symbols and symbol references. This structures are given to functions to fill and
    then used in other function.
    -- Fisrst step of program execution is to expand macros in given assembly source code
    and to produce expanded source code .am file.
    Preprocessing is done with two key functions - RegisterMacroInfo() and ExpandMacro(). Preprocessor 
    parent function (Preprocess()) reads source code, determines if line is macro definition, or macro call
    and calls RegisterMacroInfo() or ExpandMacro() accordingly. 
    Also on this step source line reference list is filled. Source line reference is an integer array
    stored in Errors structure that allows to determine for each line in expanded source file on which
    line (line number) it was in original source file. It is used for printing errors line numbers for user.
    -- Second step is to produce initial binary representation. Parent function ProduceInitialBinary()
    reads expanded source file, determines type of command - instruction, or directive, calls
    for parsing functions to break down the line and the it calls ToBinary functions to translate it to
    machine code. ToBinary() functions take data, translate it to binary code and write it to 
    binary segments. Also ProduceInitialBinary() fills symbols table and symbol references list while 
    parsing the code. Resulting initial binary code contains full data image and code image with placeholder
    words (0) for label reference arguments.
    -- Third step is to resolve placeholder argument words in code segment. ResolveReferences() function is used.
    Every symbol reference from references list is searched in symbols table and then symbol information is
    written to code segment. LabelReference contains code segment address where label definition address should
    be written and symbols in symbols table contain label definition addresses.
    After this step full binary image is created and stored in memory in BinarySegment structures.
    -- Final step is file output functions. WriteBinaryObject() translates binary word of binary segments
    to "special" base and writes object file. WriteEntries() and WriteExterns() use symbols table and references
    list to write .ent and .ext files.
    -- On errors processing:
    Every function that expects input from source code file (direct, or parsed data) recieves Errors array
    data structure as argument. If errors of source code are encountered on any step they are added
    to Errors collection. Because every line of source code does not depend on other lines if line produced errors
    functions that process this line register them, return failure indication (usually NULL pointer) and move on to
    the next line. So if errors are encountered while processing data incorrect data is skipped and processing continues.
    This allows to collect errors for whole source code file and show them to user. If errors were encountered output files
    are not produced.
   Input:
    Application is given as aguments assembly source code file names (without extensions). Those source code files are
    application input.
   Assumtions:
    Almost every function assumes that given input is correct and ready for processing - pointers are not NULL, 
    strings have content and termination, and integer values are in correct ranges, etc. Usually if function is given some argument
    it is checked before calling the function, because usually this argument is produced by another function
    and functions produce correct arguments, or NULL pointers (or -1, or 0 integers) when failed.
   Note:
    I wrote preprocessor stage thinking about C preprocessor and my preprocessing function not only expands
    macros, but also removes empty lines and comments. When I noticed that project description demands leaving comments and
    empty lines in expanded file I didn't have time to fix it, because further logic assumes that expanded file has only
    instructions and directives. I hope that this is not a big error. 
   */

#include "assembler.h"

/* Main function.
   Processes every file name given as argument in following manner:
   Calls Preprocess and produces .am file.
   Calls ProduceInitialBinary and fills binary code and data arrays. References to symbols as arguments left as 0s in code segment.
   Also fills references and symbols tables.
   Calls ResolveArguments to substitute symbol reference data words in code segment.
   After that step full binary image of the assembly code is created.
   If errors were encountered while producing binary image they are printed and output is not written (except for .am file).
   If there were no errors calls for Output.h functions and writes .ob .ent and .ext files.
   */
int main(int argc, char **argv) {
    int argn; /* Argument number. */

    /* Running assembler for every file name passed as argument. */
    for (argn = 1; argn<argc; argn++) {
        char* file_name; /* File name without extension. */
        Errors* errors; /* List of errors. */
        BinarySegment* code; /* Structure that contains code binary representation. */
        BinarySegment* data; /* Structure that contains data binary representation. */
        List* symbols;  /* Symbols table that contains list of every symbol defined in assembly code.*/
        List* references; /* List of unresolved label arguments. Reference is use of label as instruction argument. */

        printf("Processing file [ %s.as ]\n", file_name);

        /* Referencing the file name. */
        file_name = argv[argn];

        /* Initializing errors list. */ 
        errors = CreateErrors();

        /* Initializing binary segments. */
        code = CreateBinary();
        code->base = 100; /* Setting code initial address to 100. */
        data = CreateBinary();

        /* Initializing symbols table. */
        symbols = CreateList();

        /* Initializing references list. */
        references = CreateList();

        /* Preprocessing the file. Expanding macros, removing comments and empty lines and creating .am file. */
        Preprocess(file_name, errors);

        printf("Preprocess finished, resulting file is [ %s.am ]\n", file_name);

        /* Processing expanded .am file. Creates initial code and data binary segments and fills symbols table. */
        ProduceInitialBinary(file_name, code, data, symbols, references, errors);

        printf("Initial binary representation is created.\n");

        /* Checking if symbols table is valid. */
        ValidateSymbolsTable(symbols, errors);
        /* Resolving symbol reference arguments in binary segments. */
        ResolveReferences(code, symbols, references, errors);

        printf("Symbol references are resolved.\n");

        /* If no errors encountered writing resulting files. */
        if (errors->count == 0) {
            printf("File [ %s.as ] processed successfully.\n", file_name);
            printf("Writing object file [ %s.ob ]\n", file_name);
            WriteBinaryToObject(file_name, code, data);
            printf("Writing entries file [ %s.ent ]\n", file_name);
            WriteEntries(file_name, symbols);
            printf("Writing externals file [ %s.ext ]\n", file_name);
            WriteExterns(file_name, symbols, references);
        }
        else { /* Or printing errors. */ 
            printf("Failed to process file [ %s.as ]\n", file_name);
            printf("%d errors are encountered:\n", errors->count);
            SortErrors(errors);
            PrintErrorsList(errors);
        }

        /* Removing errors list. */
        FreeErrors(errors);

        /* Removing binary segments. */
        FreeBinary(code);
        FreeBinary(data);

        /* Removing symbols table. */
        FreeListAndData(symbols);

        /* Removing references table. */
        FreeListAndData(references);
    }
    return 0;
}