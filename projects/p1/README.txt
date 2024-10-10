George Nakhla gn158
Jeff Chen jc2936


Workload 1-3 follows the required test cases that the write up asks for. The first workload malloc and frees a 1-byte object 120 times. The second workload uses malloc to get 120 1-bytes objects, storing the pointers in an array, then using free. The third workload creates an array with 120 pointers and then repeatedly makes a choice between allocating a 1-byte object and adding the pointer to the array and deallocating a previously allocated object until it has been done 120 times. It then deallocates any remaining objects.

Workload 4 sequentially allocates larger chunks and then deallocates them. 

Workload 5 allocates 60 2-byte chunks and 60 4-byte chunks and then frees them in a reverse order.



test_unallocated_free, test_not_start, double_free are all edge case tests for free and will all cause the program to exit. They are commented out in memgrind.
The names are relatively self explanatory, we induce a memory leak and ensure its reported, we allocate memory more
than the size of the heap, and we attempt to free a pointer that is not pointing to the beginning of the data.


For our malloc, we first have a check to make sure if the heap is initialized. This is done by utilizing an global static int (not stored on heap) initialized as a boolean. 
In the malloc function, it always calls the initialized heap function, checking whether this int is true or false. If it is false, that means the heap is not initialized, therefore, it would create a Header which is a struct that we made. 
With this Header struct, we essentially implement a Linked List. This makes it simple to iterate through the list, check for memory leaks, and combine adjacent chunks. Our malloc creates a new header and iterates through the
For our malloc, we search for the first chunk that is allocated, and able to provide enough space for our memory. If the requested size is too large, 0, or theres no space left, we properly handle and return NULL. this is tested via our: test_overallocation

When using free, we have checks in place to ensure that the free is not being called on an unallocated block, on a ptr to something malloc didnt allocate, and
checking if we have a ptr to the middle of the chunk. Again we take advantage of the LL like structure, checking if adjacent blocks can be combined.

In our initalization function, we have an atexit() call with a memory leak test as well. We iterate through every chunk and tally up the number of still-allocated chunks
along with the size of memory they contain then report them.

