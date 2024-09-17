# CS214 (Systems Programming) Lecture 4
## Macros and Objects

### The preprocessor
- Lines in a C source starting with # are preprocessor directives
    - Part of a higher level meta language that is used to generate source code
- The preprocessor is a source transformation pass that is run over the code before the compiler gets to it
- What are some common examples?
    - #include:
        ```
        #include "some_file.h" // specifies a file in the same directory
        #include "my_library/some_interface.h" // specifies a filepath
        #include <library_file.h> // specifies a file in an "include" directory
        ```
        - Any file can be included, not just headers or even C source code
            - Typically only used with header files
    - #define
        - creates macros/text replacements
        - Creates a token that should be replaced by something else
        ```
        #define STRLENGTH 128 // first word is the token to replace
                              // remainder of the line will be the replacement
        char buf[STRLENGTH];

        for (i = 0; i < STRLENGTH; i++){
            buf[i]
        }

        // STRLENGTH ==> 128
        // STRLENGTH+1 ==> 128+1
        // STRLENGTHS ==> STRLENGTHS
        // "STRLENGTH+1" ==> "STRLENGTH+1"

        #define TTPO(X) X * 2 + 1


         struct node *ptr; = ...
        #define foreach(P) for( ; P; P = P->next)

        foreach(ptr){
            // todo
        }


        ```
        - only complete tokens are replaced
        - can use \ to continue definitions of on the next line
        ```
        #define strict(X){ \
            fprintf(stderr, "something is wrong on line %d", __LINE__);\
        }
        ```
        - pseudo-macros:
            - ```__FILE__``` is replaced by a string literal containing the source file's name
            - ```__LINE__``` is replaced by an integer literal of the line number
        

