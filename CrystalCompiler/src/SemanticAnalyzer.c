/*!
 
   SemanticAnalyzer.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-10-28
 
 */

#include "SemanticAnalyzer.h"
#include "SemanticFunctions.h"

#include "ArrayLiteral.h"
#include "Atom.h"
#include "Attribution.h"
#include "Command.h"
#include "Declarations.h"
#include "Expression.h"
#include "Expressions.h"
#include "IfBody.h"
#include "Program.h"
#include "Type.h"


// 1 When retrieving parameters when parsing a function, 0 otherwise
static int retrievingFunctionParameters = 0;

int initializeSemanticAnalyzer(const char* outputFilename, const char* sourceCodeFilename) {
    
    // Initialize base symbol table and notify lexical analyzer
    SymbolTableId staticSymbolTable = initializeNewSymbolTable();
    setLexicalAnalyzerSymbolTable(staticSymbolTable);
    
    // Push symbol table to the stack
    pushSymbolTableToStack(staticSymbolTable);
    
    // Open output file
    return initializeCodeGenerator(outputFilename, sourceCodeFilename);
    
}


void executeSemanticAction(SubAutomatonIdentifier originSubAutomaton, SubAutomatonState originState, Terminal terminal, Token* token) {
    
    switch (originSubAutomaton) {
        
        // Program sub-automaton
        case saiPRGM:
            
            switch (terminal) {
                    
                case prgmVoid: pushPrimitiveTypeToStack(token->value.intValue); break;
                    
                // New function or struct declaration.
                case prgmIdentifier:
                    // New function declaration with struct return type
                    if (originState == 0) pushStructTypeToStack(token->value.intValue);
                    // New function declaration
                    else if (originState == 1 || originState == 2) newFunctionDeclaration(token->value.intValue);
                    // New struct declaration
                    else newStructDeclaration(token->value.intValue);
                    break;
                
                // End of struct declaration.
                case prgmEndstruct:
                    setSymbolSizeAndAddress();
                    endStructDeclaration();
                    break;
                    
                // Function with array return type.
                case prgmInteger:
                    addDimensionSize(token->value.intValue);
                    break;
                    
                // Start getting parameters, got function name and return size.
                case prgmOpenParenthesis:
                    retrievingFunctionParameters = 1;
                    startRetrievingFunctionParametersAndVariables();
                    break;
                
                // End of parameters list.
                case prgmCloseParenthesis:
                    setSymbolSizeAndAddress(retrievingFunctionParameters);
                    retrievingFunctionParameters = 0;
                    break;

                case prgmBegin:
                    // Function or main with variables.
                    if (originState == 9 || originState == 20) setSymbolSizeAndAddress(retrievingFunctionParameters);
                    if (originState == 7 || originState == 9) beginMainExecution();
                    else if (originState == 18 || originState == 20) beginFunctionExecution();
                    break;
                
                // End of function declaration or end of main.
                case prgmEnd:
                    // End of main.
                    if (originState == 10) endMain();
                    // End of function declaration.
                    else if (originState == 21) endFunctionDeclaration();
                    break;
                
                // Start main declaration
                case prgmMain:
                    mainDeclaration();
                    break;
                default: break;
            }
            
            break;
        
        // Declarations sub-automaton
        case saiDCLS:
            
            switch (terminal) {
                    
                case dclsIdentifier:
                    // Struct type variable or parameter.
                    if (originState == 0) pushStructTypeToStack(token->value.intValue);
                    // New parameter or variable.
                    else if (originState == 1) newVariableDeclaration(token->value.intValue);
                    break;
                    
                // Read array dimension size
                case dclsInteger: addDimensionSize(token->value.intValue); break;
                    
                // Finished reading variable or parameter
                case dclsComma: setSymbolSizeAndAddress(retrievingFunctionParameters); break;
                    
                default: break;
            }
            break;
            
        // Command sub-automaton
        case saiCMND:
            switch (terminal) {
                    
                // Evaluate expression
                case cmndCloseParenthesis:
                    if (originState == 11 || originState == 12 || originState == 16) evaluateExpression(eetEndOfExpression);
                    break;
                    
                // While
                case cmndWhile:
                    newWhileCommand();
                    break;
                
                // New expression
                case cmndOpenParenthesis:
                    newOperator(oprOpenParenthesis);
                    break;
                    
                case cmndColon:
                    // While after condition
                    if (originState == 13) whileTest();
                    break;
                    
                // End while
                case cmndEndwhile:
                    endWhileCommand();
                    break;
                    
                // Scan
                case cmndScan:
                    newOperator(oprScan);
                    break;
                    
                // Operand
                case cmndIdentifier:
                    newOperand(opdtVariable, token->value.intValue);
                    break;
                    
                // Print
                case cmndPrint:
                    newOperator(oprPrint);
                    break;
                    
                default:break;
                    
            }
            break;
            
        // Attribution sub-automaton
        case saiATTR:
            switch (terminal) {
                    
                // Operand or function call
                case attrIdentifier:
                    if (originState == 0) newOperandOrFunctionCall(token->value.intValue);
                    break;
                    
                // Evaluate expression
                case attrCloseParenthesis:
                    evaluateExpression(eetEndOfExpression);
                    break;
                    
                // Attribution
                case attrEqualSign:
                    newOperator(oprEqualSign);
                    break;
                    
                default: break;
                    
            }
            break;
            
        // Expressions sub-automaton
        case saiEXPS:
            
            switch (terminal) {
                // New expression, evaluate last expression
                case expsComma: evaluateExpression(eetComma); break;
                    
                // New string literal
                case expsString: newStringOperand(token->value.stringValue); break;
                
                // End of expression list
                default: if (originState == 1) evaluateExpression(eetEndOfExpression); break;
                    
            }
            break;
            
        // Expression sub-automaton
        case saiEXPR:
            
            // Push operator
            switch (terminal) {
                case exprNot: newOperator(oprNot); break;
                case exprMinusSign: (originState == 0) ? newOperator(oprMinus) : newOperator(oprSubtract); break;
                case exprAnd: if (originState == 3) newOperator(oprLogicAnd); break;
                case exprOr: if (originState == 4) newOperator(oprLogicOr); break;
                case exprPlusSign: newOperator(oprAdd); break;
                case exprAsterisk: newOperator(oprMultiply); break;
                case exprSlash: newOperator(oprDivide); break;
                case exprPercentageSign: newOperator(oprModulus); break;
                case exprEqualSign: newOperator(oprEqualSign); break;
                case exprExclamationSign: newOperator(oprDifferent); break;
                case exprBiggerThan: newOperator(oprBiggerThan); break;
                case exprSmallerThan: newOperator(oprSmallerThan); break;
                default: break;
            }
            break;
            
        // Atom sub-automaton
        case saiATOM:
            
            // Push operand
            switch (terminal) {
                case atomOpenParenthesis: if (originState == 0 || originState == 2) newOperator(oprOpenParenthesis); break;
                case atomCloseParenthesis: evaluateExpression(eetCloseParenthesis); break;
                case atomInteger:
                    if (originState == 0) newOperand(opdtInteger, token->value.intValue);
                    else accessArrayDimension(token->value.intValue); break;
                case atomCharacter: newOperand(opdtChar, token->value.charValue); break;
                case atomTrue: newOperand(opdtBoolean, 1); break;
                case atomFalse: newOperand(opdtBoolean, 0); break;
                case atomIdentifier:
                    if (originState == 0) newOperandOrFunctionCall(token->value.intValue);
                    else accessStructField(token->value.intValue);
                    break;
                default:
                    break;
            }
            break;
            
        // If body sub-automaton
        case saiIFBY:
            
            switch (terminal) {
                case ifbyOpenParenthesis: newOperator(oprOpenParenthesis); break;
                case ifbyCloseParenthesis: evaluateExpression(eetEndOfExpression); break;
                case ifbyColon: if (originState == 3) newIfCommand(); break;
                case ifbyElse: newElseCommand(); break;
                case ifbyEndif: endIfCommand(); break;
                default: break;
            }
            break;
        
        // Found primitive type
        case saiTYPE: if (terminal == typeType) pushPrimitiveTypeToStack(token->value.intValue); break;
            
        default: break;
            
    }
    
    freeToken(token);
    
}

void executeSemanticActionAtTheEndOfSubAutomaton(SubAutomatonIdentifier currentSubAutomaton, SubAutomatonState finalState, SubAutomatonIdentifier returnSubAutomaton, SubAutomatonState returnState) {
    
    switch (currentSubAutomaton) {
    
        case saiATTR:
            // End of attribution
            evaluateExpression(eetEndOfExpression);
            break;
        
        case saiEXPR:
            // Function return
            if (returnSubAutomaton == saiCMND && returnState == 1) {
                evaluateExpression(eetEndOfExpression);
                functionReturn();
            }
            break;
            
        default: break;
            
    }
    
}


void freeSemanticAnalyzer() {
    freeSemanticAnalyzerInternalStructures();
}

