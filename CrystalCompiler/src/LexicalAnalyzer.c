/*!
 
   LexicalAnalyzer.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-11-19
 
 */

#include "LexicalAnalyzer.h"

#define COUNT_OF_LEXICAL_ANALYZERS_STATES 22
#define COUNT_OF_CHARACTER_TYPES          11
#define COUNT_OF_SPECIAL_CHARACTERS       18
#define COUNT_OF_DELIMITERS               6
#define COUNT_OF_RESERVED_WORDS           25
#define MAX_RESERVED_WORD_SIZE            64

/*!
   @typedef LexicalAnalyzerState
 
   @abstract This enumeration groups all states in which the Lexical Analyzer's automaton can be.
 
   @discussion 
 
   The name of the states are build in the following way: (exceptions are ST (START => initial state) and ER (ERROR => error state), refer to the CharType enumeration for regular expressions)
 
    - First character: indicates the type of token being read
                 
        [N] => A number (be it an integer or a float).
               A valid number is sequence of digits (0-9) and optionally only one dot (.) with at least one digit before OR after ('.3' and '3.' are accepted forms).
               Regex: ( DG* DT DG+ | DG+ ( DT DG* )? )
 
        [W] => A word (which can be classified as a declared symbol or a reserved word).
               A valid word is a sequence of alphanumerical characters beginning with a letter (a-z or A-Z).
               Regex: LT ( LT | DG )*
 
        [S] => A special symbol.
               A valid special symbol is represented by just one character which can be found in the SPECIAL_CHARACTERS array.
               Regex: SS
 
        [H] => A char literal.
               A char literal is one single character between single quotes.
               Regex: SQ CX* SQ [CX means any character]
 
        [T] => A string literal.
               A string literal is a sequence of characters between double quotes.
               Regex: DQ CX* DQ [CX means any character]
 
        [C] => A comment.
               Comments can contain any character. They are simply ignored by the Lexical Analyzer.
               A valid comment begins with '//' (all the rest of the line becomes a comment) or begins with '/''*' and ends with '*''/' (without the quotation marks).
               Regex: SL ( SL CX* LB | AS CX* AS SL ) [CX means any character]
 
        [D] => A delimiter.
               A valid delimiter is represented by just one character which can be found in the DELIMITERS array.
               Regex: DL
 
    - Second character:
 
        [a number] => Indicates the number of the state (refer to the provided automaton schema).
            
        [F]        => The final state. When the automaton reaches a final state, it goes back to the initial state (ST) and in most of the cases return a token.
 */
typedef enum {
    // Initial state
    ST,
    // Number states
    N1, N2, N3, N4, NF,
    // Word states
    W1, WF,
    // Special symbol states
    S1, SF,
    // Char states
    H1, H2, HF,
    // String states
    T1, TF,
    // Comment states
    C1, C2, C3, C4, CF,
    // Delimiter state
    DF,
    // Error state
    ER
} LexicalAnalyzerState;

/*!
   @typedef   CharType
   @abstract  The type of a character.
   @discussion This information is used as a input of the lexical analyzer automaton.
 
   @constant  LT  Letter:          characters between 'a' and 'z' or 'A' and 'Z'.
   @constant  DG  Digit:           characters between '0' and '9'.
   @constant  DT  Dot:             character '.'.
   @constant  SL  Slash:           character '/'.
   @constant  AS  Asterisk:        character '*'.
   @constant  SQ  Single quote:    character '''.
   @constant  DQ  Double quote:    character '"'.
   @constant  LB  Line break:      character '\n'.
   @constant  DL  Delimiter:       any character present in the DELIMITERS constant array.
   @constant  SS  Special symbol:  any character present in the SPECIAL_CHARACTERS constant array.
   @constant  TR  Trash:           any character which does not belong to any of the previous categories.
 */
typedef enum {
    LT,
    DG,
    DT,
    SL,
    AS,
    SQ,
    DQ,
    LB,
    DL,
    SS,
    TR
} CharType;

/*!
   @const RESERVED_WORDS
   @abstract Pre-defined array containing all reserved words.
 */
static const char* const RESERVED_WORDS[COUNT_OF_RESERVED_WORDS] = {
    
    // Types
    "void", "int", "float", "boolean", "char", "string",
    
    // Struct
    "struct", "endstruct",
    
    // Control
    "begin", "end", "if", "elsif", "else", "endif", "while", "endwhile", "for", "endfor", "return", "not",
    
    // Values
    "true", "false",
    
    // Function names
    "main", "scan", "print"
    
};

/*!
   @const SPECIAL_CHARACTERS
   @abstract This constant array groups all the special characters in the language.
 */
static const char SPECIAL_CHARACTERS[COUNT_OF_SPECIAL_CHARACTERS] = {
    
    '(' , ')' , ';' , ':' , ',' , '=', '<', '>', '!', '&', '|', '+', '-', '*', '/', '%', '[', ']'
    
};

/*!
   @const DELIMITERS
   @abstract This constant array groups all the separators in the language.
 */
static const char DELIMITERS[COUNT_OF_DELIMITERS] = {
    
    ' ', '\n', '\t', EOF
    
};

/*!
   @const NEXT_STATE
   @abstract Table indicating the automaton's transitions.
   @discussion The lines in this table represent the current state, and the columns represent the type of the input character.
 */
const LexicalAnalyzerState NEXT_STATE[COUNT_OF_LEXICAL_ANALYZERS_STATES][COUNT_OF_CHARACTER_TYPES] = {
    
  /**  LT   DG   DT   SL   AS   SQ   DQ   LB   DL   SS   TR  **/
    
    {  W1 , N1 , N2 , C1 , S1 , H1 , T1 , DF , DF , S1 , ER  },  /** ST **/
    {  NF , N1 , N3 , NF , NF , NF , NF , NF , NF , NF , ER  },  /** N1 **/
    {  ST , N4 , ER , ER , ER , ER , ER , ER , ER , ER , ER  },  /** N2 **/
    {  NF , N3 , ER , NF , NF , NF , NF , NF , NF , NF , ER  },  /** N3 **/
    {  NF , N4 , ER , NF , NF , NF , NF , NF , NF , NF , ER  },  /** N4 **/
    {  ST , ST , ST , ST , ST , ST , ST , ST , ST , ST , ER  },  /** NF **/
    {  W1 , W1 , WF , WF , WF , WF , WF , WF , WF , WF , ER  },  /** W1 **/
    {  ST , ST , ST , ST , ST , ST , ST , ST , ST , ST , ER  },  /** WF **/
    {  SF , SF , SF , SF , SF , SF , SF , SF , SF , SF , ER  },  /** S1 **/
    {  ST , ST , ST , ST , ST , ST , ST , ST , ST , ST , ER  },  /** SF **/
    {  H2 , H2 , H2 , H2 , H2 , ER , H2 , ER , H2 , H2 , H2  },  /** H1 **/
    {  ER , ER , ER , ER , ER , HF , ER , ER , ER , ER , ER  },  /** H2 **/
    {  ST , ST , ST , ST , ST , ST , ST , ST , ST , ST , ER  },  /** HF **/
    {  T1 , T1 , T1 , T1 , T1 , T1 , TF , ER , T1 , T1 , T1  },  /** T1 **/
    {  ST , ST , ST , ST , ST , ST , ST , ST , ST , ST , ER  },  /** TF **/
    {  SF , SF , SF , C2 , C3 , SF , SF , SF , SF , SF , ER  },  /** C1 **/
    {  C2 , C2 , C2 , C2 , C2 , C2 , C2 , CF , C2 , C2 , C2  },  /** C2 **/
    {  C3 , C3 , C3 , C3 , C4 , C3 , C3 , C3 , C3 , C3 , C3  },  /** C3 **/
    {  C3 , C3 , C3 , CF , C4 , C3 , C3 , C3 , C3 , C3 , C3  },  /** C4 **/
    {  ST , ST , ST , ST , ST , ST , ST , ST , ST , ST , ER  },  /** CF **/
    {  ST , ST , ST , ST , ST , ST , ST , ST , ST , ST , ST  },  /** DF **/
    {  ER , ER , ER , ER , ER , ER , ER , ER , ER , ER , ER  }   /** ER **/
    
};

/*!
   @var sourceCode
   @abstract Pointer to the source code file.
 */
static FILE* sourceCode;

/*!
   @var currentLexicalAnalyzerState
   @abstract Indicates the current state of the automaton.
 */
static LexicalAnalyzerState currentLexicalAnalyzerState = ST;

/*!
   @var buffer
   @abstract Auxiliary buffer for string construction.
 */
char* buffer = NULL;

/*!
   @var counter
   @abstract 
 */
static int counter = 0;

/*!
   @var lastLineNumber
   @abstract Number of the line of the source code currently being read.
 */
static int lastLineNumber = 1;

/*!
   @var lastCharacterNumber
   @abstract Number of the character in the line of the source code currently being read.
 */
static int lastCharacterNumber = 0;

/*!
   @var symbolTable
   @abstract Indicates which symbol table should be used to store identifiers.
 */
static SymbolTableId symbolTable;

/*!
   @function initializeLexicalAnalyzer
   @abstract Initializes the lexical analyzer with the provided file name.
   @param filename
        The filename of the source code.
   @result
        1 if the lexical analyzer could be initialized, 0 otherwise
 */
int initializeLexicalAnalyzer(const char* filename) {
    
    // Try to open file.
    sourceCode = fopen(filename, "r");
    
    // If file could be opened, continue, otherwise return 0.
    return sourceCode != NULL;
    
}

/*!
   @function lookupReservedWord
   @abstract This function is used to look for a reserved word in the table.
   @param word
        The string to look for.
   @result
        The index of the word in the table if it is found, (-1) otherwise.
 */
int lookupReservedWord(char* word) {
    
    // Iterate over the reserved words array.
    for (int index = 0; index < COUNT_OF_RESERVED_WORDS; index++) {
        
        // If the given string equals the string in the array, return the index.
        if (strcmp(word, RESERVED_WORDS[index]) == 0) return index;
        
    }
    
    // Otherwise, return -1 indicating that the word was not found.
    return -1;
    
}

/*!
   @function typeOfCharacter
   @abstract Analyzes and returns the type of the character (one among the ones defined in the CharType enumeration).
   @param character
        The character to be analyzed.
   @result
        The type of the character (CharType)
 */
CharType typeOfCharacter(char character) {
    
    if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z')) return LT;
    else if (character >= '0' && character <= '9') return DG;
    else if (character == '.') return DT;
    else if (character == '/') return SL;
    else if (character == '*') return AS;
    else if (character == '\'') return SQ;
    else if (character == '"') return DQ;
    else if (character == '\n') return LB;
    else {
        // Verify if it is a delimiter.
        for (int i = 0; i < COUNT_OF_DELIMITERS; i++) if (DELIMITERS[i] == character) return DL;
        
        // Verify if it is a special character.
        for (int i = 0; i < COUNT_OF_SPECIAL_CHARACTERS; i++) if (SPECIAL_CHARACTERS[i] == character) return SS;
    };
    
    // Return trash if character type could not be identified.
    return TR;
    
}

/*!
   @function appendCharacterToString
   @abstract Appends a character to a string in a buffer.
   @param string
        A pointer to the string to which the character must be appended.
   @param character
        The character to be appended.
 */
void appendCharacterToString(char** string, char character) {
    
    // If string is NULL, allocate it, if it is not, reallocate it with the right size.
    if (*string == NULL) {
        *string = malloc(2 * sizeof(char));
        strcpy(*string, "");
    }
    else {
        *string = realloc(*string, strlen(*string) + 2 * sizeof(char));
    }
    
    // Append the character.
    strncat(*string, &character, 1);
    
}

/*!
   @function appendStringToString
   @abstract Appends a string to another string in a buffer.
   @param string
        A pointer to the string to which the character must be appended.
   @param character
        The character to be appended.
 */
void appendStringToString(char** dest, const char* src) {
    
    // If destination string is NULL, allocate it and copy source string to it.
    if (*dest == NULL) {
        *dest = malloc(strlen(src) + 1);
        strcpy(*dest, src);
    }
    // If it is not, reallocate it with the right size and concatenate the source string.
    else {
        *dest = realloc(*dest, (strlen(src) + strlen(*dest) + 1) * sizeof(char));
        strcat(*dest, src);
    }
    
}

/*!
   @function classifyWord
   @abstract Decides whether the given word is a reserved word or a symbol, providing an index to one of the corresponding tables.
   @param word
        The word to be classified.
   @param index
        A pointer to an integer which should contain the index of the word in the reserved word table or in the symbol table at the end.
   @result
        'tokenTypeReservedWord' if the word is a reserved word, 'tokenTypeIdentifier' if it is a symbol.
 */
TokenType classifyWord(char* word, int* index) {
    
    // Verify if the word is a reserved word.
    int reservedWordIndex = lookupReservedWord(word);
    
    if (reservedWordIndex >= 0) {
        
        // If it is, set the returned index and return the reserved word token type.
        *index = reservedWordIndex;
        return tokenTypeReservedWord;
        
    } else {
        
        // If the symbol does not exist, add it to the symbol table, set the returned index and return the identifier token type.
        *index = addNewSymbolIfNonexistent(word, symbolTable);
        return tokenTypeIdentifier;
        
    };
    
}

/*!
   @function setLexicalAnalyzerSymbolTable
   @abstract Indicates to the lexical analyzer which symbol table to use when retrieving identifiers.
   @param table
        The symbol table id supposed to be used by the lexical analyzer.
 */
void setLexicalAnalyzerSymbolTable(SymbolTableId table) {
    symbolTable = table;
}

/*!
   @function generateErrorMessage
   @abstract Creates an error message indicating the position of the error in the source code.
   @param buffer
        The buffer where the error message should be allocated.
   @param errorDetail
        An additional message describing the error. This parameter must be NULL if no additional message should be displayed.
 */
void generateErrorMessage(char** buffer, const char* errorDetail) {
    
    // Standard message: message indication an error occurred. It also indicates the line and character number of the error.
    char* standardMessage = malloc(80 * sizeof(char));
    
    // Line extract: a copy of the line in which the error occurred. Only the characters before the error are displayed.
    char* lineExtract = NULL;
    
    // Pointer: a line indicating the error in the line extract.
    char* pointer = NULL;
    
    int stringLength;
    
    // Create standard message string, reallocating it to free unused memory.
    stringLength = sprintf(standardMessage, "Error found in line number %d and character %d:\n\n", lastLineNumber, lastCharacterNumber);
    standardMessage = realloc(standardMessage, stringLength);
    
    // Create line extract.
    stringLength = (lastCharacterNumber + 1) * sizeof(char);
    lineExtract = malloc(stringLength);
    fseek(sourceCode, 1 - stringLength, SEEK_CUR);
    fgets(lineExtract, stringLength + 1, sourceCode);
    appendCharacterToString(&lineExtract, '\n');
    
    // Create pointer.
    for (int i = 0; i < strlen(lineExtract) - 2; i++) {
        if (lineExtract[i] == '\t') appendCharacterToString(&pointer, '\t');
        else appendCharacterToString(&pointer, ' ');
    }
    appendStringToString(&pointer, "^\n");

    // Concatenate strings.
    appendStringToString(buffer, standardMessage);
    free(standardMessage);
    if (lineExtract != NULL) {
        appendStringToString(buffer, lineExtract);
        free(lineExtract);
    }
    if (pointer != NULL) {
        appendStringToString(buffer, pointer);
        free(pointer);
    }
    if (errorDetail != NULL) appendStringToString(buffer, errorDetail);
    
}

/*!
   @function step
   @abstract Executes a transition of the automaton and returns a type of token.
   @discussion It also updates some information about the token, such as its value, its line number and its character number.
   @param character
        The input character of the automaton.
   @param value
        The value of the token once it is defined.
   @param lineNumber
        The line number of the current token.
   @param characterNumber
        The character number of the current token.
   @result
        The identified token type.
 */
TokenType step(char character, TokenValue* value, int* lineNumber, int* characterNumber) {
    
    // Initially, the token type is undefined.
    TokenType tokenType = tokenTypeUndefined;
    
    // Get the type of the input character.
    CharType characterType = typeOfCharacter(character);
    
    // Get the current and the next states according to the type of the input character.
    LexicalAnalyzerState nextState = NEXT_STATE[currentLexicalAnalyzerState][characterType];
    
    // Verify if the next state is the error state, return tokenTypeError if it is the case.
    if (nextState == ER) {
        generateErrorMessage(&buffer, "Use of invalid character!");
        tokenType = tokenTypeError;
        value->stringValue = buffer;
    } else switch (currentLexicalAnalyzerState) {
        
    // Current state: ST (Initial state)
    case ST:
        switch (nextState) {
                    
        // Started reading a word.
        case W1:
            appendCharacterToString(&buffer, character);
            break;
                
        // Started reading a special symbol.
        case S1:
            value->charValue = character;
            break;
                
        // Started reading a number.
        case N1:
            value->intValue = character - 0x30;
            break;
                
        // Started reading a float number beginning with a dot (.).
        case N2:
            counter++;
            break;
                
        // Started reading a string
        case T1:
            appendCharacterToString(&buffer, character);
            break;
                
        // Read a delimiter.
        case DF:
            if (character == EOF) tokenType = tokenTypeEnd;
            break;
                
        default: break;
                
        }
            
        // Update line and character numbers
        *lineNumber = lastLineNumber;
        *characterNumber = lastCharacterNumber;
            
        break;
        
        // Current state: W1 (Reading a word)
        case W1:
            switch (nextState) {
                    
                // Read another letter or digit.
                case W1:
                    appendCharacterToString(&buffer, character);
                    break;
                
                // Read something else. End of token.
                case WF:
                    tokenType = classifyWord(buffer, &value->intValue);
                    break;
                    
                default: break;
            };
            break;
        
        // Current state: N1 (Reading a integer number)
        case N1:
            switch (nextState) {
                    
                // Read another digit.
                case N1:
                    value->intValue = 10 * value->intValue + character - 0x30;
                    break;
                    
                // Read a dot (.).
                case N3:
                    counter++;
                    value->floatValue = value->intValue;
                    break;
                    
                // Read something else. End of token.
                case NF:
                    tokenType = tokenTypeNumberInteger;
                    break;
                    
                default: break;
            };
            break;
        
        // Current state: N2 (Reading a float number beginning with a dot)
        case N2:
            if (nextState == N4) {
                counter++;
                value->floatValue = (character - 0x30) / 10.0;
            } else if (nextState == ST) {
                value->charValue = '.';
                tokenType = tokenTypeSpecialSymbol;
            }
            break;
            
        // Current state: N3 (Reading a float number ending with a dot)
        case N3:
            switch (nextState) {
                    
                // Read another digit.
                case N3:
                    value->floatValue += (character - 0x30) / pow(10.0, counter);
                    counter++;
                    break;
                    
                // Read something else. End of token.
                case NF:
                    tokenType = tokenTypeNumberFloat;
                    break;
                    
                default: break;
            }
            break;
            
        // Current state: N4 (Reading a float number)
        case N4:
            switch (nextState) {
                    
                // Read another digit.
                case N4:
                    value->floatValue += (character - 0x30) / pow(10.0, counter);
                    counter++;
                    break;
                    
                // Read something else. End of token.
                case NF:
                    tokenType = tokenTypeNumberFloat;
                    break;
                    
                default: break;
            }
            break;
            
        // Current state: S1 (Reading a special symbol)
        case S1:
            tokenType = tokenTypeSpecialSymbol;
            break;
        
        // Current state: H1 (Reading a character)
        case H1:
            if (nextState == H2) value->charValue = character;
            break;
            
        // Current state: H2 (Reading a character)
        case H2:
            if (nextState == HF) tokenType = tokenTypeCharacter;
            break;
            
        // Current state: T1 (Reading a string)
        case T1:
            switch (nextState) {
                
                // Read another character
                case T1:
                    appendCharacterToString(&buffer, character);
                    break;
                
                // Read quotation mark. End of token.
                case TF:
                    appendCharacterToString(&buffer, character);
                    value->stringValue = malloc((strlen(buffer) + 1) * sizeof(char));
                    strcpy(value->stringValue, buffer);
                    tokenType = tokenTypeString;
                    break;
                    
                default:
                    break;
            }
            break;
            
        // Current state: C1 (Read a slash)
        case C1:
            if (nextState == SF) {
                value->charValue = '/';
                tokenType = tokenTypeSpecialSymbol;
            }
            break;
            
        default: break;
    };
    
    // Verify if the next character of the source code should be read or not.
    if (((nextState == WF || nextState == NF || nextState == SF) && characterType != LB && characterType != DL) || currentLexicalAnalyzerState == CF || currentLexicalAnalyzerState == DF || (currentLexicalAnalyzerState == N2 && nextState == ST)) {
        
        // Make the cursor position go back one character.
        fseek(sourceCode, -sizeof(char), SEEK_CUR);
        
    } else {
        
        // If the last read character is a line break, update the line number and character number, otherwise update only the character number.
        if (characterType == LB) {
            lastLineNumber++;
            lastCharacterNumber = 0;
        } else lastCharacterNumber++;
        
    }
    
    // Go to next state.
    currentLexicalAnalyzerState = nextState;
    
    return tokenType;
    
}

/*!
   @function getNextToken
   @abstract Gets the next token from the lexical analyzer.
   @param token
        A pointer to the generated token.
 */
void getNextToken(Token** token) {
    
    TokenType tokenType;
    TokenValue tokenValue;
    int tokenLineNumber, tokenCharacterNumber;
    
    // Reset lexical analyzer's state, buffer and counter.
    currentLexicalAnalyzerState = ST;
    buffer = NULL;
    counter = 0;
    
    // Read and process character by character until a token is returned.
    do {
        
        // Get next character.
        char character = fgetc(sourceCode);
        
        // Go to next state and return a token type.
        tokenType = step(character, &tokenValue, &tokenLineNumber, &tokenCharacterNumber);
        
    } while (tokenType == tokenTypeUndefined);
    
    // Closes file if an end or error token is returned.
    if (tokenType == tokenTypeEnd || tokenType == tokenTypeError) {
        fclose(sourceCode);
        sourceCode = NULL;
    }
    
    // Create the token with the gathered information.
    createToken(token, tokenType, tokenValue, tokenLineNumber, tokenCharacterNumber);
    
}

/*!
   @function freeLexicalAnalyzer
   @abstract Frees the lexical analyzer and the associated memory blocks.
 */
void freeLexicalAnalyzer() {
    if (buffer != NULL) free(buffer);
    if (sourceCode != NULL) fclose(sourceCode);
}
