#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void terminate(const char*);

// NODE PROTOTYPES //////////////////////////////////////////////////////////////

typedef struct node {
    int value;
    struct node * next;
    struct node * prev;
} node;


node* createNode(int);
int deleteNode(node*);

// DOUBLY LINKED LIST PROTOTYPES ////////////////////////////////////////////////

typedef struct {
    node * head;
    node * tail;
} doubleLinkedList;


doubleLinkedList* createDLL();
int deleteDLL(doubleLinkedList*);
int addNode(doubleLinkedList*, node*, char);
node* removeNode(doubleLinkedList*, node*);
int inList(const doubleLinkedList*, const node*);
int isEmpty(const doubleLinkedList*);
int printList(doubleLinkedList*);

// MAIN //////////////////////////////////////////////////////////////

int main() {
    doubleLinkedList * list = createDLL();

    printList(list);

    for (int i = -100; i <= 100; i++) {
        node * n = createNode(i);
        addNode(list, n, 't');
        if (i == 20) {
            n = removeNode(list, n);
            n = removeNode(list, n);
            free(n);
        }
    }
    
    node * nodo = createNode(100200);
    addNode(list, nodo, 'h');

    printList(list);                                
    deleteDLL(list);
    
    return 0;                                    
}                            

// NODE FUNCTIONS //////////////////////////////////////////////////////////////

// Creates and returns a node with val value
node* createNode(int val) {
    node * n = malloc(sizeof(node));
    if (n == NULL) terminate("Error: Malloc failed.\n");

    n -> prev = NULL;
    n -> next = NULL;
    n -> value = val;

    return n;
}


// Deletes and frees a node (pointer will be set to NULL) (returns 1 if completed, 0 if there's an error)
int deleteNode(node * n) {
    if (n == NULL) {
        fprintf(stderr, "The operation wasn't executed because the node is invalid (NULL).\n");
        return 0;
    }

    free(n);
    n = NULL;

    return 1;
}

////////////////////////////////////////////////////////////////////




// DOUBLE LINKED LIST FUNCTIONS /////////////////////////////////////////////

// Creates an empty list
doubleLinkedList* createDLL() {
    doubleLinkedList * l = malloc(sizeof(doubleLinkedList));
    if (l == NULL) terminate("Error: Malloc failed.\n");

    l -> head = NULL;
    l -> tail = NULL;

    return l;
}


// Deletes and deallocates a list (pointer will be set to NULL) and all its nodes (return 1 if completed, 0 if there is an error)
int deleteDLL(doubleLinkedList * l) {
    if (l == NULL) {
        fprintf(stderr, "The operation wasn't executed because the list is invalid (NULL).\n");
        return 0;
    }

    node * cur = l -> head;

    while (cur != NULL) {
        node * next = cur -> next;
        free(cur);
        cur = next;
    }

    free(l);
    l = NULL;

    return 1;
}


// Adds a node to a list (position = T adds as tail, adds as head otherwise) (returns 1 if completed, 0 if there is an error)
int addNode(doubleLinkedList * l, node * n, char position) {
    if (n == NULL || l == NULL) {
        fprintf(stderr, "The operation wasn't executed because the list and/or the node are invalid (NULL).\n");
        return 0;
    }

    // Empty list case
    if (isEmpty(l)) {
        n -> next = NULL;
        n -> prev = NULL;
        l -> head = n;
        l -> tail = n;
    }
    else {
        // Add as last element
        if (position == 'T' || position == 't') {
            n -> next = NULL;
            n -> prev = l -> tail;
            l -> tail -> next = n;
            l -> tail = n;
        }
        // Add as first element
        else {
            n -> prev = NULL;
            n -> next = l -> head;
            l -> head -> prev = n;
            l -> head = n;
        }
    }

    return 1;
}


// Removes a node from a list (returns the node if completed (needs to be freed by user), NULL if there is an error)
node* removeNode(doubleLinkedList * l, node * n) {
    if (n == NULL || l == NULL) {
        fprintf(stderr, "The operation wasn't executed because the list and/or the node are invalid (NULL).\n");
        return NULL;
    }

    // Empty list case
    if (isEmpty(l)) {
        fprintf(stderr, "The operation wasn't executed because the list is empty.\n");
        return NULL;
    }
    
    // Node isn't in list
    if (!inList(l, n)) {
        fprintf(stderr, "The operation wasn't executed because the node isn't in the list.\n");
        return NULL;
    }

    // 1 element
    if (l -> head -> next == NULL && l -> head == n) {
        l -> head = NULL;
        l -> tail = NULL;
    }
    // Remove head
    else if (l -> head == n) {
        l -> head = l -> head -> next;
        l -> head -> prev = NULL;
    }
    // Remove Tail
    else if (l -> tail == n) {
        l -> tail = l -> tail -> prev;
        l -> tail -> next = NULL;
    }
    // Remove element between elements
    else {
        n -> prev -> next = n -> next;
        n -> next -> prev = n -> prev;
    }

    return n;
}


// Checks wheter an element is in a list (1 if true, 0 if false, -1 if there is an error)
int inList(const doubleLinkedList * l, const node * n) {
    if (n == NULL || l == NULL) {
        fprintf(stderr, "The operation wasn't executed because the list and/or the node are invalid (NULL).\n");
        return -1;
    }

    node * cur = l -> head;

    while (cur != NULL) {
        if (cur == n) return 1;
        cur = cur -> next;
    }

    return 0;
}


// Returns 1 if a list is empty, 0 if not, -1 if there's an error
int isEmpty(const doubleLinkedList * l) {
    if (l == NULL) {
        fprintf(stderr, "The operation wasn't executed because the list is invalid (NULL).\n");
        return -1;
    }

    if (l -> head == NULL) return 1;
    else return 0;
}


int printList(doubleLinkedList * l) {
    if (l == NULL) {
        fprintf(stderr, "The operation wasn't executed because the list is invalid (NULL).\n");
        return 0;
    }

    if (isEmpty(l)) {
        printf("Empty list.\n");
        return 1;
    }

    node * cur = l -> head;

    while (cur != NULL) {
        printf("%d\n", cur -> value);
        cur = cur -> next;
    }

    return 1;
}

/////////////////////////////////////////////////////////////////////


void terminate(const char * messaggio) {
    if (errno != 0) perror(messaggio);
    else fprintf(stderr, "%s\n", messaggio);

    exit(1);
}




























// HASH TABLE ///////////////////////////////////////////////////////////////////////////

unsigned int hash_FNV1A(int key, int tableSize) {
    unsigned int hash = 2166136261u; // FNV offset basis
    hash ^= (key & 0xFF);
    hash *= 16777619;
    hash ^= ((key >> 8) & 0xFF);
    hash *= 16777619;
    hash ^= ((key >> 16) & 0xFF);
    hash *= 16777619;
    hash ^= ((key >> 24) & 0xFF);
    return hash % tableSize;
}












typedef struct {
    int size; // Number of total cells
    int counter; // Number of elements inside
    doubleLinkedList ** list; // DLL array used for storing values
} hashTable;


// Creates an empty hash table
hashTable * createHashTable() {
    hashTable * t = malloc(sizeof(hashTable));
    if (t == NULL) terminate("Error: Malloc failed.");

    int initialSize = 50;

    t -> size = initialSize; // Initial size
    t -> counter = 0; // Initial number of elements = 0

    t -> list = malloc(initialSize * sizeof(doubleLinkedList*));
    if (t -> list == NULL) terminate("Error: Malloc failed.\n");

    // Initialize all cells to empty dlls
    for (int i = 0; i < initialSize; i++) (t -> list)[i] = createDLL();

    return t;
}


// Deletes and deallocates a table (returns 1 if completed, 0 if there's an error)
int deleteHashTable(hashTable * t) {
    if (t == NULL) {
        fprintf(stderr, "The operation wasn't executed because the table is invalid (NULL).\n");
        return 0;
    }

    // Deallocates all DLLs in the list array
    for (int i = 0; i < t -> size; i++) {
        if (t -> list[i] != NULL) deleteDLL(t -> list[i]);
    }

    // Frees the list array and the table pointer
    free(t -> list);
    free(t);

    return 1;
}


// Returns 1 if the table is empty, 0 if it isn't, -1 if there's an error
int isEmpty(hashTable * t) {
    if (t == NULL) {
        fprintf(stderr, "The operation wasn't executed because the table is invalid (NULL).");
        return -1;
    }

    for (int i = 0; i < t -> size; i++) {
        if (t -> list[i] != NULL) return 0;
    }

    return 1;
}


// Checks wheter an element is in the table (returns 1 if true, 0 if not, -1 if there's an error)
int inList(hashTable * t, int n) {
    if (t == NULL) {
        fprintf(stderr, "The operation wasn't executed because the table is invalid (NULL).");
        return -1;
    }

    // ... TO DO ...

}