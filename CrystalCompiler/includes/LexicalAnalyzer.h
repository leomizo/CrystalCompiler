#ifndef __Compiler__LexicalAnalyzer__
#define __Compiler__LexicalAnalyzer__

/*!
 
   @header LexicalAnalyzer
 
   The Lexical Analyzer parses the source code and generates tokens according
   to the found words and symbols.
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-11-19
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Token.h"
#include "SymbolTable.h"

/*!
   @typedef    ReservedWord
   @abstract   The integer reference of a reserved word.
   @discussion The enumeration value of each reserved word is equivalent of the that word's index in the RESERVED_WORDS constant array.
 */
typedef enum {
    
    // Types
    rwVoid, rwInt, rwFloat, rwBoolean, rwChar, rwString,
    
    // Struct
    rwStruct, rwEndstruct,
    
    // Control
    rwBegin, rwEnd, rwIf, rwElsif, rwElse, rwEndif, rwWhile, rwEndwhile, rwFor, rwEndfor, rwReturn, rwNot,
    
    // Values
    rwTrue, rwFalse,
    
    // Function names
    rwMain, rwScan, rwPrint
    
} ReservedWord;

/*!
   @function initializeLexicalAnalyzer
   @abstract Initializes the lexical analyzer with the provided file name.
   @param filename
        The filename of the source code.
   @result
        1 if the lexical analyzer could be initialized, 0 otherwise
 */
int initializeLexicalAnalyzer(const char* filename);

/*!
   @function getNextToken
   @abstract Gets the next token from the lexical analyzer.
   @param token
        A pointer to the generated token.
 */
void getNextToken(Token** token);

/*!
   @function setLexicalAnalyzerSymbolTable
   @abstract Indicates to the lexical analyzer which symbol table to use when retrieving identifiers.
   @param table
        The symbol table id supposed to be used by the lexical analyzer.
 */
void setLexicalAnalyzerSymbolTable(SymbolTableId table);

/*!
   @function generateErrorMessage
   @abstract Creates an error message indicating the position of the error in the source code.
   @param buffer
        The buffer where the error message should be allocated.
   @param errorDetail
        An additional message describing the error. This parameter must be NULL if no additional message should be displayed.
 */
void generateErrorMessage(char** buffer, const char* errorDetail);

/*!
   @function freeLexicalAnalyzer
   @abstract Frees the lexical analyzer's associated memory blocks.
 */
void freeLexicalAnalyzer();

#endif /* defined(__Compiler__LexicalAnalyzer__) */
