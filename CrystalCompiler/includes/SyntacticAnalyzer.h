#ifndef SyntacticAnalyzer_h
#define SyntacticAnalyzer_h

/*!
 
   @header SyntacticAnalyzer
 
   The Syntactic Analyzer implements a pushdown automaton which recognizes the language
   by invoking the lexical analyzer to retrieve tokens and calls the semantic actions
   in the semantic analyzer.
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-11-19
 
 */

#include <stdio.h>

// Compiles the source code (Crystal code) to the output file (MVN Assembly).
void compile(const char* inputFile, const char* outputFile);

#endif /* SyntacticAnalyzer_h */
