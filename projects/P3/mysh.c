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
#include <glob.h>
#include "arraylist.h"

// tokenize our input
ArrayList *tokenize_input(char str[])
{
    ArrayList *tokens = newList(2);

    char *pch = strtok(str, " ");
    while (pch != NULL)
    {
        add(tokens, pch);
        pch = strtok(NULL, " ");
    }
    return tokens;
}

void remove_newlines_from_arraylist(ArrayList *list)
{
    // Iterate through the elements in the ArrayList
    for (size_t i = 0; i < list->size; ++i)
    {
        char *str = (char *)list->data[i]; // Cast the void pointer to char pointer

        // Check if the current element is a valid string
        if (str != NULL)
        {
            size_t len = strlen(str);

            // If a newline character is found at the end, remove it
            if (len > 0 && str[len - 1] == '\n')
            {
                str[len - 1] = '\0';
            }
        }
    }
}

ArrayList *makeCommandList(ArrayList *tokens)
{
    ArrayList *args = newList(2);         // Temporary list to hold arguments
    ArrayList *commandsList = newList(2); // List of all commands
    Command *currCommand = NULL;          // Initialize to NULL

    const char *pipe = "|";
    const char *l_redirect = "<";
    const char *r_redirect = ">";

    for (int i = 0; i < tokens->size; i++)
    {
        if (strcmp(tokens->data[i], l_redirect) == 0 || strcmp(tokens->data[i], r_redirect) == 0 || strcmp(tokens->data[i], pipe) == 0)
        {

            // Allocate a new Command structure if needed
            if (currCommand == NULL)
            {
                currCommand = (Command *)malloc(sizeof(Command));
                currCommand->arguments = NULL;
                currCommand->execpath = NULL;
                currCommand->inputfile = NULL;
                currCommand->outputfile = NULL;
            }

            currCommand->arguments = args;

            // Add current command to the list of commands
            add(commandsList, currCommand);

            ArrayList *operatorlist = newList(2);
            add(operatorlist, tokens->data[i]);
            Command *operatorCommand = (Command *)malloc(sizeof(Command));

            operatorCommand->arguments = operatorlist;
            add(commandsList, operatorCommand);

            // Reset for the next command
            args = newList(2);
            currCommand = NULL; // Ensure a new Command is allocated in the next iteration
        }
        else
        {
            add(args, tokens->data[i]);
        }
    }

    if (currCommand == NULL)
    {
        currCommand = (Command *)malloc(sizeof(Command));
        currCommand->arguments = NULL;
        currCommand->execpath = NULL;
        currCommand->inputfile = NULL;
        currCommand->outputfile = NULL;
    }

    currCommand->arguments = args;
    add(commandsList, currCommand);

    return commandsList;
}

void match_files(const char *pattern)
{
    glob_t results;
    int ret;

    ret = glob(pattern, 0, NULL, &results);

    if (ret == 0)
    {
        printf("Matched files:\n");
        for (size_t i = 0; i < results.gl_pathc; i++)
        {
            printf("  %s\n", results.gl_pathv[i]);
        }
    }
    else if (ret == GLOB_NOMATCH)
    {
        printf("No matches found for pattern: %s\n", pattern);
    }
    else
    {
        printf("Error occurred while processing pattern: %s\n", pattern);
    }

    globfree(&results);
}

void printlist(ArrayList *list)
{
    for (int i = 0; i < list->size; i++)
    {
        printf("%s ", (char *)(list->data[i]));
    }
}

void printCommands(ArrayList *commands)
{
    for (int i = 0; i < commands->size; i++)
    {
        printlist(((Command *)commands->data[i])->arguments);
        printf("\n");
    }
}

char *BareSearch(const char *filename)
{
    struct dirent *entry;
    DIR *dir = opendir("/usr/local/bin");

    if (dir == NULL)
    {
        perror("opendir");
        return NULL; // Error opening directory
    }

    char *full_path = malloc(4096); // Allocate memory for the path
    if (full_path == NULL)
    {
        perror("malloc");
        closedir(dir);
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, filename) == 0)
        {
            snprintf(full_path, 4096, "%s/%s", "/usr/local/bin", filename);
            free(full_path);
            closedir(dir);
            return "/usr/local/bin"; // Return the full path
        }
    }
    closedir(dir);

    dir = opendir("/usr/bin");
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, filename) == 0)
        {
            snprintf(full_path, 4096, "%s/%s", "/usr/bin", filename);
            free(full_path);
            closedir(dir);
            return "/usr/bin"; // Return the full path
        }
    }
    closedir(dir);

    dir = opendir("/bin");
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, filename) == 0)
        {
            snprintf(full_path, 4096, "%s/%s", "/bin", filename);
            free(full_path);
            closedir(dir);
            return "/bin"; // Return the full path
        }
    }

    closedir(dir);
    free(full_path);
    return NULL;
}

void callCommands(ArrayList *commands)
{

    for (int i = 0; i < commands->size; i++)
    {
        Command *cmd = (Command *)commands->data[i];
        ArrayList *args = cmd->arguments;
        remove_newlines_from_arraylist(args);

        // Ensure there is at least one argument (the command itself)
        if (args->size > 0)
        {
            char *command_name = (char *)args->data[0];

            if (strcmp(command_name, "cd") == 0)
            {
                if (args->size == 2)
                {

                    // Get the directory to change to
                    char *target_dir = (char *)args->data[1];

                    // Strip any trailing newline characters
                    size_t len = strlen(target_dir);
                    if (len > 0 && target_dir[len - 1] == '\n')
                    {
                        target_dir[len - 1] = '\0';
                    }

                    // Try to change the directory
                    if (chdir(target_dir) == 0)
                    {
                        // Successfully changed directory
                        printf("Changed directory to: %s\n", target_dir);
                    }
                    else
                    {
                        // Error changing directory
                        perror("mysh: cd");
                    }
                }
                else if (args->size == 1)
                {
                    // Too few arguments
                    printf("mysh: cd: missing argument\n");
                }
                else
                {
                    // Too many arguments
                    printf("mysh: cd: too many arguments\n");
                }
            }

            if (strcmp(command_name, "pwd") == 0)
            {
                if (args->size == 1)
                {
                    char cwd[FILENAME_MAX];
                    if (getcwd(cwd, sizeof(cwd)) != NULL)
                    {
                        printf("Current working directory: %s\n", cwd);
                    }
                    else
                    {
                        perror("getcwd() error");
                    }
                }
                else
                {
                    printf("mysh: pwd: too many arguments\n");
                }
            }

            if (strcmp(command_name, "which") == 0)
            {
                if (args->size == 2)
                {
                    char *target_file = (char *)(args->data[1]);

                    char *result = BareSearch(target_file);

                    if (result)
                    {
                        printf("%s\n", result);
                    }
                }
            }

            if (command_name[0] != '/')
            {
                char *bare_path = BareSearch(command_name);
                if (NULL == bare_path)
                {
                    // command doesn't exist
                    printf("command not recognized\n");
                }
                else
                {
                    size_t path_length = strlen(bare_path) + strlen(command_name) + 2;
                    char *command_path = (char *)malloc(path_length);
                    if (command_path == NULL)
                    {
                        perror("malloc failed");
                        exit(EXIT_FAILURE);
                    }

                    // Concatenate the base path and command name
                    strcpy(command_path, bare_path);
                    strcat(command_path, "/");
                    strcat(command_path, command_name);
                    char *ptr = NULL;
                    add(args, ptr);
                    char *t[args->size];
                    for (int i = 0; i < args->size; i++)
                    {
                        t[i] = args->data[i];
                    }

                    // fork() call, wait for execv command to be called

                    int pid = fork();

                    if(pid < 0){
                        perror("fork failure");
                    }else if(pid == 0){
                        // execute
                        execv(command_path, t);

                    }else{
                        wait(NULL);
                    }

                    // Free the allocated memory
                    free(command_path);
                }
            }
        }
    }
}


int main(int argc, char **args)
{
    char *str = NULL;
    size_t len = 0;
    FILE *input = NULL;
    bool mode_flag = true; // false for batch, true for interactive

    if (argc > 1)
    {
        input = fopen(args[1], "r");
        if (input == NULL)
        {
            perror("Error opening file");
            return 1;
        }
    }
    else if (isatty(fileno(stdin)))
    {
        printf("Welcome to my (s)hell!\n");
        printf("mysh> ");
        input = stdin;
    }
    else
    {
        mode_flag = false;
        input = stdin;
    }

    // Read the entire input
    while (getline(&str, &len, input) != -1)
    {
        if (strcmp(str, "exit\n") == 0 && mode_flag)
        {
            printf("mysh: exiting\n");
            break;
        }
        // Tokenize and process input line by line
        ArrayList *tokens = tokenize_input(str);
        ArrayList *commands = makeCommandList(tokens);
        callCommands(commands);
        freelist(tokens);
        freecommandlist(commands);
        if (mode_flag)
        {
            printf("mysh> ");
        }
    }

    match_files("*.txt");

    free(str);
    if (input != stdin)
    {
        fclose(input);
    }

    return 0;
}
