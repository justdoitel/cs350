//THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - EL YIRDAW

#include "my_malloc.h"
#include <unistd.h>

FreeListNode head = NULL;
MyErrorNo my_errno=MYNOERROR;

FreeListNode removefromfree(size_t size) { //find and return a block in the free list that is equal to or bigger than than the necessary size (null if no fit found)
    if (head == NULL) return NULL;
   
    FreeListNode curr = head;  
    FreeListNode prev = NULL; 

    while (curr!=NULL){ 
        if(curr->size>=size){ //if we found a good fit, remove and return it
            if (prev==NULL) head = curr->flink; 
            else prev->flink = curr->flink;
            return curr;
        } else{ //else go to the next node
            prev = curr;
            curr=curr->flink;
        }
    }
    return NULL; //return null if no fit found
}

void my_free(void *ptr) { //frees a pointer given 
    uint32_t *blockbegins = (uint32_t *)ptr-2; //the block begins two above
    if ((void*)(blockbegins+1) > sbrk(0) || *(blockbegins+1) != 0xcafecafe || (*blockbegins)<16) { //returns error if memory isn't allocated by my_malloc
        my_errno = MYBADFREEPTR;
        return;
    }
    
    size_t size = *blockbegins; //the size stored in the block is obtained
    
    FreeListNode newnode = (FreeListNode) blockbegins; //block casted into a node "newnode" to be in the free list
    newnode->size = size; //the new node gets a size
    
    if (head == NULL || head >= newnode){ //if the newnode belongs at the beginning, we put it there
        newnode -> flink = head;
        head = newnode;
        return;
    }

    FreeListNode curr = head;  
    while (curr->flink != NULL && curr->flink < newnode) { //we traverse the list until we find where the newnode belongs
        curr = curr->flink;
    }

    //put the newnode where it belongs
    newnode->flink = curr->flink; 
    curr->flink = newnode;
}

void* my_malloc(uint32_t size){ // mallocs the size given
    if(size<0) {
        my_errno = MYENOMEM;
        return NULL; 
    }
    size+=8; //increased by 8 for bookkeeping
    size = 8*(1 + ((size -1) / 8)); //rounded up to the nearest 8-divisible number
    void * toreturn = NULL; 
    if(size<16) size = 16; //minimum size is 16
    FreeListNode freenode = removefromfree(size); //find a node in the freelist that's at least size lengths
    if (freenode!=NULL){ //if a match is found, assign it
        toreturn = freenode;

        if(freenode->size-size>=16){ //if we get more than 16 bytes of what we need, we put a header on the rest and free it.
            *((uint32_t *)(toreturn+size)) = freenode->size-size;
            *((uint32_t *)(toreturn+size+4)) = 0xcafecafe;
            my_free(toreturn+size+8);
        } else { //if we've gotten less than 16 bytes of what we need, we just keep it to pad
            size = freenode->size;
        }

    } else { //if a match isn't found
        if (size<=8192){ //if the size is less than a page, we ask for a page and store extra we may have
                    toreturn = sbrk(8192);
                    if(toreturn == (void *)-1) {
                        my_errno = MYENOMEM;
                        return NULL; 
                    }
                    
                    if (8192-size>=16){ //if we get more than 16 bytes of what we need, we put a header on the rest and free it.
                        *((uint32_t *)(toreturn+size)) = 8192-size;
                        *((uint32_t *)(toreturn+size+4)) = 0xcafecafe;
                        my_free(toreturn+size+8);
                                
                    } else { //if we've gotten less than 16 bytes of what we need, we just keep it to pad
                        size = 8192;
                    }                                
        } else { 
            toreturn = sbrk (size); //we just use sbrk and return it (no need to free since the system hasn't allocated the entire page for us)
            if(toreturn == (void *)-1){
                my_errno = MYENOMEM;
                return NULL;
            }
        }
    }
    //bookkeeping
    *((uint32_t *) toreturn) = size; 
    *((uint32_t *) (toreturn+4)) = 0xcafecafe;
    return toreturn + 8; //return the actual memory a user can use 
}


FreeListNode free_list_begin(void) {
    return head;
}

void coalesce_free_list(void) { //loop through every free_list to see if they're next to each other and merge them if so
    FreeListNode node = head;
    while (node != NULL) {
        if ((void *)node + node->size == node->flink) {
            node->size += node->flink->size;
            node->flink = node->flink->flink;
        } else {
            node = node->flink;
        }
    }
}

