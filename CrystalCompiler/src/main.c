/*!
 
   main.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */

#include <stdio.h>
#include "SyntacticAnalyzer.h"

int main(int argc, const char * argv[]) {
    
    // Verify if both input and output files have been provided.
    if (argc < 2) return -1;
    
    // Compile the source code (first argument) to output file (second argument).
    compile(argv[1], argv[2]);
    
    return 0;
    
}
