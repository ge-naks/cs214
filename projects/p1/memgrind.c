#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mymalloc.h>

// Case 1: malloc and freeing a 1-byte object, 120 times
void workload1() {
    for (int i = 0; i < 120; i++) {
        char *ptr = (char *)mymalloc(1);
        myfree(ptr);
    }
}

// Case 2: Using malloc() to get 120 1-byte objects, storing the pointers in an array, then using free() 
void workload2() {
    char *ptrs[120];
    for (int i = 0; i < 120; i++) {
        ptrs[i] = (char *)mymalloc(1);
    }
    for (int i = 0; i < 120; i++) {
        myfree(ptrs[i]);
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
            ptrs[allocated] = (char *)mymalloc(1);
            allocated++;
        } else if (allocated > 0) {  
            int index = rand() % allocated;
            myfree(ptrs[index]);
            ptrs[index] = ptrs[--allocated];
        }
    }
    for (int i = 0; i < allocated; i++) {
        myfree(ptrs[i]);
    }
}

//Case 4: Sequentially allocate increasingly larger chunks, then deallocate them
void workload4(){
    char *ptrs[30];
    for (int i = 0; i < 30; i++) {
        ptrs[i] = (char *)mymalloc(i + 1);  // Allocate i+1 bytes
    }
    for (int i = 0; i < 30; i++) {
        myfree(ptrs[i]);  // Deallocate
    }
}

//Case 5: Allocate 60 2-byte chunks and 60 4-byte chunks, then free all in reverse order
void workload5(){
    char *ptrs[120];
    for (int i = 0; i < 60; i++) {
        ptrs[i] = (char *)mymalloc(2);  // Allocate 2 bytes
    }
    for (int i = 60; i < 120; i++) {
        ptrs[i] = (char *)mymalloc(4);  // Allocate 4 bytes
    }
    // Free all in reverse order
    for (int i = 119; i >= 0; i--) {
        myfree(ptrs[i]);
    }
}
}

int main() {
    struct timeval start, end;
    long seconds, microseconds;
    double totalTime;
    gettimeofday(&start, NULL);
    for (int i = 0; i < 50; i++) {
            workload1(); break;
            workload2(); break;
            workload3(); break;
            workload4(); break;
            workload5(); break;
    }
    gettimeofday(&end, NULL);
    seconds = end.tv_sec - start.tv_sec;
    microseconds = end.tv_usec - start.tv_usec;
    totalTime = seconds + microseconds * 1e-6;    
    double average = totalTime / 50;
    printf("%f", average);
    return 0;
}