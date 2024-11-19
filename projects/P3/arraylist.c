#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arraylist.h"


ArrayList *newList(void) {
  char **items = malloc(4 * sizeof(char *));
  ArrayList *list = malloc(sizeof(ArrayList));
  list->length = 0;
  list->capacity = 4;
  list->items = items;
  return list;
}

void check(ArrayList *list) {
  if (list->length >= list->capacity) {
    list->capacity = list->capacity * 2;
    list->items = realloc(list->items, list->capacity * sizeof(char *));
    if (list->items == NULL) {
      exit(1);
    }
  }
}

void add(ArrayList *list, const char *s) {
  check(list);
  list->items[list->length] = malloc(strlen(s)+1);
  strcpy(list->items[list->length], s);
  list->length++;
}



void freelist(ArrayList *list) {
    for (int i = 0; i < list->length; i++) {
        free(list->items[i]);
    }
    free(list->items);
    free(list);
}

void freelistcontent(ArrayList *list) {
    for (int i = 0; i < list->length; i++) {
        free(list->items[i]);
    }
}


void addCommand(ArrayList *list, const Command *cmd) {
    // Ensure the list is not null
    check(list);

    // Allocate memory for the new Command structure
    Command *newCommand = malloc(sizeof(Command));
    if (!newCommand) {
        perror("Failed to allocate memory for new Command");
        exit(EXIT_FAILURE);
    }

    // Copy execpath
    if (cmd->execpath) {
        newCommand->execpath = malloc(strlen(cmd->execpath) + 1);
        if (!newCommand->execpath) {
            perror("Failed to allocate memory for execpath");
            exit(EXIT_FAILURE);
        }
        strcpy(newCommand->execpath, cmd->execpath);
    } else {
        newCommand->execpath = NULL;
    }

    // Copy inputfile
    if (cmd->inputfile) {
        newCommand->inputfile = malloc(strlen(cmd->inputfile) + 1);
        if (!newCommand->inputfile) {
            perror("Failed to allocate memory for inputfile");
            exit(EXIT_FAILURE);
        }
        strcpy(newCommand->inputfile, cmd->inputfile);
    } else {
        newCommand->inputfile = NULL;
    }

    // Copy outputfile
    if (cmd->outputfile) {
        newCommand->outputfile = malloc(strlen(cmd->outputfile) + 1);
        if (!newCommand->outputfile) {
            perror("Failed to allocate memory for outputfile");
            exit(EXIT_FAILURE);
        }
        strcpy(newCommand->outputfile, cmd->outputfile);
    } else {
        newCommand->outputfile = NULL;
    }

    // Deep copy the arguments ArrayList
    if (cmd->arguments) {
        newCommand->arguments = malloc(sizeof(ArrayList));
        if (!newCommand->arguments) {
            perror("Failed to allocate memory for arguments");
            exit(EXIT_FAILURE);
        }

        // Assume deep copy of the ArrayList is handled by a function
        deepCopyArrayList(newCommand->arguments, cmd->arguments);
    } else {
        newCommand->arguments = NULL;
    }

    // Add the new Command to the ArrayList
    list->items[list->length] = newCommand;
    list->length++;
}

void deepCopyArrayList(ArrayList *dest, const ArrayList *src) {
    dest->length = src->length;
    dest->capacity = src->capacity;
    dest->items = malloc(dest->capacity * sizeof(void *));
    if (!dest->items) {
        perror("Failed to allocate memory for ArrayList items");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < src->length; i++) {
        // Assume items are strings for simplicity
        dest->items[i] = malloc(strlen(src->items[i]) + 1);
        if (!dest->items[i]) {
            perror("Failed to allocate memory for ArrayList item");
            exit(EXIT_FAILURE);
        }
        strcpy(dest->items[i], src->items[i]);
    }
}
