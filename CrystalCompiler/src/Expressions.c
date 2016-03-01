/*!
 
   Expressions.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */

#include "Expressions.h"

/*!
 
 @const EXPS_NEXT_STATE
 
 @abstract Table indicating to which state the automaton must go given a current state and the value of the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. An error transition goes to a state represented by -1.
 
 */
const SubAutomatonState EXPS_NEXT_STATE[EXPS_COUNT_OF_STATES][EXPS_COUNT_OF_SYMBOLS] = {
    
    // ","  string  other
    {    1,      1,     1  }, // State 0
    {    0,      1,     1  }  // State 1
    
};

/*!
 
 @const EXPS_SUB_AUTOMATON_CALL
 
 @abstract Table indicating if a sub automaton should be called given the current state and the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. If no sub automaton is to be token, this is expressed by the value "saiNONE". If the sub automaton reaches its end, this is represented by the value "saiFSTE".
 
 */
const SubAutomatonIdentifier EXPS_SUB_AUTOMATON_CALL[EXPS_COUNT_OF_STATES][EXPS_COUNT_OF_SYMBOLS] = {
    
    // ","      string   other
    {  saiEXPR, saiNONE, saiEXPR }, // State 0
    {  saiNONE, saiFSTE, saiFSTE }  // State 1
    
};

int GET_EXPS_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall) {
    
    ExpressionsTerminal terminalIndex = expsOther;
    
    if (token->type == tokenTypeSpecialSymbol && token->value.charValue == ',') terminalIndex = expsComma;
    else if (token->type == tokenTypeString) terminalIndex = expsString;
    
    *nextState = EXPS_NEXT_STATE[currentState][terminalIndex];
    *subAutomatonCall = EXPS_SUB_AUTOMATON_CALL[currentState][terminalIndex];
    
    return terminalIndex;
    
}