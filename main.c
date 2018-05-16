#include <stdio.h>
#include <stdlib.h>
#include "mylib.c"

int main(){
    int n, chk_size;
    void *ptr5,*ptr1,*ptr2,*ptr3,*ptr4,*ptr6, *ptr7,*ptr8;

    printf("Please enter the number of bytes required and the chunk size: ");
    scanf("%d %d", &n, &chk_size);
    mem_init(n,chk_size);
    mem_init(20,16);
    mem_dump();

    ptr1=mem_alloc(50);// allocating 320 bytes actually, as the input is in terms of number of chunks required
    ptr2=mem_alloc(10);
    ptr3=mem_alloc(30);
    ptr4=mem_alloc(40);
    ptr5=mem_alloc(20);
    ptr6=mem_alloc(36);
    ptr7=mem_alloc(42);
    ptr8=mem_alloc(45);
    mem_dump();

    mem_free(ptr2);
    mem_dump();
    mem_free(ptr3);
    mem_dump();
    mem_free(ptr6);
    mem_dump();
    mem_free(ptr5);
    mem_dump();
    mem_free(ptr4);
    mem_dump();
    mem_free(ptr8);
    mem_dump();
    mem_free(ptr7);
    mem_dump();
    mem_exit();
    mem_free(ptr1);
    printf("All nodes freed!");
    mem_dump();
    ptr1=mem_alloc(65);// allocating 320 bytes actually, as the input is in terms of number of chunks required
    ptr2=mem_alloc(100);
    ptr3=mem_alloc(8);
    ptr4=mem_alloc(32);
    ptr5=mem_alloc(12);
    ptr6=mem_alloc(50);
    ptr7=mem_alloc(35);
    ptr8=mem_alloc(82);
    mem_dump();

    mem_free(ptr2);
    mem_free(ptr3);
    mem_free(ptr6);
    mem_free(ptr5);
    mem_free(ptr4);
    mem_free(ptr8);
    mem_free(ptr7);
    mem_dump();
    mem_free(ptr1);
    printf("All nodes freed!");

    mem_dump();

    mem_exit();


    return 0;
}
