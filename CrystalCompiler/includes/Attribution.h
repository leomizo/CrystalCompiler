#ifndef Attribution_h
#define Attribution_h

/*!
 
   @header Attribution
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-10-28
 
 */

#include <stdio.h>
#include "SubAutomata.h"

#define ATTR_COUNT_OF_STATES 12
#define ATTR_COUNT_OF_SYMBOLS 10

typedef enum {
    attrOpenParenthesis,
    attrCloseParenthesis,
    attrOpenBrackets,
    attrCloseBrackets,
    attrEqualSign,
    attrDot,
    attrIdentifier,
    attrInteger,
    attrString,
    attrOther
} AttributionTerminal;

int GET_ATTR_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall);

#endif /* Attribution_h */
