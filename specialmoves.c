#include <stdio.h>

#define CHUNK_SIZE 1000000
#define INPUT_BUFFER_SIZE 1
#define OUTPUT_BUFFER_SIZE 1
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

//enums the current state of the FSM
typedef 
	enum _FSM_state
	{
		//Left side of a move declaration (before :)
		DECLARATION_LEFT,
		
		//Right side of a move declaration (after :)
		DECLARATION_RIGHT,
		
		//Input reading
		USER_INPUT,
	}
FSM_state;

typedef
	struct _io_chunk
	{
		char chars[CHUNK_SIZE];
	}
io_chunk;

//perform declaration_left operations, and return the new current state
FSM_state declaration_left(unsigned char current_char)
{
	if(current_char == ':')
		return DECLARATION_RIGHT;
	if(current_char == '\n')
		return USER_INPUT;
	
	return DECLARATION_LEFT;
}

//perform declaration_right operations, and return the new current state
FSM_state declaration_right(unsigned char current_char)
{
	if(current_char == '\n')
		return DECLARATION_LEFT;
		
	return DECLARATION_RIGHT;
}

//perform user_input
FSM_state user_input(unsigned char current_char)
{
	return USER_INPUT;
}

//The entry point of the application
int main(int argc, const char *argv[])
{
	//Var declaration
	io_chunk input_buffer[INPUT_BUFFER_SIZE]; //input buffer
	int read_chunks;	//number of bytes read
	int input_bytes_left;
	int bytes_to_read;

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
		bytes_to_read = MIN(input_bytes_left, CHUNK_SIZE);
		
		//reads from file
		read_chunks = fread(input_buffer, bytes_to_read, INPUT_BUFFER_SIZE, fin);
		
		fwrite(input_buffer, read_chunks * bytes_to_read, read_chunks, fout);
		
		//printf("bytes_read: %d\n", read_chunks * bytes_to_read);
	}
	
	//close files
	fclose(fin);
	fclose(fout);

	return 0;
}
