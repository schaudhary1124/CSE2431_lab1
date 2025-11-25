#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

void print_role(const char *role) { // Print role method to make code smaller
    printf("ROLE=%s PID=%ld PPID=%ld\n", role, (long)getpid(), (long)getppid());
    fflush(stdout);
}

int main(void) {
    pid_t c1 = fork();

    if (c1 < 0) {
        return EXIT_FAILURE;
    }

    if (c1 == 0) {
        // Child 1 Process
        print_role("child1");
        
        pid_t gc1 = fork();
        
        if (gc1 < 0) {
            return EXIT_FAILURE;
        }

        if (gc1 == 0) {
            // Grandchild 1 Process
            print_role("grandchild1");
            return 0; // Grandchild 1 exits after printing
        }
        
        // Child 1 waits for Grandchild 1
        waitpid(gc1, NULL, 0);
        return 0; // Child 1 exits after reaping
    }

    // Root Process continues to fork child 2
    pid_t c2 = fork();
    
    if (c2 < 0) {
        // Need to wait for c1 if c2 failed
        waitpid(c1, NULL, 0); 
        return EXIT_FAILURE;
    }

    if (c2 == 0) {
        // Child 2 Process
        print_role("child2");
        
        pid_t gc2 = fork();
        
        if (gc2 < 0) {
            return EXIT_FAILURE;
        }

        if (gc2 == 0) {
            // Grandchild 2 Process
            print_role("grandchild2");
            return 0; // Grandchild 2 exits after printing
        }
        
        // Child 2 waits for Grandchild 2
        waitpid(gc2, NULL, 0);
        return 0; // Child 2 exits after reaping
    }

    // Parent prints role and waits for both children
    print_role("root");
    
    // Root waits for Child 1 and Child 2
    waitpid(c1, NULL, 0);
    waitpid(c2, NULL, 0);
    
    printf("TREE_COMPLETE=success\n");
    fflush(stdout);
    return 0;
}
