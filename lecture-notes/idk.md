# Lecture IDK anymore

## Threads

### Tasks
- An instance of a fetch execute cycle
- Has an instruction pointer, usually a stack

### Process
- Has at least one task
- Virtual memory space

### Multi Tasking
- a multi taksing environment runs multiple tasks "at the same time"
- we can switch between multiple tasks on a single processor
- can schedule tasks on different processors (multiprocessing)

### Virtual Memory
- Allows for better safety and isolation between programs
- think - each program writes to "virtual" memory. process A's virtual address is 1000 and address B's virtual is 1000, but the physical memory address is different
- this isolation makes it difficult for processes to
- this is where threads come in
- threads are multiple tasks within a single process
    - shared virtual memory!
- multi threading allows for interference and miscommunication (danger!)
- two major kinds of multithreading
    - OS threads / Kernal Threads
        - Threads visible to OS
        - Scheduling handled by OS
        - OS can schedule threads on separate processors
    - Green threads / library threads / language threads
        - Threading implemented within programming language
        - Process schedules threads itself
        - lower overhead than OS threads
    

### Need for ~~Speed~~ Synchronization
- Race conditions
    - Outcome of program depends on which threads finishes first
    - Non-determinism introduced by scheduler

    ```
    Thread A   Thread B
     X = 1      X = 2

    does it print 1 or 2?
    Yes!
    Non-deterministic, CPU scheduler is what runs it and there is an element of randomness
    ```

- Data race
    - Multiple threads make unynchronized accesses (reads/writes) to a piece of data (at least one writing)
    - C programs containing data races are undefined. dont use them


```
// Thread A
for( int i = 0; i < 100; i ++){
    X = 1;
    A[i] = X;
}

// Thread B
X = 0;
```
- Possible ourcomes:
    - no 0s in A
    - one 0 in A

```
// Thread A OPTIMIZED
X = 1;
for( int i = 0; i < 100; i ++){
    A[i] = X;
}

// Thread B
X = 0;
```
- Now 101 possible outcomes when run with thread B
    - A is all 1s
    - A is all 0s
    - A starts out w a 1 then all 0s

- Only 2 of these outcomes could occur with the unoptomized code

We want to allowed the compuler to make optimizations like these so instead C forbids data races

Soln dont have data races
- forbid shared mutable data (like rust)

- synchronize sccess to shared data
    - enforce sequential access

for example, a memory fence is an instrictio that pases a thread untill all pending writes have finalized

knowing where and when to use a memory fence can be difficuly
using too many fences slows down our threads
not using the right fences can lead to data races


compilers typically arent smart enough to know where fences are needed. usually, libraries written by experts will use fences and provide a simple interface