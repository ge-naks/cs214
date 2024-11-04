#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "words.h"


int main(int argv, char* args[]) {
    // call 1 func

    node* head = (node*)malloc(sizeof(node));

    head->word = "DUMMY NODE SHOULD NOT SHOW UP!!!!";
    node* dummy_ptr = head;

    call_traverse(argv, args, dummy_ptr);
    sort_list(&dummy_ptr);
    print_list(head);
}