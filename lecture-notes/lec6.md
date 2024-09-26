# Lecture 6
## My Malloc

### Tips
```
unsigned int n;


n & ~7 // guaranteed to be a multiple of 8


11111111 11111000 ~7 in 16 bit binary
00000000 00001010 10
00000000 00001000 8
```

### Errors to detect
- Attempt to malloc() too much memory
    - Print error message, return NULL
- Attempt to free address outside the heap
- Attempt to free address within the heap, but not at the start of the object
- Attempt to free address within the heap, at the start of an object, but that object is not allocated
    - for all of these, print error message, terminate process with exit(2)


### Alignment
- All objects must begin at addresses divisible by 8 (make life easier)
- Easiest to have memory begin at a multiple of 8