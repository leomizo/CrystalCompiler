/*!
 
   SyntacticAnalyzer.c
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-11-19
 
 */

#include "SyntacticAnalyzer.h"
#include "SemanticAnalyzer.h"
#include "LexicalAnalyzer.h"
#include "SubAutomata.h"
#include "SymbolTable.h"
#include "IntegerStack.h"
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


// Indicates the current sub-automaton. The initial sub-automation is the Program sub-automaton (PRGM).
static SubAutomatonIdentifier currentSubAutomaton = saiPRGM;

// Indicates the current state in the sub-automaton. The initial state is 0.
static SubAutomatonState currentSubAutomatonState = 0;

// The stack which keeps the return states as sub-automata are called.
// Together with the subAutomatonCallStack, these stacks implement the pushdown automaton stack.
static IntegerStack* stateStack = NULL;

// The stack which keeps the references to the sub-automata of the return states as other sub-automata are called.
// Together with the stateStack, these stacks implement the pushdown automaton stack.
static IntegerStack* subAutomatonCallStack = NULL;

// The last token read by the lexical analyzer.
static Token* currentToken = NULL;


// Triggers the next transition of the pushdown automaton.
// Returns 1 if the pushdown automaton has not yet reached its final state or an error state, 0 otherwise.
int triggerSyntacticAnalyzerTransition() {
    
    // If the current token is NULL, this means the next token must be read.
    if (currentToken == NULL) getNextToken(&currentToken);
    
    // Verify if an error or the final token have been read.
    if (currentToken->type != tokenTypeError && currentToken->type != tokenTypeEnd) {
        
        SubAutomatonState nextState = -1;
        SubAutomatonIdentifier subAutomatonCall = saiNONE;
        Terminal terminal = -1;
        
        // Get next transition (next state and sub-automaton call).
        switch (currentSubAutomaton) {
            case saiPRGM: terminal = GET_PRGM_TRANSITION(currentSubAutomatonState, currentToken, &nextState, &subAutomatonCall); break;
            case saiDCLS: terminal = GET_DCLS_TRANSITION(currentSubAutomatonState, currentToken, &nextState, &subAutomatonCall); break;
            case saiCMND: terminal = GET_CMND_TRANSITION(currentSubAutomatonState, currentToken, &nextState, &subAutomatonCall); break;
            case saiATTR: terminal = GET_ATTR_TRANSITION(currentSubAutomatonState, currentToken, &nextState, &subAutomatonCall); break;
            case saiARYL: terminal = GET_ARYL_TRANSITION(currentSubAutomatonState, currentToken, &nextState, &subAutomatonCall); break;
            case saiIFBY: terminal = GET_IFBY_TRANSITION(currentSubAutomatonState, currentToken, &nextState, &subAutomatonCall); break;
            case saiEXPS: terminal = GET_EXPS_TRANSITION(currentSubAutomatonState, currentToken, &nextState, &subAutomatonCall); break;
            case saiEXPR: terminal = GET_EXPR_TRANSITION(currentSubAutomatonState, currentToken, &nextState, &subAutomatonCall); break;
            case saiATOM: terminal = GET_ATOM_TRANSITION(currentSubAutomatonState, currentToken, &nextState, &subAutomatonCall); break;
            case saiTYPE: terminal = GET_TYPE_TRANSITION(currentSubAutomatonState, currentToken, &nextState, &subAutomatonCall); break;
            default: break;
        }
   
        // Syntactic error detected, generate error message.
        if (nextState == -1) {
            char* errorBuffer = NULL;
            generateErrorMessage(&errorBuffer, "Error in syntax!\n");
            printf("%s", errorBuffer);
            return 0;
        }
        
        // End of current sub-automaton.
        else if (subAutomatonCall == saiFSTE) {
            
            SubAutomatonState finalState = currentSubAutomatonState;
            SubAutomatonIdentifier finishedSubAutomaton = currentSubAutomaton;
            
            // Pop return state and sub-automaton from the stacks.
            popIntegerFromStack(&stateStack, &currentSubAutomatonState);
            popIntegerFromStack(&subAutomatonCallStack, &currentSubAutomaton);
            
            executeSemanticActionAtTheEndOfSubAutomaton(finishedSubAutomaton, finalState, currentSubAutomaton, currentSubAutomatonState);
            
        }
        
        // Simple transition, token consumption.
        else if (subAutomatonCall == saiNONE) {
            
            // Call semantic action based on the transition.
            executeSemanticAction(currentSubAutomaton, currentSubAutomatonState, terminal, currentToken);
            
            // Indicate token consumption and go to next state.
            currentToken = NULL;
            currentSubAutomatonState = nextState;
            
        }
        
        // Sub-automaton call.
        else {
            
            // Push return state and sub-automaton to the stacks.
            pushIntegerToStack(&stateStack, nextState);
            pushIntegerToStack(&subAutomatonCallStack, currentSubAutomaton);
            
            // Go to the initial state of the sub-automaton call.
            currentSubAutomaton = subAutomatonCall;
            currentSubAutomatonState = 0;
            
        }
        
        return 1;
        
    } else return 0;
    
}

// Frees the syntactic analyzer's associated memory blocks.
void freeSyntacticAnalyzer() {
    free(stateStack);
    free(subAutomatonCallStack);
}

// Compiles the source code (Crystal) to the output file (Assembly).
void compile(const char* inputFile, const char* outputFile) {
    
    // Initialize lexical analyzer.
    if (initializeLexicalAnalyzer(inputFile)) {
        
        // Initialize semantic analyzer.
        if (initializeSemanticAnalyzer(outputFile, inputFile)) {
            
            // Execute transitions until the end of the code.
            while (triggerSyntacticAnalyzerTransition());
            
            // Free semantic and syntactic analyzers' memory blocks.
            freeSemanticAnalyzer();
            freeSyntacticAnalyzer();
            
        }
        
        // Free lexical analyzer's memory blocks.
        freeLexicalAnalyzer();
        
    }
    
}


