#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h> // Include the header file for wait()

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void ChildandParentProcessCanWorkOnSameFile(){
// Open a file for writing
    int fd = open("example.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    // Fork a child process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        // Write to the file
        dprintf(fd, "Child process writing to the file.\n");
        // Close the file descriptor
        close(fd);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        // Wait for the child to finish
        wait(NULL);
        // Write to the file
        dprintf(fd, "Parent process writing to the file.\n");
        // Close the file descriptor
        close(fd);
        exit(EXIT_SUCCESS);
    }   
}
// Cleanup function to be called at program exit
void cleanup() {
    printf("Cleanup function called. Performing cleanup tasks...\n");
    // Add cleanup tasks here
}

int ExitYapildiginda() {
    // Register the cleanup function using atexit
    if (atexit(cleanup) != 0) {
        fprintf(stderr, "Failed to register cleanup function\n");
        return EXIT_FAILURE;
    }

    printf("Main function: Performing main tasks...\n");

    // Simulate program execution
    // ...

    // Exit the program
    return EXIT_SUCCESS;
}


int main() {
    
}