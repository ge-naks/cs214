# CS214(Systems Programming) Lecture 2
## C types

### C's type system is weak
- Not used for protection
- just tells the compuler what operations to use and how much space to reserve

### Numbers
- int, char, float, double
    - int and char can be signed or unsigned
    - char < short int < long int
    - short int <= int <= long int
- in the iLabs:
    - char = 1 byte (1 byte = 8 bits)
    - short int = 2 bytes
    - int 4 bytes
    - long int 8 bytes
- if you're ever unsure - sizeof(<object>) -> evaluates the the number of bytes to store the passed type
- float and double, the floating point number
    - in practice, these are IEEE 745 single-precision and double-precision
    - C compilers generally use double for intermediate FP math
    - in practice, use double unless you plan on saving space

```
int x; // compiler reserves 4 bytes
x = x + 1 // uses integer arithmetic
unsigned u; // computer will reserve 4 bytes
u = u + 1 // uses integer arithmetic

x < 10 // uses signed comparison
u < 10 // uses unsigned comparison

int x = 3;
float y = 2;
int w = 2;

float z = x / y; // promotes x's value to a float, uses float division
// z = 1.5

float z2 = x / w; // users integer division, promotes result to float
// z2 = 1.0

// we can explicity cast by writing the name of the type we want in parentheses

(float) x; // converts x to a float
((float) x ) / y
(float) (x / y)

// integer literals are implicitly signed

5 // signed regular sized
5U // unsinged int
5L // long int
5UL // unsigned long int

//character literals are also ints

// 'A' is the same as writing 65
char x = 'A'; // implicity truncates the int into a char

int x = INT_MAX
int y = x + 1 // undefined!
```

### Arrays

- One or more values of a type stored contiguously in memory
- What's the difference between an array of size 1 and an individual value?
    - There is none!
- Does not include its length
- Does not include boundaties
- Cannot check if index is in bounds
- keep track of your array!
- array variables are declared with a type and dimension
- int a[20];
    - a refers to an array of 20 integers
    - allocation and deallocation code written by the compiler
    - sizeof(a) // number of bytes used by a
    - sizeof(a) / sizeof(a[0]) // trick would work but kinda dumb
- array indexing:
    - a[i] get index from a
    - a[0] gets first element

- multi-dimensional arrays
    - int m [5][5]; // 25 integers in memory
    - m[row][col] // first index idientifies subarray, second index is the index within that subarray
    - m[row] // indentifies a single sub-array
- array dimensions must be constant expressions

### Strings
- C does not have a string type!
- Use a character array to store strings
- C's string functions assume C-style string data
    - Array of characters ending with a terminator character
    - terminator character: '\0' == 0
    - do not confuse with '0'!

- string literals implicity create C-style strings
- foo has 4 bytes, 'f' 'o' 'o' '\0'
- array initialization:
- int a[5] = {1, 2, 3, 4, 5};
    - the above is the same as:
        - a[0] = 1
        - a[1] = 2 ...
- if we use braces, can leave the dimension implicit:
    - int a[] = {1, 2, 3, 4, 5};

- char s1[] = "foo";
- char s2[20] = "foo"; // only first 4 indices are initialized
- char s3[20] = {'f', 'o', 'o', '\0'}
- note, we cant assign string literals to arrays
    - s3 = "bar"; // nope!
    - strcpy(s3, "bar"); // okay!

### Enums
- Represent ideas, really just integers under the hood
- example:
```
enum direction { north, south, east, west };
enum direction heading = north;

if (heading != south){
    // todo
}

enum direction { red, yellow, blue, green };

heading = red // allowed!
```

### Struct
- big daddy of object
- can bundle together values, refer to them by namespace
```
struct point {
    int x;
    int y;
}

struct point p;

p.x = 0
p.y = 20;
// p now represents a point at (0,20)

//structs are passed by value

void move(struct point p){
    p.x +=20; // wouldnt do anything to the *actual* struct, this only affects the local
}

struct point move(struct point p){
    p.x += 20;
    return p; // would create a new struct and return it, can be stored in main program
}
```

- fields in a struct can have any type, including other structs

```
struct circle { 
    strict point center;
    double radius;
    enum color background;
}

```

- note, structs cannot be directly recursive

```
struct node{
    data_t item;
    struct node next; // not allowed, recursiev!
}
```

- fields can be arrays

### Union
- a value that can be one of several things
```
union intorfloat {
    int i;
    float f;
};

union intorfloat x;

x.i = 5;
// x has an int

x.f = 0.0
// x has a float now
```

### TypeDef
- allows us to create a new name for an existing type (usually shorter)
- typedef struct pointer PT;

- struct point PT;

PT center;