#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX 50
int main(int argc, char **argv){
	int pids[MAX];
	pid_t pid;
	if (argc!=2){
		printf("(Only) One argument expected!\n");
		exit(1);
	}
	else {
		int fd[2];
		int i;
		for (i=0;i<atoi(argv[1]);i++){
			if (pipe(fd)<0){
				perror(argv[0]);
				exit(1);
			}
			if ((pids[i]=fork())<0){
				perror(argv[0]);
			}
			if (pids[i]==0){
				close(fd[0]);
				srand(getpid());
				int number=rand();
				if ((write(fd[1],&number,sizeof(int))!=sizeof(int))) {
					perror(argv[0]);
					exit(1);
				}	
				exit(0);
			}
			else {
				int n;
				if (read(fd[0],&n,sizeof(int))!=sizeof(int)){
					perror(argv[0]);
				}
				printf("%d %d\n",pids[i],n);
				close(fd[1]);
			}
		}
	}
	int i;
	for(i=0;i<atoi(argv[1]);i++){
		waitpid(pids[i],NULL,0);
	}
	return 0;
}