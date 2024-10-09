#include <stdio.h>

// global initalization variable
static int initialized = 0;

#define MEMLENGTH 4096
#define CHUNK_HEADER_SIZE = sizeof(chunk_header)
static union
{
    char bytes[MEMLENGTH];
    double not_used;
} heap;

typedef struct Header
{
    unsigned int size; // size of payload
    int is_free;       // allocation of memblock 1 true 0 false
} Header;

size_t align8(size_t size)
{
    return (size + 7) & ~7;
}

void init_heap()
{
    if (!initialized)
    {
        Header *first_header = (Header *)heap.bytes;
        first_header->size = MEMLENGTH - sizeof(Header); // Entire heap is initially free, minus header
        first_header->is_free = 1;
        initialized = 1;
    }
}

char *mymalloc(int size)
{
    init_heap();
    size = (size + 7) & ~7;
    Header *current = (Header *)heap.bytes;

    while ((char *)current < heap.bytes + MEMLENGTH)
    {
        if (current->is_free && current->size >= size)
        {
            size_t remaining_size = current->size - size - sizeof(Header);

            if (remaining_size > sizeof(Header))
            {
                Header *new_header = (Header *)((char *)current + sizeof(Header) + size);
                new_header->size = remaining_size;
                new_header->is_free = 1;

                current->size = size;
            }

            current->is_free = 0;
            return (void *)((char *)current + sizeof(Header));
        }

        current = (Header *)((char *)current + sizeof(Header) + current->size);
    }

    printf("");
    return NULL; // No free chunk found large enough
}



int main(int argc, char **argv)
{
    mymalloc(5);
    return 0;
}
