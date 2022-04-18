#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define Max_size 1500 // Maximum size of heap 
//* data structure used is Double Linked List 
struct node
{
    int srt;
    int end;
    char var[100];
    struct node *next;
    struct node *prev;
    bool isFree;
};
struct heap
{
    struct node *ptr;
};


//* This function is to create heap with size of max_size
struct node *createFreeNode(int srt, int end)
{
    struct node *nptr = (struct node *)malloc(sizeof(struct node));
    if (nptr != NULL)
    {
        nptr->srt = srt;
        nptr->end = end;
        nptr->isFree = true;
        nptr->next = NULL;
        nptr->prev = NULL;
    }
    return nptr;
}
// function to check variable name in the heap
bool isNamePresent(struct heap *hp, char name[])
{
    struct node *lptr = hp->ptr;
    bool retVal = false;
    while (lptr != NULL)
    {
        if (!(lptr->isFree) && strcmp(name, lptr->var) == 0)
        {
            retVal = true;
        }
        lptr = lptr->next;
    }
    return retVal;
}

//* This function is to create heap node with required size and variable name and return pointer to it
struct node *CreateNodewithData(int srt, int end, char variable[])
{
    struct node *nptr = (struct node *)malloc(sizeof(struct node));
    if (nptr != NULL)
    {
        nptr->srt = srt;
        nptr->end = end;
        strcpy(nptr->var, variable);
        nptr->isFree = false;
        nptr->next = NULL;
        nptr->prev = NULL;
    }
    return nptr;
}

// function for allocation memory in heap
void firstfitallocation(struct heap *hptr, int siz, char name[], int i)
{
    // *  we are using first fit allocation method
    // here we get the heap pointer which is not NULL and that pointer node has required size is available

    struct node *lptr = hptr->ptr;//pointer to heap node which is not null 
    struct node *nptr = lptr, *freeptr = NULL;//creating nptr and freeptr to traverse the heap and find the free available node
    int flags = 0;
    while (nptr != NULL && !flags)
    {
        if (nptr->isFree && (nptr->end - nptr->srt + 1) >= siz)
        {
            freeptr = nptr;
            flags = 1;
        }
        nptr = nptr->next;
    }
    //here ptr is the pointer to the node which has free space so create a node with data 
    struct node *aptr = CreateNodewithData(freeptr->srt, i, name);
    if (aptr == NULL)
    {
        printf("Heap is full.\n");
    }
    else
    {
        // TODO :now we have to update the heap and insert tha data into the heap
        //*here we get two conditions :
        //1. node with size is equal to required size
        //2. node with size is greater than required size

        //case 1
        //here i is the end of the node we require
        if (i == freeptr->end)
        {
            lptr = aptr;//adding node to heap
            if (freeptr->prev != NULL)//if free node prev is not null then 
            {
                freeptr->prev->next = lptr;//next of prev ptr(free pointer) is now lptr(new node)
                lptr->prev = freeptr->prev;//prev of lptr is now prev of ptr
            }
            if (freeptr->next != NULL)
            {
                freeptr->next->prev = lptr;
                lptr->next = freeptr->next;
            }
            free(freeptr);
        }
        else
        {
            //case 2
            freeptr->srt = i + 1;
            lptr = aptr;
            if (freeptr->prev != NULL)
            {
                freeptr->prev->next = lptr;
                lptr->prev = freeptr->prev;
            }
            lptr->next = freeptr;
            freeptr->prev = lptr;
        }
        printf("\nSpace is allocated.\n\n");
        // Asign the previous node with the new node
        while (lptr->prev != NULL)
        {
            lptr = lptr->prev;
        }
        hptr->ptr = lptr;
    }
}

// function to check for the heap is full or not
bool isfull(struct heap *hp)
{
    bool retVal = false;//heap is not full
    struct node *lptr = hp->ptr;
    while (lptr != NULL && !(lptr->isFree))
    {
        lptr = lptr->next;//go to next node
    }
    if (lptr == NULL)
        retVal = true;//heap is full
    return retVal;
}

// function to check for the heap is empty or not
bool isempty(struct heap *hp)
{
    bool retVal = true;
    struct node *lptr = hp->ptr;
    while (lptr != NULL && lptr->isFree)
    {
        lptr = lptr->next;
    }
    if (lptr != NULL)
        retVal = false;
    return retVal;
}

// function to print the free nodes int the heap
void displayfreelist(struct heap *hp)
{
    if (isfull(hp))
        printf("\nNo free space available in heap.\n");
    else
    {
        struct node *lptr = hp->ptr;
        printf("\nstart \t End\n ---------------------------------------------------------------------------\n");
        while (lptr != NULL)
        { 
            if (lptr->isFree)
            {
                printf("%d \t %d\n", lptr->srt, lptr->end);
            }
            lptr = lptr->next;
        }
    }
}

//function to print the allocated nodes in the heap
void displayAllocated(struct heap *hptr)
{
    struct node *lptr = hptr->ptr;
    if (isempty(hptr))
        printf("\nNo free space available");
    else
    {   printf("startindex\tendindex\tvariable\n---------------------------------------------------------------------------\n");
        while (lptr != NULL)
        {
            if (!(lptr->isFree))
            {
                printf("\n%d \t\t %d \t\t %s", lptr->srt, lptr->end, lptr->var);
            }
            lptr = lptr->next;
        }
    }
}

// function to print the heap all nodes
void display(struct heap *hp)
{
    printf("\n memory usages and available space \n ");
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("free memory locations in heap:: \n ");
    displayfreelist(hp);
    printf("\n");
    printf("allocated memory locations in heap::\n ");
    displayAllocated(hp);
    printf("\n");
}

void deallocate(struct heap *hp, char name[])
{
    // create a node pointer for iteration
    struct node *lptr = hp->ptr, *aptr = NULL;
    bool t = false;
    //search for variable name in the heap
    while (lptr != NULL && !t)
    {
        if (!(lptr->isFree) && strcmp(name, lptr->var) == 0)
        {
            aptr = lptr;
            t = true;
        }
        lptr = lptr->next;
    }
    //we get variable
    struct node *ptr;
    aptr->isFree = true;// aptr is pointer node we want to free

    if (aptr->prev != NULL && aptr->prev->isFree)
    {
        ptr = aptr->prev;//assign prev of pointer to free = ptr
        aptr->srt = ptr->srt;//assign start of aptr to start of ptr
        if (ptr->prev != NULL)
        {
            ptr->prev->next = aptr;
        }
        aptr->prev = ptr->prev;
        free(ptr);//then  free ptr 
    }
    if (aptr->next != NULL && aptr->next->isFree)
    {
        ptr = aptr->next;
        aptr->end = ptr->end;
        if (ptr->next != NULL)
        {
            ptr->next->prev = aptr;
        }
        aptr->next = ptr->next;
        free(ptr);
    }
    lptr = aptr;
    while (lptr->prev != NULL)
    {
        lptr = lptr->prev;
    }
    printf("\nSpace is deallocated.\n");
    hp->ptr = lptr;
}

int main()
{
    struct heap newheap; // created a heap object
    struct heap *hp = &newheap;//creating the object pointer of heap and adding addresses to it

    struct node *nptr = createFreeNode(0, Max_size);// creating a node object which is free heap memory
    if (nptr == NULL)
    {
        printf("Memory allocation failed , no space available\n");
        return 0;
    }
    else
    {   //assign heap pointer to the node pointer
        hp->ptr = nptr;
        int t = 1;
        //till now we have created the heap with max size
        while (t != 0)
        {
            printf("---------------------Maximum memory storage is %d----------------------------------\n", Max_size);
            printf("enter 1-> memory allocation in heap \n");
            printf("enter 2-> memory deallocation in heap \n");
            printf("enter 3-> display heap with complete data used and available \n");
            printf("enter 4-> display heap with only available data (print allocated nodes) \n");
            printf("enter 5-> display heap with only used data (print free nodes) \n");
            printf("enter 0-> exit \n");
            printf("Enter your choice: ");
            int choice;
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
            {       //* memory allocation in heap
                int size;
                char variable[100];

                printf("Enter the variable name: ");
                scanf("%s", variable);
                printf("Enter the size of the variable: ");
                scanf("%d", &size);
                // check for the variable name present or not 
                if (isNamePresent(hp, variable))
                {
                    printf("Variable name already exists\n");
                }
                else
                {
                    //check for heap pointer of type node is null or not
                    struct node *lptr = hp->ptr;
                    if (lptr == NULL)
                    {
                        printf("No space available \n");
                    }
                    else
                    {
                        // ? if heap pointer is not null then check for the required size is available or not
                        if (lptr->srt + size > Max_size)
                        {
                            printf("No space available for given size\n");
                        }
                        else
                        {   //here i is an end for that variable we are allocating
                            int i = lptr->srt + size - 1;
                            firstfitallocation(hp, size, variable, i);
                        }
                    }
                }
                break;
            }
            case 2:
            {
                char variable1[100];
                printf("Enter the variable name: ");
                scanf("%s", variable1);
                if (isempty(hp))
                {
                    printf("No free space available\n");
                }
                else
                {
                    if (isNamePresent(hp, variable1))
                    {
                        //deallocate the variable as variable name is present
                        deallocate(hp, variable1);
                    }
                    else
                    {
                        printf("Variable name does not exist\n");
                    }
                }

                break;
            }
            case 3:
            {   
                // * display heap memory usage and available space
                display(hp);
                break;
            }
            case 4:
            {
                // * display heap memory usage 
                printf("\n memory allocated in heap\n");
                displayAllocated(hp);
                printf("\n");
                break;
            }
            case 5:
            {
                // * display heap memory  available space
                printf("\nmemory  available in heap \n");

                displayfreelist(hp);
                printf("\n");
                break;
            }
            case 0:
            {
                // * exit from the program
                t = 0;
                break;
            }
            default:
            {   
                // ! for the wrong choice
                printf("Invalid choice\n");
                break;
            }
            }
        }
    }
    return 0;
}
