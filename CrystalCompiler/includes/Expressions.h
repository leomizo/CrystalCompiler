#ifndef Expressions_h
#define Expressions_h

/*!
 
   @header Expressions
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-10-28
 
 */

#include <stdio.h>
#include "SubAutomata.h"

#define EXPS_COUNT_OF_STATES 2
#define EXPS_COUNT_OF_SYMBOLS 3

typedef enum {
    expsComma,
    expsString,
    expsOther
} ExpressionsTerminal;

int GET_EXPS_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall);

#endif /* Expressions_h */
