#ifndef DATA_H
    #define DATA_H

#include "Definitions.h"

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
    struct ListNode* head;
    int count;
} List;

/* Creates new linked list.
   Returns:
    Empty linked list.
 */
List* CreateList();

/* Adds node containing given data to a list.
   If given list does not exists (NULL) does nothing.
   Arguments:
    list    -- list to add to.
    data    -- pointer to data of new node.
 */
void ListAdd(List* list, void* data);

/* Frees memory occupied by list and its data.
   Frees every data field, every node and list 
   itself.*/
void FreeListAndData(List* list);


/* Dynamic array of integer type.
   Size of this array can be expanded by calling 
   a function. Size is expanded in steps.
   Steps are defined when creaing this structure.
   Elements can be added one after another with 
   a function. In this case array is expanded automatically.
   When user creates this array he should decide if he will
   add elements directly to ->data field and use function to expand
   capacity, or use function to add elements one after another in
   which case array will be expanded automatically.
    */
typedef struct DynArr {
   int* data;  /* Array holding data. */
   int size; /* Size of data array. */
   int step; /* Expansion step in cells. */
   int count; /* Number of added elements. */
} DynArr;

/* Creates dynamic array of integer type.
   Arguments:
    step    -- Expansion step in cells.
   Returns:
    New dynamic array of size equal step. */
DynArr* CreateDynArr(int step);

/* Expands dynamic integer array by one step.
   Arguments:
    arr  -- Dynamic array for expansion. */
void ExpandDynArr(DynArr* arr);

/* Writes data to dynamic array at position ->count
   and increases this counter.
   Arguments:
    arr     -- Dynamic array.
    data    -- New data element. */
void AddDynArr(DynArr* arr, int data);

/* Frees memory allocated for dynamic array.
   Removes data and structure.
   Arguments:
    arr  -- Dynamic array to free. */
void FreeDynArr(DynArr* arr);




/* Type of dynamic array for storing
   binary representation of the code or data
   segments. 
   Structure should be created by calling CreateBinary().
   Elements should be added by calling AddBinary()
   and array will be expanded automatically and counter will be increased.
   Elemements can be read by calling GetBinary() and set by SetBinary().
   Memory can be freed by calling FreeBinary().
    */
typedef struct BinarySegment {
   int base;     /* Address of first word in memory. */
   int counter;  /* Offset from base pointing to empty word after last added word. (also word counter) */
   /* Base+counter form an adress of a word in memory. */
   int* words;   /* Array for storing binary words. */
   int step;     /* Step of array expansion in sizeof(int). */
   int capacity; /* Current capacity of the array.*/
} BinarySegment;

/* Initializes BinarySegment dynamic array structure.
   Returns pointer to BinarySegment allocated on heap. */
BinarySegment* CreateBinary();

/* Adds new element to binary segment array.
   Automatically expands capacity and advances counter.
   Arguments:
    bin     -- Pointer to binary segment structure.
    val     -- Value to add.
*/
void AddBinary(BinarySegment* bin, int val);

/* Gets binary word from BinarySegment structure
   by given address.
   If adress is incorrect returns 0.
   Arguments:
    bin  -- Binary segment structure.
    adr  -- Address of word in binary segment.
   Returns:
    Binary word value. If adr incorrect returns 0. */
int GetBinary(BinarySegment* bin, int adr);

/* Sets binary word in BinarySegment structure.
   Only changes value of existing word, does not add new words.
   If address in incorrect does nothing.
   Arguments:
    bin  -- Binary segment structure.
    adr  -- Address of word in binary segment.
    val  -- New value. */
void SetBinary(BinarySegment* bin, int adr, int val);


/* Frees memory occupied by binary segment structure.
   Frees data and removes structure itself.
   Arguments:
    bin  -- Binary segment structure. */
void FreeBinary(BinarySegment* bin);

/* Returns next address in binary segment pointed by counter.
   Arguments:
    bin  -- Binary segment structure.
   Returns:
    Next address (bin->base + bin->counter). */
int NextSegmentAddress(BinarySegment* bin);

#endif