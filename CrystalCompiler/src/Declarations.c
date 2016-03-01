/*!
 
   Declarations.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */

#include "Declarations.h"

/*!
 
 @const DCLS_NEXT_STATE
 
 @abstract Table indicating to which state the automaton must go given a current state and the value of the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. An error transition goes to a state represented by -1.
 
 */
const SubAutomatonState DCLS_NEXT_STATE[DCLS_COUNT_OF_STATES][DCLS_COUNT_OF_SYMBOLS] = {
    
    // "["   ","   "]"   id   int   other
    {    1 ,   1 ,   1 ,  1 ,   1 ,     1   }, // State 0
    {   -1 ,  -1 ,  -1 ,  2 ,  -1 ,    -1   }, // State 1
    {    3 ,   0 ,   2 ,  2 ,   2 ,     2   }, // State 2
    {   -1 ,  -1 ,  -1 , -1 ,   4 ,    -1   }, // State 3
    {   -1 ,  -1 ,   2 , -1 ,  -1 ,    -1   }  // State 4
    
};

/*!
 
 @const DCLS_SUB_AUTOMATON_CALL
 
 @abstract Table indicating if a sub automaton should be called given the current state and the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. If no sub automaton is to be token, this is expressed by the value "saiNONE". If the sub automaton reaches its end, this is represented by the value "saiFSTE".
 
 */
const SubAutomatonIdentifier DCLS_SUB_AUTOMATON_CALL[DCLS_COUNT_OF_STATES][DCLS_COUNT_OF_SYMBOLS] = {
    
    // "["      ","      "]"      id       int      other
    {  saiTYPE, saiTYPE, saiTYPE, saiNONE, saiTYPE, saiTYPE }, // State 0
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 1
    {  saiNONE, saiNONE, saiFSTE, saiFSTE, saiFSTE, saiFSTE }, // State 2
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 3
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }  // State 4
    
};

int GET_DCLS_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall) {
    
    DeclarationsTerminal terminalIndex = dclsOther;
    
    switch (token->type) {
        case tokenTypeSpecialSymbol:
            switch (token->value.charValue) {
                case '[': terminalIndex = dclsOpenBrackets; break;
                case ',': terminalIndex = dclsComma; break;
                case ']': terminalIndex = dclsCloseBrackets; break;
                default: break;
            }
            break;
        case tokenTypeIdentifier:    terminalIndex = dclsIdentifier; break;
        case tokenTypeNumberInteger: terminalIndex = dclsInteger; break;
        default:break;
    }
    
    *nextState = DCLS_NEXT_STATE[currentState][terminalIndex];
    *subAutomatonCall = DCLS_SUB_AUTOMATON_CALL[currentState][terminalIndex];
    
    return terminalIndex;
    
}