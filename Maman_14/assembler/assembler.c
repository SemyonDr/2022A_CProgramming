/* Program description:
    This program produces binary object files from pseudo 
    assembly language source files. Also produced files that describe symbols
    for linking object files but linker is not presented in this project.
   Program operations:
    Takes file names as arguments, then for each file expands
    macros, reads source code line by line, parses it and translates to binary
    machine code. If no errors were encountered on any stage object and symbols 
    files are written. If errors encountered a list or errors is printed.
   Modules:
     

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