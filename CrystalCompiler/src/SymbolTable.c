/*!
 
   SymbolTable.c
  
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-09-18
 
 */

#include "SymbolTable.h"


// A symbol table
typedef struct SymbolTable {
    SymbolTableId id;
    SymbolTableRow* firstRow;
    SymbolTableId parentSymbolTable;
    struct SymbolTable* nextSymbolTable;
} SymbolTable;


// Linked list of all symbol tables, ordered by their identifiers
SymbolTable* symbolTables = NULL;


int initializeNewSymbolTable() {
    
    SymbolTableId newSymbolTableId;
    
    // Create first row
    SymbolTableRow* newSymbolTableFirstRow = malloc(sizeof(SymbolTableRow));
    newSymbolTableFirstRow->symbol = NULL;
    newSymbolTableFirstRow->dimensionSizes = NULL;
    newSymbolTableFirstRow->nextRow = NULL;
    newSymbolTableFirstRow->category = scUndefined;
    
    // Create table
    SymbolTable* newSymbolTable = malloc(sizeof(SymbolTable));
    newSymbolTable->firstRow = newSymbolTableFirstRow;
    newSymbolTable->parentSymbolTable = -1;
    newSymbolTable->nextSymbolTable = NULL;
    
    // Push table to the symbol table linked list
    if (symbolTables == NULL) {
        symbolTables = newSymbolTable;
        newSymbolTableId = 0;
    } else {
        SymbolTable* table;
        newSymbolTableId = 1;
        for (table = symbolTables; table->nextSymbolTable != NULL; table = table->nextSymbolTable) newSymbolTableId++;
        table->nextSymbolTable = newSymbolTable;
    }
    
    newSymbolTable->id = newSymbolTableId;
    
    return newSymbolTableId;
}


SymbolTable* getSymbolTable(SymbolTableId symbolTableId) {
    
    if (symbolTables == NULL || symbolTableId == -1) return NULL;
    else {
        SymbolTable* table = symbolTables;
        for (SymbolTableId i = 0; i < symbolTableId; i++) table = table->nextSymbolTable;
        return table;
    }
    
}


SymbolTableRow* lookupSymbol(char* symbol, SymbolTableId symbolTableId) {
    
    SymbolTable* table = getSymbolTable(symbolTableId);
    
    if (table != NULL) do {
        
        SymbolTableRow* row = table->firstRow;
        
        // If the symbol of the first row is NULL, this means that the table is empty.
        if (row->symbol == NULL) return NULL;
        else do {
            // Iterate over the linked list, return the current index if the symbol was found.
            if (strcmp(symbol, row->symbol) == 0) return row;
            row = row->nextRow;
        } while (row != NULL);
        
        table = getSymbolTable(table->parentSymbolTable);
        
    } while (table != NULL);
    
    // Symbol not found
    return NULL;
    
}


SymbolTableRow* getSymbol(int index, SymbolTableId symbolTableId) {
    
    SymbolTable* table = getSymbolTable(symbolTableId);
    SymbolTableRow* row = table->firstRow;
    
    // Iterate until the right node is found.
    for (int i = 0; i < index && row != NULL; i++) row = row->nextRow;
    
    // If the index is valid, return a copy of the symbol, it it is not, return NULL
    return row;
    
}


int addNewSymbolIfNonexistent(char* symbol, SymbolTableId symbolTableId) {
    
    SymbolTable* table = getSymbolTable(symbolTableId);
    
    // If the table is empty, insert a copy of the symbol to the first node.
    if (table->firstRow->symbol == NULL) {
        
        table->firstRow->symbol = malloc(sizeof(*symbol));
        strcpy(table->firstRow->symbol, symbol);
        table->firstRow->id = 0;
        return 0;
        
    } else {
        
        int index = 0;
        SymbolTableRow* newRow;
        SymbolTableRow* row = table->firstRow;
        SymbolTableRow* lastRow = NULL;
        
        // Iterate until the last row or the symbol is found.
        do {
            if (strcmp(row->symbol, symbol) == 0) return index;
            index++;
            lastRow = row;
            row = row->nextRow;
        } while (lastRow->nextRow != NULL) ;
        
        // Alocate a new row and insert a copy of the given symbol to it
        newRow = malloc(sizeof(SymbolTableRow));
        newRow->symbol = malloc(sizeof(*symbol));
        newRow->id = index;
        newRow->dimensionSizes = NULL;
        newRow->category = scUndefined;
        strcpy(newRow->symbol, symbol);
        
        // Update the former last row and return the index of the new last row
        newRow->nextRow = NULL;
        lastRow->nextRow = newRow;
        return index;
        
    }
    
}

void setSymbolTableParent(SymbolTableId child, SymbolTableId parent) {
    
    SymbolTable* table = getSymbolTable(child);
    table->parentSymbolTable = parent;
    
}

SymbolTableId getSymbolTableParent(SymbolTableId symbolTable) {
    
    return getSymbolTable(symbolTable)->parentSymbolTable;
    
}


void freeAllSymbolTables() {
    
    
    SymbolTable* table = symbolTables;
    
    while (table != NULL) {
        
        SymbolTable* tableToFree = table;
        table = table->nextSymbolTable;
        
        SymbolTableRow* row = tableToFree->firstRow;
        
        // Iterate over all nodes and free each one of them
        while (row != NULL) {
            SymbolTableRow* rowToFree = row;
            row = row->nextRow;
            free(rowToFree);
        }
        
        free(tableToFree);
        
    }
    
}


