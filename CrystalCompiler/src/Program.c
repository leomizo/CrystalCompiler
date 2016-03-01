/*!
 
   Program.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */

#include "Program.h"

/*!
 
 @const PRGM_NEXT_STATE
 
 @abstract Table indicating to which state the automaton must go given a current state and the value of the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. An error transition goes to a state represented by -1.
 
 */
const SubAutomatonState PRGM_NEXT_STATE[PRGM_COUNT_OF_STATES][PRGM_COUNT_OF_SYMBOLS] = {
    
    // "void"   "main"   "begin"   "end"   "struct"   "endstruct"   "("   ":"   ")"   "["   "]"   id   int   other
    {       1 ,      2 ,       2 ,     2 ,        3 ,           2 ,   2 ,   2 ,   2 ,   2 ,   2 ,  2 ,   2 ,     2  }, // State 0
    {      -1 ,      5 ,      -1 ,    -1 ,       -1 ,          -1 ,  -1 ,  -1 ,  -1 ,  -1 ,  -1 ,  4 ,  -1 ,    -1  }, // State 1
    {      -1 ,     -1 ,      -1 ,    -1 ,       -1 ,          -1 ,  -1 ,  -1 ,  -1 ,  -1 ,  -1 , 13 ,  -1 ,    -1  }, // State 2
    {      -1 ,     -1 ,      -1 ,    -1 ,       -1 ,          -1 ,  -1 ,  -1 ,  -1 ,  -1 ,  -1 ,  6 ,  -1 ,    -1  }, // State 3
    {      -1 ,     -1 ,      -1 ,    -1 ,       -1 ,          -1 ,  15 ,  -1 ,  -1 ,  -1 ,  -1 , -1 ,  -1 ,    -1  }, // State 4
    {      -1 ,     -1 ,      -1 ,    -1 ,       -1 ,          -1 ,  -1 ,   7 ,  -1 ,  -1 ,  -1 , -1 ,  -1 ,    -1  }, // State 5
    {      -1 ,     -1 ,      -1 ,    -1 ,       -1 ,          -1 ,  -1 ,   8 ,  -1 ,  -1 ,  -1 , -1 ,  -1 ,    -1  }, // State 6
    {       9 ,      9 ,      10 ,     9 ,        9 ,           9 ,   9 ,   9 ,   9 ,   9 ,   9 ,  9 ,   9 ,     9  }, // State 7
    {      11 ,     11 ,      11 ,    11 ,       11 ,          11 ,  11 ,  11 ,  11 ,  11 ,  11 , 11 ,  11 ,    11  }, // State 8
    {      -1 ,     -1 ,      10 ,    -1 ,       -1 ,          -1 ,  -1 ,  -1 ,  -1 ,  -1 ,  -1 , -1 ,  -1 ,    -1  }, // State 9
    {      10 ,     10 ,      10 ,    12 ,       10 ,          10 ,  10 ,  10 ,  10 ,  10 ,  10 , 10 ,  10 ,    10  }, // State 10
    {      -1 ,     -1 ,      -1 ,    -1 ,       -1 ,           0 ,  -1 ,  -1 ,  -1 ,  -1 ,  -1 , -1 ,  -1 ,    -1  }, // State 11
    {      12 ,     12 ,      12 ,    12 ,       12 ,          12 ,  12 ,  12 ,  12 ,  12 ,  12 , 12 ,  12 ,    12  }, // State 12
    {      -1 ,     -1 ,      -1 ,    -1 ,       -1 ,          -1 ,  15 ,  -1 ,  -1 ,  14 ,  -1 , -1 ,  -1 ,    -1  }, // State 13
    {      -1 ,     -1 ,      -1 ,    -1 ,       -1 ,          -1 ,  -1 ,  -1 ,  -1 ,  -1 ,  -1 , -1 ,  19 ,    -1  }, // State 14
    {      16 ,     16 ,      16 ,    16 ,       16 ,          16 ,  16 ,  16 ,  17 ,  16 ,  16 , 16 ,  16 ,    16  }, // State 15
    {      -1 ,     -1 ,      -1 ,    -1 ,       -1 ,          -1 ,  -1 ,  -1 ,  17 ,  -1 ,  -1 , -1 ,  -1 ,    -1  }, // State 16
    {      -1 ,     -1 ,      -1 ,    -1 ,       -1 ,          -1 ,  -1 ,  18 ,  -1 ,  -1 ,  -1 , -1 ,  -1 ,    -1  }, // State 17
    {      20 ,     20 ,      21 ,    20 ,       20 ,          20 ,  20 ,  20 ,  20 ,  20 ,  20 , 20 ,  20 ,    20  }, // State 18
    {      -1 ,     -1 ,      -1 ,    -1 ,       -1 ,          -1 ,  -1 ,  -1 ,  -1 ,  -1 ,  13 , -1 ,  -1 ,    -1  }, // State 19
    {      -1 ,     -1 ,      21 ,    -1 ,       -1 ,          -1 ,  -1 ,  -1 ,  -1 ,  -1 ,  -1 , -1 ,  -1 ,    -1  }, // State 20
    {      21 ,     21 ,      21 ,     0 ,       21 ,          21 ,  21 ,  21 ,  21 ,  21 ,  21 , 21 ,  21 ,    21  }  // State 21
    
};

/*!
 
 @const PRGM_SUB_AUTOMATON_CALL
 
 @abstract Table indicating if a sub automaton should be called given the current state and the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. If no sub automaton is to be token, this is expressed by the value "saiNONE". If the sub automaton reaches its end, this is represented by the value "saiFSTE".
 
 */
const SubAutomatonIdentifier PRGM_SUB_AUTOMATON_CALL[PRGM_COUNT_OF_STATES][PRGM_COUNT_OF_SYMBOLS] = {
    
    // "void"   "main"   "begin"  "end"    "struct" "endstruct"  "("      ":"      ")"      "["      "]"      id       int      other
    {  saiNONE, saiTYPE, saiTYPE, saiTYPE, saiNONE, saiTYPE,     saiTYPE, saiTYPE, saiTYPE, saiTYPE, saiTYPE, saiNONE, saiTYPE, saiTYPE }, // State 0
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 1
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 2
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 3
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 4
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 5
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 6
    {  saiDCLS, saiDCLS, saiNONE, saiDCLS, saiDCLS, saiDCLS,     saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS }, // State 7
    {  saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS,     saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS }, // State 8
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 9
    {  saiCMND, saiCMND, saiCMND, saiNONE, saiCMND, saiCMND,     saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND }, // State 10
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 11
    {  saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE,     saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE }, // State 12
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 13
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 14
    {  saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS,     saiDCLS, saiDCLS, saiNONE, saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS }, // State 15
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 16
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 17
    {  saiDCLS, saiDCLS, saiNONE, saiDCLS, saiDCLS, saiDCLS,     saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS, saiDCLS }, // State 18
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 19
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,     saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 20
    {  saiCMND, saiCMND, saiCMND, saiNONE, saiCMND, saiCMND,     saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND }  // State 21
    
};

int GET_PRGM_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall) {
    
    ProgramTerminal terminalIndex = prgmOther;
    
    switch (token->type) {
        case tokenTypeReservedWord:
            switch (token->value.intValue) {
                case rwVoid:      terminalIndex = prgmVoid; break;
                case rwMain:      terminalIndex = prgmMain; break;
                case rwBegin:     terminalIndex = prgmBegin; break;
                case rwEnd:       terminalIndex = prgmEnd; break;
                case rwStruct:    terminalIndex = prgmStruct; break;
                case rwEndstruct: terminalIndex = prgmEndstruct; break;
                default: break;
            }
            break;
        case tokenTypeSpecialSymbol:
            switch (token->value.charValue) {
                case '(': terminalIndex = prgmOpenParenthesis; break;
                case ':': terminalIndex = prgmColon; break;
                case ')': terminalIndex = prgmCloseParenthesis; break;
                case '[': terminalIndex = prgmOpenBrackets; break;
                case ']': terminalIndex = prgmCloseBrackets; break;
                default: break;
            }
            break;
        case tokenTypeIdentifier: terminalIndex = prgmIdentifier; break;
        case tokenTypeNumberInteger: terminalIndex = prgmInteger; break;
        default: break;
    }
    
    *nextState = PRGM_NEXT_STATE[currentState][terminalIndex];
    *subAutomatonCall = PRGM_SUB_AUTOMATON_CALL[currentState][terminalIndex];
    
    return terminalIndex;
    
}
