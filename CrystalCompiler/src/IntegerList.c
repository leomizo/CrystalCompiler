/*!
 
   IntegerList.c
 
   Authors: Gabriela Marques and Leonardo Mizoguti
   Updated: 2015-11-29
 
 */

#include "IntegerList.h"

int pushIntegerToList(IntegerList** list, int integer) {

    IntegerListNode* newNode = malloc(sizeof(IntegerListNode));
    newNode->Integer = integer;
    newNode->nextNode = NULL;
    
    if (*list == NULL) {
        *list = newNode;
        return 0;
    } else {
        int index = 0;
        IntegerListNode* node = *list;
        while (node->nextNode != NULL) {
            node = node->nextNode;
            index++;
        }
        node->nextNode = newNode;
        return index;
    }
    
}

int getIntegerFromList(IntegerList* list, int index) {
    IntegerListNode* node = list;
    for (int i = 0; i < index; i++) node = node->nextNode;
    return node->Integer;
}

int getLastNode(IntegerList* list, IntegerList** lastNode) {
    *lastNode = list;
    int index = 0;
    for (index = 0; (*lastNode)->nextNode != NULL; (*lastNode) = (*lastNode)->nextNode) index++;
    return index;
}

int integerListLength(IntegerList* list) {
    if (list == NULL) return 0;
    else {
        IntegerListNode* node;
        return getLastNode(list, &node) + 1;
    }
}

