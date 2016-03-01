#ifndef SemanticAnalyzer_h
#define SemanticAnalyzer_h

/*!
 
   @header SemanticAnalyzer
 
   The Semantic Analyzer implements semantic functions that are called
   by the Syntactic Analyzer as the pushdown automaton's transitions occur.
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-11-20
 
 */

#include <stdio.h>
#include "SubAutomata.h"

typedef int Terminal;

// Initializes the semantic analyzer with the provided output file name.
int initializeSemanticAnalyzer(const char* outputFilename, const char* sourceCodeFilename);

// Executes the semantic action associated to a transition of the pushdown automaton
void executeSemanticAction(SubAutomatonIdentifier originSubAutomaton, SubAutomatonState originState, Terminal terminal, Token* token);

// Executes the semantic action associated to a transition of the pushdown automaton
void executeSemanticActionAtTheEndOfSubAutomaton(SubAutomatonIdentifier currentSubAutomaton, SubAutomatonState finalState, SubAutomatonIdentifier returnSubAutomaton, SubAutomatonState returnState);

// Frees the semantic analyzer's associated memory blocks.
void freeSemanticAnalyzer();

#endif /* SemanticAnalyzer_h */
