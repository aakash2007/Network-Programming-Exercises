#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t pid = 0, root;
int n, i;

void handler()
{
	sleep(0.1);
}

void handler2()
{
	sleep(0.1);
}

int main(int argc, char const *argv[])
{
	if(argc < 2){
		return 0;
	}
	n = atoi(argv[1]);
	root = getpid();

	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler2);
	
	for(i = 1; i < n; i++) {
		pid = fork();
		if(pid != 0)
			break;
	}

	if(pid != 0)
		pause();

	if(getpid() != root)
		kill(getppid(), SIGUSR2);
	
	for(int j = 0; j < 1000; j++) {
		if(getpid() == root)
			printf("%d\t", j+1);
		
		if(getpid() != root)
			pause();

		printf("%d\t", getpid());
		fflush(stdout);
		
		if(pid != 0) {
			kill(pid, SIGUSR1);
			pause();
		}

		if(pid == 0)
			printf("\n");

		if(getpid() != root)
			kill(getppid(), SIGUSR2);
	}
	wait(NULL);
	return 0;
}
