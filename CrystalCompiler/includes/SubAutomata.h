#ifndef SubAutomata_h
#define SubAutomata_h

/*!
 
   @header SubAutomata
 
   This header file declares some important definitions for syntactic analyzers's sub-automata implementation.
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-10-28
 
 */

#include <stdio.h>
#include "Token.h"
#include "LexicalAnalyzer.h"

#define COUNT_OF_SUB_AUTOMATA 10

// The sub-automaton state is represented by a integer value.
typedef int SubAutomatonState;


// Each value represents a sub automaton.
// It is used to access the transition tables. It can indicate the current sub-automaton or a sub-automaton call.
typedef enum {
    saiNONE = -1, // No sub automaton to be called
    saiPRGM,      // Program: main sub automaton
    saiDCLS,      // Declarations
    saiCMND,      // Commands
    saiATTR,      // Attribution and function call
    saiARYL,      // Array literal
    saiIFBY,      // If body
    saiEXPS,      // List of expressions
    saiEXPR,      // Expression
    saiATOM,      // Atom in expression
    saiTYPE,      // Type of variable
    saiFSTE       // Final state: end of the current sub automaton, call return state
} SubAutomatonIdentifier;


#endif /* SubAutomata_h */
