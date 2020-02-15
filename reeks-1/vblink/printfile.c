#include <stdio.h>
#include <stdlib.h>

extern int lijnnr = 0;


void printfile(const char *naam) {
	FILE *f = fopen(*naam, "r");

	char *buffer = NULL;
	// zie man getline -- buffer = null
	// -> getline zal zelf buffer initialiseren


	int n = 0;
	int aantal = getline(&buffer, &n, f);

	// als aantal < 0 -> EOF
	while (aantal >= 0) {
		println(buffer, aantal);
		aantal = getline(&buffer, &n, f);
	}
	free(buffer);
	fclose(f);
}

int main (void) {
	printfile("/etc/psswd");
	return 0;
}

// compileren met -c -> object files aanmaken
// gcc -c printline.c & gcc -c printfile.o
// gcc printline.o printfile.o
