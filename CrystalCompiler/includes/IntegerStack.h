#ifndef IntegerStack_h
#define IntegerStack_h

/*!
 
   @header IntegerStack
 
   A stack of integers.
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-11-29
 
 */

#include <stdio.h>
#include <stdlib.h>

// Integer stack node
typedef struct IntegerStackNode {
    int integer;
    struct IntegerStackNode* nextNode;
} IntegerStackNode;

typedef IntegerStackNode IntegerStack;

// Push an integer to a stack
void pushIntegerToStack(IntegerStack** stack, int integer);

// Pop an integer from a stack
void popIntegerFromStack(IntegerStack** stack, int* poppedInteger);

#endif /* IntegerStack_h */
