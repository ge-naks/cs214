George Nakhla gn158
Jeff Chen jc2936


Workload 1-3 follows the required test cases that the write up asks for. The first workload malloc and frees a 1-byte object 120 times. The second workload uses malloc to get 120 1-bytes objects, storing the pointers in an array, then using free. The third workload creates an array with 120 pointers and then repeatedly makes a choice between allocating a 1-byte object and adding the pointer to the array and deallocating a previously allocated object until it has been done 120 times. It then deallocates any remaining objects.

Workload 4 sequentially allocates larger chunks and then deallocates them. 

Workload 5 allocates 60 2-byte chunks and 60 4-byte chunks and then frees them in a reverse order.

For our malloc, we first have a check to make sure if the heap is initialized. This is done by utilizing an int initialized as a boolean. In the malloc function, it would call the initialized heap function which would check whether this int is true or false. If it is false, that means the heap is not initialized, therefore, it would create a Header which is a struct that we made. With this Header struct, we have two ints, one unsigned which is the size of the memory and then another int which is a boolean for whether it is free or not.
