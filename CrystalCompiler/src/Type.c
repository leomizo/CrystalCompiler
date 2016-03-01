/*!
 
   Type.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */

#include "Type.h"

/*!
 
 @const TYPE_NEXT_STATE
 
 @abstract Table indicating to which state the automaton must go given a current state and the value of the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. An error transition goes to a state represented by -1.
 
 */
const SubAutomatonState TYPE_NEXT_STATE[TYPE_COUNT_OF_STATES][TYPE_COUNT_OF_SYMBOLS] = {
    
    // type   other
    {     1 ,    -1  }, // State 0
    {     1 ,     1  }  // State 1
    
};

/*!
 
 @const TYPE_SUB_AUTOMATON_CALL
 
 @abstract Table indicating if a sub automaton should be called given the current state and the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. If no sub automaton is to be token, this is expressed by the value "saiNONE". If the sub automaton reaches its end, this is represented by the value "saiFSTE".
 
 */
const SubAutomatonIdentifier TYPE_SUB_AUTOMATON_CALL[TYPE_COUNT_OF_STATES][TYPE_COUNT_OF_SYMBOLS] = {
    
    // type     other
    {  saiNONE, saiNONE }, // State 0
    {  saiFSTE, saiFSTE }  // State 1
    
};

int GET_TYPE_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall) {
    
    TypeTerminal terminalIndex = typeOther;
    
    if (token->type == tokenTypeReservedWord && token->value.intValue >= rwVoid && token->value.intValue <= rwString) terminalIndex = typeType;

    *nextState = TYPE_NEXT_STATE[currentState][terminalIndex];
    *subAutomatonCall = TYPE_SUB_AUTOMATON_CALL[currentState][terminalIndex];
    
    return terminalIndex;
    
}