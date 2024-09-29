#include <stdio.h>

// global initalization variable
static int init = 0;

#define MEMLENGTH 4096

static union {
char bytes[MEMLENGTH];
double not_used;
} heap;


struct metadata {
    unsigned int payload; // size of payload
    int allocated;  // allocation of memblock 1 true 0 false
};

void get8(int n){
    n & ~7;
}



void init_heap(){
    // init 1 header and chunk
    if(!init){
    struct metadata *header;

    header->payload = MEMLENGTH - 8;
    header->allocated = 0;
    init = 1;

    int n = sizeof(header);
    printf("%d", n);
    }
}



char* mymalloc(int memory){
    init_heap();

}


int main(int argc, char** argv){

    mymalloc(5);
    return 0;
}





