#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char** argv){ //int argc, char** argv

	pid_t pid;
	char *newenviron[] ={NULL};
	(void)argc;
	int childStat;

	pid = fork();

	if(pid != 0){
		fprintf(stderr, "The PID of child is %d\n",pid);
	}

	if(pid == 0){
		execve(argv[1],argv+1,newenviron);
		perror("execve failed");
		return 1;
	}

	else{
	waitpid(pid,&childStat,0);

	if(WIFEXITED(childStat)){
		fprintf(stderr,"return value of child is %d\n",WEXITSTATUS(childStat));
	}
	else if(WIFSIGNALED(childStat)){
		fprintf(stderr,"return value of child is %d\n",WTERMSIG(childStat));
	}
	else if(WIFSTOPPED(childStat)){
		fprintf(stderr,"return value of child is %d\n",WSTOPSIG(childStat));
	}
}
	return 0;
}