#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

typedef struct node
{
    size_t freq;
    char *word;
    struct node *next;
} node;

void insert(char *target_word, node *ptr)
{
    while (ptr != NULL)
    {
        if (strcmp(ptr->word, target_word) == 0)
        {
            ptr->freq++;
            return;
        }
        if (ptr->next == NULL)
            break; // stop at last node
        ptr = ptr->next;
    }

    // Add new node at the end of the list
    node *new_node = (node *)malloc(sizeof(node));
    new_node->freq = 1;
    new_node->word = strdup(target_word); // Duplicate the word
    new_node->next = NULL;

    if (ptr != NULL)
    {
        ptr->next = new_node; // Link new node to the last node in list
    }
}

void sort_list(node **head)
{
    if (*head == NULL)
        return;

    int swapped;
    node *ptr, *lptr = NULL;

    do
    {
        swapped = 0;
        ptr = *head;

        while (ptr->next != lptr)
        {
            // Check if the current node should swap with the next node
            if ((ptr->freq < ptr->next->freq) ||
                (ptr->freq == ptr->next->freq && strcmp(ptr->word, ptr->next->word) > 0))
            {
                // Swap frequencies
                size_t temp_freq = ptr->freq;
                ptr->freq = ptr->next->freq;
                ptr->next->freq = temp_freq;

                // Swap words
                char *temp_word = ptr->word;
                ptr->word = ptr->next->word;
                ptr->next->word = temp_word;

                swapped = 1;
            }
            ptr = ptr->next;
        }
        lptr = ptr;
    } while (swapped);
}

void print_list(node *head)
{
    while (head != NULL)
    {
        printf("Word: %s, Frequency: %zu\n", head->word, head->freq);
        head = head->next;
    }
}

// Function to free the list memory
void free_list(node *head)
{
    node *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp->word);
        free(temp);
    }
}

int test_ll()
{
    node *head = (node *)malloc(sizeof(node));
    head->word = strdup("dummy");
    head->freq = 0;
    head->next = NULL;

    // Test words
    insert("apple", head);
    insert("banana", head);
    insert("apple", head);
    insert("orange", head);
    insert("banana", head);
    insert("cherry", head);
    insert("banana", head);
    insert("apple", head);

    sort_list(&head->next);
    print_list(head->next);

    free_list(head);

    return 0;
}

void process_txt_file(const char *filepath) {
    // Placeholder function to process .txt files
    printf("Processing file: %s\n", filepath);
}

void traverse(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (!dir) {
        fprintf(stderr, "Could not open directory: %s\n", path);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Ignore entries starting with a period
        if (entry->d_name[0] == '.') {
            continue;
        }

        // Construct full path for the entry
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        // Check if entry is a directory
        struct stat entry_stat;
        if (stat(fullpath, &entry_stat) == 0 && S_ISDIR(entry_stat.st_mode)) {
            // Recursively traverse subdirectories
            traverse(fullpath);
        } else if (stat(fullpath, &entry_stat) == 0 && S_ISREG(entry_stat.st_mode)) {
            // Process regular files ending with ".txt"
            if (strstr(entry->d_name, ".txt") == entry->d_name + strlen(entry->d_name) - 4) {
                process_txt_file(fullpath);
            }
        }
    }

    closedir(dir);
}

int test_traverse(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory_or_file>\n", argv[0]);
        return 1;
    }

    struct stat path_stat;
    if (stat(argv[1], &path_stat) != 0) {
        fprintf(stderr, "Error: %s does not exist.\n", argv[1]);
        return 1;
    }

    // Check if the argument is a directory or a regular file
    if (S_ISDIR(path_stat.st_mode)) {
        traverse(argv[1]);
    } else if (S_ISREG(path_stat.st_mode)) {
        // If it's a file, ensure it ends with ".txt" before processing
        if (strstr(argv[1], ".txt") == argv[1] + strlen(argv[1]) - 4) {
            process_txt_file(argv[1]);
        } else {
            fprintf(stderr, "Error: %s is not a .txt file.\n", argv[1]);
            return 1;
        }
    } else {
        fprintf(stderr, "Error: %s is neither a directory nor a .txt file.\n", argv[1]);
        return 1;
    }

    return EXIT_SUCCESS;
}


void count_words(){
    node* dummy_head = (node*)malloc(sizeof(node));

    dummy_head->word = "THIS IS A DUMMY NODE";

}