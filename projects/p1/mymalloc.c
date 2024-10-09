#include "mymalloc.h"
#include <stdio.h>

#define MEMLENGTH 4096
#define META_SIZE sizeof(block_meta)

static union {
    char bytes[MEMLENGTH];
    double not_used; // Ensures 8-byte alignment
} heap;

typedef struct block_meta {
    size_t size;
    int is_free;
    struct block_meta *next;
} block_meta;

static int heap_initialized = 0; // Only allowed static variable


void init_heap() {
    block_meta *initial_block = (block_meta*) heap.bytes;
    initial_block->size = MEMLENGTH - META_SIZE;
    initial_block->is_free = 1;
    initial_block->next = NULL;
    heap_initialized = 1;
    atexit(leak_detector);
}


void *mymalloc(size_t size, char *file, int line) {
    if (size == 0) return NULL;

    if (!heap_initialized) {
        init_heap();
    }

    block_meta *current = (block_meta*) heap.bytes;
    size = (size + 7) & ~7; // Align size to 8 bytes

    while (current) {
        if (current->is_free && current->size >= size) {
            if (current->size >= size + META_SIZE + 8) {
                // Split the block
                block_meta *new_block = (block_meta*)((char*)current + META_SIZE + size);
                new_block->size = current->size - size - META_SIZE;
                new_block->is_free = 1;
                new_block->next = current->next;
                current->size = size;
                current->next = new_block;
            }
            current->is_free = 0;
            return (void*)((char*)current + META_SIZE);
        }
        current = current->next;
    }

    fprintf(stderr, "malloc error at %s:%d - no memory available\n", file, line);
    return NULL;
}

void myfree(void *ptr, char *file, int line) {
    if (!ptr) {
        fprintf(stderr, "free error at %s:%d - NULL pointer\n", file, line);
        return;
    }

    block_meta *block = (block_meta*)((char*)ptr - META_SIZE);

    if (block->is_free) {
        fprintf(stderr, "free error at %s:%d - double free detected\n", file, line);
        return;
    }

    block->is_free = 1;

    // Coalesce adjacent free blocks
    block_meta *current = (block_meta*) heap.bytes;
    while (current && current->next) {
        if (current->is_free && current->next->is_free) {
            current->size += META_SIZE + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}


void leak_detector() {
    block_meta *current = (block_meta*) heap.bytes;
    int leak_found = 0;

    while ((char*)current < heap.bytes + MEMLENGTH) {
        if (!current->is_free) {
            printf("Memory leak detected: Block at address %p, size %zu bytes\n",
                   (void*)((char*)current + META_SIZE), current->size);
            leak_found = 1;
        }
        // Move to the next block based on its size
        current = (block_meta*)((char*)current + META_SIZE + current->size);
    }

    if (!leak_found) {
        printf("No memory leaks detected.\n");
    }
}


















// #include <stdio.h>

// // global initalization variable
// static int initialized = 0;

// #define MEMLENGTH 4096

// #define CHUNK_HEADER_SIZE = sizeof(chunk_header)

// static union
// {
//     char bytes[MEMLENGTH];
//     double not_used;
// } heap;

// typedef struct Header
// {
//     unsigned int size; // size of payload
//     int is_free;       // allocation of memblock 1 true 0 false
// } Header;

// size_t align8(size_t size)
// {
//     return (size + 7) & ~7;
// }

// void init_heap()
// {
//     if (!initialized)
//     {
//         Header *first_header = (Header *)heap.bytes;
//         first_header->size = MEMLENGTH - sizeof(Header); // Entire heap is initially free, minus header
//         first_header->is_free = 1;
//         initialized = 1;
//     }
// }

// char *mymalloc(int size)
// {
//     init_heap();
//     size = (size + 7) & ~7;
//     Header *current = (Header *)heap.bytes;

//     while ((char *)current < heap.bytes + MEMLENGTH)
//     {
//         if (current->is_free && current->size >= size)
//         {
//             size_t remaining_size = current->size - size - sizeof(Header);

//             if (remaining_size > sizeof(Header))
//             {
//                 Header *new_header = (Header *)((char *)current + sizeof(Header) + size);
//                 new_header->size = remaining_size;
//                 new_header->is_free = 1;

//                 current->size = size;
//             }

//             current->is_free = 0;
//             return (void *)((char *)current + sizeof(Header));
//         }

//         current = (Header *)((char *)current + sizeof(Header) + current->size);
//     }

//     printf("");
//     return NULL; // No free chunk found large enough
// }

// void leak_detector() {
//     Header *current = (Header *)heap.bytes;
//     while (current != NULL) {
//         if (!current->is_free) {
//             printf("Memory leak detected: %zu bytes not freed\n", current->size);
//         }
//         current = (Header *)((char *)current + sizeof(Header) + current->size);
//     }
// }

// void free(ptr){

// }



// int main(int argc, char **argv)
// {
//     mymalloc(5);
//     return 0;
// }
