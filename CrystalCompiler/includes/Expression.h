#ifndef Expression_h
#define Expression_h

/*!
 
   @header Expressions
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-10-28
 
 */

#include <stdio.h>
#include "SubAutomata.h"

#define EXPR_COUNT_OF_STATES 8
#define EXPR_COUNT_OF_SYMBOLS 13

typedef enum {
    exprNot,
    exprPlusSign,
    exprMinusSign,
    exprAsterisk,
    exprSlash,
    exprPercentageSign,
    exprExclamationSign,
    exprEqualSign,
    exprAnd,
    exprOr,
    exprBiggerThan,
    exprSmallerThan,
    exprOther
} ExpressionTerminal;

int GET_EXPR_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall);

#endif /* Expression_h */
