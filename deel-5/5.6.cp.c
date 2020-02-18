#include <stdio.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>

const int BUFF_SIZE = 4096;

void copy(const char *, const char *);
int is_valid_file(const char *);

const char * program_name;

int main (int argc, char ** argv) {
	program_name = argv[0];
	if (argc < 3) {
		fprintf(stderr, "%s: Must supply src and dest file path\n", program_name);
		exit(1);
	}
	copy(argv[1], argv[2]);
	return 0;
}

int is_dir(const char * path) {
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISDIR(path_stat.st_mode);
}

void copy(const char * src, const char * dest) {
	if (is_dir(src)) {
		fprintf(stderr, "%s: %s is a directory\n", program_name, src);
		exit(1);
	}

	if (is_dir(dest)) {
		fprintf(stderr, "%s: %s is a directory\n", program_name, dest);
		exit(1);
	}

	int fd_in, fd_out;

	if ( (fd_in = open(src, O_RDONLY)) < 0) {
		perror(program_name);
		exit(1);
	}
	// 0666 -> RW for U, GR, R
	if ( (fd_out = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0666 )) < 0) {
		perror(program_name);
		exit(1);
	}

	char buffer [BUFF_SIZE]; 
	size_t n;

	while ( (n = read(fd_in, buffer, BUFF_SIZE)) ) {
		if (n < 0) {
			perror(program_name);
			unlink(dest);
			close(fd_in);
			close(fd_out);
			exit(1);
		}
		write(fd_out, buffer, n);
	}


	if (close(fd_in) < 0) {
		perror(program_name);
		close(fd_out);
		exit(1);
	}

	if (close(fd_out) < 0) {
		perror(program_name);
		exit(1);
	}
}
