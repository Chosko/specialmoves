#include <stdio.h>

#define INPUT_BUFFER_SIZE 1024

int main(int argc, const char *argv[])
{
	//creates input buffer
	unsigned char input_buffer[INPUT_BUFFER_SIZE];
	int i;
	int read_bytes;

	//open input file
	FILE *fin;
	fin = fopen(".\\small\\input.txt", "rb");
	
	//open output file
	FILE *fout;
	fout = fopen(".\\output.txt", "wb");
	
	//controls files not found
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
	
	while(!feof(fin))
	{
		read_bytes = fread(input_buffer, sizeof(unsigned char), INPUT_BUFFER_SIZE, fin);
		fwrite(input_buffer, sizeof(unsigned char), read_bytes, fout);
	}
	
	//close files
	fclose(fin);
	fclose(fout);

	return 0;
}
