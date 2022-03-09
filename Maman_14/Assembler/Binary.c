#include "Binary.h"

/* Assumes that pre-processed file exists.
   Produces 4 tables:
    instructions (structures)
    data (structures)
    external symbols
    entry symbols
   Also catches parsing errors.
  */
void ParseExpanded(char* source_file_name, List* errors) {
    FILE* file; /* Expanded source file handler. */
    char* full_fname; /* Buffer for holding full file name with extension. */
    int full_fname_len; /* Length of filename with extension. */
    char line[MAX_STATEMENT_LEN+2]; /* Buffer for holding line from file. */
    int icount=100;     /* Instruction counter. */
    int dcount=0;     /* Data counter. */

    /* Opening expanded (preprocessed) file */
    /* Allocating buffer for full file name */
    full_fname_len = StringLen(source_file_name) + 3;
    full_fname = (char*)malloc(sizeof(char)*(full_fname_len+1));
    ConcatenateStrings(source_file_name, ".am", full_fname, full_fname_len);
    file = fopen(full_fname, "r");
    if (file == NULL) {
        perror("Failed to open file.");
        exit(2);
    }

    /* Reading lines from file in a loop. */
    while (fgets(line, MAX_STATEMENT_LEN+1, file) != NULL) {
        int pos = 0; /* Position in line. */
        char label[MAX_LABEL_LEN+1]; /* Buffer for holding statement label. */
        char word[MAX_STATEMENT_LEN+2]; /* Buffer for holding word from line. */
        char* label_res; /* Variable for storing label reading result. */
        char* res;  /* Variable for storing word reading result. */
        
        /* Trying to get label before statement. */
        label_res = TryGetLabel(line, &pos, label, MAX_LABEL_LEN);

        /* Getting next word */
        res = GetNextWord(line, &pos, word, MAX_STATEMENT_LEN+1, NULL);

        if (res==NULL)


    }
    

    /* Closing the file. */
    fclose(file);



}

void ProduceBinary(char* source_file_name) {
    /* Parse expanded */    

    /* Make binary code (array of int) */

    /* write binary in requested format */

    /* write tables */
}