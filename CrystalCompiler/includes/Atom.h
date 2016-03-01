#ifndef Atom_h
#define Atom_h

/*!
 
   @header Atom
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-10-28
 
 */

#include <stdio.h>
#include "SubAutomata.h"

#define ATOM_COUNT_OF_STATES 10
#define ATOM_COUNT_OF_SYMBOLS 12

typedef enum {
    atomTrue,
    atomFalse,
    atomOpenParenthesis,
    atomCloseParenthesis,
    atomOpenBrackets,
    atomCloseBrackets,
    atomDot,
    atomIdentifier,
    atomInteger,
    atomFloat,
    atomCharacter,
    atomOther
} AtomTerminal;

int GET_ATOM_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall);

#endif /* Atom_h */
