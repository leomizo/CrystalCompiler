/*!
 
   ArrayLiteral.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */

#include "ArrayLiteral.h"

/*!
 
 @const ARYL_NEXT_STATE
 
 @abstract Table indicating to which state the automaton must go given a current state and the value of the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. An error transition goes to a state represented by -1.
 
 */
const SubAutomatonState ARYL_NEXT_STATE[ARYL_COUNT_OF_STATES][ARYL_COUNT_OF_SYMBOLS] = {
    
    // "["   "]"   ","   other
    {    2 ,   1 ,   1 ,     1  }, // State 0
    {    1 ,   1 ,   1 ,     1  }, // State 1
    {    3 ,   3 ,   3 ,     3  }, // State 2
    {   -1 ,   4 ,  -1 ,    -1  }, // State 3
    {    4 ,   4 ,   5 ,     4  }, // State 4
    {    2 ,  -1 ,  -1 ,    -1  }  // State 5
    
};

/*!
 
 @const ARYL_SUB_AUTOMATON_CALL
 
 @abstract Table indicating if a sub automaton should be called given the current state and the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. If no sub automaton is to be token, this is expressed by the value "saiNONE". If the sub automaton reaches its end, this is represented by the value "saiFSTE".
 
 */
const SubAutomatonIdentifier ARYL_SUB_AUTOMATON_CALL[ARYL_COUNT_OF_STATES][ARYL_COUNT_OF_SYMBOLS] = {
    
    // "["      "]"      ","      other
    {  saiNONE, saiEXPS, saiEXPS, saiEXPS }, // State 0
    {  saiFSTE, saiFSTE, saiFSTE, saiFSTE }, // State 1
    {  saiARYL, saiARYL, saiARYL, saiARYL }, // State 2
    {  saiNONE, saiNONE, saiNONE, saiNONE }, // State 3
    {  saiFSTE, saiFSTE, saiNONE, saiFSTE }, // State 4
    {  saiNONE, saiNONE, saiNONE, saiNONE }, // State 5
    
};

int GET_ARYL_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall) {
    
    ArrayLiteralTerminal terminalIndex = arylOther;
    
    if (token->type == tokenTypeSpecialSymbol) {
        switch (token->value.charValue) {
            case '[': terminalIndex = arylOpenBrackets; break;
            case ']': terminalIndex = arylCloseBrackets; break;
            case ',': terminalIndex = arylComma; break;
            default: break;
        }
    }
    
    *nextState = ARYL_NEXT_STATE[currentState][terminalIndex];
    *subAutomatonCall = ARYL_SUB_AUTOMATON_CALL[currentState][terminalIndex];
    
    return terminalIndex;
    
}
