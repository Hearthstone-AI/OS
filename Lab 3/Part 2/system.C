#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string.h>

using namespace std; 

class system{

   char filename[8];      //The input file
   char diskname[5];      //The disk
   char buf[1024];        //Stores data from read 
   char name[8];          //Name of the file that the command is manipulating
   int size;              //Size of the file

   fstream disk;
   char *superblock;
   char *freeblocklist;  //Points to beginning of superblock

    
   struct inode{
    char name[8];
    int32_t size;
    int32_t blockPointers[8];
    int32_t used;
   };

   //array of inodes
   inode *inodes;

  public:

    //Compares strings.
    bool strcmp(char n1[8], char n2[8]){
      int i = 0;
      for(i = 0; i < 8; i++){
        if(n1[i] != n2[i]){
          return false;
        }
      }
      return true;
    }


    //Determines if an inode exists in the inode array. Used by read, write and delete
    int exists(char name[8]){
      int index = 0;
      while(index < 16){
      if(strcmp(inodes[index].name, name) == true){
        return index;
      }
       else {
        index++;
      }
     }
     return -1;
    }
}

//Main loop for the whole program. Simply runs the mail loop of FileSystem.
int main(int argc, char *argv[]){
  FileSystem file;
  return file.main();
}