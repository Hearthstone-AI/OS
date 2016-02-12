#include <stdlib.h>	
#include <unistd.h>	
#include <stdio.h>
#include <signal.h>
#include <errno.h>	
#include <string.h>

int main(int argc, char **argv){

	//Prompt for the user.
	puts("Enter a process to kill: ");
	int pid;
	scanf("%d",&pid);

	//The value is stored in pid, which kill is passed.
	int killed = kill(pid,SIGKILL);

	//If the process doesn't exist, or if the process was unable to be killed
	//then an error is printed.
	if(killed < 0)
		printf("%s\n", strerror(errno));

	//Otherwise, prints a sucess message
	else
		printf("Killing %d ...\n",pid);
	
	//The return value of kill is returned.
	return killed;

}