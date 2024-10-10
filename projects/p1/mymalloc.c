#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "mymalloc.h"

#define MEMLENGTH 4096
#define META_SIZE sizeof(block_meta)

static union {
    char bytes[MEMLENGTH];
    double not_used;
} heap;

typedef struct block_meta {
    size_t size;
    int is_free;
    struct block_meta *next;
} block_meta;

static int heap_initialized = 0;

void leak_detector() {
    block_meta *current = (block_meta*) heap.bytes;
    int leak_found = 0;

    int num_objects = 0;
    int leaked_memory = 0;

    while ((char*)current < heap.bytes + MEMLENGTH) {
        if (!current->is_free) {
            leaked_memory += current->size + META_SIZE;
            num_objects++;
            leak_found = 1;
        }
        // Move to the next block based on its size
        current = (block_meta*)((char*)current + META_SIZE + current->size);
    }

    if (leak_found) {
        printf("mymalloc: %d bytes leaked in %d objects.", leaked_memory, num_objects);
    }
}



void init_heap() {
    block_meta *initial_block = (block_meta*) heap.bytes;
    initial_block->size = MEMLENGTH - META_SIZE;
    initial_block->is_free = 1;
    initial_block->next = NULL;
    heap_initialized = 1;
    atexit(leak_detector);
}



void *mymalloc(size_t size, char *file, int line) {
    if (size == 0){
        printf("Can't allocate nothing.");
        return NULL;
    } 

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

    fprintf(stderr, "mymalloc error at %s:%d - no memory available\n", file, line);
    return NULL;
}

void myfree(void *ptr, char *file, int line) {
    if (!ptr) {
        fprintf(stderr, "free: Inappropriate pointer %s:%d\n", file, line);
        exit(2);
    }

    // Check if the pointer is within the heap range
    if ((char*)ptr < heap.bytes || (char*)ptr >= heap.bytes + MEMLENGTH) {
        fprintf(stderr, "free: Inappropriate pointer %s:%d\n", file, line);
        exit(2);
    }

    // Iterate through the blocks to find a match for the given pointer
    block_meta *current = (block_meta*) heap.bytes;
    block_meta *target_block = NULL;
    
    while ((char*)current < heap.bytes + MEMLENGTH) {
        if ((void*)((char*)current + META_SIZE) == ptr) {
            target_block = current;
            break;
        }
        current = (block_meta*)((char*)current + META_SIZE + current->size);
    }

    // If no matching block was found, ptr is not the start of a valid block
    if (target_block == NULL) {
        fprintf(stderr, "free: Inappropriate pointer %s:%d\n", file, line);
        exit(2);
    }

    if (target_block->is_free) {
        fprintf(stderr, "free: Inappropriate pointer %s:%d\n", file, line);
        exit(2);
    }

    target_block->is_free = 1;

    // Coalesce adjacent free blocks
    current = (block_meta*) heap.bytes;
    while (current && current->next) {
        if (current->is_free && current->next->is_free) {
            current->size += META_SIZE + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}
