#include <stdlib.h>	
#include <unistd.h>	
#include <stdio.h>	


int main(int argc, char **argv) {

	//Process ids of the child and the grandchild
	pid_t pid = fork();
	pid_t gpid;

	//Creates the first child. It prints after the grandchild exits.
	if(pid == 0){
		gpid = fork();
		if(gpid == 0){
			//Grandchild is created. It prints after waiting for 10 seconds.
			sleep(10);
			printf("Grandchild process with process ID %d has completed.\n",getpid());
			exit(0);
		}else if(gpid < 0){
			perror("ERROR: forking child process failed\n");
      		exit(3);
		}
		
		//Child waits for grandshild to exit, then prints its message here.
		int returnStatus;    
    	waitpid(gpid, &returnStatus, 0);
    	if(returnStatus == 0){
			printf("Child process with process ID %d has completed.\n",getpid());
			exit(0);
		}

	}
	else if(pid < 0){
			perror("ERROR: forking child process failed\n");
      		exit(3);
		}
	//Parent waits for the child to exit, and then prints its ID.
	else{
		int returnStatus;    
	    waitpid(pid, &returnStatus, 0);

	    if (returnStatus == 0) 
	       printf("Parent process with process ID %d has completed.\n",getpid());    
		}
	
}