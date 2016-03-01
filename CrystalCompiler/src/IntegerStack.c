/*!
 
   IntegerStack.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-11-29
 
 */

#include "IntegerStack.h"

void pushIntegerToStack(IntegerStack** stack, int integer) {
    IntegerStackNode* newNode = malloc(sizeof(IntegerStackNode));
    newNode->integer = integer;
    newNode->nextNode = *stack;
    *stack = newNode;
}

void popIntegerFromStack(IntegerStack** stack, int* poppedInteger) {
    if (*stack != NULL) {
        IntegerStackNode* poppedNode = *stack;
        *poppedInteger = poppedNode->integer;
        *stack = poppedNode->nextNode;
        free(poppedNode);
    }
    else poppedInteger = NULL;
}
