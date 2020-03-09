#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#define N 60

int main(int argc, char ** argv) {
	int pid[N];
	int fd[N][2];
	int fd2[N][2];
	int i;

	for (i = 0; i < N; i++) {
		if (pipe(fd[i]) < 0) {
			perror(argv[0]);
			exit(1);
		}
		if (pipe(fd2[i]) < 0) {
			perror(argv[0]);
			exit(1);
		}
		if ((pid[i] = fork()) < 0) {
			perror(argv[0]);
			exit(1);
		}
		if (pid[i] == 0) {
			// child i...
			srand(getpid());
			close(fd[i][0]);
			close(fd2[i][1]);
			unsigned int g = rand();
			printf("Child %d has generated: %d\n", i, g);
			write(fd[i][1], &g, sizeof(unsigned int));
			int pid_winner;
			read(fd2[i][0], &pid_winner, sizeof(unsigned int));
			if (pid_winner == getpid()) {
				printf("PID %d: I'm the winner!\n", getpid());
			} else {
				printf("PID %d is the winner!\n", pid_winner);
			}
			exit(0);
		}
		close(fd[i][1]);
		close(fd2[i][0]);
	}
	
	unsigned int n;
	read(fd[i][0], &n, sizeof(unsigned int));
	unsigned int max = n; unsigned int max_pid = pid[0];
	for (i = 1; i < N; i++) {
		read(fd[i][0], &n, sizeof(unsigned int));
		if (n > max) {
			max = n;
			max_pid = pid[i];
		}
		printf("Read %u from child %d\n", n, i);
	}
	printf("Read max %d from child %d\n", max, max_pid);
	for (i = 0; i < N; i++) {
		write(fd2[i][1], &max_pid, sizeof(max_pid));
	}
	for (i = 0; i < N; i++) {
		waitpid(pid[i], NULL, 0);
	}
	return 0;
}
