#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arraylist.h"


// Initialize the ArrayList
ArrayList *newList(size_t initial_capacity) {
    ArrayList *list = malloc(sizeof(ArrayList));
    if (!list) {
        perror("Failed to allocate memory for ArrayList");
        exit(EXIT_FAILURE);
    }
    list->data = malloc(initial_capacity * sizeof(void *));
    if (!list->data) {
        perror("Failed to allocate memory for ArrayList data");
        free(list);
        exit(EXIT_FAILURE);
    }
    list->size = 0;
    list->capacity = initial_capacity;
    return list;
}

// Add an element to the ArrayList
void add(ArrayList *list, void *element) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;  // Double the capacity
        list->data = realloc(list->data, list->capacity * sizeof(void *));
        if (!list->data) {
            perror("Failed to reallocate memory for ArrayList data");
            exit(EXIT_FAILURE);
        }
    }
    list->data[list->size++] = element;
}


// Free the ArrayList
void freelist(ArrayList *list) {
    free(list->data);
    free(list);
}


void freecommand(Command* command) {
    freelist(command->arguments);
    free(command->execpath);
    free(command->inputfile);
    free(command->outputfile);
}

void freecommandlist(ArrayList* commandsList) {
    for (size_t i = 0; i < commandsList->size; i++) {
    Command *cmd = (Command *)commandsList->data[i];
    free(cmd->arguments->data);
    free(cmd->arguments);
    if (cmd->inputfile) free(cmd->inputfile);
    if (cmd->outputfile) free(cmd->outputfile);
    free(cmd);
}
    free(commandsList->data);
    free(commandsList);

}