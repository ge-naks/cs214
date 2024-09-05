# CS214 (Systems Programming) Lecture Notes

## Lecture 1 - Course Overview
Systems Programming:

### Run time System
- standard library features like IO, memory management, etc

### What is an OS (operating systems):
- Shared implementation of commonly used features
- Management of shared resources
    - protect programs from other programs
    - protect users from other users
    - maintain abstractions
- common utilities
    - UI

### How will we learn about systems programming?
- C programming language
- Unix-style operating system (Linux, Posix)
    - posix -> portable OS
    - files and file systems
    - multitasking, process control (and how to manage them)
    - multi-threading (pthread library)
    - network communication (sockets, Posix library)

### Why C?
- C is simple
    - good visibility to lower level aspects in the computer
    - can run on low powered hardware

### C vs Java
- Similar syntax and orientation
- Java is interpreted: java source code -javac--> "bytecode" --java--> execution
    - MyClass.java --> MyClass.class
- C is compiled
    - myprogram.c -gcc--> myprogram
    - Compuler translates source code to machine code
        - Source code is readable by humans
        - machine code is binary, understood by CPU

### Types of errors in C programs
- Compile Time Error
    - Problems with the code that prevent it from being compiled
        - Things like syntax errors, type errors, naming errors

- Static Analysis
    - Determine properties that are true for all executions of a program

- Dynamic Analysis
    - Determine properties that are true fora  specific execution (the current one)
    - Can have dynamic analysis for checking if a memory reference is called that doesn't exist

- Run Time Error
    - Compiled code is executing, but issues arise during execution


### Compilation in C
- Preprocessing, source to source translation
- Compilation, translates source code to machine code
    - creates an "object file" (not related to normal programming objects), machine code that doesn't contain all of its definitions
- Linking, combines multiple object files into an executable. Links references to their definitions

- Why separate compiling and linking?
    - Recompuling source code takes time. Imagine every time you make a change to your source code, you recompile the standard C library... every time. That is time intensive. Instead, we compile one, and only recompile what was changed, and link the files

### Using GCC to compule C programs
- gcc <filename.c>
    - compile and link and create an executable named "a.out"
- gcc <filename.c> -o <desired-output-name>

- gcc -c <filename.c>
    - links and creates and creates an object file
- gcc <target-filename.o> -o <desired-executable-name>
    - gcc links and creates an executable named "hello"

- can test w/ below code:
```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
    puts("Hello, World!);
    return EXIT_SUCCESS
}
```

### Recommended Compiler Options
- Wall - all warnings
- fsanitize=address - activates AddressSanitizer, a dynamic analyzer
- fsanitize=undefined - activates UBSan (undefined behavior sanitizer)





