//Maxwell Sanders
//png alterer????
//The goals of this project
//read png
//alter png??
//profit???
//put back png
//act like nothing happened

//good ol' libs
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//takes command line input
int main(int argv, char* args[]){
	//check if there is a filename
	if(argv != 2){
		printf("The format of this program is ./progname filename.png\n"); 
	}
	else{
		//create those variables
		FILE *ifp; //input file		
		FILE *ofp; //output file
		unsigned char c; //the byte that will go through the file
		int length = 0; //the int that will hold the length of the file
		int i; //generic counter variable
		unsigned char* data; //holds the data
		char name[5]; //holds the header name

		//zero delimit for good measure
		name[4] = 0;		

		//malloc it
		data = malloc(1);
		
		//open the file for reading
		ifp = fopen(args[1], "r");

		//start off skipping the first few bytes
		fseek(ifp, 8, SEEK_SET);

		while(1){ 
			//grab the length consider the endianness
			length = 0;
			for( i = 0; i < 3; i ++){
				//append and then shift right
				//this will work like swapping
				length |= fgetc(ifp);
				length = length << 8;
			}
			length |= fgetc(ifp);
			
			//display the length proudly
			printf("Length of chunk: %d\n", length);
			
			//get the name of the chunk
			fread(name, 1, 4, ifp);
			printf("The name is %s\n\n", name);
		
			data = realloc(data, length);
			
			//print that crap
			for(i = 0; i < length; i++){
				data[i] = fgetc(ifp);
				//printf("0x%x\n", data[i]);
			}
			
			//skip the checksum
			fseek(ifp, 4, SEEK_CUR);
	
			//stop the program
			if(strncmp(name, "IEND", 4) == 0){
				break;
			}
		}

		free(data);

		fclose(ifp);
	}
}

