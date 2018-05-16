#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

typedef struct block_node{
int node_id ;
int size;
struct block_node *nxt;
}block_node;//The size is 16 bytes

void delete_node(block_node*);

int x=sizeof(block_node),overhead, used_size, free_size,init=0,CHUNK_SIZE,NO_OF_BYTES;
block_node *used_head=NULL,*free_head=NULL,*current_used=NULL, *current_free=NULL, *free_temp, *used_temp, *new_free, *new_used,*delete_used,*temp;

void mem_init(int no_of_bytes, int chunk_size){
    if (init==1){
        printf("Memory already initialised!\n");
        return;
    }
    if(chunk_size!=16 && chunk_size!=32 && chunk_size!=64 && chunk_size!=128){
        printf("Please enter correct chunk size!\n");
        return;
    }
    overhead=x*no_of_bytes/chunk_size;
    NO_OF_BYTES=no_of_bytes;
    used_head=(block_node*)malloc(overhead+no_of_bytes);
    free_head=used_head+1;
    used_head->node_id=0;
    free_head->node_id=0;
    used_head->nxt=NULL;
    free_head->nxt=NULL;
    used_head->size=0;
    free_head->size=no_of_bytes;
    used_size=0;
    free_size=no_of_bytes;
    CHUNK_SIZE=chunk_size;
    init=1;
    printf("Memory initialisation complete with %d bytes and chunk size %d!\n", no_of_bytes, chunk_size);
}

void mem_dump(){
    printf("\n Total free number of bytes: %d", free_size);
    printf("\n Total used number of bytes: %d", used_size);
    printf("\n--Free list--");
    current_free=free_head;
    while(current_free!=NULL){
        printf("\n Node ID number: %d \t Size: %d",current_free->node_id, current_free->size);
        current_free=current_free->nxt;
    }
    printf("\n--Used list--");
    current_used=used_head;
    while(current_used!=NULL){
        printf("\n Node ID number: %d \t Size: %d",current_used->node_id, current_used->size);
        current_used=current_used->nxt;
    }

}

void* mem_alloc(int n){
    int s,req_size=n*CHUNK_SIZE;//in bytes
    void* pntr;

    if (req_size>NO_OF_BYTES){
        printf("\n The required size is bigger than the total block size. Allocation is not possible!");
        exit(0);
    }
    printf("\n Size required is %d \n",req_size);
    used_size+=req_size;
    free_size-=req_size;
    free_temp=free_head;
    if (free_temp->nxt==NULL){//This part assigns the first node in the used list
        new_used=free_temp+1;
        new_used->node_id=1;
        new_used->size=req_size;
        new_used->nxt=NULL;
        used_head->nxt=new_used;
        new_free=new_used+1+req_size/16;
        new_free->node_id=1;
        new_free->size=free_temp->size-req_size;
        new_free->nxt=NULL;
        free_head->size=0;

    }
    else{//If the used node is not the first node
        while (free_temp->nxt->nxt!=NULL && free_temp->nxt->size<req_size)
            free_temp=free_temp->nxt;
        if (free_temp->nxt->nxt==NULL && free_temp->nxt->size<req_size){
            printf("\n Error! Not enough memory!");
            exit(0);
        }
        s=free_temp->nxt->size-req_size;
        new_free=free_temp->nxt+1+req_size/16;
        new_free->node_id=free_temp->nxt->node_id;
        new_free->size=s;
        new_free->nxt=free_temp->nxt->nxt;

        new_used=free_temp->nxt;
        new_used->size=req_size;
        new_used->nxt=NULL;

        current_used=used_head;
        while (current_used->nxt!=NULL)
            current_used=current_used->nxt;
        current_used->nxt=new_used;
        new_used->node_id=current_used->node_id+1;
    }
    free_temp->nxt=new_free;
    pntr=(void*)(new_used+1);
    printf("\n Memory allocated at %p!", pntr);
    return pntr;
}

void mem_free(void *ptr){
    int add_size;
    printf("\n --Deleting Node--\n");
    delete_used=(block_node*)(ptr)-1;
    delete_node(delete_used);
    used_size-=delete_used->size;
    free_temp=free_head;
    while (free_temp->nxt!=NULL && free_temp->nxt<delete_used)
        free_temp=free_temp->nxt;
    if (free_temp->nxt!=NULL){
        if (16*(int)(free_temp->nxt-free_temp)==delete_used->size+free_temp->size+2*x){//When the used node lies between two free nodes(node merging done here)
            free_temp->size=free_temp->size+free_temp->nxt->size+delete_used->size;
            free_temp->nxt=free_temp->nxt->nxt;
            free_size+=delete_used->size;
            while(free_temp->nxt!=NULL){
                free_temp=free_temp->nxt;
                free_temp->node_id-=1;
            }
        }
        else if (16*(int)(free_temp->nxt-delete_used)==delete_used->size+x){//When the used node is just above a free node
            delete_used->nxt=free_temp->nxt->nxt;
            delete_used->node_id=free_temp->nxt->node_id;
            free_size+=delete_used->size+16;
            delete_used->size=free_temp->nxt->size+delete_used->size+16;
            free_temp->nxt=delete_used;
        }
        else if(16*(int)(delete_used-free_temp)==free_temp->size+x){//When the used node is just below a free node
            free_temp->size=free_temp->size+delete_used->size+16;
            free_size+=delete_used->size+16;
        }
        else{//When the used node is between two used nodes
            delete_used->nxt=free_temp->nxt;
            delete_used->node_id=free_temp->node_id+1;
            free_size+=delete_used->size;
            temp=free_temp->nxt;
            while(temp!=NULL){
                temp->node_id+=1;
                temp=temp->nxt;
            }
            free_temp->nxt=delete_used;
        }
    }
    else{//In case the used node is at the end of the memory location
        free_temp->nxt=delete_used;
        free_size+=delete_used->size;
        delete_used->node_id=free_temp->node_id+1;
        delete_used->nxt=NULL;
    }
    temp=free_head;//defragmentation when all the used nodes have been freed
    if (temp->nxt->nxt==NULL){
        add_size=temp->size+temp->nxt->size;
    }
    if (add_size>=free_size){
        free_head->size=NO_OF_BYTES;
	free_size=NO_OF_BYTES;
        free_head->nxt=NULL;
    }
}

void delete_node(block_node* delete_used){
    temp=used_head;
    while (temp->nxt!=delete_used){
        temp=temp->nxt;
    }
    if (temp->nxt==NULL){
        printf("Invalid user node!");
        exit(0);
    }
    temp->nxt=delete_used->nxt;
    while (temp->nxt!=NULL){
        temp=temp->nxt;
        temp->node_id-=1;
    }
}

void mem_exit(){
    if (used_head->nxt!=NULL){
        printf("\n All the used nodes have not been deallocated!");
        return;
    }
    else{
        printf("\n All used nodes have been removed. Memory exit successful! \n");
        free(used_head);
        exit(0);
    }

}
