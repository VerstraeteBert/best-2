#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main (int argc, char ** argv) {
	if (argc!=2) {
		fprintf(stderr, "%s: No file provided", argv[0]);
		return 1;
	}

	int fd;
	if ( (fd = open(argv[1], O_RDONLY)) < 0) {
		perror(argv[1]);
		return 1;
	}

	struct stat sb;
	if (fstat(fd, &sb) < 0) {
		perror(argv[1]);
		return 1;
	}

	if (!S_ISREG(sb.st_mode)) {
		fprintf(stderr, "%s: is not a regular file\n", argv[1]);
		exit(1);
	}

	int orig = sb.st_mtime;
	while (1) {
		fstat(fd, &sb);
		if (orig != sb.st_mtime) {
			printf("Gewijzigd!\n");
			orig = sb.st_mtime;
		}
	}

	close(fd);

	return 0;
}