#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

/*
	Door de manier waarop schedulers werken zal parent proces eerst verder uitgevoerd, voor de gespawnde kindprocessen.
	Om geen zombieprocessen te creeren moet er gewacht worden in het parent proces op het klaar zijn van de kindprocessen.
	Dit omdat het exit-status van het kindproces door het ouderproces gelezen moet worden. 

	Test uit door dit te runnen; ps aux | grep ...

	1. Met sleep(x) -> niet ideaal 
	2. waitpid(int pid, status * stat, int options) -> goed :-)
*/
int main (int argc, char ** argv) {
	int pid;
	if ((pid = fork()) < 0) {
		perror(argv[0]);
	} else if (pid ==0) {
		sleep(10);
		printf("I'm the child with PID=%d and my parent is %d \n", getpid(), getppid());
		return 0;
	}
	printf("I'm the parent with pid=%d and I've created a child with %d\n", getpid(), pid);
	//sleep(15);
	waitpid(pid, NULL, 0);
	return 0;
}