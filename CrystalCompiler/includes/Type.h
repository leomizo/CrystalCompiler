#ifndef Type_h
#define Type_h

/*!
 
   @header Type
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-10-28
 
 */

#include <stdio.h>
#include "SubAutomata.h"

#define TYPE_COUNT_OF_STATES 2
#define TYPE_COUNT_OF_SYMBOLS 2

typedef enum {
    typeType,
    typeOther
} TypeTerminal;

int GET_TYPE_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall);

#endif /* Type_h */
