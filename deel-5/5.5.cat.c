#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

char const * program_name;
size_t BUFF_SIZE = 4096;

void read_file_to_stdout(const char *);
void read_stdi_to_stdout();
int is_regular_file(const char *);
void do_cat(size_t, const char **);


int main(int argc, const char ** argv) {
	program_name = argv[0];

	if (argc == 1) {
		// geen argumenten, lezen van std
		read_stdi_to_stdout();
	} else {
		do_cat(argc - 1, argv + 1);
	}
	exit(0);
}

int is_regular_file(const char *path) {
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}

void read_stdi_to_stdout() {
	char buffer [BUFF_SIZE]; 
	int n;
	while ( (n = read(fd, buffer, BUFF_SIZE)) > 0) {
		write(STDOUT_FILENO, buffer, n);
	}
}

void read_file_to_stdout(const char * path) {
	if (!is_regular_file(path)) {
		fprintf(stderr, "%s: %s: Is a directory\n", program_name, path);
		return;
	}

	int fd;
	if ( (fd = open(path, O_RDONLY)) < 0) {
		perror(program_name);
		return;
	}

	char buffer [BUFF_SIZE];
	int n;

	while ( (n = read(fd, buffer, BUFF_SIZE)) ) {
		if (n < 0) {
			// iets foutgelopen tijdens lezen
			perror(program_name);
			close(fd);
			exit(1);
		}
		write(STDOUT_FILENO, buffer, n);
	}

	if (close(fd) < 0) {
		perror(program_name);
		exit(1);
	}
}

void do_cat(size_t len, const char * args[]) {
	size_t i;
	for (i = 0; i < len; i++) {
		if (strcmp(args[i], "-") == 0) {
			// lezen van std ; schrijven naar stdout
			read_stdi_to_stdout();
		} else {
			read_file_to_stdout(args[i]);
		}
	}
}
