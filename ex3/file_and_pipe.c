/*
	AAKASH BAJAJ
	2015A2PS0586P
*/

#include <stdlib.h>	
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int n,i;
pid_t pid=0, root;

void handler(){
	// printf("Signal Recieved in %d\n", getpid());
}

void file_transfer(int n){
	FILE *in_fl, *out_fl;
	signal(SIGUSR1, handler);
	for (i = 1; i < n; ++i)
	{
		pid=fork();
		if(pid!=0)
			break;
	}

	if(getpid() != root){
		pause();
	}
	
	char c;
	// printf("In: %d\n", getpid());
	printf("File Reading Started in %d\n", getpid());
	in_fl = fopen("tmpout", "r");
	out_fl = fopen("tmp", "w");
	c = fgetc(in_fl);
	while(c != EOF){
		fputc(c, out_fl);
		c = fgetc(in_fl);
	}
	fclose(in_fl);
	remove("tmpout");
	fclose(out_fl);
	rename("tmp", "tmpout");
	printf("File Writing Done in %d\n", getpid());
	kill(pid, SIGUSR1);
	wait(NULL);
}

void pipe_transfer(int n){
	int fd[n][2], i;
	char buf[2048];
	fd[0][0] = open("tmpout", O_RDONLY);
	pid = 0;
	for(i = 1; i < n; i++) {
		pipe(fd[i]);
		pid = fork();
		if(pid != 0)	break;
	}
	if(getpid() != 0)	close(fd[i-1][1]);
	if(pid != 0)	close(fd[i][0]);
	int readfd = fd[i-1][0], writefd;
	if(pid != 0)	writefd = fd[i][1];
	else	writefd = open("tmp", O_CREAT|O_WRONLY);
	size_t read_size;
	while((read_size = read(readfd, buf, 2048)) != 0)
		write(writefd, buf, read_size);
	close(writefd);
	close(readfd);
}

int main()
{
	root = getpid();
	remove("tmpout");
	remove("tmp");

	// Creating 1GB file
	FILE *fl;
	fl = fopen("tmpout","w");
	for (int i = 0; i < 1024; ++i)
	{
		fseek(fl, (1024*1024), SEEK_CUR);
		fprintf(fl, "c");
	}
	fclose(fl);

	scanf("%d", &n);

	printf("Tranfer Using Temporary Files:\n");
	struct timeval t1, t2;
	double elapsedTime;
	gettimeofday(&t1, NULL);
	file_transfer(n);	
	gettimeofday(&t2, NULL);

	elapsedTime = (t2.tv_sec - t1.tv_sec)*1000.0;
	elapsedTime += (t2.tv_usec - t1.tv_usec)/1000.0;
	if(getpid() == root){
		printf("Total time in File Tranfer: %f ms.\n", elapsedTime);
	}
	
	if(getpid() != root){
		exit(0);
	}

	printf("\n");

	printf("Tranfer Using Pipe:\n");

	gettimeofday(&t1, NULL);
	pipe_transfer(n);
	gettimeofday(&t2, NULL);

	elapsedTime = (t2.tv_sec - t1.tv_sec)*1000.0;
	elapsedTime += (t2.tv_usec - t1.tv_usec)/1000.0;
	if(getpid() == root){
		printf("Total time in Pipe Tranfer: %f ms.\n", elapsedTime);
	}

	return 0;
}
