#ifndef Command_h
#define Command_h

/*!
 
   @header Command
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-10-28
 
 */

#include <stdio.h>
#include "SubAutomata.h"

#define CMND_COUNT_OF_STATES 27
#define CMND_COUNT_OF_SYMBOLS 19

typedef enum {
    cmndIf,
    cmndWhile,
    cmndFor,
    cmndScan,
    cmndPrint,
    cmndReturn,
    cmndEndwhile,
    cmndEndfor,
    cmndOpenBrackets,
    cmndCloseBrackets,
    cmndOpenParenthesis,
    cmndCloseParenthesis,
    cmndComma,
    cmndSemiColon,
    cmndColon,
    cmndDot,
    cmndInteger,
    cmndIdentifier,
    cmndOther
} CommandTerminal;

int GET_CMND_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall);

#endif /* Command_h */
