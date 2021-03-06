#include <sys/inotify.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int is_reg_file(const char *);

const int BUFF_SIZE = 4096;
const char * program_name;

int main (int argc, char ** argv) {
	program_name = argv[0];

	if (argc < 2) {
		fprintf(stderr, "%s: please provide a file to watch", program_name);
		exit(1);
	}

	if (!is_reg_file(argv[1])) {
		fprintf(stderr, "%s: %s is not a valid file", program_name, argv[1]);
		exit(1);
	}

	int fd_notif;
	if ( (fd_notif = inotify_init()) == -1) {
		perror("inotify_init");
		exit(1);
	}

	int fd_file_watch;
	if ( ( fd_file_watch = inotify_add_watch(fd_notif, argv[1], IN_MODIFY)) == -1) {
		perror("inotify_add_watch");
		exit(1);
	}

	
    unsigned char buf[BUFF_SIZE];
    int len;
    struct inotify_event * e_ptr; 

    while ( len = read(fd_notif, buf, BUFF_SIZE) ) {
    	if (len == -1) {
    		// file watch automatisch geclosed na closing van notif fd
    		perror(program_name);
    		close(fd_notif);
    		exit(1);
    	}

    	if (len > 0) {
    		int i;
    		for (i = 0; i < len; i++) {
    			e_ptr = (struct inotify_event *) &(buf[i]);
    			if (e_ptr->mask & IN_MODIFY) {
    				printf("%s\n", "File modified");	
    			}
    		}
    	}
    }
}

int is_reg_file(const char * path) {
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}
