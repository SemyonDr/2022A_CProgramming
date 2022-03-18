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
    list->head = NULL;
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
            perror("Failed to allocate memory.");
            exit(1); 
        }

        node->data = data;
        node->next = NULL;

        /* Adding node to the list. */
        if (list->head == NULL) /* If list is empty new node becomes head. */
            list->head = node;
        else { /* If list is not empty searching last node. */
            ListNode* cur = list->head;
            while ((cur->next) != NULL) 
                cur = (cur->next);
            cur->next = node;
        }
        /* Increasing number of elements in the list. */
        (list->count)++;
    }
}

/* Frees memory occupied by list and its data.
   Frees every data field, every node and list 
   itself.*/
void FreeListAndData(List* list) {
    if (list != NULL) {
        if (list->count >0) {
            ListNode* prev; /* Node before cur node. */
            ListNode* cur = list->head; /* Iterator. */
            while (cur != NULL) {
                prev = cur; /* Referencing previous node. */
                cur = cur->next; /* Advancing iterator. */
                free(prev->data); /* Freeing data. */
                free(prev); /* Freeing node. */
            }
            free(list); /* Freeing list structure. */
        }
        /* If there is no nodes. */
        else {
            /* Freeing list structure. */
            free(list);
            return;
        }
    }
}

/* Creates dynamic array of integer type.
   Arguments:
    step    -- Expansion step in cells.
   Returns:
    New dinamic array of size equal step. */
DynArr* CreateDynArr(int step) {
    /* Allocating structure */
    DynArr* arr = (DynArr*)malloc(sizeof(DynArr));
    if (arr == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    /* Allocating data array */
    arr->data = (int*)malloc(sizeof(int)*step);
    if (arr->data == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }
    
    /* Setting initial values */
    arr->count = 0;
    arr->step = step;
    arr->size = step;

    return arr;
}

/* Expands dynamic integer array by one step.
   Arguments:
    arr  -- Dynamic array for expansion. */
void ExpandDynArr(DynArr* arr) {
    int newSize; /* New array size */
    int* result; /* Resulting data pointer after reallocation. */

    /* Expanding data array */
    newSize = (arr->size)+arr->step;
    result = (int*)realloc(arr->data, sizeof(int)*newSize);
    if (result == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    /* Setting new properties */
    arr->data = result;
    arr->size = newSize;
}

/* Writes data to dynamic array at position ->count
   and increases this counter.
   Arguments:
    arr     -- Dynamic array.
    data    -- New data element. */
void AddDynArr(DynArr* arr, int data) {
    /* Checking if array needs expansion. */
    if (arr->count == arr->size)
        ExpandDynArr(arr);
    
    /* Writing data */
    (arr->data)[arr->count] = data;
    (arr->count)++;
}

/* Frees memory allocated for dynamic array.
   Removes data and structure.
   Arguments:
    arr  -- Dynamic array to free. */
void FreeDynArr(DynArr* arr) {
    /* Removing data. */
    free(arr->data);
    /* Removing structure. */
    free(arr);
}

/* Initializes BinarySegment dynamic array structure.
   Returns pointer to BinarySegment allocated on heap. */
BinarySegment* CreateBinary() {
   /* Allocating structure. */
   BinarySegment* bin = (BinarySegment*)malloc(sizeof(BinarySegment));
   if (bin == NULL) {
      perror("Failed to allocate memory.");
      exit(1);
   }

   /* Setting initial values.*/
   bin->base = 0;
   bin->counter = 0;
   bin->step = 64;
   bin->capacity = bin->step;

   /* Allocating data array. */
   bin->words = (int*)malloc(sizeof(int)*(bin->capacity));
   if (bin->words == NULL) {
      perror("Failed to allocate memory.");
      exit(1);
   }

   return bin;
}

/* Adds new element to binary segment array.
   Automatically expands capacity and advances counter.
   Arguments:
    bin     -- Pointer to binary segment structure.
    val     -- Value to add.
*/
void AddBinary(BinarySegment* bin, int val) {
   /* Checking if capacity should be expanded. */
   if (bin->counter == (bin->capacity - 1)) {
      int new_cap = bin->capacity + bin->step; /* New capacity. */
      int* res; /* Result of reallocation. */
      res = (int*)realloc(bin->words, sizeof(int)*new_cap); /* Reallocating words. */
      if (res == NULL) {
         perror("Failed to allocate memory.");
         exit(1);
      }
      /* Setting new capacity. */
      bin->capacity = new_cap;
   }
   
   /* Writing new value */
   (bin->words)[bin->counter] = val;
   /* Advancing counter. */
   (bin->counter)++;
}

/* Gets binary word from BinarySegment structure
   by given address.
   If adress is incorrect returns 0.
   Arguments:
    bin  -- Binary segment structure.
    adr  -- Address of word in binary segment.
   Returns:
    Binary word value. If adr incorrect returns 0. */
int GetBinary(BinarySegment* bin, int adr) {
   /* Getting position in words array. */
   int index = adr-(bin->base);
   if (index >= 0 && index < bin->counter)
      return (bin->words)[index];
   else
      return 0;
}

/* Sets binary word in BinarySegment structure.
   Only changes value of existing word, does not add new words.
   If address in incorrect does nothing.
   Arguments:
    bin  -- Binary segment structure.
    adr  -- Address of word in binary segment.
    val  -- New value. */
void SetBinary(BinarySegment* bin, int adr, int val) {
   /* Getting position in words array. */
   int index = adr-(bin->base);
   if (index >= 0 && index < bin->counter)
      (bin->words)[index] = val;
}


/* Frees memory occupied by binary segment structure.
   Frees data and removes structure itself.
   Arguments:
    bin  -- Binary segment structure. */
void FreeBinary(BinarySegment* bin) {
   if (bin->words != NULL)
      free(bin->words);

   free(bin);
}

/* Returns next address in binary segment pointed by counter.
   Arguments:
    bin  -- Binary segment structure.
   Returns:
    Next address (bin->base + bin->counter). */
int NextSegmentAddress(BinarySegment* bin) {
    return (bin->base)+(bin->counter);
}