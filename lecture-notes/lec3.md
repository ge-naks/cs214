# CS214 (Systems Programming) Lecture 3

### Pointers
- All data is stored in memory, every byte of memory is addressable
- Implication: every variable has an address

```
int i;

// &i == "the memory address of i" (technically speaking, each byte has its own address
// but we refer to the variable with its first address)
```
- Every element of a struct or array has an address
```
&point.x
&array[15]
```

- Every function has an address
    - We can get its address by writing the function name w/ no parentheses

- to actually use these addresses, we need to know what data we have
    - address's ("pointer values") are types

```
int * // type of a pointer to an integer
int *p; // holds the address of an integer

int i;
int *p;

p = &i;

// *p dereferences p. allows us to talk about the object that p points to

*p = 5; // address of i now holds 5

p = &array[5];
array[3] = *p;
```
- fun fact, array variables decay to pointers, this is how array indexing works
```
int a[6];

// a by itself and &a mean the same thing

int a[10];
int *p;

p = a;

p[0] == a[0] // same thing!
p[1] == a[1] // same thing!

p = &a[5];

p[0] == a[5];
p[1] == a[6];
p[-1] == a[4] // neat!
```

- Pointer arithmetic:
```
int a[5];
int *p;
p = a; // rn p is pointing at a[0]

p = p + 1; // same as &a[1]

*p == a[0]
*(p + 1) == a[1]

```


### Declarations and definitions


