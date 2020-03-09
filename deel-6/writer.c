#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

const int N = 10;

int main (int argc, char ** argv) {
	int kids [N];
	int i;
	for (i = 0; i < N; i++) {
		if ((kids[i] = fork()) < 0) {
			perror(argv[0]);
			return 1;
		} else if (kids[i] == 0) {
			// execv -> string arr meegeven
			/**
			char * args [3] = {"writestring", "hanlonk", NULL};
			if (execv("/Users/bertverstraete/ugent/semester-2/besturings2/deel-6/writestring", args) < 0) {
				perror(argv[0]);
				return 1;
			}
			**/

			// execl -> lijst van strings ez
			if (execl("/Users/bertverstraete/ugent/semester-2/besturings2/deel-6/writestring", "writestring", "hanlonkers", (char*) 0) < 0) {
				perror(argv[0]);
				return 1;
			}

		} 	
	}
	for (i = 0; i < N; i++) {
		waitpid(kids[i], NULL, 0);
	}
	return 0;
}