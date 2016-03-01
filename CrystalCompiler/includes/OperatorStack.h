#ifndef OperatorStack_h
#define OperatorStack_h

/*!
 
   @header OperatorStack
 
   A stack of operators.
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-11-29
 
 */

#include <stdio.h>
#include <stdlib.h>

// Enumeration of operators
typedef enum {
    oprEqualSign,
    oprOpenParenthesis,
    oprScan,
    oprPrint,
    oprFunctionCall,
    oprNot,
    oprMinus,
    oprModulus,
    oprDivide,
    oprMultiply,
    oprSubtract,
    oprAdd,
    oprSmallerOrEqualThan,
    oprSmallerThan,
    oprBiggerOrEqualThan,
    oprBiggerThan,
    oprEquals,
    oprDifferent,
    oprLogicAnd,
    oprLogicOr,
    oprComma,
    oprCloseParenthesis
} Operator;

// Operator stack node
typedef struct OperatorStackNode {
    Operator operator;
    int functionIndex;  // Code generator reference
    struct OperatorStackNode *nextNode;
} OperatorStackNode;

typedef OperatorStackNode OperatorStack;

// Operators precedence array
extern const int OPERATOR_PRECEDENCE[];

// Push an operator to a stack
void pushOperatorToStack(OperatorStack** stack, Operator operator, int functionIndex);

// Pop an operator from a stack
Operator popOperatorFromStack(OperatorStack** stack, int* functionIndex);

#endif /* OperatorStack_h */
