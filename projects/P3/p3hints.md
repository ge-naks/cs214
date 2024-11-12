# P3

### general structure
Should generally have 3 layers:
1) Tokenizer - turn input stream into words (whole command)
2) Command Parser: determines the command that corresponds to the tokens
    - Identify redirection
    - Identidy pipelines
    - Expanding wildcards

    ```
    struct command {
        arraylist_t arguments;
        char *execpath;
        char *inputfile;
        char *outputfile;
    };
    ```
3) Launcher / command executor
    - Sets up pipes
    - Forks child process(es)
    - Waits for child(ren) to terminate

### Note!
Use ```isatty()``` to determine whether standard input is a terminal or a file
we can write shell scripts for our shell script kekw

### signals
- recall: signalls are messages sent to a process asynchronously
- signal types are indicated by an integer code

- for each signal, a process can indicate a disposition
    - ignore
    - terminate
    - run a signal hander function
    - (others)

- we can use a signal mask to temporarily block certain signal
- blocked signals can be delivered later once they are unblocked
- child processes inherit the signal mask and signal dispositions of their parent
- signal dispositions can be reset after exec()

what if a signal arrives when we are blocked?
we can choose to:
- restart the system call after the signal is handled
- abort the system call, return failure and set errno to EINTR

    