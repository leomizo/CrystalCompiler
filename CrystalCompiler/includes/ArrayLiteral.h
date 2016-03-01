#ifndef ArrayLiteral_h
#define ArrayLiteral_h

/*!
 
   @header ArrayLiteral
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-10-28
 
 */

#include <stdio.h>
#include "SubAutomata.h"

#define ARYL_COUNT_OF_STATES 6
#define ARYL_COUNT_OF_SYMBOLS 4

typedef enum {
    arylOpenBrackets,
    arylCloseBrackets,
    arylComma,
    arylOther
} ArrayLiteralTerminal;

int GET_ARYL_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall);

#endif /* ArrayLiteral_h */
