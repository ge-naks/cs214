typedef struct node {
    size_t freq;
    char* word;
    struct node *next;
} node;

void insert(char* target_word, node* ptr);
void print_list(node* head);
void free_list(node* head);
int test_ll();
void sort_list(node** head);
void traverse(char *path);
int test_traverse(int argc, char *argv[]);