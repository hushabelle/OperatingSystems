#include <stdio.h> //for input output
#include <stdlib.h> //for functions like malloc, free
#define BLOCKSIZE 1 //macro
#define maxInput  (5 * BLOCKSIZE) //macro
struct node {
    //integer stored in the node
    int i;
    //right ptr to next node
    struct node *next;
    //left ptr to prev node
    struct node *prev;
};

struct list {
    //ptr to left most node
    struct node *head;
    //ptr to right most node
    struct node *tail;
    //ptr to last added even integer
    struct node *right;
    //ptr to last added odd integer
    struct node *left;
    //no of nodes in free space
    int length;
};

//void* generic ptr type
void *allocator(int size, int *counter) { //size = bytes to be allocated
    void *ptr = malloc(size);
    //ptr not null
    if (ptr) {
        (*counter)++;
    }
    return ptr;
}

//frees space
void deAllocator(void *p, int *counter) {
    if (p) {
        free (p);
        (*counter)--;
    }
}

void initialiseList(struct list *pList, int *counter) {
    //allocate memory for head and tail of the list
    pList->head = (struct node*) allocator(sizeof(struct node), counter);
    pList->tail = (struct node*) allocator(sizeof(struct node), counter);
    //set the next and prev pointers of the head and tail
    pList->head->next = pList->tail;
    pList->tail->prev = pList->head;
    //set the prev pointer of the head to null
    pList->head->prev = NULL;
    //set the next pointer of the tail to null
    pList->tail->next = NULL;
    pList->head->i = 0;
    pList->tail->i = 0;
    pList->left = pList->head;
    pList->right = pList->tail;
    pList->length = 0;
}

void freeList(struct list *pList, int *counter) {
    if (pList->left == pList->head && pList->right == pList->tail && pList->length == 0) {
        deAllocator(pList->head, counter);
        deAllocator(pList->tail, counter);
        pList->head = NULL;
        pList->tail = NULL;
        pList->left = NULL;
        pList->right = NULL;
        pList->length = -1;
    }
}

//code from CS2850 moodle plain text code20221213 Appendix Listing 6
//prints values of nodes in linked list, and value of counter variable
void printList(struct list *pList, int *counter) { 
    //ptr to node, initialised to the head
    struct node *cur = pList->head;
    //iterate through list starting from head
    while (cur) {
        printf(" | %d | ", cur->i);
        //increment ptr
        cur = cur->next; 
    }
    //check if the head or tail pointers are null, or if the counter variable is equal to 0
    if (!(pList->head) || !(pList->tail) || *counter == 0) {
        printf(" | | ");
    }
    printf("\ncounter = %d\n", *counter); 
}

//inserts block in the middle
void allocateBlock(struct list *pList, int *counter, int nNodes) {
    for(int i = 0; i < nNodes; i++) {
        struct node *newNode = (struct node*) allocator(sizeof(struct node), counter);
        //link new node to the existing ones
        //should only mention left or right throughout the assignments because mentioning left&right assumes nothing else exists between them
        //this allocation adds to the end of the list before the right
        newNode->prev = pList->right->prev;
        newNode->next = pList->right;
        pList->right->prev->next = newNode;
        pList->right->prev = newNode;
        //set value of new node to -1
        newNode->i = -1;
        //increment length of the list
        pList->length++;
    }
}

//code from CS2850 moodle plain text code20221213 Appendix Listing 7
void deAllocateBlock(struct list *pList, int *counter, int nNodes) {
    int n = (pList->length) - nNodes;
    while ((pList->length) > n) {
        struct node *temp = pList->left->next;
	    pList->left->next->next->prev = pList->left;
        pList->left->next = temp->next;
        deAllocator(temp, counter);
        (pList->length)--;
    }
}

//inserts integer into free space
void pushInt(struct list *pList, int *counter, int i) {
    //if list has no free space, increase size
    if (pList->length == 0) {
        allocateBlock(pList, counter, BLOCKSIZE);
    }
    //if odd, insert to the end of left, change most recent left to the new node
    if (i % 2 == 1) {
        pList->left->next->i = i;
        pList->left = pList->left->next;
    }
    //if even, insert to the start of right, change most recent right to the new node
    if (i % 2 == 0) {
        pList->right->prev->i = i;
        pList->right = pList->right->prev;
    }
    //decrement amount of free space
    pList->length--;
}

//removes integer from most recently added
void pullInt(struct list *pList, int *counter, int i) {
    //removes from left stack
    if (i % 2 == 1) {
        if (pList->left != pList->head) {
            pList->left->i = -1;
            pList->left = pList->left->prev;
            //increment amount of free space
            pList->length++;
        }
    }
    //removes from right stack
    if (i % 2 == 0) {
        if (pList->right != pList->tail) {
            pList->right->i = -1;
            pList->right = pList->right->next;
            pList->length++;
        }
    }
    //removes free nodes
    if (pList->length > BLOCKSIZE) {
        deAllocateBlock(pList, counter, BLOCKSIZE);
    }
}

void clearList(struct list *pList, int *counter) {
    //iterates through left(odd)
    while (pList->left != pList->head) {
        pullInt(pList, counter, 1);
    }
    //iterates through right(even)
    while (pList->right != pList->tail) {
        pullInt(pList, counter, 0);
    }
    //removes free nodes and frees list
    deAllocateBlock(pList, counter, pList->length);
    freeList(pList, counter);
}

int getInput(char *s) {
    //null is '0', not "0" as thats a string
    char c = '\0';
    int i = 0;
    while (c != '\n' && i < maxInput) {
        c = getchar();
        if (c >= '0' && c <= '9') {
            s[i] = c;
            i++;
        }
    }
    if (i == maxInput) {
        while (c != '\n') {
            c = getchar();
            break;
        }
    } 
    return i;
}

int main() {
    int counter = 0;
    struct list myList;
    initialiseList(&myList, &counter);
    printList(&myList, &counter);
    char sPush[maxInput];
    char sPull[maxInput];
    int lenPush = getInput(sPush);
    int lenPull = getInput(sPull);
    int i = 0;
    int j = 0;
    while ((i + j) < (lenPush + lenPull)) {
    if (i < lenPush) {
        //have to subtract the ASCII value, i.e. '0' which is a value of 48
        pushInt(&myList, &counter, sPush[i] - '0');
        i++;
        printList(&myList, &counter);
    }
    if (j < lenPull) {
        pullInt(&myList, &counter, sPull[j] - '0');
        j++;
        printList(&myList, &counter);
    }
    }
    printList(&myList, &counter);
    clearList(&myList, &counter);
    printList(&myList, &counter);
}
