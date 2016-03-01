/*!
 
   OperatorStack.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-11-29
 
 */

#include "OperatorStack.h"

const int OPERATOR_PRECEDENCE[] = {
    INT32_MAX,
    INT32_MAX,
    10,
    10,
    10,
    9,
    9,
    8,
    7,
    6,
    5,
    5,
    4,
    4,
    4,
    4,
    3,
    3,
    2,
    1,
    0,
    -1
};

void pushOperatorToStack(OperatorStack** stack, Operator operator, int functionIndex) {
    
    OperatorStackNode* newNode = malloc(sizeof(OperatorStackNode));
    newNode->operator = operator;
    newNode->functionIndex = functionIndex;
    newNode->nextNode = *stack;
    *stack = newNode;
    
}

Operator popOperatorFromStack(OperatorStack** stack, int* functionIndex) {
    
    OperatorStackNode* poppedNode = *stack;
    
    Operator poppedOperator = poppedNode->operator;
    *functionIndex = poppedNode->functionIndex;
    *stack = (*stack)->nextNode;
    
    free(poppedNode);
    return poppedOperator;
    
}