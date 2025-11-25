#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
	int localVar = 10; //Declare prior to fork()

	pid_t child_pid = fork();
	if(child_pid < 0) { // Checks for failure in fork()
		return EXIT_FAILURE;
	}

	if(child_pid == 0) { // Child Process
		localVar = 20; // localVar for child process is different
		printf("PROCESS_TYPE=child PID=%ld PPID=%ld FORK_RETURN=0 LOCAL_VAR=%d\n", (long)getpid(), (long)getppid(), localVar);
	} else {
		waitpid(child_pid, NULL, 0); // Wait for child pid
		printf("PROCESS_TYPE=parent PID=%ld PPID=%ld FORK_RETURN=%ld LOCAL_VAR=%d\n", (long)getpid(), (long)getppid(), (long)child_pid, localVar);
	}
	fflush(stdout); // fflush for better print ordering
	return 0;
}
