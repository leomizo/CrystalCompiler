#ifndef OperandStack_h
#define OperandStack_h

/*!
 
   @header OperandStack
 
   A stack of operands.
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-11-29
 
 */

#include <stdio.h>
#include <stdlib.h>

// Operand type: defines how the operand is accessed
typedef enum {
    
    // Value = offset in activation record
    opdtVariable,
    opdtTemporary,
    
    // Value = literal value
    opdtInteger,
    opdtChar,
    opdtBoolean,
    
    // Value = string address in string buffer
    opdtString
    
} OperandType;

typedef struct {
    OperandType type;
    int operandSymbolType; // int, float, char, boolean, string
    int value;
} Operand;

// Operand stack node
typedef struct OperandStackNode {
    Operand operand;
    struct OperandStackNode *nextNode;
} OperandStackNode;

typedef OperandStackNode OperandStack;

// Push an operand to a stack
void pushOperandToStack(OperandStack** stack, Operand operand);

// Pop an operand from a stack
Operand popOperandFromStack(OperandStack** stack);

#endif /* OperandStack_h */
