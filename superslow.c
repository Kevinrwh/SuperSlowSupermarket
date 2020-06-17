/*
 Kevin Ramos
 06/05/2020
 COP3502
 superslow.c
*/

/*
 This program reads in information about customers in a supermarket
 and outputs their checkout times.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMLINES 12 // size of line array
#define EMPTY -1 // signifies empty queue
#define MAXITEMS 101 // used as the standard "leastitems"

typedef struct customer{
    char* name; // stores customer name
    int noOfItems; // stores no of items customer has
    int lineNo; // line number
    int timeEntry; // stores time number they enter their queue
}customer;

// Used in forming the LL of customers for a queue
struct node{
    customer* curCust; // points to a customer struct
    struct node* next; // points to following customer in the queue
};

// holds our queue
typedef struct queue{
    struct node* front; // points to front of queue
    struct node* back; // points to back of queue
}queue;

// Needed queue functions
void init(queue* qPtr); // initializes our queue (front and back to NULL)
int enqueue(queue* qPtr, customer* curCust); // enqueues customer (takes a queuePtr and a customer)
int dequeue(queue* qPtr); // dequeues a customer and *WILL* return a pointer to the struct memory address
int empty(queue* qPtr); // checks if the queue is empty (checks if front/back are NULL)
char* front(queue* qPtr); // shows who is at the front of the queue

int main(void) {
    
    int t = 0, c = 0; // loop variable, number of customers
    
    // An array of 12 queues
    queue QueuesArray[NUMLINES];
    
    printf("enter # test cases:\n");
    scanf("%d", &c);
    
    // while loop variable is less than cases
    while(t < c)
    {
        // number of customers, first customer's index, a line number, a time number, the current time
        int n = 0, earlyIndex = 0, line = 0, time = 0, curTime = 0;
        
        // least items (comparing past), least items if none in the past, the lines in which the least items is at
        int leastItems = 0, leastItemsFut = 0, leastItemsLine = 0, leastItemsFutLine = 0;
        
        // the first customers line, dequeue boolean
        int firstCustomerLine = 0, dq = 0;
        
        // flag for dequeue loop
        int dequeueFlag = 1;
        
        printf("test case: %d\n", t+1);
        
        // initialize all the queues
        for(int i = 0; i < NUMLINES; i++)
        {
            init(&QueuesArray[i]);
        }
        
        // ask user for number of customers
        printf("enter number customers:\n");
        scanf("%d", &n);
        
        // create an array of customers of size n
        customer customerArray[n];
        
        // Read info for n customers
        for(int j = 0; j < n; j++)
        {
            char name[20]; // temporary placeholder
            
            // prompt time of entry, line number, their name, and their number of items
            printf("time, line, name, noOfItems?\n");
            scanf("%d %d %s %d", &customerArray[j].timeEntry, &customerArray[j].lineNo, name, &customerArray[j].noOfItems);
            
            // Get our start time
            if(j == 0)
            {
                time = customerArray[j].timeEntry;
                earlyIndex = j;
            }
            else
            {
                // if later index has earlier time, replace time
                if(customerArray[j].timeEntry < time)
                {
                    time = customerArray[j].timeEntry;
                    earlyIndex = j; // i think i was using this for something else but not necessary...may be redundant
                }
            }
                
            // allocate space for the name of the customer and copy it
            customerArray[j].name = malloc((strlen(name)+1)*sizeof(char));
            strcpy(customerArray[j].name, name);
            
            // verify we entered the information correctly
            printf("verification: you entered: %d %d %s %d\n", customerArray[j].timeEntry, customerArray[j].lineNo, customerArray[j].name, customerArray[j].noOfItems);
        }
        
        // set our current time based on first customer
        printf("first customer time of entry: %d\n", time);

        // Enqueue n customers to their respective queue in QueuesArray indicated by their line numbers.
        // Enqueue function takes a queue pointer and a customer struct
        for(int k = 0; k < n; k++)
        {
            //line = customerArray[k].lineNo; // could replace this later... i think i was using it for a print statement
            enqueue(&QueuesArray[customerArray[k].lineNo], &customerArray[k]);
            
            // Note the line with the first customer
            if(k == 0)
            {
                firstCustomerLine = line;
            }
        }
        
        // Check who is at the front of the line
        
        // line = 0;
//        for(int l = 0; l < n; l++)
//        {
//
//            printf("line %d\n", line);
//            printf("%s\n", front(&QueuesArray[line]));
//        }
        
        // Get current time which is the time of our first customer + 5 * their no of items.
        curTime = QueuesArray[firstCustomerLine].front->curCust->timeEntry + (QueuesArray[firstCustomerLine].front->curCust->noOfItems * 5);
        printf("time after first customer %d\n", curTime);
        
        // Dequeue first customer -- this currently just states whether it occurred and which line.
        dq = dequeue(&QueuesArray[firstCustomerLine]);
        if(dq)
        {
            printf("successful dq from line %d\n", firstCustomerLine);
        }
        else if(!dq)
        {
            printf("unsuccessful dq\n");
        }
        
        /*
         Dequeue Process:
            Iterate through queues
                If any queues have people (have a front)
                    Case 1: If there exist fronts with TOEs <= curTime
                        of these determine the first index with the least number of items
                        update curTime (equal to curTime + 5* items
                        dequeue
                    Case 2: If all TOEs are > curTime
                        determine the first index with the least number of items
                        update curTime (equal to this TOE + 5 * items
                        dequeue
                    Case 3: If all queues are empty (no front).
                        Break from while loop
         */
        
        // this isn't working right now... not showing others get dequeued
        while(1)
        {
            leastItems = MAXITEMS;
            leastItemsFut = MAXITEMS;
            int flag = 0;
            
            // go through all lines. enter those that are non-empty and check which front
            // has least items, update the leastItemsIndex.
            for(int m = 0; m < NUMLINES; m++)
            {
                // if specific queue is non-empty, compare least items at front
                // iff their TOE is less than or equal to the current time
                // will auto return the first index only
                if(!empty(&QueuesArray[m]))
                {
                    printf("queue non-empty... comparing time\n");
                    
                    if(QueuesArray[m].front->curCust->timeEntry <= curTime)
                    {
                        printf("within time\n");
                        flag = 1; // flag indicates candidate <= curTime
                        
                        if(QueuesArray[m].front->curCust->noOfItems < leastItems)
                        {
                            printf("past/present | updating least items\n");
                            leastItems = QueuesArray[m].front->curCust->noOfItems;
                            printf("least items %d\n", leastItems);
                            leastItemsLine = m;
                            printf("least items line: %d\n", m);
                        }
                    }
                    else if(flag == 0)
                    {
                        // check if we're done.
                        // if we are at the end of the queues array and there is no one there
                        // and thus, no number of items to check, break from while loop
                        if(m == NUMLINES-1 && empty(&QueuesArray[m]) == 1)
                        {
                            break;
                        }
                        // if a past or present leastItems hasn't been found
                        // compare least items anyway
                        // as soon as we find a timeEntry past or present, we'll overwrite it
                        // anyways
                        // attributing this to leastitems is fucking things up
                        if(QueuesArray[m].front->curCust->noOfItems < leastItemsFut)
                        {
                            printf("future | updating future least items\n");
                            leastItemsFut = QueuesArray[m].front->curCust->noOfItems;
                            printf("least items fut %d\n", leastItemsFut);
                            leastItemsFutLine = m;
                            printf("least items fut line: %d\n", m);
                        }
                    }
                }
            }
            
            // update our current time based on whether the customer we're taking care of
            // was before/during or after our current time.
            if(flag == 1)
            {
                curTime += QueuesArray[leastItemsLine].front->curCust->noOfItems*5;
                printf("time after this customer %d\n", curTime);
            }
            else if(flag == 0)
            {
                curTime = QueuesArray[leastItemsFutLine].front->curCust->timeEntry + (QueuesArray[leastItemsFutLine].front->curCust->noOfItems*5);
            }
            
            // dequeue the queue with leastItems at front
            dq = dequeue(&QueuesArray[leastItemsLine]);
            if(dq)
            {
                printf("successful dq from line %d\n", leastItemsLine);
            }
            else if(dq == EMPTY)
            {
                printf("unsuccessful dq\n");
                break;
            }
        }
               
        printf("done dequeueing for test case %d\n", t+1);
    }
    
    printf("done with all test cases\n");
    return 0;
}

/* I copy and pasted Guha's functions for queues but may need to alter them for my program. For example, each linked list contains a customer stract as its data.
 */

// Pre-condition: qPtr points to a valid struct queue.
// Post-condition: The struct that qPtr points to will be set up to represent an empty queue.
void init(queue* qPtr) {
     
     // Just set both pointers to NULL!
     qPtr->front = NULL;
     qPtr->back = NULL;
}

// Pre-condition: qPtr points to a valid struct queue and curCust is the customer struct to enqueue into the queue pointed to by qPtr.
// Post-condition: If the operation is successful, 1 will be returned; otherwise, no change will be made to the queue and 0 will be returned.
int enqueue(queue* qPtr, customer* curCust) {


    printf("enqueing %s\n", curCust->name);
    struct node* temp;
    
    // Allocate space for a new node to add into the queue.
    temp = (struct node*)malloc(sizeof(struct node));
    
    // This case checks to make sure our space got allocated.
    if (temp != NULL) {
             
        // Set up our node to enqueue into the back of the queue.
        temp->curCust = curCust;
        temp->next = NULL;
        
        // If the queue is NOT empty, we must set the old "last" node to point
        // to this newly created node.
        if (qPtr->back != NULL)
            qPtr->back->next = temp;
        
        // Now, we must reset the back of the queue to our newly created node.
        qPtr->back = temp;
        
        // If the queue was previously empty we must ALSO set the front of the
        // queue.
        if (qPtr->front == NULL)
            qPtr->front = temp;
        
        // Signifies a successful operation.
        return 1;
    }
    
    // No change to the queue was made because we couldn't find space for our
    // new enqueue.
    else
        return 0;
}

// Pre-condition: qPtr points to a valid struct queue.
// Post-condition: If the queue pointed to by qPtr is non-empty, then the customer at the front of the queue is deleted from the queue and 1 is returned. Otherwise, -1 is returned to signify that the queue was already empty when the dequeue was attempted.

// We want to return a pointer to the address of the customer struct so that we can get their information.
// Right now we are just returning their line number to indicate which line was dequeued.
int dequeue(queue* qPtr) {
    
    struct node* tmp;
    //struct customer* retCust; if we need to return a customer later
    
    // Check the empty case.
    if (qPtr->front == NULL)
        return EMPTY;
    
    // Store the front value to return.
    //retval = qPtr->front->data; // this will need to be changed to a customer struct
    //retCust = qPtr->front->curCust;
    
    // Set up a temporary pointer to use to free the memory for this node.
    tmp = qPtr->front;
    
    // Make front point to the next node in the queue.
    qPtr->front = qPtr->front->next;
    
    // If deleting this node makes the queue empty, we have to change the back pointer also!
    if (qPtr->front == NULL)
        qPtr->back = NULL;
        
    // Free our memory. // of the node... not the customer struct
    free(tmp);
    
    // Return 1 to show success
    return 1;
}

// Pre-condition: qPtr points to a valid struct queue.
// Post-condition: returns true iff the queue pointed to by pPtr is empty.
int empty(queue* qPtr) {
    return qPtr->front == NULL;
}

// Pre-condition: qPtr points to a valid struct queue.
// Post-condition: if the queue pointed to by qPtr is non-empty, the value stored at the front of the queue is returned. Otherwise, -1 is returned to signify that this queue is empty.
char* front(queue* qPtr) {
    if (qPtr->front != NULL)
        return qPtr->front->curCust->name;
    else
        return "EMPTY";
}


