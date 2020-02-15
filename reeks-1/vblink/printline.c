#include <stdio.h>

int lijnnr;

void println(char * str, int n) {
	printf(" %d", lijnnr++);
	int i;
	for (i = 0; i < n; i++) {
		printf("%c", str[i]);
	}
}
