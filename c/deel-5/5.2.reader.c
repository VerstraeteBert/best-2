// man page van open 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// hier zitten zowat all belangrijke systeemaanroepen
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

const int MAX = 32 * 1024;

int main(int argc, char ** argv) {
	int buff_size;
	char buffer [MAX];s
	for (buff_size = 1; buff_size < MAX * 2; buff_size <<= 1) {
		int fd;
		// O_RDONLY => read mode
		if ((fd=open(argv[1], O_RDONLY)) < 0) {
			// argv[0] -> naam v/ programma
			// perror schrijf wat tussen haakjes staat op scherm 
			// gevolgd door inhoud errno (die automisch geset wordt)
			perror(argv[0]);
			exit(1);
		}

		double start = clock();

		int n;
		while ( (n = read(fd,buffer,buff_size)) == buff_size );
		if (n < 0) {
			perror(argv[0]);
			// niet vergeten te sluiten!!
			close(fd);
			exit(1);
		}

		double time = (clock() - start) / CLOCKS_PER_SEC;
		printf("BUFF_SIZE= %d, TIME=%.2f\n", buff_size, time);

		if (close(fd) < 0) {
			perror(argv[0]);
			exit(1);
		}
	}
	return 0;
}