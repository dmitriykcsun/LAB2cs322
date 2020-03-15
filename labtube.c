#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char** argv){ //int argc, char** argv
	
	int pipefd[2];
	char buf;
	pid_t child_a;
	pid_t child_b;
	char *newenviron[] ={NULL};
	char* instr_one[argc];
	char* instr_two[argc];
	int commasaver;
	int argsaver=0;

	for(int i=0;i<argc;i++){
		printf("arg %d is %s\n",i,argv[i]);
	}
	for(int i=0;i<argc;i++){
		if(argv[i] != ","){
			instr_one[i] = argv[i+1];
			commasaver=i+1;
		}
		
	}


	for(int i=0;i<argc;i++){
		printf("instr_one is %s\n",instr_one[i]);
	}

	for(int i=0;i<argc-commasaver;i++){
		instr_two[i] = argv[i+commasaver];
	}


	if (pipe(pipefd)==-1){
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	child_a = fork();

	if(child_a != 0){

		child_b = fork();

		if(child_b!=0){
		fprintf(stderr,"The PID of the first child is %d\n",child_a);
		fprintf(stderr,"The PID of the second child is %d\n",child_b);
		close(pipefd[0]);
		close(pipefd[1]);
		}
	}

	if(child_a == 0){
		printf("this is child A\n");
		dup2(pipefd[0],0);
		dup2(pipefd[1],1);
	}

	if(child_b == 0){
		printf("this is child B\n");
		dup2(pipefd[1],1);
		dup2(pipefd[0],0);
	}

	(void)argc;
	(void)argv;


	return 0;
}
