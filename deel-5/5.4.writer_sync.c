// man page van open 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// hier zitten zowat all belangrijke systeemaanroepen
#include <unistd.h>

#include <stdlib.h>

#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>

/**
	O_SYNC

**/

const size_t MAX = 1024 * 32;

char get_ran_char() {
	return 'a' + rand() % 26;
}

void vul(char * tab, int n) {
	int i;
	for (i = 0; i < n; i++) {
		tab[i] = get_ran_char();
	}
}

int main (int argc, char ** argv) {
	// tijd gebruiken kan -> om de seconde uniek
	// process id opvragen is echter een beter idee, altijd uniek (op systeem)
	// srand(time(NULL));
	srand(getpid());

	char buffer [MAX];
	// file_descriptor
	int fd, buff_size;

	for (buff_size = 1; buff_size < MAX * 2; buff_size <<= 1) {
		double start = clock();
		// O_CREAT -> maakt file aan 
		// O_WRONLY -> write only
		if ((fd=open(argv[1], O_CREAT | O_WRONLY | O_SYNC)) < 0) {
			// argv[0] -> naam v/ programma
			// perror schrijf wat tussen haakjes staat op scherm 
			// gevolgd door inhoud errno (die automisch geset wordt)
			perror(argv[0]);
			exit(1);
		}
		int tot = 0;
		vul(buffer, buff_size);

		write(fd, buffer, buff_size);

		while (tot < 10 * 1024 * 1024 - buff_size ) {
			tot += buff_size;
			vul(buffer, buff_size);
			write(fd, buffer, buff_size);
		}

		vul(buffer, 10 * 1024 * 1024 - tot);
		write(fd, buffer, 10 * 1024 * 1024 - tot);

		if (close(fd) < 0) {
			perror(argv[0]);
			exit(1);
		}


		unlink(argv[1]);

		struct stat st;
		stat(argv[1], &st);
		int size = st.st_size;

		double time = (clock() - start) / CLOCKS_PER_SEC;
		printf("BUFF_SIZE= %d, TIME=%.2f, FILE_SIZE=%d\n", buff_size, time, size);
	}
	exit(0);
}
