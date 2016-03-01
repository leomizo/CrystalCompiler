/*!
 
   Token.c
  
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-09-18
 
 */

#include "Token.h"


void createToken(Token** token, TokenType tokenType, TokenValue tokenValue, int tokenLineNumber, int tokenCharacterNumber) {
    
    // Allocate token
    *token = malloc(sizeof(Token));
    
    // Fill out its fields
    (*token)->type = tokenType;
    (*token)->value = tokenValue;
    (*token)->lineNumber = tokenLineNumber;
    (*token)->characterNumber = tokenCharacterNumber;
    
}

void freeToken(Token* token) {
    
    if (token != NULL) {
        if (token->type == tokenTypeString) free(token->value.stringValue);
        free(token);
    }
    
}
