//Maxwell Sanders steganographic generator and decrypter

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//the main function
int main(int argv, char* args[]){
	//if there is no arguments then tell the user
	if(argv<5){
		printf("Please use '-d [H] [S] [N]' to decrypt\n");  
		printf("Please use '-e [M] [S] [N] [P]' to encrypt\n");
		printf("H - modulated medium\n");
		printf("S - bytes to skip\n");
		printf("M - original medium\n");
		printf("N - intervals to implement\n");
		printf("P - payload\n");
	}
	//is it encrypting check for the other arguments
	else if(strcmp(args[1],"-e") == 0){
		printf("We are encrypting\n");
		//open the file
		char output[100];
		sprintf(output,"modified_%s", args[2]);
		FILE *ofp = fopen(output, "w");
		FILE *ifp = fopen(args[2], "r");

		//set up the variables necessary
		int c;
		int offset = atoi(args[3]);
		int interval = atoi(args[4]);	
		long int pos;	
		char message[1000];
		int byte;
		int rot = 1;
		int length = strlen(args[5]);
		int count = 0;

		//strcpy becuase I don't care about secure programming		
		strncpy(message, args[5], strlen(args[5]));
	
		//debug
		printf("off,interval,message = %d,%d,%s\n", offset,interval,message);
		
		//iterate through the file byte by byte
		c = fgetc(ifp);
		while(c != EOF){
			//get the position 
			pos = ftell(ifp);
			//check if we are past the offset and on interval
			if(count <= length+1 
				&& pos >= offset 
				&& (pos - offset)%interval==0){
				//debug
				printf("%x  %ld\n",c, pos);

				//delete the last 2 bytes
				c &= 0xfc;
	
				//put in the new bytes
				if(rot == 1){
					rot = 2;
					//first two bits
					byte = message[count] & 0xc0;
					//shift 6 to the right
					byte = byte >> 6;
					//append the bytes on c
					c |= byte;
				}
				else if(rot == 2){
					rot = 3;
					//first two bits
					byte = message[count] & 0x30;
					//shift 4 to the right
					byte = byte >> 4;
					//append the bytes on c
					c |= byte;
				}
				else if(rot == 3){
					rot = 4;
					//first two bits
					byte = message[count] & 0x0c;
					//shift 2 to the right
					byte = byte >> 2;
					//append the bytes on c
					c |= byte;
				}
				else if(rot == 4){
					rot = 1;
					//first two bits
					byte = message[count] & 0x03;
					//append the bytes on c
					c |= byte;
					//go to the next letter
					count ++;
					//clean debug
					printf("\n");
				}
				
				//put the new bytes into the file
				fprintf(ofp,"%c",c);
			}
			else{
				//put the unchanged byte into the file
				fprintf(ofp,"%c",c);
			}
			//grab the next byte
			c = fgetc(ifp);
		}
		printf("You have hidden the message!\n");

		//close the file;
		fclose(ofp);
		fclose(ifp);
	}
	//is it decrypting check for the other arguments
	else if(strcmp(args[1],"-d") == 0){
		printf("We are decrypting\n");
		//open the file
		char output[100];
		FILE *ifp = fopen(args[2], "r");

		//set up the variables necessary
		int c;
		int offset = atoi(args[3]);
		int interval = atoi(args[4]);	
		long int pos;	
		char message[1000];
		int byte;
		int rot = 1;
		int count = 0;

		c = fgetc(ifp);
		while(c != EOF){
			//get the position 
			pos = ftell(ifp);
			//check if we are past the offset and on interval
			if(pos >= offset  
				&& (pos - offset)%interval==0){
				//debug
				printf("%x  %ld\n",c, pos);

				//save the last two bytes
				c &= 0x03;
	
				//put in the new bytes
				if(rot == 1){
					rot = 2;
					//shift 6 to the left
					c = c << 6;
					//put the byte in the message
					message[count] &= 0x00;
					message[count] |= c;
				}
				else if(rot == 2){
					rot = 3;
					//shift 4 to the left
					c = c << 4;
					//put the byte in the message
					message[count] |= c;
				}
				else if(rot == 3){
					rot = 4;
					//shift 2 to the left
					c = c << 2;
					//put the byte in the message
					message[count] |= c;
				}
				else if(rot == 4){
					rot = 1;
					//put the byte in the message
					message[count] |= c;
					//go to the next letter
					count ++;
					//clean debug
					printf("\n");	
				
					//break out of the loop if we got termination
					if(message[count-1] == 0){
						break;
					}
				}
				
			}
			//grab the next byte
			c = fgetc(ifp);
		}
		
		//print the message
		printf("The message: %s\n", message);


		//close the file;
		fclose(ifp);
	}
	//if you didn't find the command then just give the debug
	else{
		printf("Please use '-d [H] [S] [N]' to decrypt\n");  
		printf("Please use '-e [M] [S] [N] [P]' to encrypt\n");
		printf("H - modulated medium\n");
		printf("S - bytes to skip\n");
		printf("M - original medium\n");
		printf("N - intervals to implement\n");
		printf("P - payload\n");
	}
}
