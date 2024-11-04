#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "words.h"



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
    while (head != NULL && head->freq != 0)
    {
        printf("%s %zu\n", head->word, head->freq);
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

void process_txt_file(const char *filepath, node* ptr)
{
    char *buffer = 0;
    long length;
    FILE *f = fopen(filepath, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }

    if (buffer)
    {
        count_words(buffer, ptr);
    }
}

void traverse(const char *path, node* ptr)
{
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (!dir)
    {
        fprintf(stderr, "Could not open directory: %s\n", path);
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Ignore entries starting with a period
        if (entry->d_name[0] == '.')
        {
            continue;
        }

        // Construct full path for the entry
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        // Check if entry is a directory
        struct stat entry_stat;
        if (stat(fullpath, &entry_stat) == 0 && S_ISDIR(entry_stat.st_mode))
        {
            // Recursively traverse subdirectories
            traverse(fullpath, ptr);
        }
        else if (stat(fullpath, &entry_stat) == 0 && S_ISREG(entry_stat.st_mode))
        {
            // Process regular files ending with ".txt"
            if (strstr(entry->d_name, ".txt") == entry->d_name + strlen(entry->d_name) - 4)
            {
                process_txt_file(fullpath, ptr);
            }
        }
    }

    closedir(dir);
}

int call_traverse(int argc, char *argv[], node* ptr)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <directory_or_file>\n", argv[0]);
        return 1;
    }

    struct stat path_stat;
    if (stat(argv[1], &path_stat) != 0)
    {
        fprintf(stderr, "Error: %s does not exist.\n", argv[1]);
        return 1;
    }

    // Check if the argument is a directory or a regular file
    if (S_ISDIR(path_stat.st_mode))
    {
        traverse(argv[1], ptr);
    }
    else if (S_ISREG(path_stat.st_mode))
    {
        // If it's a file, ensure it ends with ".txt" before processing
        if (strstr(argv[1], ".txt") == argv[1] + strlen(argv[1]) - 4)
        {
            process_txt_file(argv[1], ptr);
        }
        else
        {
            fprintf(stderr, "Error: %s is not a .txt file.\n", argv[1]);
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Error: %s is neither a directory nor a .txt file.\n", argv[1]);
        return 1;
    }

    return EXIT_SUCCESS;
}

bool is_valid_hyphen_position(const char *word, int pos, int length)
{
    return (pos > 0 && pos < length - 1 && isalpha(word[pos - 1]) && isalpha(word[pos + 1]))||
           (pos > 0 && pos < length - 1 && word[pos - 1] == '\'' && word[pos + 1] == '\'');
}

void count_words(const char *text, node* ptr)
{
    int length = 0;
    int text_len = strlen(text);

    // Buffer to hold each word temporarily.
    char word[256];

    for (int i = 0; i <= text_len; i++)
    {
        char ch = text[i];

        // Check if the current character is part of a word.
        if (isalpha(ch) || ch == '\'' || (ch == '-' && is_valid_hyphen_position(text, i, text_len)))
        {
            word[length++] = ch; // Add the character to the word buffer.
        }
        else if (length > 0)
        {
            // We hit a non-word character, so finalize the word.
            word[length] = '\0'; // Null-terminate the string.
            insert(word, ptr);   // Insert the word into the hashmap.
            length = 0;          // Reset for the next word.
        }
    }

    // Edge case: insert the last word if the text ends with a word.
    if (length > 0)
    {
        word[length] = '\0';
        insert(word, ptr);
    }
}
