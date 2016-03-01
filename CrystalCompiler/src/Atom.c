/*!
 
   Atom.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */

#include "Atom.h"
#include "SubAutomata.h"

/*!
 
 @const ATOM_NEXT_STATE
 
 @abstract Table indicating to which state the automaton must go given a current state and the value of the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. An error transition goes to a state represented by -1.
 
 */
const SubAutomatonState ATOM_NEXT_STATE[ATOM_COUNT_OF_STATES][ATOM_COUNT_OF_SYMBOLS] = {
    
    // "true"  "false"  "("  ")"  "["  "]"  "."  id  int  float  char  other
    {       1,       1,   3,   1,   1,   1,   1,  2,   1,     1,    1,     1  }, // State 0
    {       1,       1,   1,   1,   1,   1,   1,  1,   1,     1,    1,     1  }, // State 1
    {       2,       2,   7,   2,   5,   2,   6,  2,   2,     2,    2,     2  }, // State 2
    {       4,       4,   4,   4,   4,   4,   4,  4,   4,     4,    4,     4  }, // State 3
    {      -1,      -1,  -1,   1,  -1,  -1,  -1, -1,  -1,    -1,   -1,    -1  }, // State 4
    {      -1,      -1,  -1,  -1,  -1,  -1,  -1, -1,   8,    -1,   -1,    -1  }, // State 5
    {      -1,      -1,  -1,  -1,  -1,  -1,  -1,  9,  -1,    -1,   -1,    -1  }, // State 6
    {       4,       4,   4,   4,   4,   4,   4,  4,   4,     4,    4,     4  }, // State 7
    {      -1,      -1,  -1,  -1,  -1,   9,  -1, -1,  -1,    -1,   -1,    -1  }, // State 8
    {       9,       9,   9,   9,   5,   9,   6,  9,   9,     9,    9,     9  }  // State 9
    
};

/*!
 
 @const ATOM_SUB_AUTOMATON_CALL
 
 @abstract Table indicating if a sub automaton should be called given the current state and the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. If no sub automaton is to be token, this is expressed by the value "saiNONE". If the sub automaton reaches its end, this is represented by the value "saiFSTE".
 
 */
const SubAutomatonIdentifier ATOM_SUB_AUTOMATON_CALL[ATOM_COUNT_OF_STATES][ATOM_COUNT_OF_SYMBOLS] = {
    
    // "true"   "false"  "("      ")"      "["      "]"      "."      id       int      float    char     other
    {  saiNONE, saiNONE, saiNONE, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiNONE, saiNONE, saiNONE, saiNONE, saiEXPR }, // State 0
    {  saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE }, // State 1
    {  saiFSTE, saiFSTE, saiNONE, saiFSTE, saiNONE, saiFSTE, saiNONE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE }, // State 2
    {  saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR }, // State 3
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 4
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 5
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 6
    {  saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS }, // State 7
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 8
    {  saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiNONE, saiFSTE, saiNONE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE }  // State 9
    
};

int GET_ATOM_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall) {
    
    AtomTerminal terminalIndex = atomOther;
    
    switch (token->type) {
        case tokenTypeReservedWord:
            switch (token->value.intValue) {
                case rwTrue:  terminalIndex = atomTrue; break;
                case rwFalse: terminalIndex = atomFalse; break;
                default: break;
            }
            break;
        case tokenTypeSpecialSymbol:
            switch (token->value.charValue) {
                case '(': terminalIndex = atomOpenParenthesis; break;
                case ')': terminalIndex = atomCloseParenthesis; break;
                case '[': terminalIndex = atomOpenBrackets; break;
                case ']': terminalIndex = atomCloseBrackets; break;
                case '.': terminalIndex = atomDot; break;
                default: break;
            }
            break;
        case tokenTypeIdentifier:    terminalIndex = atomIdentifier; break;
        case tokenTypeNumberInteger: terminalIndex = atomInteger; break;
        case tokenTypeNumberFloat:   terminalIndex = atomFloat; break;
        case tokenTypeCharacter:     terminalIndex = atomCharacter; break;
        default: break;
    }
    
    *nextState = ATOM_NEXT_STATE[currentState][terminalIndex];
    *subAutomatonCall = ATOM_SUB_AUTOMATON_CALL[currentState][terminalIndex];
    
    return terminalIndex;
    
}