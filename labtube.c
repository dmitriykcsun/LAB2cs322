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
	char* temp_arr[argc];
	int commasaver=0;
	int string_val;
	int status;

	for(int i=1;i<argc+1;i++){
		string_val = strcmp(argv[i],",");
		if(string_val != 0){
			temp_arr[i-1]=argv[i];
		}
		if(string_val == 0){
			commasaver = i;
			break;
		}
		
	}

	char* instr_one[commasaver];
	char* instr_two[argc-commasaver];

	for(int i=0;i<commasaver-1;i++){
		instr_one[i] = temp_arr[i];
		instr_one[commasaver-1] = NULL;
	}


	for(int i=0;i<argc-commasaver-1;i++){
		instr_two[i] = argv[i+1+commasaver];
		instr_two[argc-commasaver-1] = NULL;
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
		waitpid(-1, &status, 0);
		waitpid(-1, &status, 0);
		}
	}

	if(child_a == 0){
		printf("this is child A\n");
		dup2(pipefd[1],1);
		close(pipefd[0]);
		execve(instr_one[0],instr_one,newenviron);
		perror("exec failed");
		return 1;
	}

	if(child_b == 0){
		printf("this is child B\n");
		dup2(pipefd[0],0);
		close(pipefd[1]);
		execve(instr_two[0],instr_two,newenviron);
		perror("exec failed");
		return 1;
	}

	return 0;
}