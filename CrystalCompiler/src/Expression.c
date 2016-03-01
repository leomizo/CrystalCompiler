/*!
 
   Expression.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */

#include "Expression.h"

/*!
 
 @const EXPR_NEXT_STATE
 
 @abstract Table indicating to which state the automaton must go given a current state and the value of the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. An error transition goes to a state represented by -1.
 
 */
const SubAutomatonState EXPR_NEXT_STATE[EXPR_COUNT_OF_STATES][EXPR_COUNT_OF_SYMBOLS] = {
    
    // "not"  "+"  "-"  "*"  "/"  "%"  "!"  "="  "&"  "|"  ">"  "<"  other
    {      2,   1,   2,   1,   1,   1,   1,   1,   1,   1,   1,   1,     1  }, // State 0
    {     -1,   2,   2,   2,   2,   2,   5,   5,   3,   4,   6,   6,     1  }, // State 1
    {      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,     7  }, // State 2
    {     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   2,  -1,  -1,  -1,    -1  }, // State 3
    {     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   2,  -1,  -1,    -1  }, // State 4
    {     -1,  -1,  -1,  -1,  -1,  -1,  -1,   2,  -1,  -1,  -1,  -1,    -1  }, // State 5
    {      7,   7,   7,   7,   7,   7,   7,   2,   7,   7,   7,   7,     7  }, // State 6
    {      7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,     7  }  // State 7
    
};

/*!
 
 @const EXPR_SUB_AUTOMATON_CALL
 
 @abstract Table indicating if a sub automaton should be called given the current state and the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. If no sub automaton is to be token, this is expressed by the value "saiNONE". If the sub automaton reaches its end, this is represented by the value "saiFSTE".
 
 */
const SubAutomatonIdentifier EXPR_SUB_AUTOMATON_CALL[EXPR_COUNT_OF_STATES][EXPR_COUNT_OF_SYMBOLS] = {
    
    // "not"    "+"      "-"      "*"      "/"      "%"      "!"      "="      "&"      "|"      ">"      "<"      other
    {  saiNONE, saiATOM, saiNONE, saiATOM, saiATOM, saiATOM, saiATOM, saiATOM, saiATOM, saiATOM, saiATOM, saiATOM, saiATOM }, // State 0
    {  saiFSTE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiFSTE }, // State 1
    {  saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR }, // State 2
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 3
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 4
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 5
    {  saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiNONE, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR }, // State 6
    {  saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE }  // State 7
    
};

int GET_EXPR_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall) {
    
    ExpressionTerminal terminalIndex = exprOther;
    
    if (token->type == tokenTypeReservedWord && token->value.intValue == rwNot) terminalIndex = exprNot;
    else if (token->type == tokenTypeSpecialSymbol) {
        switch (token->value.charValue) {
            case '+': terminalIndex = exprPlusSign; break;
            case '-': terminalIndex = exprMinusSign; break;
            case '*': terminalIndex = exprAsterisk; break;
            case '/': terminalIndex = exprSlash; break;
            case '%': terminalIndex = exprPercentageSign; break;
            case '!': terminalIndex = exprExclamationSign; break;
            case '=': terminalIndex = exprEqualSign; break;
            case '&': terminalIndex = exprAnd; break;
            case '|': terminalIndex = exprOr; break;
            case '>': terminalIndex = exprBiggerThan; break;
            case '<': terminalIndex = exprSmallerThan; break;
            default: break;
        }
    }
    
    *nextState = EXPR_NEXT_STATE[currentState][terminalIndex];
    *subAutomatonCall = EXPR_SUB_AUTOMATON_CALL[currentState][terminalIndex];
    
    return terminalIndex;
    
}