#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

const int N = 100;

int main (int argc, char ** argv) {
	int kids [N];
	int i;
	for (i = 0; i < N; i++) {
		if ((kids[i] = fork()) < 0) {
			perror(argv[0]);
		} else if (kids[i] == 0) {
			printf("I'm the %dth child with PID=%d and my parent is %d \n", i, getpid(), getppid());
			sleep(3);
			return 0;
		} 
		printf("I'm the parent with PID=%d and I've created a child with %d\n", getpid(), kids[i]);
		//waitpid(pid, NULL, 0);
	}
	for (i = 0; i < N; i++) {
		waitpid(kids[i], NULL, 0);
	}
	return 0;
}