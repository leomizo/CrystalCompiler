//
//  CodeGenerator.c
//  CrystalCompiler
//
//  Created by Leonardo Mizoguti on 17/11/15.
//  Copyright © 2015 Mizoguti. All rights reserved.
//

#include "CodeGenerator.h"
#include "IntegerStack.h"
#include "IntegerList.h"
#include "LexicalAnalyzer.h"

#define LABEL_SIZE 6
#define BREAK_LINE "\n"
#define INSTRUCTION_PADDING "        "

// Output file pointer
static FILE* outputCode;

// While optimization
static int temporaryVariableAlreadyCreated = 0;

// Label management
static IntegerStack* labelStack = NULL;
static int functionLabelCounter = -1;
static int internalFunctionLabelCounter = -1;

// String buffer
static char stringBuffer[256][18];
static int stringBufferCounter = 0;


int initializeCodeGenerator(const char* outputFilename, const char* sourceCodeFilename) {
    
    // Try to open file.
    outputCode = fopen(outputFilename, "w");
    
    // If file could be opened, continue, otherwise return 0.
    if (outputCode != NULL) {
        fprintf(outputCode, BREAK_LINE);
        fprintf(outputCode, ";\n");
        fprintf(outputCode, ";  Crystal compiler \n");
        fprintf(outputCode, ";  Generated MVN Assembly code for %s\n", sourceCodeFilename);
        fprintf(outputCode, ";\n");
        fprintf(outputCode, BREAK_LINE);
        fprintf(outputCode, "main    >\n");
        fprintf(outputCode, "stacks  >\n");
        fprintf(outputCode, "stacke  >\n");
        fprintf(outputCode, "strbct  >\n");
        fprintf(outputCode, "strbs   >\n");
        fprintf(outputCode, "evaddr  <\n");
        fprintf(outputCode, "evval   <\n");
        fprintf(outputCode, "evoffs  <\n");
        fprintf(outputCode, "evsign  <\n");
        fprintf(outputCode, "evtemp  <\n");
        fprintf(outputCode, "ercad   <\n");
        fprintf(outputCode, "erwrt   <\n");
        fprintf(outputCode, "errd    <\n");
        fprintf(outputCode, "ercpb   <\n");
        fprintf(outputCode, "svbptr  <\n");
        fprintf(outputCode, "svsptr  <\n");
        fprintf(outputCode, "svsize  <\n");
        fprintf(outputCode, "srpsa   <\n");
        fprintf(outputCode, "srppa   <\n");
        fprintf(outputCode, "srptv   <\n");
        fprintf(outputCode, "srwfra  <\n");
        fprintf(outputCode, "srrfra  <\n");
        fprintf(outputCode, "scani   <\n");
        fprintf(outputCode, "scans   <\n");
        fprintf(outputCode, "puti    <\n");
        fprintf(outputCode, "puts    <\n");
        fprintf(outputCode, "putb    <\n");
        fprintf(outputCode, "pbrkl   <\n");
        fprintf(outputCode, BREAK_LINE);
        fprintf(outputCode, INSTRUCTION_PADDING);
        fprintf(outputCode, "&  /0000 \n");
        return 1;
        
    } return 0;
    
}

void generateOffsetFromBasePointer(int offset) {
    fprintf(outputCode, "%sLD  svbptr\n", INSTRUCTION_PADDING);
    fprintf(outputCode, "%sMM  evaddr\n", INSTRUCTION_PADDING);
    fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, offset);
    fprintf(outputCode, "%sMM  evoffs\n", INSTRUCTION_PADDING);
}

// Result in evval
void generateReadingVariable(int offset) {
    generateOffsetFromBasePointer(offset);
    fprintf(outputCode, "%sSC  errd\n", INSTRUCTION_PADDING);
}

// Value already in evval
void generateWritingVariable(int offset) {
    generateOffsetFromBasePointer(offset);
    fprintf(outputCode, "%sSC  erwrt\n", INSTRUCTION_PADDING);
}

void generateArithmeticOperation(Operator operation, Operand leftOperand, Operand rightOperand, int resultAddressOffset) {
    
    if (leftOperand.type == opdtVariable || leftOperand.type ==opdtTemporary) {
        generateReadingVariable(leftOperand.value);
        fprintf(outputCode, "%sLD  evval\n", INSTRUCTION_PADDING);
    } else fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, leftOperand.value);
    
    fprintf(outputCode, "%sMM  evtemp\n", INSTRUCTION_PADDING);
    
    if (rightOperand.type == opdtVariable || rightOperand.type == opdtTemporary) generateReadingVariable(rightOperand.value);
    else {
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, rightOperand.value);
        fprintf(outputCode, "%sMM  evval\n", INSTRUCTION_PADDING);
    }
    
    fprintf(outputCode, "%sLD  evtemp\n", INSTRUCTION_PADDING);
    
    switch (operation) {
        case oprAdd: fprintf(outputCode, "%s+   evval\n", INSTRUCTION_PADDING); break;
        case oprSubtract: fprintf(outputCode, "%s-   evval\n", INSTRUCTION_PADDING); break;
        case oprMultiply: fprintf(outputCode, "%s*   evval\n", INSTRUCTION_PADDING); break;
        case oprDivide: fprintf(outputCode, "%s/   evval\n", INSTRUCTION_PADDING); break;
        default: break;
    }
    
    fprintf(outputCode, "%sMM  evval\n", INSTRUCTION_PADDING);
    
    if (resultAddressOffset >= 0) generateWritingVariable(resultAddressOffset);
    
}

void generateInternalFunctionLabel(char label[7]) {
    sprintf(label, "f%02x_%02x", functionLabelCounter, internalFunctionLabelCounter);
    internalFunctionLabelCounter++;
}

void generateInternalFunctionLabelWithIndex(char label[7], int index) {
    sprintf(label, "f%02x_%02x", functionLabelCounter, index);
}

void generateFunctionLabel(int functionIndex, char label[7]) {
    
    if (functionIndex <= 255) sprintf(label, "f%02x", functionIndex);
    
}

void generateAttribution(Operand leftOperand, Operand rightOperand) {
    if (rightOperand.type == opdtVariable || rightOperand.type == opdtTemporary) generateReadingVariable(rightOperand.value);
    else {
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, rightOperand.value);
        fprintf(outputCode, "%sMM  evval\n", INSTRUCTION_PADDING);
    }
    generateWritingVariable(leftOperand.value);
}

// Result in temp
void generateRelationalComparison(Operator comparison, Operand leftOperand, Operand rightOperand, int resultAddressOffset) {
    
    char conditionLabel[7];
    char endLabel[7];
    int trueIfJump = 0;
    
    // Generate labels
    generateInternalFunctionLabel(conditionLabel);
    generateInternalFunctionLabel(endLabel);
    
    // Make left operand minus right operand
    generateArithmeticOperation(oprSubtract, leftOperand, rightOperand, -1);
    
    switch (comparison) {
            
        case oprSmallerOrEqualThan:
            fprintf(outputCode, "%sJZ  %s\n", INSTRUCTION_PADDING, conditionLabel);
        case oprSmallerThan:
            fprintf(outputCode, "%sJN  %s\n", INSTRUCTION_PADDING, conditionLabel);
            trueIfJump = 1;
            break;
            
        case oprBiggerThan:
            fprintf(outputCode, "%sJZ  %s\n", INSTRUCTION_PADDING, conditionLabel);
        case oprBiggerOrEqualThan:
            fprintf(outputCode, "%sJN  %s\n", INSTRUCTION_PADDING, conditionLabel);
            break;
            
        case oprEquals:
            trueIfJump = 1;
        case oprDifferent:
            fprintf(outputCode, "%sJZ  %s\n", INSTRUCTION_PADDING, conditionLabel);
            break;
        
        default:break;
    }
    
    if (trueIfJump) {
        fprintf(outputCode, "%sLV  /000\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sJP  %s\n", INSTRUCTION_PADDING, endLabel);
        fprintf(outputCode, "%s  LV  /001\n", conditionLabel);
    } else {
        fprintf(outputCode, "%sLV  /001\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sJP  %s\n", INSTRUCTION_PADDING, endLabel);
        fprintf(outputCode, "%s  LV  /000\n", conditionLabel);
    }
    
    fprintf(outputCode, "%s  MM  evval\n", endLabel);
    
    // Write result
    if (resultAddressOffset > 0) generateWritingVariable(resultAddressOffset);
    
}

void generateLogicalOperation(Operator operation, Operand leftOperand, Operand rightOperand, int resultAddressOffset) {
    
    char leftConditionLabel[7];
    char rightConditionLabel[7];
    
    if (leftOperand.type == opdtVariable || leftOperand.type == opdtTemporary) {
        generateInternalFunctionLabel(leftConditionLabel);
        generateReadingVariable(leftOperand.value);
        fprintf(outputCode, "%sJZ  %s\n", INSTRUCTION_PADDING, leftConditionLabel);
        fprintf(outputCode, "%sLV  /001\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%s  MM  evtemp\n", leftConditionLabel);
    } else {
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, leftOperand.value != 0 ? 1 : 0);
        fprintf(outputCode, "%s  MM  evtemp\n", INSTRUCTION_PADDING);
    }
    
    if (rightOperand.type == opdtVariable || rightOperand.type == opdtTemporary) {
        generateInternalFunctionLabel(rightConditionLabel);
        generateReadingVariable(rightOperand.value);
        fprintf(outputCode, "%sJZ  %s\n", INSTRUCTION_PADDING, rightConditionLabel);
        fprintf(outputCode, "%sLV  /001\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%s  MM  evval\n", rightConditionLabel);
    } else {
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, rightOperand.value != 0 ? 1 : 0);
        fprintf(outputCode, "%s  MM  evval\n", INSTRUCTION_PADDING);
    }
    
    fprintf(outputCode, "%sLD  evtemp\n", INSTRUCTION_PADDING);
    
    if (operation == oprLogicAnd) fprintf(outputCode, "%s*   evval\n", INSTRUCTION_PADDING);
    else if (operation == oprLogicOr) fprintf(outputCode, "%s+   evval\n", INSTRUCTION_PADDING);

    fprintf(outputCode, "%sMM  evval\n", INSTRUCTION_PADDING);
    
    // Write result
    if (resultAddressOffset > 0) generateWritingVariable(resultAddressOffset);
    
}

int generateFunctionDeclaration(char* functionName, int activationRecordSize) {

    char functionLabel[LABEL_SIZE];
    
    functionLabelCounter++;
    internalFunctionLabelCounter = 0;
    
    generateFunctionLabel(functionLabelCounter, functionLabel);
    fprintf(outputCode, BREAK_LINE);
    fprintf(outputCode, "; Function: %s  [Label: %s]\n", functionName, functionLabel);
    fprintf(outputCode, "%s     K   /0\n", functionLabel);
    
    // Push new activation record
    fprintf(outputCode, "%sLV  /%03x    ; Write function return address to activation record\n", INSTRUCTION_PADDING, activationRecordSize);
    fprintf(outputCode, "%sMM  svsize\n", INSTRUCTION_PADDING);
    fprintf(outputCode, "%sSC  srpsa\n", INSTRUCTION_PADDING);
    
    // Write return address to activation record
    fprintf(outputCode, "%sLD  %s\n", INSTRUCTION_PADDING, functionLabel);
    fprintf(outputCode, "%sMM  evval\n", INSTRUCTION_PADDING);
    fprintf(outputCode, "%sSC  srwfra  ; Function code starts below \n", INSTRUCTION_PADDING);
    
    return functionLabelCounter;
    
}

void generateFunctionEnd(int functionAddress) {
    
    char functionLabel[LABEL_SIZE];
    char returnLabel[7];
    
    generateFunctionLabel(functionAddress, functionLabel);
    generateInternalFunctionLabelWithIndex(returnLabel, 255);
    
    fprintf(outputCode, "%s  SC  srrfra  ; Retrieve return address and pop activation record\n", returnLabel);
    fprintf(outputCode, "%sMM  %s\n", INSTRUCTION_PADDING, functionLabel);
    fprintf(outputCode, "%sSC  srppa\n", INSTRUCTION_PADDING);
    fprintf(outputCode, "%sRS  %s\n", INSTRUCTION_PADDING, functionLabel);
    
}

void generateNewTemporaryVariable() {
    if (!temporaryVariableAlreadyCreated) {
        fprintf(outputCode, "%sSC  srptv\n", INSTRUCTION_PADDING);
    } else temporaryVariableAlreadyCreated = 0;
}

// IF

void generateIfCommand(Operand condition) {
    
    char ifLabell[7];
    
    if (condition.type == opdtVariable || condition.type == opdtTemporary) {
        generateReadingVariable(condition.value);
        fprintf(outputCode, "%sLD  evval\n", INSTRUCTION_PADDING);
    } else fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, condition.value);
    
    generateInternalFunctionLabel(ifLabell);
    pushIntegerToStack(&labelStack, internalFunctionLabelCounter - 1);
    
    fprintf(outputCode, "%sJZ  %s  ; If command\n", INSTRUCTION_PADDING, ifLabell);
    
}

void generateElseCommand() {
    
    char elseLabel[7];
    char endLabel[7];
    
    int elseLabelIndex;
    popIntegerFromStack(&labelStack, &elseLabelIndex);

    generateInternalFunctionLabelWithIndex(elseLabel, elseLabelIndex);
    generateInternalFunctionLabel(endLabel);
    
    pushIntegerToStack(&labelStack, internalFunctionLabelCounter - 1);
    
    fprintf(outputCode, "%sJP  %s\n", INSTRUCTION_PADDING, endLabel);
    fprintf(outputCode, "%s  OS  /000    ; Else\n", elseLabel);
    
}

void generateEndIf() {
    
    char endLabel[7];
    int endLabelIndex;
    popIntegerFromStack(&labelStack, &endLabelIndex);
    
    generateInternalFunctionLabelWithIndex(endLabel, endLabelIndex);
    
    fprintf(outputCode, "%s  OS  /000    ; Endif\n", endLabel);
    
}

// WHILE

void generateWhileCommand() {
    
    char whileLabel[7];
    
    generateInternalFunctionLabel(whileLabel);
    pushIntegerToStack(&labelStack, internalFunctionLabelCounter - 1);
    
    generateNewTemporaryVariable();
    temporaryVariableAlreadyCreated = 1;
    fprintf(outputCode, "%s  OS  /000    ; While\n", whileLabel);
    
}

void generateWhileTest(Operand condition) {
    
    char endLabel[7];
    
    generateInternalFunctionLabel(endLabel);
    pushIntegerToStack(&labelStack, internalFunctionLabelCounter - 1);
    
    if (condition.type == opdtVariable || condition.type == opdtTemporary) {
        generateReadingVariable(condition.value);
        fprintf(outputCode, "%sLD  evval\n", INSTRUCTION_PADDING);
    } else fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, condition.value);
    
    fprintf(outputCode, "%sJZ  %s    ; While Condition\n", INSTRUCTION_PADDING, endLabel);
}

void generateEndWhile() {
    
    char whileLabel[7];
    char endLabel[7];
    
    int whileLabelIndex;
    int endLabelIndex;
    
    popIntegerFromStack(&labelStack, &endLabelIndex);
    generateInternalFunctionLabelWithIndex(endLabel, endLabelIndex);
    
    popIntegerFromStack(&labelStack, &whileLabelIndex);
    generateInternalFunctionLabelWithIndex(whileLabel, whileLabelIndex);
    
    fprintf(outputCode, "%sJP  %s\n", INSTRUCTION_PADDING, whileLabel);
    fprintf(outputCode, "%s  OS  /000    ; Endwhile\n", endLabel);
    
}

void generateFunctionReturn(Operand* returnOperand) {
    
    char endLabel[7];
    
    generateInternalFunctionLabelWithIndex(endLabel, 255);
    
    if (returnOperand != NULL) {
        if ((*returnOperand).type == opdtVariable || (*returnOperand).type == opdtTemporary) {
            generateReadingVariable((*returnOperand).value);
            
        } else {
            fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, (*returnOperand).value);
            fprintf(outputCode, "%sMM  evval\n", INSTRUCTION_PADDING);
        }
        
        generateWritingVariable(2); // TO DO: Arrays, structs
    }
    
    fprintf(outputCode, "%sJP  %s  ; Return\n", INSTRUCTION_PADDING, endLabel);
    
}

void generatePassingParameter(Operand parameter, int address, int size) {
    
    if (size == 1) {
        if (parameter.type == opdtVariable || parameter.type == opdtTemporary) {
            generateReadingVariable(parameter.value);
        } else {
            fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, parameter.value);
            fprintf(outputCode, "%sMM  evval\n", INSTRUCTION_PADDING);
        }
        fprintf(outputCode, "%sLD  svsptr\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sMM  evaddr\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, address + 1);
        fprintf(outputCode, "%sMM  evoffs\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sSC  erwrt\n", INSTRUCTION_PADDING);
    } else {
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, parameter.value);
        fprintf(outputCode, "%sMM  evaddr\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, address + 1);
        fprintf(outputCode, "%s+   svsptr\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sMM  evval\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, size);
        fprintf(outputCode, "%sMM  evoffs\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sSC  ercpb\n", INSTRUCTION_PADDING);
    }
    
}

void generateFunctionCall(int functionAddress, int resultAddressOffset, int returnValueSize) {
    
    char functionLabel[7];
    
    generateFunctionLabel(functionAddress, functionLabel);
    
    fprintf(outputCode, "%sSC  %s      ; Call function\n", INSTRUCTION_PADDING, functionLabel);
    
    if (returnValueSize == 1) {
        fprintf(outputCode, "%sLD  svsptr\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sMM  evaddr\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, 3);
        fprintf(outputCode, "%sMM  evoffs\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sSC  errd\n", INSTRUCTION_PADDING);
        generateWritingVariable(resultAddressOffset);
    } else {
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, 3);
        fprintf(outputCode, "%s+   svsptr\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sMM  evaddr\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, resultAddressOffset);
        fprintf(outputCode, "%sMM  evval\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, returnValueSize);
        fprintf(outputCode, "%sMM  evoffs\n", INSTRUCTION_PADDING);
        fprintf(outputCode, "%sSC  ercpb\n", INSTRUCTION_PADDING);
    }
    
}

void generateMain() {
    fprintf(outputCode, BREAK_LINE);
    fprintf(outputCode, ";  MAIN \n");
    fprintf(outputCode, "main    LV  stacks\n");
    fprintf(outputCode, "%sMM  svbptr\n", INSTRUCTION_PADDING);
    fprintf(outputCode, "%sLV  stacke\n", INSTRUCTION_PADDING);
    fprintf(outputCode, "%sMM  svsptr\n", INSTRUCTION_PADDING);
}

void generateMainEnd(int mainSize) {
    fprintf(outputCode, "%sHM  main\n", INSTRUCTION_PADDING);
    fprintf(outputCode, BREAK_LINE);
    fprintf(outputCode, ";  STRING BUFFER \n");
    fprintf(outputCode, "strbct  K  /%04x\n", stringBufferCounter);
    if (stringBufferCounter == 0) {
        fprintf(outputCode, "strbs   $  /0100\n");
    } else {
        for (int i = 0; i < stringBufferCounter / 2; i++) fprintf(outputCode, "%s", stringBuffer[i]);
        fprintf(outputCode, "%s$  /%04x\n", INSTRUCTION_PADDING, 256 - stringBufferCounter);
    }
    fprintf(outputCode, BREAK_LINE);
    fprintf(outputCode, ";  STACK \n");
    fprintf(outputCode, "stacks  K   /0\n");
    while (mainSize) {
        fprintf(outputCode, "%sK   /0\n", INSTRUCTION_PADDING);
        mainSize--;
    }
    fprintf(outputCode, "stacke  K   /0\n");
    fprintf(outputCode, BREAK_LINE);
    fprintf(outputCode, "%s#   main\n", INSTRUCTION_PADDING);
}

void generateStringScan(int resultAddressOffset) {
    fprintf(outputCode, "%sSC  scans\n", INSTRUCTION_PADDING);
    generateWritingVariable(resultAddressOffset);
}

void generateIntScan(int resultAddressOffset) {
    
    fprintf(outputCode, "%sSC  scani\n", INSTRUCTION_PADDING);
    generateWritingVariable(resultAddressOffset);
    
}

void generateBooleanScan(int resultAddressOffset) {
    
    char scanLabel[7];
  
    generateInternalFunctionLabel(scanLabel);

    fprintf(outputCode, "%sSC  scani\n", INSTRUCTION_PADDING);
    fprintf(outputCode, "%sLD  evval\n", INSTRUCTION_PADDING);
    fprintf(outputCode, "%sJZ  %s\n", INSTRUCTION_PADDING, scanLabel);
    fprintf(outputCode, "%sLV  /001\n", INSTRUCTION_PADDING);
    fprintf(outputCode, "%s  MM  evval\n", scanLabel);
    generateWritingVariable(resultAddressOffset);
}

void generateStringPrint(Operand string) {
    if (string.type == opdtVariable || string.type == opdtTemporary) {
        generateReadingVariable(string.value);
        fprintf(outputCode, "%sSC  puts\n", INSTRUCTION_PADDING);
    } else {
        if (string.value >= 0) {
            fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, string.value);
            fprintf(outputCode, "%sMM  evval\n", INSTRUCTION_PADDING);
            fprintf(outputCode, "%sSC  puts\n", INSTRUCTION_PADDING);
        } else generateBreakLinePrint();
    }
}

void generateBreakLinePrint() {
    fprintf(outputCode, "%sSC  pbrkl\n", INSTRUCTION_PADDING);
}

void generateIntPrint(Operand integer) {
    if (integer.type == opdtVariable || integer.type == opdtTemporary) {
        generateReadingVariable(integer.value);
    } else {
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, integer.value);
        fprintf(outputCode, "%sMM  evval\n", INSTRUCTION_PADDING);
    }
    fprintf(outputCode, "%sSC  puti\n", INSTRUCTION_PADDING);
}

void generateBooleanPrint(Operand boolean) {
    if (boolean.type == opdtVariable || boolean.type == opdtTemporary) {
        generateReadingVariable(boolean.value);
    } else {
        fprintf(outputCode, "%sLV  /%03x\n", INSTRUCTION_PADDING, boolean.value);
        fprintf(outputCode, "%sMM  evval\n", INSTRUCTION_PADDING);
    }
    fprintf(outputCode, "%sSC  putb\n", INSTRUCTION_PADDING);
}

// Returns the string address in the buffer
int generateStringLiteral(char* string) {
    int stringAddress = stringBufferCounter;
    for (int i = 1; i < strlen(string) - 1; i += 2) {
        char first = string[i];
        char second = string[i + 1];
        if (stringBufferCounter == 0) {
            sprintf(stringBuffer[0], "strbs   K  /%02x%02x\n", first, second);
        } else {
            sprintf(stringBuffer[stringBufferCounter / 2], "%sK  /%02x%02x\n", INSTRUCTION_PADDING, first, second);
        }
        stringBufferCounter += 2;
    }
    sprintf(stringBuffer[stringBufferCounter / 2], "%sK  /0\n", INSTRUCTION_PADDING);
    stringBufferCounter += 2;
    return stringAddress;
}

