#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

/**
	Oef:
		Ouder stuurt getal1 door naar kind 1
		Kind 1 stuurt getal1 door naar kind 2 
		Ouder stuurt getal 2 direct door naar kind 2
*/	
int main(int argc, char ** argv) {
	int fd[2]; // fd[0] -> read; fd[1] -> write
	if (pipe(fd) < 0) {
		perror(argv[0]);
		return 1;
	}
	int pid;
	if ((pid = fork()) < 0) {
		perror(argv[0]);
		exit(1);	
	}
	if (pid == 0) {
		// child
		close(fd[1]); // write deel kan gesloten worden :-)
		int num;
		read(fd[0], &num, 4);
		printf("CHILD...I've read number %d\n", num);
		return 0; // kindproces sluiten
	}
	// parent
	int n = 0x676;
	write(fd[1], &n, sizeof(n));
	waitpid(pid, NULL, 0); // wachten op completion van kind
	return 0;
}
