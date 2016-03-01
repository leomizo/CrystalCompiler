/*!
 
   OperandStack.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-11-29
 
 */

#include "OperandStack.h"

void pushOperandToStack(OperandStack** stack, Operand operand) {
    
    OperandStackNode* newNode = malloc(sizeof(OperandStackNode));
    
    newNode->operand = operand;
    newNode->nextNode = *stack;
    
    *stack = newNode;
    
}

Operand popOperandFromStack(OperandStack** stack) {
    
    OperandStackNode* poppedNode = *stack;
    
    Operand poppedOperand = poppedNode->operand;
    *stack = (*stack)->nextNode;
    
    free(poppedNode);
    
    return poppedOperand;
    
}