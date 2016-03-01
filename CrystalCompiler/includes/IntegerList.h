#ifndef IntegerList_h
#define IntegerList_h

/*!
 
   @header IntegerList
 
   A linked list of integers.
 
   @author Gabriela Marques and Leonardo Mizoguti
   @updated 2015-11-29
 
 */

#include <stdio.h>
#include <stdlib.h>

// Linked list node
typedef struct IntegerListNode {
    int Integer;
    struct IntegerListNode* nextNode;
} IntegerListNode;

typedef IntegerListNode IntegerList;

// Add integer at the end of the list
int pushIntegerToList(IntegerList** list, int integer);

// Get integer at a given index
int getIntegerFromList(IntegerList* list, int index);

// Return the length of the list
int integerListLength(IntegerList* list);

#endif /* IntegerList_h */
