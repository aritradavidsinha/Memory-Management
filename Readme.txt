The library provides following function calls that can be linked from the source code written in 'C'.

    void mem_init(no_of_bytes,chunk_size)
        Gets memory heap from the system and initializes it. This memory in turn will be used for all mem_alloc() functions in multiples         of chunks. Chunk size can be 16,32,64,128
    void* mem_alloc(int no_of_bytes)
        Allocate number of chunks from the available memory pool such that allocated memory is greater than or equal to no_of_bytes
    void mem_free(void *ptr)
        Frees up the chunks (a block) of allocated memory and makes it available for the future use
    void mem_dump()
        Output snapshot of memory usauge. Prints for each block: size in chunks and status (allocated/free)
    void mem_exit()
        Returns error if used_list is not NULL
        Otherwise de-allocates the heap.

The program maintains two lists i) used_list and ii) Free_list.
Each block node is represented by following structure.
struct block_node {
int node_id ;
int size;
struct block_node *nxt;
}
Mem_init() function initializes used_list and free_list.
First 16 bytes of the heap memory are used to store head node of the used list. Similarly next 16 bytes are used for storing first node of free list.

The main program contains an example code to show how the library functions work.


First, the memory is initialised twice to show that the 2nd mem_init returns an error message.


Then, a number of pointers are allocated. These pointers are deallocated one by one to exibhit merging of two or more nodes.


After this, we try to free the memory with one node remainin-to get an error message(as all the nodes have not yet been freed).


Then, we free the last node and call the mem_free function to clear the memory.


This entire process is repeated once for different sizes of allotted chunks.


NOTE: The size of each block can be modified by making the subsequent changes in the main.c. 
The GUI has not been developed for this program. Feel free to do so as per requirement.
