#include <stdio.h>

#define CHUNK_SIZE 1000000
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

//The entry point of the application
int main(int argc, const char *argv[])
{
	//Var declaration
	unsigned char input_buffer[CHUNK_SIZE]; //input buffer
	int input_buffer_count;	//input buffer count
	int has_read;	//indicates if the loop has read something
	int input_bytes_left;	//the bytes left to reach the end of file
	int bytes_to_read;	//the bytes to read for each iteration
	int i;	//counter

	//open input file
	FILE *fin;
	fin = fopen(".\\large\\input.txt", "rb");
	
	//open output file
	FILE *fout;
	fout = fopen(".\\output.txt", "wb");
	
	//files not found
	if(!fin)
	{
		printf("input file not found");
		return 0;
	}
	if(!fout)
	{
		printf("output file not created");
		return 0;
	}
	
	fseek(fin, 0, SEEK_END); // seek to end of file
	input_bytes_left = ftell(fin);
	fseek(fin, 0, SEEK_SET);
	
	//the I/O loop
	while(!feof(fin))
	{
		//the number of bytes to read
		bytes_to_read = MIN(input_bytes_left, CHUNK_SIZE);
		
		//read from file
		has_read = fread(input_buffer, bytes_to_read, 1, fin);
		
		//read succesful
		if(has_read)
		{
			input_buffer_count = bytes_to_read;
			for(i=0; i<bytes_to_read; i++)
			{
				if(1);
				else;
			}
			
			//write to file
			fwrite(input_buffer, input_buffer_count, 1, fout);
		}		
	}

	return 0;
}
