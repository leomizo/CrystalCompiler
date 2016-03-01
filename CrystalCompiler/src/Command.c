/*!
 
   Command.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */

#include "Command.h"

/*!
 
 @const CMND_NEXT_STATE
 
 @abstract Table indicating to which state the automaton must go given a current state and the value of the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. An error transition goes to a state represented by -1.
 
 */
const SubAutomatonState CMND_NEXT_STATE[CMND_COUNT_OF_STATES][CMND_COUNT_OF_SYMBOLS] = {
    
    // "if"   "while"   "for"   "scan"   "print"   "return"   "endwhile"   "endfor"   "["   "]"   "("   ")"   ","   ";"   ":"   "."   int   id   other
    {     2,        3,      4,       5,        6,         7,           1,         1,    1,    1,    1,    1,    1,    1,    1,    1 ,   1,   1,      1  }, // State 0
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   -1,   -1,   -1,    8,   -1,   -1 ,  -1,  -1,     -1  }, // State 1
    {     8,        8,      8,       8,        8,         8,           8,         8,    8,    8,    8,    8,    8,    8,    8,    8 ,   8,   8,      8  }, // State 2
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   10,   -1,   -1,   -1,   -1,   -1 ,  -1,  -1,     -1  }, // State 3
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   18,   -1,   -1,   -1,   -1,   -1 ,  -1,  -1,     -1  }, // State 4
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   15,   -1,   -1,   -1,   -1,   -1 ,  -1,  -1,     -1  }, // State 5
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,    9,   -1,   -1,   -1,   -1,   -1 ,  -1,  -1,     -1  }, // State 6
    {     1,        1,      1,       1,        1,         1,           1,         1,    1,    1,    1,    1,    1,    8,    1,    1 ,   1,   1,      1  }, // State 7
    {     8,        8,      8,       8,        8,         8,           8,         8,    8,    8,    8,    8,    8,    8,    8,    8 ,   8,   8,      8  }, // State 8
    {    11,       11,     11,      11,       11,        11,          11,        11,   11,   11,   11,   11,   11,   11,   11,   11 ,  11,  11,     11  }, // State 9
    {    12,       12,     12,      12,       12,        12,          12,        12,   12,   12,   12,   12,   12,   12,   12,   12 ,  12,  12,     12  }, // State 10
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   -1,    1,   -1,   -1,   -1,   -1 ,  -1,  -1,     -1  }, // State 11
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   -1,   13,   -1,   -1,   -1,   -1 ,  -1,  -1,     -1  }, // State 12
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   -1,   -1,   -1,   -1,   14,   -1 ,  -1,  -1,     -1  }, // State 13
    {    14,       14,     14,      14,       14,        14,           8,        14,   14,   14,   14,   14,   14,   14,   14,   14 ,  14,  14,     14  }, // State 14
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 ,  -1,  16,     -1  }, // State 15
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   17,   -1,   -1,    1,   15,   -1,   -1,   15 ,  -1,  17,     -1  }, // State 16
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 ,  23,  -1,     -1  }, // State 17
    {    19,       19,     19,      19,       19,        19,          19,        19,   19,   19,   19,   19,   19,   19,   19,   19 ,  19,  19,     19  }, // State 18
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   -1 ,  -1,  -1,     -1  }, // State 19
    {    21,       21,     21,      21,       21,        21,          21,        21,   21,   21,   21,   21,   21,   21,   21,   21 ,  21,  21,     21  }, // State 20
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   -1,   -1,   -1,   22,   -1,   -1 ,  -1,  -1,     -1  }, // State 21
    {    24,       24,     24,      24,       24,        24,          24,        24,   24,   24,   24,   24,   24,   24,   24,   24 ,  24,  24,     24  }, // State 22
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   16,   -1,   -1,   -1,   -1,   -1,   -1 ,  -1,  -1,     -1  }, // State 23
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   -1,   25,   -1,   -1,   -1,   -1 ,  -1,  -1,     -1  }, // State 24
    {    -1,       -1,     -1,      -1,       -1,        -1,          -1,        -1,   -1,   -1,   -1,   -1,   -1,   -1,   26,   -1 ,  -1,  -1,     -1  }, // State 25
    {    26,       26,     26,      26,       26,        26,          26,         8,   26,   26,   26,   26,   26,   26,   26,   26 ,  26,  26,     26  }  // State 26
    
};

/*!
 
 @const CMND_SUB_AUTOMATON_CALL
 
 @abstract Table indicating if a sub automaton should be called given the current state and the input token.
 
 @discussion The lines in this table represent the current state, and the columns represent the value of the input token. If no sub automaton is to be token, this is expressed by the value "saiNONE". If the sub automaton reaches its end, this is represented by the value "saiFSTE".
 
 */
const SubAutomatonIdentifier CMND_SUB_AUTOMATON_CALL[CMND_COUNT_OF_STATES][CMND_COUNT_OF_SYMBOLS] = {
    
    // "if"     "while"  "for"   "scan"   "print"   "return" "endwhile" "endfor" "["      "]"      "("      ")"      ","      ";"      ":"      "."      int      id       other
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiATTR,   saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR }, // State 0
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 1
    {  saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY,   saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY, saiIFBY }, // State 2
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 3
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 4
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 5
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 6
    {  saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR,   saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiNONE, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR }, // State 7
    {  saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE,   saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE, saiFSTE }, // State 8
    {  saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS,   saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS, saiEXPS }, // State 9
    {  saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR,   saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR }, // State 10
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 11
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 12
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 13
    {  saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiNONE,   saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND }, // State 14
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 15
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 16
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 17
    {  saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR,   saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR }, // State 18
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 19
    {  saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR,   saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR, saiEXPR }, // State 20
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 21
    {  saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR,   saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR, saiATTR }, // State 22
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 23
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 24
    {  saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE,   saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE, saiNONE }, // State 25
    {  saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND,   saiNONE, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND, saiCMND }  // State 26
    
};

int GET_CMND_TRANSITION(SubAutomatonState currentState, Token* token, SubAutomatonState* nextState, SubAutomatonIdentifier* subAutomatonCall) {
    
    int terminalIndex = cmndOther;
    
    switch (token->type) {
        case tokenTypeReservedWord:
            switch (token->value.intValue) {
                case rwIf:       terminalIndex = cmndIf; break;
                case rwWhile:    terminalIndex = cmndWhile; break;
                case rwFor:      terminalIndex = cmndFor; break;
                case rwScan:     terminalIndex = cmndScan; break;
                case rwPrint:    terminalIndex = cmndPrint; break;
                case rwReturn:   terminalIndex = cmndReturn; break;
                case rwEndwhile: terminalIndex = cmndEndwhile; break;
                case rwEndfor:   terminalIndex = cmndEndfor; break;
                default: break;
            }
        case tokenTypeSpecialSymbol:
            switch (token->value.charValue) {
                case '[': terminalIndex = cmndOpenBrackets; break;
                case ']': terminalIndex = cmndCloseBrackets; break;
                case '(': terminalIndex = cmndOpenParenthesis; break;
                case ')': terminalIndex = cmndCloseParenthesis; break;
                case ',': terminalIndex = cmndComma; break;
                case ';': terminalIndex = cmndSemiColon; break;
                case ':': terminalIndex = cmndColon; break;
                case '.': terminalIndex = cmndDot; break;
                default: break;
            }
            break;
        case tokenTypeNumberInteger: terminalIndex = cmndInteger; break;
        case tokenTypeIdentifier:    terminalIndex = cmndIdentifier; break;
        default:break;
    }
    
    *nextState = CMND_NEXT_STATE[currentState][terminalIndex];
    *subAutomatonCall = CMND_SUB_AUTOMATON_CALL[currentState][terminalIndex];
    
    return terminalIndex;
    
}
