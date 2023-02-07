#include <stdio.h> //for input output
#include <stdlib.h> //for functions like malloc, free

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

int main() {
  int counter = 0;
  struct list myList;
  initialiseList(&myList, &counter);
  printList(&myList, &counter);
  freeList(&myList, &counter);
  printList(&myList, &counter);
}
