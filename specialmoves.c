#include <stdio.h>

#define INPUT_BUFFER_SIZE 1000000

int main(int argc, const char *argv[])
{
	//creates input buffer
	unsigned char input_buffer[INPUT_BUFFER_SIZE];
	int i;


	//open input file
	FILE *fin;
	fopen_s(&fin, "input.txt", "r");
	

	while(!feof(fin))
	{
		int read_bytes = fread(input_buffer, sizeof(unsigned char), INPUT_BUFFER_SIZE, fin);

		for (i = 0; i < INPUT_BUFFER_SIZE; i++)
		{
			printf("%c", input_buffer[i]);
		}
	}

	return 0;
}