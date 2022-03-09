#ifndef PREPROCESSOR_H
    #define PREPROCESSOR_H

#include <stdio.h>

#include "Data.h"
#include "Errors.h"
#include "Parsing.h"

/* Searches macro in macros list by name.
   Arguments:
    macros  -- List of macros.
    name    -- Name of a macro to find.
   Returns:
    MacroInfo with provided name.
    NULL if not found.*/
MacroInfo* FindMacroByName(List* macros, char* name);

/* Checks if given line consists of blank characters (' ', '\t', '\n').
   Arguments:
    line    -- Line to check (null-terminated).
   Returns:
    0       -- If line is not blank.
    1       -- If line is blank.*/
int IsLineBlank(char* line);

/* Checks if given line is a comment (First non-blank character is ';').
   Arguments:
    line    -- Line to check (null-terminated).
   Returns:
    0       -- Line is not a comment.
    1       -- Line is a comment. */
int IsLineComment(char* line);

/* Checks if given line is a macro declaration.
   (String with first word "macro").
   Arguments:
    line    -- Line to check (null-terminated).
   Returns:
    0       -- Line is not a macro declaration.
    1       -- Line is a macro declaration. */
int IsLineMacroDef(char* line);

/* Checks if line is an end line of macro definition.
   (First word of line is "endm").
   Arguments:
    line    -- Line to check.
   Returns:
    0       -- Line is not a macro definition end.
    1       -- Line is a macro definition end. */
int IsLineMacroDefEnd(char* line);

/* Checks if given line is a macro call (macro name).
   Arguments:
    line    -- Source file line (terminated).
    macros  -- List of macros.
   Returns:
    0   -- If line is not a macro call.
    1   -- If line is a macro call. */
int IsLineMacroCall(char* line, List* macros);

/* Gets macro name from macro definition line.
   Checks for macro definition line errors.
   Returned name will be allocated on heap.
   Assumes that line is macro definition line (first word is "macro")
   and does not check for that.
   Arguments:
    line        -- Macro definition line.
    defLineNum  -- Number of macro definition line in source file.
    errors      -- List of errors.
   Returns:
    Macro name string, null-terminated.
    NULL if no name is found, or it is illegal. */
char* GetMacroName(char* line, int defLineNum, List* errors);

/* Gets macro info from source file.
   Arguments:
    source      -- Pointer to source file handler.
    num_lines   -- Variable for returning number of macro lines.
    def_line    -- Line (string) where macro name is defined.
    defLineNum  -- Number of line in source file where macro name is defined.
    errors      -- Errors list.
   Returns:
    Directly returns new MacroInfo structure. NULL will be returned if errors encountered.
    Writes number of lines in macro body to num_lines pointer even if getting info failed. */
MacroInfo* GetMacroInfo(FILE** source, int* num_lines, char* defLine, int defLineNum, List* errors);

/* Registers macro definition in list of macros.
   Will set source file position to the first character
   after macro closing tag line.
   Gives no indication if macro was failed to register.
   Arguments:
    source      -- Pointer to source file handler.
    macros      -- Macros list.
    def_line    -- Line (string) where macro name is defined.
    defLineNum  -- Number of line in source file where macro name is defined.
    errors      -- Errors list.
   Returns:
    Number of line in source file after macro closing tag.*/
int RegisterMacroInfo(FILE** source, List* macros, char* defLine, int defLineNum, List* errors);

/* Expands macro by name defined in callLine.
   Copies macro body lines from source file to
   current position in target (expanded) file.
   Arguments:
    source      -- Pointer to source file hanler.
    target      -- Pointer to target (expanded) file handler.
    srcPos      -- Current position in source file (position after macro call line)
    slRef       -- Source line reference array.
    callLine    -- Line of macro call (first word is a macro name)
    callLineNum -- Number of a call line in source file.
    macros      -- List of registered macros.
    errors      -- List of errors. */
void ExpandMacro(FILE** source, FILE** target, long srcPos, DArrayInt* slRef, char* callLine, int callLineNum, List* macros, List* errors);

/* Frees memory occupied by macros list.
   Removes macro info objects, 
   list nodes and list structure itself.
   Arguments:
    macros  -- Macros list. */
void FreeMacrosList(List* macros);

/* Executes pre-processing step on assembly source code file:
   Removes comments and blank lines and expands macros.
   Arguments:
    sourceFileName      -- Name of source file without extension.
    SourceLineReference -- Array of source line references.
    errors              -- List of errors.
   Returns:
    Source line reference. */
void Preprocess(char* sourceFileName, DArrayInt* SourceLineReference, List* errors);

#endif