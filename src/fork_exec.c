#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if(argc < 2){
        fprintf(stderr, "Usage: %s <program> [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Print PARENT_START before fork
    printf("PARENT_START PID=%ld\n", (long)getpid());
    fflush(stdout); // Flush for better print ordering

    pid_t child = fork();
    
    if(child < 0){ // Checks for failure in fork()
        return EXIT_FAILURE;
    }
    
    if(child == 0){
        // Child Process
        printf("CHILD_START PID=%ld\n", (long)getpid());
        printf("BEFORE_EXEC\n");
        fflush(stdout);
        
        // execvp replaces the current process image
        execvp(argv[1], &argv[1]);
        
        // runs only if execvp fails
        perror("execvp");
        printf("AFTER_EXEC\n"); // Required print on execvp failure
        fflush(stdout);
        return EXIT_FAILURE;
        
    } else {
        // Parent Process
        int status;
        waitpid(child, &status, 0); //wait for child pid to avoid zombies
        
        // Parent waited
        printf("PARENT_WAITED PID=%ld STATUS=%d\n", (long)child, status); 
        fflush(stdout);
        return EXIT_SUCCESS;
    }
}
