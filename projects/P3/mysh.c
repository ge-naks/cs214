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
#include <fcntl.h>

#define MAX_ARGS 100
#define MAX_COMMANDS 100

void handle_redirections(char *cmd[], int *input_fd, int *output_fd)
{
    for (int i = 0; cmd[i] != NULL; i++)
    {
        if (strcmp(cmd[i], "<") == 0)
        {
            *input_fd = open(cmd[i + 1], O_RDONLY);
            if (*input_fd == -1)
            {
                perror("open input file");
                exit(1);
            }
            cmd[i] = NULL;
        }
        else if (strcmp(cmd[i], ">") == 0)
        {
            *output_fd = open(cmd[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0640);
            if (*output_fd == -1)
            {
                perror("open output file");
                exit(1);
            }
            cmd[i] = NULL;
        }
    }
}

char *has_wc(char *cmds[MAX_COMMANDS][MAX_ARGS], int num_commands)
{
    for (int i = 0; i < num_commands; i++)
    {
        for (int arg_index = 0; cmds[i][arg_index] != NULL; arg_index++)
        {
            for (int char_index = 0; cmds[i][arg_index][char_index] != '\0'; char_index++)
            {
                if (cmds[i][arg_index][char_index] == '*')
                {
                    return cmds[i][arg_index]; // Return the argument containing the wildcard
                }
            }
        }
    }
    return NULL; // No wildcard found
}

char *BareSearch(const char *filename)
{
    struct dirent *entry;
    DIR *dir;
    const char *paths[] = {"/usr/local/bin", "/usr/bin", "/bin"};
    static char full_path[4096]; // Fixed-size array for the path

    for (int i = 0; i < 3; i++)
    {
        dir = opendir(paths[i]);
        if (dir == NULL)
        {
            perror("opendir");
            continue;
        }

        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, filename) == 0)
            {
                snprintf(full_path, sizeof(full_path), "%s/%s", paths[i], filename);
                closedir(dir);
                return full_path; // Return the full path
            }
        }
        closedir(dir);
    }

    return NULL; // Command not found
}

glob_t *match_files(const char *pattern)
{
    glob_t *results = malloc(sizeof(glob_t)); // Allocate memory for results
    if (results == NULL)
    {
        perror("malloc");
        return NULL;
    }

    int ret = glob(pattern, 0, NULL, results);
    if (ret == 0)
    {
        return results; // Successfully matched files
    }
    else
    {
        free(results); // Free the allocated memory
        if (ret == GLOB_NOMATCH)
        {
            return NULL; // No matches found
        }
    }
}

void execute_pipe(char *cmds[MAX_COMMANDS][MAX_ARGS], int num_commands, bool mode_flag) {
    // Terminate the program if we get exit first
    if (strcmp(cmds[0][0], "exit") == 0) {
        if(mode_flag){
             printf("mysh: exiting\n");
            for(int i = 0; i < num_commands; i ++){
                int idx = 1;
                while(cmds[i][idx] != NULL){
                    printf("%s\n", cmds[i][idx]);
                    idx++;
                }
            }
            exit(0);
        }else if (!mode_flag){
            printf("Exiting my shell.\n");
            for(int i = 0; i < num_commands; i ++){
                int idx = 1;
                while(cmds[i][idx] != NULL){
                    printf("%s\n", cmds[i][idx]);
                    idx++;
                }
            }
            exit(0);
        }
    }

    int pipefd[2 * (num_commands - 1)];
    int input_fd = -1, output_fd = -1;

    

    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipefd + i * 2) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    for (int i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            if (i > 0) {
                dup2(pipefd[(i - 1) * 2], STDIN_FILENO);
            }
            if (i < num_commands - 1) {
                dup2(pipefd[i * 2 + 1], STDOUT_FILENO);
            }

            handle_redirections(cmds[i], &input_fd, &output_fd);

            if (input_fd != -1) {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            if (output_fd != -1) {
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }

            // Close all pipe file descriptors
            for (int j = 0; j < 2 * (num_commands - 1); j++) {
                close(pipefd[j]);
            }

            // Get the full path of the command
            char *cmd_path = BareSearch(cmds[i][0]);
            if (cmd_path == NULL) {
                perror("Command not found");
                exit(1);
            }

            // Execute the command with execv
            if (execv(cmd_path, cmds[i]) == -1) {
                perror("execv");
                exit(1);
            }
        }
    }

    // Close all pipe file descriptors in the parent process
    for (int i = 0; i < 2 * (num_commands - 1); i++) {
        close(pipefd[i]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }
}


void print_commands(char *cmds[MAX_COMMANDS][MAX_ARGS], int num_commands)
{
    printf("Parsed Commands and Arguments:\n");
    for (int i = 0; i < num_commands; i++)
    {
        printf("Command %d:\n", i + 1);
        int arg_index = 0;
        while (cmds[i][arg_index] != NULL)
        {
            printf("  Arg %d: %s\n", arg_index, cmds[i][arg_index]);
            arg_index++;
        }
    }
    printf("\n");
}

void handle_cd(char **args)
{
    if (args[1] == NULL)
    {
        printf("mysh: cd: missing argument\n");
        return;
    }
    char *target_dir = args[1];
    size_t len = strlen(target_dir);
    if (len > 0 && target_dir[len - 1] == '\n')
    {
        target_dir[len - 1] = '\0';
    }
    if (chdir(target_dir) != 0)
    {
        perror("mysh: cd");
    }
}

// Function to handle "pwd"
void handle_pwd()
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

// Function to handle "which"
void handle_which(char **args)
{
    if (args[1] == NULL)
    {
        return;
    }
    char *target_file = args[1];
    char *result = BareSearch(target_file);
    if (result)
    {
        printf("%s\n", result);
    }
    else
    {
        printf("mysh: which: %s not found\n", target_file);
    }
}

void handle_barename(char *cmd[])
{
    pid_t pid = fork();

    if (pid == -1)
    {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // Child process
        // Execute the command
        execv(cmd[0], cmd);

        // If execv returns, it means an error occurred
        perror("execv");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        // Check if child process exited normally
        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0)
            {
                fprintf(stderr, "Command exited with status %d\n", exit_status);
            }
        }

        // Check if child process was terminated by a signal
        if (WIFSIGNALED(status))
        {
            int signal_number = WTERMSIG(status);
            fprintf(stderr, "Command terminated by signal %d\n", signal_number);
        }
    }
}

void handle_wildcards_and_execute(char *cmds[MAX_COMMANDS][MAX_ARGS], int num_commands, bool mode_flag)
{
    // Dynamically track allocated memory
    char *allocated_memory[MAX_COMMANDS * MAX_ARGS] = {NULL};
    int allocated_count = 0;

    for (int i = 0; i < num_commands; i++)
    {
        char *updated_cmds[MAX_ARGS] = {NULL};
        int updated_arg_count = 0;

        for (int j = 0; cmds[i][j] != NULL; j++)
        {
            char *token = cmds[i][j];

            // Check if the argument contains a wildcard
            if (strchr(token, '*') != NULL)
            {
                glob_t files;
                if (glob(token, GLOB_TILDE, NULL, &files) == 0)
                {
                    // Add matched files to updated_cmds
                    for (size_t k = 0; k < files.gl_pathc && updated_arg_count < MAX_ARGS - 1; k++)
                    {
                        char *match = strdup(files.gl_pathv[k]);
                        updated_cmds[updated_arg_count++] = match;
                        allocated_memory[allocated_count++] = match; // Track for cleanup
                    }
                    globfree(&files);
                }
                else
                {
                    fprintf(stderr, "No matches found for pattern: %s\n", token);
                }
            }
            else
            {
                // Add the argument as-is
                updated_cmds[updated_arg_count++] = token;
            }
        }

        updated_cmds[updated_arg_count] = NULL; // Null-terminate the argument list

        // Replace the original cmds[i] with updated_cmds
        for (int j = 0; updated_cmds[j] != NULL; j++)
        {
            cmds[i][j] = updated_cmds[j];
        }
        cmds[i][updated_arg_count] = NULL; // Ensure cmds[i] is null-terminated
    }

    // Execute the updated commands
    execute_pipe(cmds, num_commands, mode_flag);

    // Free dynamically allocated memory
    for (int i = 0; i < allocated_count; i++)
    {
        free(allocated_memory[i]);
    }
}

int main(int argc, char **args)
{
    char *str = NULL;
    size_t len = 0;
    FILE *input = NULL;
    bool mode_flag = true; // false for batch mode, true for interactive mode

    // Determine input source
    if (argc > 1)
    {
        input = fopen(args[1], "r");
        if (input == NULL)
        {
            perror("Error opening file");
            return 1;
        }
        mode_flag = false; // Batch mode
    }
    else if (isatty(fileno(stdin)))
    {
        // Interactive mode with welcome message
        printf("Welcome to my (s)hell!\n");
        printf("mysh> ");
        input = stdin;
    }
    else
    {
        // Batch mode from stdin
        mode_flag = false;
        input = stdin;
    }

    // Read input line by line
    while (getline(&str, &len, input) != -1)
    {
        // Check for "exit" command in interactive mode
        if (mode_flag && strcmp(str, "exit\n") == 0)
        {
            printf("mysh: exiting\n");
            break;
        }
        else if (strcmp(str, "exit\n") == 0)
        {
            printf("Exiting my shell.\n");
            break;
        }

        // Parse the command line into commands
        char *commands[MAX_COMMANDS];
        int num_commands = 0;

        // Tokenize by pipe "|"
        char *token = strtok(str, "|");
        while (token != NULL && num_commands < MAX_COMMANDS)
        {
            commands[num_commands++] = token;
            token = strtok(NULL, "|");
        }

        // Parse each command into arguments
        char *cmds[MAX_COMMANDS][MAX_ARGS] = {NULL};
        for (int i = 0; i < num_commands; i++)
        {
            int num_args = 0;
            token = strtok(commands[i], " \t\n");
            while (token != NULL && num_args < MAX_ARGS - 1)
            {
                cmds[i][num_args++] = token;
                token = strtok(NULL, " \t\n");
            }
            cmds[i][num_args] = NULL; // Null-terminate the argument list
        }

        // Print the parsed commands for debugging
        // print_commands(cmds, num_commands);
        // printf("%d\n", num_commands);

        char *wc_pattern = has_wc(cmds, num_commands); // contains a pattern if it exists, NULL otherwise
        glob_t *files = match_files(wc_pattern);
        // Check for built-in commands and call the corresponding function
        if (wc_pattern == NULL || files == NULL)
        {
            // we dont worry about handling wildcards in here, operate normally
            if (strcmp(cmds[0][0], "cd") == 0)
            {
                handle_cd(cmds[0]);
            }
            else if (strcmp(cmds[0][0], "pwd") == 0)
            {
                handle_pwd();
            }
            else if (strcmp(cmds[0][0], "which") == 0)
            {
                handle_which(cmds[0]);
            }
            else if (cmds[0][0] != NULL && strncmp(cmds[0][0], "/", 1) == 0)
            {
                handle_barename(cmds[0]);
            }
            else
            {
                // Execute the command normally if it's not a built-in
                execute_pipe(cmds, num_commands, mode_flag);
            }
        }
        else
        {
            // need to handle wildcards
            handle_wildcards_and_execute(cmds, num_commands, mode_flag);

            globfree(files);
            free(files);
        }

        // Print shell prompt in interactive mode
        if (mode_flag)
        {
            printf("mysh> ");
        }
    }

    // Free the input buffer and close the file if not stdin
    free(str);
    if (input != stdin)
    {
        fclose(input);
    }

    return 0;
}
