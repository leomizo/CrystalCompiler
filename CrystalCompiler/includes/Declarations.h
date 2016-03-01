#ifndef Declarations_h
#define Declarations_h

/*!
 
   @header Declarations
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-10-28
 
 */

#include <stdio.h>
#include "SubAutomata.h"

#define DCLS_COUNT_OF_STATES 5
#define DCLS_COUNT_OF_SYMBOLS 7

typedef enum {
    dclsOpenBrackets,
    dclsComma,
    dclsCloseBrackets,
    dclsIdentifier,
    dclsInteger,
    dclsOther
} DeclarationsTerminal;

int GET_DCLS_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall);

#endif /* Declarations_h */
