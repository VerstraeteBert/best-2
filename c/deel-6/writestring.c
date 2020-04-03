#include <unistd.h>
#include <stdio.h>

int main(int argc, char ** argv) {
	if (argc != 2) return 0;
	printf("PID:%d zegt %s\n", getpid(), argv[1]);
	sleep(10);
	return 0;
}
