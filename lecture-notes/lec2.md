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
