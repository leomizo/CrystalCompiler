/*!
 
   SemanticFunctions.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */


#include "SemanticFunctions.h"


// Variable and parameters counters
static int cumulativeAddress = 0;
static int temporaryVariablesCounter = 0;
static int mainSize = 0;

// Functions, parameters and variables stacks
static IntegerStack* symbolStack = NULL;
static IntegerStack* symbolTableStack = NULL;
static IntegerStack* typeStack = NULL;

// Expression evaluation stacks
static OperandStack* operandStack = NULL;
static OperatorStack* operatorStack = NULL;

// Expression evaluation variables
static int operatorFunctionIndex = -1;
static int operandSymbolIndex = -1;
static int operandSymbolTable = -1;
static int operandDimensionAccessCount = 0;


void pushSymbolTableToStack(SymbolTableId table) {
    pushIntegerToStack(&symbolTableStack, table);
}

void pushStructTypeToStack(int symbolIndex) {
    
    // Retrieve type symbol from the symbol table
    SymbolTableRow* typeSymbol = getSymbol(symbolIndex, symbolTableStack->integer);
    
    // Verify if struct is declared in the current scope
    if (typeSymbol->category == scStruct && typeSymbol->symbolTable) pushIntegerToStack(&typeStack, typeSymbol->type);
    else {
        
        // Lookup for struct declaration on the parent scope
        typeSymbol = lookupSymbol(typeSymbol->symbol, getSymbolTableParent(symbolTableStack->integer));
        
        // Push type to the stack if it is found
        if (typeSymbol->category == scStruct && typeSymbol->symbolTable) pushIntegerToStack(&typeStack, typeSymbol->type);
        else {
            
            // ERROR: STRUCT NOT DECLARED
            
        }
        
    }
    
}

void pushPrimitiveTypeToStack(ReservedWord typeWord) {
    switch (typeWord) {
        case rwBoolean: pushIntegerToStack(&typeStack, stBoolean); break;
        case rwChar: pushIntegerToStack(&typeStack, stChar); break;
        case rwInt: pushIntegerToStack(&typeStack, stInt); break;
        case rwFloat: pushIntegerToStack(&typeStack, stFloat); break;
        case rwString: pushIntegerToStack(&typeStack, stString); break;
        case rwVoid: pushIntegerToStack(&typeStack, stVoid); break;
        default:
            // ERROR: INVALID TYPE
            break;
    }

}

void newFunctionDeclaration(int symbolIndex) {
    
    // Retrieve function symbol from the symbol table
    SymbolTableRow* function = getSymbol(symbolIndex, symbolTableStack->integer);
    
    // Set function's return type and symbol's category
    int functionType;
    popIntegerFromStack(&typeStack, &functionType);
    function->category = scFunction;
    function->type = functionType;
    
    // Push function to the stack
    pushIntegerToStack(&symbolStack, symbolIndex);
    
    // Reset temporary variables counter
    temporaryVariablesCounter = -1;
    
}

void startRetrievingFunctionParametersAndVariables() {
    
    // Retrieve function symbol from the symbol table.
    SymbolTableRow* function = getSymbol(symbolStack->integer, symbolTableStack->integer);
    
    // Define return type size.
    if (function->type == stVoid) function->totalSize = 0;
    else if (function->type >= stStruct) function->totalSize = getSymbol(function->type - stStruct, symbolTableStack->integer)->totalSize;
    else function->totalSize = 1;
    
    // Multiply total size by dimension sizes, if any.
    if (function->dimensionSizes != NULL) {
        for (int i = 0; i < integerListLength(function->dimensionSizes); i++) {
            function->totalSize *= getIntegerFromList(function->dimensionSizes, i);
        }
    }
    
    // Create new symbol table for the declared function.
    function->symbolTable = initializeNewSymbolTable();
    
    // Push the newly created symbol table to the stack and notify lexical analyzer
    setSymbolTableParent(function->symbolTable, symbolTableStack->integer);
    pushIntegerToStack(&symbolTableStack, function->symbolTable);
    setLexicalAnalyzerSymbolTable(function->symbolTable);
    
    // Reset cumulative address (starts at two due to activation record structure)
    cumulativeAddress = 2;
    
}

void endFunctionDeclaration() {
    
    SymbolTableId functionSymbolTable;
    int functionSymbolIndex;
    SymbolTableRow* functionSymbol;
    
    // Pop function symbol and symbol table
    popIntegerFromStack(&symbolTableStack, &functionSymbolTable);
    popIntegerFromStack(&symbolStack, &functionSymbolIndex);
    
    // Retrieve function symbol
    functionSymbol = getSymbol(functionSymbolIndex, getSymbolTableParent(functionSymbolTable));
    
    // Generate function return
    generateFunctionEnd(functionSymbol->address);
    
    // Notify lexical analyzer
    setLexicalAnalyzerSymbolTable(symbolTableStack->integer);
    
}

void beginFunctionExecution() {
    
    // Retrieve function symbol from the symbol table
    SymbolTableRow* function = getSymbol(symbolStack->integer, getSymbolTableParent(symbolTableStack->integer));
    
    // Generate new function.
    function->address = generateFunctionDeclaration(function->symbol, cumulativeAddress);
    
    // Reset expression evaluation stacks
    operandStack = NULL;
    operatorStack = NULL;
    
}

void functionReturn() {
    
    Operand* returnValue = NULL;
    
    if (operandStack != NULL) {
        Operand poppedOperand = popOperandFromStack(&operandStack);
        returnValue = &poppedOperand;
    }
    
    generateFunctionReturn(returnValue);
    
}

void newStructDeclaration(int symbolIndex) {
    
    // Retrieve symbol.
    SymbolTableRow* structSymbol = getSymbol(symbolIndex, symbolTableStack->integer);
    
    // Set symbol category as struct.
    structSymbol->category = scStruct;
    structSymbol->type = symbolIndex + stStruct;
    
    // Push new symbol table
    structSymbol->symbolTable = initializeNewSymbolTable();
    setSymbolTableParent(structSymbol->symbolTable, symbolTableStack->integer);
    pushIntegerToStack(&symbolTableStack, structSymbol->symbolTable);
    setLexicalAnalyzerSymbolTable(structSymbol->symbolTable);
    
    // Push struct to the stack
    pushIntegerToStack(&symbolStack, symbolIndex);
    
    // Reset cumulative address
    cumulativeAddress = 0;
    
}

void endStructDeclaration() {
    
    SymbolTableId structSymbolTable;
    int structSymbolIndex;
    
    SymbolTableRow* structField;
    SymbolTableRow* structSymbol;
    
    // Pop symbol and symbol table from the stacks and notify lexical analyzer
    popIntegerFromStack(&symbolTableStack, &structSymbolTable);
    popIntegerFromStack(&symbolStack, &structSymbolIndex);
    setLexicalAnalyzerSymbolTable(symbolTableStack->integer);
    
    // Retrieve struct symbol and start calculating its size
    structSymbol = getSymbol(structSymbolIndex, symbolTableStack->integer);
    structSymbol->totalSize = 0;
    
    // Iterate over all struct fields to retrieve struct total size.
    for (structField = getSymbol(0, structSymbolTable); structField != NULL; structField = structField->nextRow) structSymbol->totalSize += structField->totalSize;
    
}

void newVariableDeclaration(int symbolIndex) {
    
    // Retrieve symbol.
    SymbolTableRow* symbol = getSymbol(symbolIndex, symbolTableStack->integer);
    
    // Set symbol category as variable.
    symbol->category = scVariable;
    
    // Set symbol type.
    popIntegerFromStack(&typeStack, &symbol->type);
    
    // Push symbol to stack.
    pushIntegerToStack(&symbolStack, symbolIndex);
    
    // Increment main size
    mainSize++;
    
}

// Array declration: Variable or parameter name already read, on the top of the stack
void addDimensionSize(int size) {
    
    // Retrieve the symbol on the top of the stack from the symbol table
    SymbolTableRow* symbol = getSymbol(symbolStack->integer, symbolTableStack->integer);
    
    // Add dimension size to the symbol
    pushIntegerToList(&symbol->dimensionSizes, size);
        
}

void setSymbolSizeAndAddress(int retrievingParameters) {
    
    // Retrieve symbol.
    int symbolIndex;
    popIntegerFromStack(&symbolStack, &symbolIndex);
    SymbolTableRow* symbol = getSymbol(symbolIndex, symbolTableStack->integer);
    
    // Set symbol address
    symbol->address = cumulativeAddress;
    
    // Define symbol total size
    if (symbol->type == stVoid) {} // Error: invalid symbol type
    
    // Primitive type, size = 1
    else if (symbol->type < stStruct) symbol->totalSize = 1;
    
    // Struct type, retrieve total size
    else {
        
        // Retrieve struct symbol and its total size.
        int structSymbolIndex = symbol->type - stStruct;
        SymbolTableRow* structSymbol = getSymbol(structSymbolIndex, symbolTableStack->integer);
        
        // If the struct is not found in the current scope, look for it in the parent scope
        if (structSymbol == NULL || structSymbol->category != scStruct) structSymbol = getSymbol(structSymbolIndex, getSymbolTableParent(symbolTableStack->integer));
        
        // Indicate struct total size and symbol table
        symbol->totalSize = structSymbol->totalSize;
        symbol->symbolTable = structSymbol->symbolTable;
        
    }
    
    // Multiply total size by array dimensions, if any.
    if (symbol->dimensionSizes != NULL) {
        for (int i = 0; i < integerListLength(symbol->dimensionSizes); i++) {
            symbol->totalSize *= getIntegerFromList(symbol->dimensionSizes, i);
        }
    }
    
    // Define parameter category if symbol is a parameter
    if (retrievingParameters) {
        
        // Retrieve function symbol
        SymbolTableRow* functionSymbol = getSymbol(symbolStack->integer, 0);
        
        // Update dimension sizes
        if (functionSymbol->dimensionSizes == NULL) pushIntegerToList(&functionSymbol->dimensionSizes, 1);
        else functionSymbol->dimensionSizes->Integer++;
        
        symbol->category = scParameter;
        
    }
    
    // Update cumulative address
    cumulativeAddress += symbol->totalSize;
    
}

void mainDeclaration() {
    // Reset counters
    cumulativeAddress = 2;
    temporaryVariablesCounter = -1;
    mainSize = 0;
}

void beginMainExecution() {

    // Generate main
    generateMain();
    
    // Reset expression evaluation stacks
    operandStack = NULL;
    operatorStack = NULL;
    
}

void endMain() {
    generateMainEnd(mainSize);
}

void evaluateNextOperation() {
    
    Operator operator = popOperatorFromStack(&operatorStack, &operatorFunctionIndex);
    
    if (operator != oprOpenParenthesis && operator != oprComma) {
        
        // Function call
        if (operator == oprFunctionCall) {
            
            SymbolTableRow* functionSymbol = getSymbol(operatorFunctionIndex, 0);
            SymbolTableId functionSymbolTable = functionSymbol->symbolTable;
            
            int parameterCount = functionSymbol->dimensionSizes->Integer;
            
            OperandStack* parameters = NULL;
            
            // Reverse parameters order
            for (int i = 0; i < parameterCount; i++) {
                pushOperandToStack(&parameters, popOperandFromStack(&operandStack));
            }
            
            int parameterId = 0;
            SymbolTableRow* parameterSymbol = getSymbol(parameterId, functionSymbolTable);
            
            while (parameterSymbol->category == scParameter) {
                Operand parameter = popOperandFromStack(&parameters);
                generatePassingParameter(parameter, parameterSymbol->address, parameterSymbol->totalSize);
                parameterId++;
                parameterSymbol = getSymbol(parameterId, functionSymbolTable);
            }
            
            temporaryVariablesCounter++;
            
            generateFunctionCall(functionSymbol->address, cumulativeAddress + temporaryVariablesCounter, functionSymbol->totalSize);
            
            generateNewTemporaryVariable();
            
            Operand result = { opdtTemporary, functionSymbol->type, cumulativeAddress + temporaryVariablesCounter };
            pushOperandToStack(&operandStack, result);

        }
        // Scan
        else if (operator == oprScan) {
        
            OperandStack* inputVariables = NULL;
        
            while (operandStack != NULL) pushOperandToStack(&inputVariables, popOperandFromStack(&operandStack));
            
            while (inputVariables != NULL) {
                Operand input = popOperandFromStack(&inputVariables);
                if (input.operandSymbolType == stString) {
                    generateStringScan(input.value);
                } else if (input.operandSymbolType == stInt) {
                    generateIntScan(input.value);
                } else if (input.operandSymbolType == stBoolean) {
                    generateBooleanScan(input.value);
                }
                else {
                    // Error: invalid type
                }
            }
            
        }
        // Print
        else if (operator == oprPrint) {
            
            OperandStack* outputOperands = NULL;
            
            while (operandStack != NULL) pushOperandToStack(&outputOperands, popOperandFromStack(&operandStack));
            
            while (outputOperands != NULL) {
                Operand output = popOperandFromStack(&outputOperands);
                if (output.type == opdtString || output.operandSymbolType == stString) generateStringPrint(output);
                else if (output.type == opdtInteger || output.operandSymbolType == stInt) generateIntPrint(output);
                else if (output.type == opdtBoolean || output.operandSymbolType == stBoolean) generateBooleanPrint(output);
            }
            
        }
        // Operations
        else {
            
            Operand leftOperand;
            Operand rightOperand = popOperandFromStack(&operandStack);
            
            // Attribution
            if (operator == oprEqualSign) {
                
                // Pop left operand
                leftOperand = popOperandFromStack(&operandStack);
                
                if (leftOperand.type == opdtVariable) generateAttribution(leftOperand, rightOperand);
                else {
                    // Error: invalid operation
                }
                
            } else {
                
                int resultType = -1;
                
                // Binary operators
                if (operator > oprMinus) {
                    
                    // Pop left operand
                    leftOperand = popOperandFromStack(&operandStack);
                    
                    // If both operands are not temporary variables, create a new temporary variable
                    temporaryVariablesCounter++;
                    if (leftOperand.type != opdtTemporary && rightOperand.type != opdtTemporary) generateNewTemporaryVariable();
                    else temporaryVariablesCounter --;
                    
                    // Generate code
                    switch (operator) {
                            
                        case oprAdd:
                        case oprSubtract:
                        case oprMultiply:
                        case oprDivide:
                            generateArithmeticOperation(operator, leftOperand, rightOperand,  cumulativeAddress + temporaryVariablesCounter);
                            resultType = stInt;
                            break;
                            
                            
                        case oprSmallerThan:
                        case oprSmallerOrEqualThan:
                        case oprBiggerThan:
                        case oprBiggerOrEqualThan:
                            generateRelationalComparison(operator, leftOperand, rightOperand, cumulativeAddress + temporaryVariablesCounter);
                            resultType = stBoolean;
                            break;
                            
                        case oprLogicAnd:
                        case oprLogicOr:
                            generateLogicalOperation(operator, leftOperand, rightOperand, cumulativeAddress + temporaryVariablesCounter);
                            resultType = stBoolean;
                            break;
                            
                        default:
                            // Error: invalid operator
                            break;
                            
                    }
                    
                }
                // Unary operators
                else {
                    
                    // If operand is not a temporary variable, create a new temporary variable.
                    temporaryVariablesCounter++;
                    if (rightOperand.type != opdtTemporary) generateNewTemporaryVariable();
                    else temporaryVariablesCounter --;
                    
                    // Generate code
                    switch (operator) {
                            
                        default:
                            // Error: invalid operator
                            break;
                            
                    }
                    
                }
                
                // Push result operand to stack
                Operand result = { opdtTemporary, resultType, cumulativeAddress + temporaryVariablesCounter };
                pushOperandToStack(&operandStack, result);
                
            }
            
        }
        
    }

}

void evaluateExpression(ExpressionEvaluationTrigger trigger) {
    
    switch (trigger) {
        // End of expression: evaluate everything in the stacks
        case eetEndOfExpression:
            while (operatorStack != NULL) evaluateNextOperation(); break;
        
        // Comma: Expression in an expression list, evaluate until an open parenthesis or comma is found, or the stack is empty
        case eetComma:
            while (operatorStack != NULL && operatorStack->operator != oprOpenParenthesis && operatorStack->operator != oprComma) evaluateNextOperation();
            pushOperatorToStack(&operatorStack, oprComma, -1);
            break;
            
        // Close parenthesis: Evaluate until the respective open parenthesis is found
        case eetCloseParenthesis:
            while (operatorStack != NULL && operatorStack->operator != oprOpenParenthesis) evaluateNextOperation();
            if (operatorStack != NULL && operatorStack->operator == oprOpenParenthesis) popOperatorFromStack(&operatorStack, &operatorFunctionIndex);
            break;
            
        // Precedence violation: Evaluate until an open parenthesis is found
        case eetPrecedenceViolation:
            while (operatorStack != NULL && operatorStack->operator != oprOpenParenthesis) evaluateNextOperation(); break;
    }
    
}

// Push an operator to the stack
void newOperator(Operator operator) {
    
    if (operator == oprEqualSign) {
        
        if (operatorStack != NULL) {
            // Equal sign: if it comes after ! or > or <, push the right operator
            switch (operatorStack->operator) {
                case oprEqualSign:
                    popOperatorFromStack(&operatorStack, &operatorFunctionIndex);
                    pushOperatorToStack(&operatorStack, oprEquals, -1);
                    break;
                case oprBiggerThan:
                    popOperatorFromStack(&operatorStack, &operatorFunctionIndex);
                    pushOperatorToStack(&operatorStack, oprBiggerOrEqualThan, -1);
                    break;
                case oprSmallerThan:
                    popOperatorFromStack(&operatorStack, &operatorFunctionIndex);
                    pushOperatorToStack(&operatorStack, oprSmallerOrEqualThan, -1);
                    break;
                case oprDifferent: break;
                default: pushOperatorToStack(&operatorStack, operator, -1); break;
            }
        }
        // Otherwise, just push it to the stack
        else pushOperatorToStack(&operatorStack, operator, -1);
        
    } else {
        
        // Close parenthesis found, evaluate expression
        if (operator == oprCloseParenthesis) evaluateExpression(eetCloseParenthesis);
        else {
            
            // Verify precedence rule
            if (operatorStack != NULL && operatorStack->operator != oprOpenParenthesis && operatorStack->operator != oprEqualSign && OPERATOR_PRECEDENCE[operator] < OPERATOR_PRECEDENCE[operatorStack->operator]) evaluateExpression(eetPrecedenceViolation);
            
            // Push operator to the stack
            pushOperatorToStack(&operatorStack, operator, -1);
            
        }
        
    }
}

// Push a function call operator to the stack
void newFunctionOperator(int functionIndex) {
    
    // Verify if the current symbol table is the base one
    if (symbolTableStack->integer != 0) {
        // Retrieve function symbol and its id, push operator to the stack
        SymbolTableRow* functionSymbol = getSymbol(functionIndex, symbolTableStack->integer);
        functionSymbol = lookupSymbol(functionSymbol->symbol, 0);
        pushOperatorToStack(&operatorStack, oprFunctionCall, functionSymbol->id);
    } else pushOperatorToStack(&operatorStack, oprFunctionCall, functionIndex);
    
}

void newOperandOrFunctionCall(int index) {
    
    // Retrieve symbol
    SymbolTableRow* functionSymbol = getSymbol(index, symbolTableStack->integer);
    
    // Determine if it is a function call or a variable
    if (functionSymbol->category == scVariable || functionSymbol->category == scParameter) newOperand(opdtVariable, index);
    else newFunctionOperator(index);
    
}

// Push an operand to the stack
void newOperand(OperandType type, int value) {
    
    Operand operand;
    operand.type = type;
    
    if (type == opdtVariable) {
        SymbolTableRow* symbol = getSymbol(value, symbolTableStack->integer);
        operand.value = symbol->address;
        operand.operandSymbolType = symbol->type;
        operandSymbolIndex = value;
        operandSymbolTable = symbolTableStack->integer;
        operandDimensionAccessCount = 0;
    } else {
        operand.value = value;
        operand.operandSymbolType = -1;
    }
    
    pushOperandToStack(&operandStack, operand);
    
}

// New string operand (string literal
void newStringOperand(char* value) {
    
    Operand operand;
    operand.type = opdtString;
    
    if (strcmp(value, "\"\\n\"") == 0) operand.value = -1;
    else operand.value = generateStringLiteral(value);
    
    pushOperandToStack(&operandStack, operand);
    
}

void accessStructField(int symbolIndex) {
    
    SymbolTableRow* fieldSymbol = getSymbol(symbolIndex, symbolTableStack->integer);
    SymbolTableRow* variableSymbol = getSymbol(operandSymbolIndex, operandSymbolTable);
    
    fieldSymbol = lookupSymbol(fieldSymbol->symbol, variableSymbol->symbolTable);
    
    operandStack->operand.value += fieldSymbol->address;
    operandSymbolIndex = fieldSymbol->id;
    operandSymbolTable = variableSymbol->symbolTable;
    operandDimensionAccessCount = 0;
    
}

void accessArrayDimension(int index) {
    
    int cumulativePosition = index;
    SymbolTableRow* variableSymbol = getSymbol(operandSymbolIndex, operandSymbolTable);
    for (int i = operandDimensionAccessCount + 1; i < integerListLength(variableSymbol->dimensionSizes); i++) {
        cumulativePosition *= getIntegerFromList(variableSymbol->dimensionSizes, i);
    }
    if (variableSymbol->type > scStruct) {
        SymbolTableRow* structSymbol = getSymbol(variableSymbol->type - stStruct, 0);
        cumulativePosition *= structSymbol->totalSize;
    }
    operandStack->operand.value += cumulativePosition;
    operandDimensionAccessCount++;
    
}


// IF

void newIfCommand() {
    Operand condition = popOperandFromStack(&operandStack);
    generateIfCommand(condition);
}

void newElseCommand() {
    generateElseCommand();
}

void endIfCommand() {
    generateEndIf();
}

// WHILE

void newWhileCommand() {
    generateWhileCommand();
}

void whileTest() {
    Operand condition = popOperandFromStack(&operandStack);
    generateWhileTest(condition);
}

void endWhileCommand() {
    generateEndWhile();
}

void freeSemanticAnalyzerInternalStructures() {
    if (symbolStack != NULL) free(symbolStack);
    if (symbolTableStack != NULL) free(symbolTableStack);
    if (typeStack != NULL) free(typeStack);
    if (operandStack != NULL) free(operandStack);
    if (operatorStack != NULL) free(operatorStack);
}





