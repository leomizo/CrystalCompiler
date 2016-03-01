#ifndef Program_h
#define Program_h

/*!
 
   @header Program
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-10-28
 
 */

#include <stdio.h>
#include "SubAutomata.h"

#define PRGM_COUNT_OF_STATES 22
#define PRGM_COUNT_OF_SYMBOLS 14

typedef enum {
    prgmVoid,
    prgmMain,
    prgmBegin,
    prgmEnd,
    prgmStruct,
    prgmEndstruct,
    prgmOpenParenthesis,
    prgmColon,
    prgmCloseParenthesis,
    prgmOpenBrackets,
    prgmCloseBrackets,
    prgmIdentifier,
    prgmInteger,
    prgmOther
} ProgramTerminal;

int GET_PRGM_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall);

#endif /* Program_h */
