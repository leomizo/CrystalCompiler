#ifndef IfBody_h
#define IfBody_h

/*!
 
   @header IfBody
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-10-28
 
 */

#include <stdio.h>
#include "SubAutomata.h"

#define IFBY_COUNT_OF_STATES 9
#define IFBY_COUNT_OF_SYMBOLS 7

typedef enum {
    ifbyElse,
    ifbyElsif,
    ifbyEndif,
    ifbyOpenParenthesis,
    ifbyCloseParenthesis,
    ifbyColon,
    ifbyOther
} IfBodyTerminal;

int GET_IFBY_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall);

#endif /* IfBody_h */
