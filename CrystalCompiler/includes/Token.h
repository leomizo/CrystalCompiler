#ifndef __Compiler__Token__
#define __Compiler__Token__

/*!
 
   @header Token
 
   A token is a data structure which provides essential information to the
   Systactic Analyzer about the words and symbols parsed from the source code
   by the Lexical Analyzer. This header presents the functions and definitions
   for token manipulation.
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-09-18
 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Token type
typedef enum {
    tokenTypeUndefined,       // Token is not yet complete and cannot be determined.
    tokenTypeIdentifier,      // Token is an identifier defined by the source code.
    tokenTypeNumberInteger,   // Token is a integer number.
    tokenTypeNumberFloat,     // Token is a float number.
    tokenTypeCharacter,       // Token is a char literal.
    tokenTypeString,          // Token is a string literal.
    tokenTypeReservedWord,    // Token is a reserved word.
    tokenTypeSpecialSymbol,   // Token is a special symbol.
    tokenTypeEnd,             // Token indicates that there are no other tokens to be created.
    tokenTypeError,           // Token is invalid, it indicates an error.
} TokenType;


// Token value
typedef union {
    int    intValue;
    float  floatValue;
    char   charValue;
    char*  stringValue;
} TokenValue;


// Token
typedef struct Token {
    TokenType    type;
    TokenValue   value;
    int          lineNumber;
    int          characterNumber;
} Token;


// Initializes a token struct according to the provided parameters.
void createToken(Token** token, TokenType tokenType, TokenValue tokenValue, int tokenLineNumber, int tokenCharacterNumber);


//Frees the given token.
void freeToken(Token* token);


#endif /* defined(__Compiler__Token__) */
