# CS214 (Systems Programming) Lecture 8

## Memory Operations

### Const Keyword
- Disallows assignment to a given variable
    ```
    const int x = 123;
    ```
- technically we could reassign the value that is stored at this address/dereference it but dont lol
- can assign consts to pointers as well
    - p points to a location golding a const int
    - we cannot assign to *p
    ```
    const int *p

    int compare(const void *p, const void *q){
        // we can read *p but not ite to it
    }

    int something;
    int *const q = &something;

    // we cannot assign to q, q will always point to something
    const int *const w = &something

    // we cannot assign to w or *w 
    ```
```
int * const a = malloc(200 * sizeof(int));

free(a)

//ex

void qsort(
    void @ptr_to_array,
    size_t number_of_array_elements,
    size_t size_of_array_element,
    int (*comparison_function)(const void *, const void *)
)
```

### string.h
- library of useful functions for dealing w strings (and memory)
```
size_t strlen(char *);
// determine length of string (iterate until '\0')
size_t strnlen(char *, size_t);
// determine length of string (iterate until '\0' or max length)


char *strcpy(char *dest, char *source);
// copy chars from source to dest
// iterates until it finds '\0' in source
// (makes sure dest is long eought)
// returns dest for some reason

char *strcar(char *dest, char *source);

// copies chars from source to end of dest
// first iterates through dest to find '\0'

void *memcpy(void *dest, void *src, size_t bytes);
// copies the specified number of bytes from the source to the destination


char *s = ...;

unsigned slen = strlen(s);
char *copy = malloc(slen + 1);
memcpy(copy, s, s + 1);

char *strdup(char *string);
// dupes string (POSIX 2008 or later)


void *mymemcpy(vois *d, void *s, size_t bytes){
    size_t i;
    char *D = d;
    char *S = s;

    for (i = 0; i < bytes; i++){
        D[i] = S[i]
    }

    return d;
}

```

