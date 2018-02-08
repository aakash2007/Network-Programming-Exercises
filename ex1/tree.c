#include <stdio.h>
#include <stdlib.h>

void createChild(int n, int level, int pos){
	printf("%d\t%d\t%d\t%d\n", level, getpid(), getppid(), pos);
	if(n==0){
		exit(0);
	}
	for(int i=0; i<n; i++){
		int pid = fork();
		if(pid==0){
			createChild(n-1, level+1, i+1);
			exit(0);
		}
		wait(NULL);
	}
}

int main(){
	int k;
	scanf("%d", &k);
	printf("Level\tpid\tppid\tPosition\n");
	createChild(k,0,1);
	printf("All Children Exited\n");
	exit(0);
}
