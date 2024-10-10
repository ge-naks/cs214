#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "mymalloc.h"


// Case 1: malloc and freeing a 1-byte object, 120 times
void workload1() {
    for (int i = 0; i < 120; i++) {
        char *ptr = (char *)mymalloc(1, __FILE__, __LINE__);
        myfree(ptr, __FILE__, __LINE__);
    }
}

// Case 2: Using malloc() to get 120 1-byte objects, storing the pointers in an array, then using free() 
void workload2() {
    char *ptrs[120];
    for (int i = 0; i < 120; i++) {
        ptrs[i] = (char *)mymalloc(1, __FILE__, __LINE__);
    }
    for (int i = 0; i < 120; i++) {
        myfree(ptrs[i], __FILE__, __LINE__);
    }
}

// Case 3: Create an array of 120 pointers.
// Repeatedly make a random choice between allocating a 1-byte object and adding the pointer to the array and deallocating a 
// previously allocated object (if any), until you have allocated 120 times. Deallocate any remaining objects
void workload3() {
    char *ptrs[120] = {NULL};
    int allocated = 0;
    while (allocated < 120) {
        if (rand() % 2 == 0) {  
            ptrs[allocated] = (char *)mymalloc(1, __FILE__, __LINE__);
            allocated++;
        } else if (allocated > 0) {  
            int index = rand() % allocated;
            myfree(ptrs[index], __FILE__, __LINE__);
            ptrs[index] = ptrs[--allocated];
        }
    }

    for (int i = 0; i < allocated; i++) {
        myfree(ptrs[i], __FILE__, __LINE__);
    }
}

//Case 4: Sequentially allocate increasingly larger chunks, then deallocate them
void workload4(){
    char *ptrs[30];
    for (int i = 0; i < 30; i++) {
        ptrs[i] = (char *)mymalloc(i + 1, __FILE__, __LINE__);  // Allocate i+1 bytes
    }
    for (int i = 0; i < 30; i++) {
        myfree(ptrs[i], __FILE__, __LINE__);  // Deallocate
    }
}

//Case 5: Allocate 60 2-byte chunks and 60 4-byte chunks, then free all in reverse order
void workload5(){
    char *ptrs[120];
    for (int i = 0; i < 60; i++) {
        ptrs[i] = (char *)mymalloc(2, __FILE__, __LINE__);  // Allocate 2 bytes
    }
    for (int i = 60; i < 120; i++) {
        ptrs[i] = (char *)mymalloc(4, __FILE__, __LINE__);  // Allocate 4 bytes
    }
    // Free all in reverse order
    for (int i = 119; i >= 0; i--) {
        myfree(ptrs[i], __FILE__, __LINE__);
    }
}

//Tests the error that occurs when freeing the same pointer twice.
void double_free() {
        char *ptr = (char *)mymalloc(1, __FILE__, __LINE__);
        myfree(ptr, __FILE__, __LINE__);
        myfree(ptr, __FILE__, __LINE__);
}


// create a memory leak
void test_memory_leak() {
    // Allocate memory without freeing it
    char *leaked_memory1 = (char*) mymalloc(100, __FILE__, __LINE__);
    char *leaked_memory2 = (char*) mymalloc(200, __FILE__, __LINE__);

    // comment these lines to create a leak

    // myfree(leaked_memory1, __FILE__, __LINE__);
    // myfree(leaked_memory2, __FILE__, __LINE__); 
}


// allocate memory over the heap size limit
void test_overallocation() {
    char *ptrs[128];
    for (int i = 0; i < 129; i++) {
        ptrs[i] = (char *)mymalloc(8, __FILE__, __LINE__);  // Allocate 2 bytes
    }

    for (int i = 0; i < 128; i++) {
        myfree(ptrs[i], __FILE__, __LINE__);  // Deallocate
    }

}

// give ptr in middle of chunk
void test_unallocated_free(){
    int x;
    free(&x);
}

void test_not_start(){
    int *p = malloc(sizeof(int)*2);
    free(p + 1);
    free(p);
}

void test_malloc_zero(){
    int *p = malloc(0);
}


int main() {
    struct timeval start, end;
    long seconds, microseconds;
    double totalTime;
    gettimeofday(&start, NULL);
    for (int i = 0; i < 50; i++) {
            workload1(); 
            workload2(); 
            workload3(); 
            workload4(); 
            workload5();
    }
    
    gettimeofday(&end, NULL);
    seconds = end.tv_sec - start.tv_sec;
    microseconds = end.tv_usec - start.tv_usec;
    totalTime = seconds + microseconds * 1e-6;    
    double average = totalTime / 50;
    printf("Average time is: %f.\n", average);


    
    // edge case testing
    // inappropriate ptr errors 
    // will cause program to exit

    //test_malloc_zero();
    //exits at double_free
    //double_free();
    //exits here
    //test_not_start();
    //test_overallocation();
    //exits here
    //test_unallocated_free();
    //test_memory_leak();

    return 0;
}