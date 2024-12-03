typedef struct {
    void **data;      // Array of void pointers to hold generic data
    size_t size;      // Current number of elements
    size_t capacity;  // Total capacity
} ArrayList;
  
typedef struct Command {
    ArrayList* arguments;
    char *execpath;
    char *inputfile;
    char *outputfile;
    int is_pipe_output;
} Command;

ArrayList *newList(size_t initial_capacity);
void add(ArrayList *list, void *element);
void freelist(ArrayList *list);
void freecommand(Command* command);
void freecommandlist(ArrayList* commandsList);
ArrayList *tokenize_input(char str[]);