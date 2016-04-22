#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>     
#include <stdlib.h> 
#include <math.h>

using namespace std;

int main(void){

	int n; //The n-lowest bits that represent the offset
	int m; //The next m bits that represent the page number
	unsigned int mems [100]; //Array to hold addresses
	int total = 0; //Total number of addresses
	string line; //Line of the file
	
	//Open the file
	std::fstream myfile("input.txt", std::ios_base::in);

	if (myfile.is_open()){

		//Get the values of n and m from the file
	    myfile >> n >> m;
	    
	    //Adds addresses to the mems array by skipping the first two lines, m and n
	    for (int lineno = 0; getline (myfile,line); lineno++){
	    	if (lineno == 0)
	    		continue;
	    	if (lineno == 1)
	    		continue;
	    	else{
	    		//Converts the string number to an unsigned int
	    		unsigned int addr = atoll(line.c_str());
	    		mems[total] = addr;
	    		total++;
	    	}
	    	
	    	

	    }
	    //Determine the page numbers and offsets for each address
	    for (int i = 0; i < total; i++){ 
	    	//Bitmask for n
	    	int nMask = pow(2, n+1) - 1;

	    	
    		unsigned int page = mems[i]>>n; //Page number
    		unsigned int offset = mems[i]&nMask; //Offset
    		

    		printf("Virtual address %d is in page number %d and offset %d\n",mems[i],page,offset);
    	}

	    myfile.close();

	}
	return 0;

}