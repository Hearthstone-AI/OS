#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	//prints the current working directory for user clarity
	printf("starting batch-shell\n");
	char dir[1000];
	getcwd(dir, sizeof(dir));
	fprintf(stdout, "Current directory is: %s\n", dir);
	
	//begin main loop of program
	while(1){
		printf("\n");
		printf("batch-shell> ");
		
		//recive user input and handle the quit command
		char inputLine[1000];
		if (strcmp((fgets(inputLine,sizeof inputLine,stdin)), "quit\n") == 0)
			exit(0);
		
		//trim the newline at the end of user input
		char* tokens;
		tokens = strtok(inputLine, "\n");
		strcpy(inputLine, tokens);
		
		//prepare the return status and tokenize the user input by space
		int returnStatus;
		tokens = strtok(inputLine, " ");
		
		//execute each program
		while(tokens != NULL){
		
			//fork the process
			pid_t id = fork();
			switch(id){
				case -1: //case error
				{
					perror("ERROR: forking child process failed\n");
					exit(3);
				}
				case 0: //case child
				{
					printf("\nStarting program: %s, pid: %d\n", tokens, getpid());
					//execute the program and handle file not found
					if(execl(tokens, tokens, NULL) == -1){
						perror("ERROR");
						exit(3);
					}
					exit(0);
				}
				default: //case parent
				{
					//wait for child to finish before executing next program and handle errors from child
					waitpid(id, &returnStatus, 0);
					if(WEXITSTATUS(returnStatus) == 3)
						printf("\nProgram: %s, pid: %d encountered a nonfatal error\n", tokens, getpid());
				}
			}
			//get next token
			tokens = strtok(NULL, " ");
		}
	}
}
