#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

char *search_for_file(const char *directory, const char *filename) {
    struct dirent *entry;
    DIR *dir = opendir(directory);

    if (dir == NULL) {
        perror("opendir");
        return NULL; // Error opening directory
    }

    // Buffer to store the full path
    char *full_path = malloc(4096); // Allocate memory for the path
    if (full_path == NULL) {
        perror("malloc");
        closedir(dir);
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, filename) == 0) {
            snprintf(full_path, 4096, "%s/%s", directory, filename);
            closedir(dir);
            return full_path; // Return the full path
        }
    }

    closedir(dir);
    free(full_path); // Free the memory if the file is not found
    return NULL;
}

int main() {
    const char *directory = "/usr/bin";
    const char *filename = "echo"; // Replace with your file's name

    char *file_path = search_for_file(directory, filename);
    if (file_path != NULL) {
        printf("File '%s' found at path: %s\n", filename, file_path);
        free(file_path); // Free the allocated memory
    } else {
        printf("File '%s' not found in directory '%s'.\n", filename, directory);
    }

    return 0;
}
