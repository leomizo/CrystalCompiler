#ifndef SemanticFunctions_h
#define SemanticFunctions_h

/*!
 
   @header SemanticFunctions
 
   This module implements all semantic functions that are called by the semantic analyzer.
   It also calls the code generator to generate code.
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-11-20
 
 */

#include <stdio.h>

#include "SymbolTable.h"
#include "CodeGenerator.h"
#include "LexicalAnalyzer.h"
#include "IntegerStack.h"
#include "OperandStack.h"
#include "OperatorStack.h"

// Type of situation which triggers the evalutation of the expression in the stack
typedef enum {
    eetEndOfExpression,     // Expression reached its end
    eetCloseParenthesis,    // A closing parenthesis was found
    eetComma,               // A comma was found in an expression list
    eetPrecedenceViolation  // The current operator has a lesser precedence than the one on the top of the stack
} ExpressionEvaluationTrigger;

// Stack operations
void pushSymbolTableToStack(SymbolTableId table);
void pushStructTypeToStack(int symbolIndex);
void pushPrimitiveTypeToStack(ReservedWord typeWord);

// Structs
void newStructDeclaration(int symbolIndex);
void endStructDeclaration();

// Functions
void newFunctionDeclaration(int symbolIndex);
void startRetrievingFunctionParametersAndVariables();
void endFunctionDeclaration();
void beginFunctionExecution();
void functionReturn();

// Variables and parameters
void newVariableDeclaration(int symbolIndex);
void addDimensionSize(int size);
void setSymbolSizeAndAddress();

// Main
void mainDeclaration();
void beginMainExecution();
void endMain();

// Expressions
void newOperator(Operator operator);
void newFunctionOperator(int functionIndex);
void newOperand(OperandType type, int value);
void newStringOperand(char* value);
void newOperandOrFunctionCall(int index);
void evaluateExpression(ExpressionEvaluationTrigger trigger);
void accessStructField(int symbolIndex);
void accessArrayDimension(int index);

// If
void newIfCommand();
void newElseCommand();
void endIfCommand();

// While
void newWhileCommand();
void whileTest();
void endWhileCommand();

// Free internal structures
void freeSemanticAnalyzerInternalStructures();

#endif /* SemanticFunctions_h */
