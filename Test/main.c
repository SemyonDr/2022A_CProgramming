#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "main.h"

typedef struct CRD {
    int x;
    int y;
} CRD;


/* Defines node of linked list.
   Data is stored as a pointer.
*/
typedef struct ListNode {
    void* data;
    struct ListNode* next;
} ListNode;


/* Defines Linked List structure.
   Data is stored as pointers and can be of any type. */
typedef struct List {
    ListNode* head;
    int count;
} List;

/* Creates new linked list.
   Returns:
    Empty linked list.
 */
List* CreateList() {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    list->count = 0;

    return list;
}

void ListAdd(List* list, void* data) {
    if (list != NULL) {
        ListNode* node; /* New node */
        printf("DEBUG: ~~data; Adding something to list\n");
        printf("DEBUG: ~~data; Size of node is %lu\n", sizeof(ListNode));
        /* Setting up node */
        node = (ListNode*)malloc(sizeof(ListNode));
        if (node == NULL) { perror("Failed to allocate memory"); exit(1); }
        printf("DEBUG: ~~data; \tAllocated memory for node. \n");
        node->data = data;
        printf("DEBUG: ~~data; \tCreated node. \n");
        /* Adding node to the list. */
        if (list->head == NULL) {/* If list is empty new node becomes head. */
            printf("DEBUG: ~~data; \tNode added as head. \n");
            list->head = node;
        }
        else { /* If list is not empty searching last node. */
            ListNode* cur = list->head;
            while (cur->next != NULL) {
                cur = cur->next;
                cur->next = node;
            }
            printf("DEBUG: ~~data; \tNode added as tail. \n");
        }
        /* Increasing number of elements in the list. */
        (list->count)++;
        printf("DEBUG: ~~data; Something added to list as element [%d]. \n", list->count-1);
    }
}

int main() {
    CRD** crd_arr;
    CRD* crd;
    int arr_len = 10;
    List* list;
    int i;

    crd_arr = (CRD**)malloc(sizeof(CRD*)*arr_len);
    for (i=0; i<arr_len; i++) {
        crd = (CRD*)malloc(sizeof(CRD));
        crd->x = i;
        crd->y = i+1;
        crd_arr[i] = crd;
    }

    list = CreateList();
    for (i=0; i<arr_len; i++) 
        ListAdd(list, crd_arr[i]);

    return 0;
}