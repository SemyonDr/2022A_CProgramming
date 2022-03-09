#include <stdlib.h>
#include <stdio.h>
#include "Data.h"

/* Creates new linked list.
   Returns:
    Empty linked list. */
List* CreateList() {
    /* Creating structure */
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    /* Setting initial value */
    list->count = 0;

    return list;
}

/* Adds node containing given data to a list.
   If given list does not exists (NULL) does nothing.
   Arguments:
    list    -- list to add to.
    data    -- pointer to data of new node. */
void ListAdd(List* list, void* data) {
    if (list != NULL) {
        ListNode* node; /* New node */
        /* Setting up node */
        node = (ListNode*)malloc(sizeof(ListNode));
        if (node == NULL) { 
            perror("Failed to allocate memory");
            exit(1); 
        }

        node->data = data;
        
        /* Adding node to the list. */
        if (list->head == NULL) /* If list is empty new node becomes head. */
            list->head = node;
        else { /* If list is not empty searching last node. */
            ListNode* cur = list->head;
            while (cur->next != NULL)
                cur = cur->next;
            cur->next = node;
        }
        /* Increasing number of elements in the list. */
        (list->count)++;
    }
}

/* Creates dynamic array of integer type.
   Arguments:
    step    -- Expansion step in cells.
   Returns:
    New dinamic array of size */
DArrayInt* CreateDArrayInt(int step) {
    /* Allocating structure */
    DArrayInt* ar = (DArrayInt*)malloc(sizeof(DArrayInt));
    if (ar == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    /* Allocating data array */
    ar->data = (int*)malloc(sizeof(int)*step);
    if (ar->data == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }
    
    /* Setting initial values */
    ar->count = 0;
    ar->step = step;
    ar->size = step;

    return ar;
}

/* Expands dynamic integer array by one step.
   Arguments:
    dArray  -- Dynamic array for expansion. */
void ExpandDArrayInt(DArrayInt* dArray) {
    int newSize; /* New array size */
    int* result; /* Resulting data pointer after reallocation. */

    /* Expanding data array */
    newSize = (dArray->size)+dArray->step;
    result = (int*)realloc(dArray->data, sizeof(int)*newSize);
    if (result == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    /* Setting new properties */
    dArray->data = result;
    dArray->size = newSize;
}

/* Writes data to dynamic array at position ->count
   and increases this counter.
   Arguments:
    dArray  -- Dynamic array.
    data    -- New data element. */
void DArrayIntAdd(DArrayInt* dArray, int data) {
    /* Checking if array needs expansion. */
    if (dArray->count == dArray->size)
        ExpandDArrayInt(dArray);
    
    /* Writing data */
    (dArray->data)[dArray->count] = data;
    (dArray->count)++;
}

/* Frees memory allocated for dynamic array.
   Removes data and structure.
   Arguments:
    dArray  -- Dynamic array to free. */
void DArrayIntFree(DArrayInt* dArray) {
    /* Removing data. */
    free(dArray->data);
    /* Removing structure. */
    free(dArray);
}