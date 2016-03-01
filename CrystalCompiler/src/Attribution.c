/*!
 
   Attribution.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */

#include "Attribution.h"

/*!
 
 @const ATTR_NEXT_STATE
 
 @abstract Table indicating to which state the automaton must go given a current state and the value of the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. An error transition goes to a state represented by -1.
 
 */
const SubAutomatonState ATTR_NEXT_STATE[ATTR_COUNT_OF_STATES][ATTR_COUNT_OF_SYMBOLS] = {
    
    // "("  ")"  "["   "]"   "="   "."   id   int   string   other
    {   -1,  -1,  -1 ,  -1 ,  -1 ,  -1 ,  1 ,  -1 ,     -1 ,    -1  }, // State 0
    {    5,  -1,   2 ,  -1 ,   4 ,   3 , -1 ,  -1 ,     -1 ,    -1  }, // State 1
    {   -1,  -1,  -1 ,  -1 ,  -1 ,  -1 , -1 ,   6 ,     -1 ,    -1  }, // State 2
    {   -1,  -1,  -1 ,  -1 ,  -1 ,  -1 ,  8 ,  -1 ,     -1 ,    -1  }, // State 3
    {    9,   9,  10 ,   9 ,   9 ,   9 ,  9 ,   9 ,      9 ,     9  }, // State 4
    {    7,   7,   7 ,   7 ,   7 ,   7 ,  7 ,   7 ,      7 ,     7  }, // State 5
    {   -1,  -1,  -1 ,   8 ,  -1 ,  -1 , -1 ,  -1 ,     -1 ,    -1  }, // State 6
    {   -1,   9,  -1 ,  -1 ,  -1 ,  -1 , -1 ,  -1 ,     -1 ,    -1  }, // State 7
    {   -1,  -1,   2 ,  -1 ,   4 ,   3 , -1 ,  -1 ,     -1 ,    -1  }, // State 8
    {    9,   9,   9 ,   9 ,   9 ,   9 ,  9 ,   9 ,      9 ,     9  }, // State 9
    {   11,  11,  11 ,  11 ,  11 ,  11 , 11 ,  11 ,     11 ,    11  }, // State 10
    {   -1,  -1,  -1 ,   9 ,  -1 ,  -1 , -1 ,  -1 ,     -1 ,    -1  }  // State 11
    
};

/*!
 
 @const ATTR_SUB_AUTOMATON_CALL
 
 @abstract Table indicating if a sub automaton should be called given the current state and the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. If no sub automaton is to be token, this is expressed by the value "saiNONE". If the sub automaton reaches its end, this is represented by the value "saiFSTE".
 
 */
const SubAutomatonIdentifier ATTR_SUB_AUTOMATON_CALL[ATTR_COUNT_OF_STATES][ATTR_COUNT_OF_SYMBOLS] = {
    
    // "("      ")"      "["      "]"      "="      "."      id       int      string   other
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE  }, // State 0
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE  }, // State 1
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE  }, // State 2
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE  }, // State 3
    {  saiEXPR, saiEXPR, saiNONE, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiNONE, saiEXPR  }, // State 4
    {  saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS  }, // State 5
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE  }, // State 6
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE  }, // State 7
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE  }, // State 8
    {  saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE  }, // State 9
    {  saiARYL, saiARYL, saiARYL, saiARYL, saiARYL, saiARYL, saiARYL, saiARYL, saiARYL, saiARYL  }, // State 10
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE  }  // State 11
    
};

int GET_ATTR_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall) {
    
    AttributionTerminal terminalIndex = attrOther;
    
    switch (token->type) {
        case tokenTypeSpecialSymbol:
            switch (token->value.charValue) {
                case '(': terminalIndex = attrOpenParenthesis; break;
                case ')': terminalIndex = attrCloseParenthesis; break;
                case '[': terminalIndex = attrOpenBrackets; break;
                case ']': terminalIndex = attrCloseBrackets; break;
                case '=': terminalIndex = attrEqualSign; break;
                case '.': terminalIndex = attrDot; break;
                default: break;
            }
            break;
        case tokenTypeIdentifier:    terminalIndex = attrIdentifier; break;
        case tokenTypeNumberInteger: terminalIndex = attrInteger; break;
        case tokenTypeString:        terminalIndex = attrString; break;
        default:break;
    }
    
    *nextState = ATTR_NEXT_STATE[currentState][terminalIndex];
    *subAutomatonCall = ATTR_SUB_AUTOMATON_CALL[currentState][terminalIndex];
    
    return terminalIndex;
    
}
