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

//=============
//this code taken from http://www.libpng.org/pub/png/spec/1.2/PNG-CRCAppendix.html
//since I didn't want to write the crc algorithm

   /* Table of CRCs of all 8-bit messages. */
   unsigned long crc_table[256];
   
   /* Flag: has the table been computed? Initially false. */
   int crc_table_computed = 0;
   
   /* Make the table for a fast CRC. */
   void make_crc_table(void)
   {
     unsigned long c;
     int n, k;
   
     for (n = 0; n < 256; n++) {
       c = (unsigned long) n;
       for (k = 0; k < 8; k++) {
         if (c & 1)
           c = 0xedb88320L ^ (c >> 1);
         else
           c = c >> 1;
       }
       crc_table[n] = c;
     }
     crc_table_computed = 1;
   }
   
   /* Update a running CRC with the bytes buf[0..len-1]--the CRC
      should be initialized to all 1's, and the transmitted value
      is the 1's complement of the final running CRC (see the
      crc() routine below)). */
   
   unsigned long update_crc(unsigned long crc, unsigned char *buf,
                            int len)
   {
     unsigned long c = crc;
     int n;
   
     if (!crc_table_computed)
       make_crc_table();
     for (n = 0; n < len; n++) {
       c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
     }
     return c;
   }
   
   /* Return the CRC of the bytes buf[0..len-1]. */
   unsigned long crc(unsigned char *buf, int len)
   {
     return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
   }

//==============

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
			

			//reallocate the space for the data		
			data = realloc(data, length + 4);
		
			//read the data
			fread(data, 1, length + 4, ifp);
			
			//show the checksum
			printf("The checksum equals %lx\n", crc(data, length + 4));

			//get the name
			strncpy(name,(char*)data,4);
			printf("The name is %s\n\n", name);

	
			/*print that crap
			for(i = 0; i < length; i++){
				printf("0x%lx\n", data[i]);
			}*/
			
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

