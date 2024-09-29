CS 214 / Project I (cont.)
==========================
exit() and atexit()
-------------------
void exit(int status);
causes process to terminate, after doing some cleanup
exit() is called after main() with the exit code returned by main
exit status is obtained by the process which initiated our process
exit() will also call the functions registered with atexit()
int atexit(void (*function)(void));
registers a function to execute after exit() is called
returns 0 if successful, non-zero otherwise
void _exit(int status);
causes process to terminate, after doing some cleanup
does not call functions registered by atexit()
void abort(void);
terminates process by sending SIGABRT
process does not terminate normally with an exit status
~~~
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void announce_termination(void)
{
puts("Lo, I am slain.");
}
int main(int argc, char **argv)
{
int method = argc > 1 ? atoi(argv[1]) : 0;
atexit(announce_termination);
switch(method) {
case 1: exit(EXIT_FAILURE);
case 2: _exit(EXIT_FAILURE);
case 3: abort();
default:
}
return EXIT_SUCCESS;
}
~~~
header and implementation
-------------------------
your project will include a header and implementation of mymalloc
mymalloc.h - function prototypes, macro definitions
client interface
should not include any internal details
type definitions
global variables
prototypes for internal functions
mymalloc.c
definitions of functions declared in the header
internal implementation details
global variables
private functions
internal types
-> functions and globals should be static
static names are unlinked
avoids name collisions
prevents external code from accessing
you will also want to write one or more client programs
these will #include "mymalloc.h"
these will link to mymalloc.o
~~~
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"
#define MEMSIZE 4096
static union {
char bytes[MEMSIZE];
double not_used;
} heap;
static int initialized;
static void leak_checker(void) { ... }
static void initialize_heap(void)
{
...
atexit(leak_checker);
...
}
void *mymalloc(size_t bytes, char *filename, int linenum)
{
if (!initialized) initialize_heap();
...
}
void myfree(void *ptr, char *filename, int linenum) { ... }
~~~
struct node {
int allocated;
int size;
};
struct node *first_header = (struct node *) heap.bytes;
int offset = first_header->size;
struct node *header = (struct node *) (heap.bytes + offset);
make
----
Use make to automate compilation
a Makefile contains variable declarations and recipes
VAR_NAME = something
target_name: list of dependencies
commands
maybe more commands
must be indented using 1 tab
program: program.c
gcc program.c -o program
$ make program
~~~
CC = gcc
CFLAGS = -std=c99 -Wall -Wvla -fsanitize=address,undefined
memtest: memtest.o mymalloc.o
$(CC) $(CFLAGS) memtest.o mymalloc.o -o memtest
memtest.o: memtest.c
$(CC) $(CFLAGS) -c memtest.c
mymalloc.o: mymalloc.c
$(CC) $(CFLAGS) -c mymalloc.c
~~~
%.o: %.c
$(CC) -c $(CFLAGS) $<
special variables
$@ - target
$^ - complete list of dependencies
$< - first dependency
~~~
CC = gcc
CFLAGS = -std=c99 -Wall -Wvla -fsanitize=address,undefined
TARGETS = memtest memgrind errorcheck
all: $(TARGETS)
memtest: memtest.o mymalloc.o
$(CC) $(CFLAGS) -o $@ $^
memgrind: memgrind.o mymalloc.o
$(CC) $(CFLAGS) -o $@ $^
errorcheck: errorcheck.o mymalloc.o
$(CC) $(CFLAGS) -o $@ $^
%.o: %.c
$(CC) $(CFLAGS) -c $<
leakymemtest.o: memtest.o mymalloc.h
$(CC) -c $(CFLAGS) -DLEAK $< -o $@
leakytest: leakymemtest.o mymalloc.o
$(CC) $(CFLAGS) $^ -o $@
mymalloc.o memtest.o memgrind.o errorcheck.o: mymalloc.h
clean:
rm -f *.o $(TARGETS)
test: memtest
./memtest
~~~
testing strategy
----------------
The purpose of testing is to uncover bugs
- want a strategy to be confident that no bugs are undiscovered
What are the requirements for your library to be correct?
e.g., mymalloc() marks a memory region in use that
- does not overlap current allocations
- will not be overlapped
How could you tell if these requirements were violated?
What test could cause this?
black- and white-box testing
----------------------------
a "black-box test" only uses the public interface of a library
#include "mymalloc.h"
a "white-box test" has access to internal features of the library
#include "mymalloc.c"
- put source code directly into test file
- don't link against mymalloc.o