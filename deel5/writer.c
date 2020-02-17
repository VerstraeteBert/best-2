// man page van open 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// hier zitten zowat all belangrijke systeemaanroepen
#include <unistd.h>

#include <stdlib.h>

#include <pthread.h>
#include <stdio.h>

const char * FILE_NAME = "test_file";
size_t BUFF_SIZE = 16;

char get_ran_char() {
	return 'a' + rand() % 26;
}

int main (int argc, char ** argv) {
	// tijd gebruiken kan -> om de seconde uniek
	// process id opvragen is echter een beter idee, altijd uniek (op systeem)
	// srand(time(NULL));
	srand(getpid());

	// file_descriptor
	int fd;

	// O_CREAT -> maakt file aan 
	// O_WRONLY -> write only
	if ((fd=open(FILE_NAME, O_CREAT | O_WRONLY)) < 0) {
		// argv[0] -> naam v/ programma
		// perror schrijf wat tussen haakjes staat op scherm 
		// gevolgd door inhoud errno (die automisch geset wordt)
		perror("test1");
		exit(1);
	}
	int i, j;
	char buff [BUFF_SIZE];
	// 10 MB aan chars
	// 1 byte buffer is enorm traag, heel veel I/O ops nodig
	// beter om grotere buffers te maken, dan schuift het probleem
	// van I/O bottleneck naar memory bottleneck; maar memory is véél sneller
	// dan file I/O. 
	double start = clock();
	for (i = 0; i < 10 * 1024 * 1024; i++) {
		for (j = 0; j < BUFF_SIZE; j++) {
			buff[j] = get_ran_char();
		}
		int n;
		// write (int fd, const void *buf, size_t count)
		if ( (n = write(fd, buff, 1)) != 1) {
			perror(argv[0]);
			exit(1);
		}
	}
	double time = (clock() - start) / CLOCKS_PER_SEC;
	printf("BUFF_SIZE= %d, TIME=%.2f", BUFF_SIZE, time);
	// closes file descriptor (int fd)
	if (close(fd) < 0) {
		perror(argv[0]);
		exit(1);
	}
	exit(0);
}
