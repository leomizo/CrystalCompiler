/*!
 
   IfBody.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */
#include "IfBody.h"

/*!
 
 @const IFBY_NEXT_STATE
 
 @abstract Table indicating to which state the automaton must go given a current state and the value of the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. An error transition goes to a state represented by -1.
 
 */
const SubAutomatonState IFBY_NEXT_STATE[IFBY_COUNT_OF_STATES][IFBY_COUNT_OF_SYMBOLS] = {
    
    // "else"   "elsif"   "endif"   "("   ")"   ":"   other
    {      -1 ,      -1 ,      -1 ,   1 ,  -1 ,  -1 ,    -1  }, // State 0
    {       2 ,       2 ,       2 ,   2 ,   2 ,   2 ,     2  }, // State 1
    {      -1 ,      -1 ,      -1 ,  -1 ,   3 ,  -1 ,    -1  }, // State 2
    {      -1 ,      -1 ,      -1 ,  -1 ,  -1 ,   4 ,    -1  }, // State 3
    {       5 ,       7 ,       6 ,   4 ,   4 ,   4 ,     4  }, // State 4
    {      -1 ,      -1 ,      -1 ,  -1 ,  -1 ,   8 ,    -1  }, // State 5
    {       6 ,       6 ,       6 ,   6 ,   6 ,   6 ,     6  }, // State 6
    {       6 ,       6 ,       6 ,   6 ,   6 ,   6 ,     6  }, // State 7
    {       8 ,       8 ,       6 ,   8 ,   8 ,   8 ,     8  }, // State 8
    
};

/*!
 
 @const IFBY_SUB_AUTOMATON_CALL
 
 @abstract Table indicating if a sub automaton should be called given the current state and the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. If no sub automaton is to be token, this is expressed by the value "saiNONE". If the sub automaton reaches its end, this is represented by the value "saiFSTE".
 
 */
const SubAutomatonIdentifier IFBY_SUB_AUTOMATON_CALL[IFBY_COUNT_OF_STATES][IFBY_COUNT_OF_SYMBOLS] = {
    
    // "else"   "elsif"  "endif"  "("      ")"      ":"      other
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 0
    {  saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR }, // State 1
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 2
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 3
    {  saiNONE, saiNONE, saiNONE, saiCMND, saiCMND, saiCMND, saiCMND }, // State 4
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 5
    {  saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE }, // State 6
    {  saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY }, // State 7
    {  saiCMND, saiCMND, saiNONE, saiCMND, saiCMND, saiCMND, saiCMND }  // State 8
    
};

int GET_IFBY_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall) {
    
    IfBodyTerminal terminalIndex = ifbyOther;
    
    switch (token->type) {
        case tokenTypeReservedWord:
            switch (token->value.intValue) {
                case rwElse:  terminalIndex = ifbyElse; break;
                case rwElsif: terminalIndex = ifbyElsif; break;
                case rwEndif: terminalIndex = ifbyEndif; break;
                default: break;
            }
            break;
        case tokenTypeSpecialSymbol:
            switch (token->value.charValue) {
                case '(': terminalIndex = ifbyOpenParenthesis; break;
                case ')': terminalIndex = ifbyCloseParenthesis; break;
                case ':': terminalIndex = ifbyColon; break;
                default: break;
            }
            break;
        default: break;
    }
    
    *nextState = IFBY_NEXT_STATE[currentState][terminalIndex];
    *subAutomatonCall = IFBY_SUB_AUTOMATON_CALL[currentState][terminalIndex];
    
    return terminalIndex;
    
}