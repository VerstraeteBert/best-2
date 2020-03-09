#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

/*
	Oppassen met pointers bij fork -> variabelen worden gekopieerd; 
	DUS IN KINDPROCES OOK DE GEEMALLOCTE POINTERS FREE'EN 
*/
int main (int argc, char ** argv) {
	int * getal = malloc(sizeof(int));
	int pid;

	if ((pid = fork()) < 0) {
		perror(argv[0]);
	} else if (pid ==0) {
		free(getal);
		return 0;
	}
	free(getal);
	return 0;
}