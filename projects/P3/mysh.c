#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "arraylist.h"


// tokenize our input
ArrayList* tokenize_input(char str[])
{
    ArrayList* tokens = newList();

    char *pch = strtok(str, " ");
    while (pch != NULL)
    {
        add(tokens, pch);
        pch = strtok(NULL, " ");
    }
    return tokens;
}

printlist(ArrayList* list){
    for(int i = 0; i < list->length; i++){
        printf("%s", list->items[i]);
    }
}

ArrayList* makeCommands(ArrayList* tokens){
   const char* pipe = "|";
   const char* l_redirect = "<";
   const char* r_redirect = ">";


    ArrayList* commands = newList();
    ArrayList* args = newList();
    Command* command;


    for(int i = 0; i < tokens->length; i++){
        if(strcmp(tokens->items[i], pipe) == 0 || strcmp(tokens->items[i], r_redirect) == 0 || strcmp(tokens->items[i], l_redirect) == 0 ){
            printf("operator detected");

        }else{
            add(args, tokens->items[i]);
        }
    }
    printlist(args);
    freelist(args);
}



int main()
{
    char str[] = "this is a test | for now";
    ArrayList *tokens = tokenize_input(str);

    ArrayList *commands = makeCommands(tokens);
    


    freelist(tokens);
    return 0;
}

