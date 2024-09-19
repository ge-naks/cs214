# CS214 (Systems Programming) Lecture 5

## Pointers

### recap pointers:
- a typed address
    - an address is a number, known at runtime
    - Type is what indicates the type of data that is at the expected address, known at compile time
```
int a[20], b[10], c[5];

int *p[3] = {a, b, c};

int **q = &p;

```

### Special pointers
- Null pointers ```NULL```
    - A pointer that does not explicitly point to anything
    - Cannot be dereferenced. if you do will lead to a segmentation fault/SEGV (usually)
    - ```NULL``` is considered false, all non null pointers are true
    ```
    some_type *p = some_function();
    if (p){
        // execute
    }
    ```
- Void pointers:
    - ```void *p```
    - untyped address
    - indicated a location in memory but does not say what sort of thing is there
    - cannot be dereferenced
    - what can we do?
        - compare void pointers (do they point to the same address)
        - cast to ome other pointer type (assuming you know that correct type)
        ```
        void *p = foo()
        int *i = p;
        float *f = p;
        // bad !
        ```

- Function pointers
    - Every functions has an address in memory
    - the name of a function is a pointer to that fucntion. the () after the name is what causes the function call
    ```
    int inc(int x){ return x + 1; }

    int (*op)(int);
        // op will hold a pointer to the function that return int
        // and takes in
    
    op = inc;

    op(5); // same as inc(5)
    ```

### The heap
- Dynamic allocation occurs on the heap
- The number of things to allocate may not be known until runtime
- the size of things to allocate may not be known until runtime
- functions can create objects hat live longer than the function
    ```
    void *malloc(size_t);
    // argument : zie of object desired in bytes
    // returns pointer to that object (or NULL if object cant be created)
    ```
- Let's look at a dynamically sized array of ints
```
int n = obtain_number_from_user();

int *array = malloc(n * sizeof(int));

array[0] = 5
// unsafe way of doing it!
```
- we can use calloc as well
```
void *calloc(size_t, size_t);
    // similar to malloc() excelpt we provide two numbers that will be multiplied
    // "clear" allocate
    // all the bytes in the objects are set to 0


    int *array = malloc(n * sizeof(int));

    int *array = calloc(n, sizeof(int));
        // all values in the array are already 0
```
- just like how we can allocate memory, we can also deallocate (free) memory
```
void free(void *)
```
- takes a pointer previously returned by malloc (or similar)
- deallocated the specified object
- marks the object not in use
- allows the space to be used by future calls to malloc
- its good practice to free() objects as soon as they will not be needed
- free can only be used to deallocate complete objects allocated by objects allocated by malloc/calloc

- last one: ```void *realloc(void *, size_t)```
    - (simulates) changing the size of an object
    - if the size is smaller than the object, the object will be truncated
    - if the size is larger than the object, it will either:
        - make the object larger
        - allocate a new object and copy your data
        - because of this, its only safe to use realloc when we have 1 pointer pointing to this object
    ```
    array = realloc(array, n * 2 * sizeof(int))


    typedef struct {
        unsigned length;
        unsigned capacity;
        int *data;
    } arraylist_t;

    void al_init(arraylist_t *A, unsigned capacity){
        A->length = 0;
        A->capacity = capacity;
        A->data = malloc(sizeof(int) * capacity);
    }

    void al_append(arraylist_t *A, int item){
        if (A->length == A->capacity){
            A->capacity *= 2;
            int *new_array = realloc(A->data, sizeof(int) * A->capacity);
            if (new_array == NULL) { die probably }
            A->data = new_array
        }

        A->data[length] = item;
        A->length++;
    }

    ```
