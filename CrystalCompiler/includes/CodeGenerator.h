#ifndef CodeGenerator_h
#define CodeGenerator_h

/*!
 
   @header CodeGenerator
 
   This module is called by the semantic functions, and translate the
   commands into MVN code.
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-11-20
 
 */

#include <stdio.h>
#include "OperandStack.h"
#include "OperatorStack.h"

// Initialize
int initializeCodeGenerator(const char* outputFilename, const char* sourceCodeFilename);

// Functions
int generateFunctionDeclaration(char* functionName, int activationRecordSize);
void generateFunctionEnd(int functionAddress);

// Variables
void generateReadingVariable(int offset);
void generateNewTemporaryVariable();

// Operations
void generateAttribution(Operand leftOperand, Operand rightOperand);
void generateArithmeticOperation(Operator operation, Operand leftOperand, Operand rightOperand, int resultAddressOffset);
void generateRelationalComparison(Operator comparison, Operand leftOperand, Operand rightOperand, int resultAddressOffset);
void generateLogicalOperation(Operator operation, Operand leftOperand, Operand rightOperand, int resultAddressOffset);

// If
void generateIfCommand(Operand condition);
void generateElseCommand();
void generateEndIf();

// While
void generateWhileCommand();
void generateWhileTest(Operand condition);
void generateEndWhile();

// Function call
void generateFunctionReturn(Operand* returnOperand);
void generatePassingParameter(Operand parameter, int address, int size);
void generateFunctionCall(int functionAddress, int resultAddressOffset, int returnValueSize);

// Main
void generateMain();
void generateMainEnd(int mainSize);

// Scan
void generateStringScan(int resultAddressOffset);
void generateIntScan(int resultAddressOffset);
void generateBooleanScan(int resultAddressOffset);

// Print
void generateStringPrint(Operand string);
void generateBreakLinePrint();
void generateIntPrint(Operand integer);
void generateBooleanPrint(Operand boolean);

// String
int generateStringLiteral(char* string);

#endif /* CodeGenerator_h */
