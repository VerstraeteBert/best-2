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
		triviaal :-)
*/	
int main(int argc, char ** argv) {
	int fd_P_C1[2]; // fd[0] -> read; fd[1] -> write
	int fd_P_C2[2];
	int fd_C1_C2[2];
	
	if (pipe(fd_P_C1) < 0) {
		perror(argv[0]);
		return 1;
	}
	if (pipe(fd_P_C2) < 0) {
		perror(argv[0]);
		return 1;
	}
	if (pipe(fd_C1_C2) < 0) {
		perror(argv[0]);
		return 1;
	}
	int pids [2];
	if ((pids[0] = fork()) < 0) {
		perror(argv[0]);
		exit(1);	
	}

	int num;
	if (pids[0] == 0) {
		// child 1
		close(fd_P_C2[0]); //  moet toch niet meeluisteren :-)
		close(fd_P_C2[1]);
		close(fd_P_C1[1]); // write deel kan gesloten worden :-)
		close(fd_C1_C2[0]);

		read(fd_P_C1[0], &num, sizeof(int));
		printf("CHILD 1...I've read number %d\n", num);
		write(fd_C1_C2[1], &num, sizeof(int));
		return 0; // kindproces sluiten
	}

	if ((pids[1] = fork()) < 0) {
		perror(argv[0]);
		exit(1);	
	}

	if (pids[1] == 0) {
		// child 2
		close(fd_P_C1[0]); //  moet toch niet meeluisteren :-)
		close(fd_P_C1[1]);

		close(fd_C1_C2[1]); // write deel kan gesloten worden :-)
		close(fd_P_C2[1]); 

		int num2;
		read(fd_P_C2[0], &num, sizeof(int));
		read(fd_C1_C2[0], &num2, sizeof(int));
		printf("CHILD 2...COMBINING WOOO %d\n", (num + num2));
		return 0; // kindproces sluiten
	}

	// parent
	close(fd_C1_C2[0]);
	close(fd_C1_C2[1]);
	close(fd_P_C1[0]);
	close(fd_P_C2[0]);

	num = 1;
	write(fd_P_C1[1], &num, sizeof(int));

	num = 2;
	write(fd_P_C2[1], &num, sizeof(int));

	waitpid(pids[0], NULL, 0); // wachten op completion van kind
	waitpid(pids[1], NULL, 0); // wachten op completion van kind
	return 0;
}
