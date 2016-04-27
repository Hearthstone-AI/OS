#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string.h>

using namespace std;

class FileSystem {

	struct inode {
		char name[8];
		int32_t size;
		int32_t blockPointers[8];
		int32_t used;
	};

	char buf[1024];        //Stores data from read
	char name[8];          //Name of the file that the command is manipulating
	int size;              //Size of the file


	fstream disk;
	char freeblocklist[128];  //Points to beginning of superblock

	//array of inodes
	inode inodes[16];



private:

	//Compares strings.
	bool strcmp(char n1[8], char n2[8]) {
		int i = 0;
		for (i = 0; i < 8; i++) {
			if (n1[i] != n2[i]) {
				return false;
			}
		}
		return true;
	}

	//Determines if an inode exists in the inode array. Used by read, write and delete
	int exists(char name[8]) {
		int index = 0;
		while (index < 16) {
			if ((strcmp(inodes[index].name, name) == true) && (inodes[index].used != 0)) {
				return index;
			}
			else {
				index++;
			}
		}
		return -1;
	}

public:

	int createF(char name[8], int32_t size) {
		if (exists(name) != -1) {
			return 5; //Error Code 5 File already Exists
		}
		if (size > 8) {
			return 2; //Error Code 2 File too large
		}
		//Begin check for the amount of free blocks
		int freecount = 0;
		for (int i = 0; i < 127; i++) {
			if (freeblocklist[i] == 0) {
				freecount++;
			}
		}
		if (size > freecount) {
			return 3; //Error Code 3 Not Enough Space
		}
		//Begin check and assignment of free inode
		freecount = -1;
		for (int i = 0; i < 16; i++) {
			if (inodes[i].used == 0) {
				freecount = i;
				break;
			}
		}
		if (freecount == -1) {
			return 4; //Error Code 4 Too many files
		}

		//Begin assignment of blocks
		memcpy(inodes[freecount].name, name, 8);
		inodes[freecount].size = size;
		inodes[freecount].used = 1;
		int assigned = 0;
		for (int i = 0; assigned < size; i++) {
			if (freeblocklist[i] == 0) {
				freeblocklist[i] = 1;
				inodes[freecount].blockPointers[assigned++] = i;
			}
		}

		//Begin buffer copy
		memcpy(buf, freeblocklist, 128);
		for (int i = 0; i < 16; i++) {
			memcpy(buf, inodes[i].name + 128 + 48 * i, 8);
			memcpy(buf, &(inodes[i].size) + 136 + 48 * i, 4);
			memcpy(buf, inodes[i].blockPointers + 140 + 48 * i, 32);
			memcpy(buf, &inodes[i].used + 172 + 48 * i, 4);
		}
		for (int i = 728; i < 1024; i++) {
			buf[i] = 0;
		}
		//Begin Transfer
		disk.seekp(0);
		disk.write(buf, 1024);
		return 0; //Successful return
	}

	int deleteF(char name[8]) {
		int nodeindex = exists(name);
		if (nodeindex == -1) {
			return 1; //Error Code 1 File doesnt Exist
		}

		//Begin reassignment of blocks
		inodes[nodeindex].used = 0;
		int assigned = 0;
		for (int i = 0; i < inodes[nodeindex].size; i++) {
			freeblocklist[inodes[nodeindex].blockPointers[i]] = 0;
		}
		for (int i = 0; i < 8; i++) {
			inodes[nodeindex].blockPointers[i] = 0;
		}
		inodes[nodeindex].size = 0;

		//Begin buffer copy
		memcpy(buf, freeblocklist, 128);
		for (int i = 0; i < 16; i++) {
			memcpy(buf, inodes[i].name + 128 + 48 * i, 8);
			memcpy(buf, &(inodes[i].size) + 136 + 48 * i, 4);
			memcpy(buf, inodes[i].blockPointers + 140 + 48 * i, 32);
			memcpy(buf, &inodes[i].used + 172 + 48 * i, 4);
		}
		for (int i = 728; i < 1024; i++) {
			buf[i] = 0;
		}
		//Begin Transfer
		disk.seekp(0);
		disk.write(buf, 1024);
		return 0; //Successful return
	}

	int readBlock (char name[8], int32_t blockNum, char buff[1024]) {
		int nodeindex = exists(name);
		if (nodeindex == -1) {
			return 1; //Error Code 1 File doesnt Exist
		}
		if (blockNum > inodes[nodeindex].size) {
			return 6; //Error Code 6 Block access out of bounds
		}
		disk.seekg((1 + inodes[nodeindex].blockPointers[blockNum]) * 1024);
		disk.read(buff, 1024);
		return 0; //Successful return
	}

	int writeBlock(char name[8], int32_t blockNum, char buff[1024]) {
		int nodeindex = exists(name);
		if (nodeindex == -1) {
			return 1; //Error Code 1 File doesnt Exist
		}
		if (blockNum > inodes[nodeindex].size) {
			return 6; //Error Code 6 Block access out of bounds
		}
		disk.seekp((1 + inodes[nodeindex].blockPointers[blockNum]) * 1024);
		disk.write(buff, 1024);
		return 0; //Successful return
	}

	void ls() {
		for (int i = 0; i < 16; i++) {
			if (inodes[i].size > 0) {
				printf("Filename: %s Size: %d\n", inodes[i].name, inodes[i].size);
			}
		}
	}

	void init(char diskn[5]) {
		disk.open(diskn, ios_base::in | ios_base::out);
		if (!disk.is_open()) {
			return;
		}
		disk.read(buf, 1024);
		memcpy(freeblocklist, buf, 128);
		for (int i = 0; i < 16; i++) {
			memcpy(inodes[i].name, buf + 128 + 48 * i, 8);
			memcpy(&(inodes[i].size), buf + 136 + 48 * i, 4);
			memcpy(inodes[i].blockPointers, buf + 140 + 48 * i, 32);
			memcpy(&inodes[i].used, buf + 172 + 48 * i, 4);
		}
		return;
	}

	void finish() {
		disk.close();
	}
};

//Flavor code for error text
void returnHandle(int code) {
	switch (code) {
		case 0: {
			printf("Operation Success\n");
			break;
		}
		case 1: {
			printf("Error: File doesnt exist\n");
			break;
		}
		case 2: {
			printf("Error: File too large\n");
			break;
		}
		case 3: {
			printf("Error: Not enough space\n");
			break;
		}
		case 4: {
			printf("Error: Too many files\n");
			break;
		}
		case 5: {
			printf("Error: File already exists\n");
			break;
		}
		case 6: {
			printf("Error: Block address out of bounds\n");
			break;
		}
	}
}



//Main loop for the whole program. Simply runs the main loop of FileSystem.
int main(int argc, char *argv[]) {
	//Dummy Buffer
	char dummy[1024];
	for (int i = 0; i < 1024; i++) {
		dummy[i] = 4;
	}
	//Prepare input file parsing
	char* parse;
	char inputline[256];
	fstream inputfile;
	inputfile.open("lab3Input.txt", ios_base::in);

	//Begin main code
	FileSystem fs;
	//Parsing
	while (inputfile.peek() != EOF) {
		inputfile.getline(inputline, 256);
		printf("%s\n", inputline);
		parse = strtok(inputline, " ");
		//Handle ls
		if (*parse == 'L') {
			fs.ls();
		}
		//Handle cases, use 2 buffers to handle spaces in names
		else if(*parse == 'R'){
			string fn("");
			char* fnbuf = "";
			char* buf2 = "";
			while (parse != NULL) {
				parse = strtok(NULL, " ");
                fn.append(fnbuf);
				fnbuf = buf2;
				buf2 = parse;
			}
			fn.resize(8);
			char convname[8];
			memcpy(convname, fn.data(), 8);
			returnHandle(fs.readBlock(convname, atoi(fnbuf), dummy));
		}
		else if (*parse == 'W') {
			string fn("");
			char* fnbuf = "";
			char* buf2 = "";
			while (parse != NULL) {
				parse = strtok(NULL, " ");
                fn.append(fnbuf);
				fnbuf = buf2;
				buf2 = parse;
			}
			fn.resize(8);
			char convname[8];
			memcpy(convname, fn.data(), 8);
			returnHandle(fs.writeBlock(convname, atoi(fnbuf), dummy));
		}
		else if (*parse == 'C') {
			string fn("");
			char* fnbuf = "";
			char* buf2 = "";
			while (parse != NULL) {
				parse = strtok(NULL, " ");
                fn.append(fnbuf);
				fnbuf = buf2;
				buf2 = parse;
			}
			fn.resize(8);
			char convname[8];
			memcpy(convname, fn.data(), 8);
			returnHandle(fs.createF(convname, atoi(fnbuf)));
		}
		else if (*parse == 'D') {
			string fn("");
			char* fnbuf = "";
			while (parse != NULL) {
				parse = strtok(NULL, " ");
                fn.append(fnbuf);
				fnbuf = parse;
			}
			fn.resize(8);
			char convname[8];
			memcpy(convname, fn.data(), 8);
			returnHandle(fs.deleteF(convname));
		}
		else {
			fs.init(parse);
		}
	}

	fs.finish();
	return 0;
}
