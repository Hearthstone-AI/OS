#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h>


int main(int argc, char **argv) {
	while(1){
		printf("\n");
		printf("batch-shell> ");
		
		char program_string[1000];   
		if (strcmp((fgets(program_string,sizeof program_string,stdin)), "quit\n") == 0) 
			break;
	}
}