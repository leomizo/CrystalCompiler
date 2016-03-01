#ifndef __Compiler__SymbolTable__
#define __Compiler__SymbolTable__

/*!
 
   @header SymbolTable
 
   A symbol table holds all symbols that are declared and defined in the parsed source code for a specific scope.
   We consider as a symbol a word that is not a reserved word.
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-09-18
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IntegerList.h"


// Symbol table identifier in the linked list
typedef int SymbolTableId;


// Symbol category defines what the symbol is
typedef enum {
    scUndefined, // Symbol is used in the scope but its definition is not in the table
    scFunction,
    scParameter,
    scVariable,
    scStruct
} SymbolCategory;


// Symbol type: variable and function return types
typedef enum {
    stVoid,
    stInt,
    stFloat,
    stBoolean,
    stChar,
    stString,
    stStruct
} SymbolType;


// Symbol table row, represents a symbol in a table
typedef struct SymbolTableRow {
    int id;
    char* symbol;
    SymbolCategory category;
    int type;
    SymbolTableId symbolTable;
    int address;
    int totalSize;
    IntegerList* dimensionSizes;
    struct SymbolTableRow* nextRow;
} SymbolTableRow;


// Initializes the symbol table. The table MUST be initialized before it is used.
// Returns the identifier of the newly created table.
int initializeNewSymbolTable();


// Given a string (symbol name), looks for the symbol in the given table.
// Returns a pointer to the respective table row, or NULL if the symbol is not found.
SymbolTableRow* lookupSymbol(char* symbol, SymbolTableId symbolTable);


// Given an index (position of a row in the table) and a table, returns a pointer to the
// respective row, or NULL if the index is not valid.
SymbolTableRow* getSymbol(int index, SymbolTableId symbolTable);


// Given a table (child), define its parent table
void setSymbolTableParent(SymbolTableId child, SymbolTableId parent);


// Given a table (child), returns the identifier of the parent table
SymbolTableId getSymbolTableParent(SymbolTableId symbolTable);


// Adds a new symbol to the table if it does not exist yet. Either way, returns the index of the word in the table.
int addNewSymbolIfNonexistent(char* symbol, SymbolTableId symbolTableId);


// Frees all the symbol tables and the associated memory blocks.
void freeAllSymbolTables();

#endif /* defined(__Compiler__SymbolTable__) */
