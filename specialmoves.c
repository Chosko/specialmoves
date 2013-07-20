#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024 * 1024
#define MOVLEN 128
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define CBUFINC(X) (X+1 == MOVLEN) ? X+1 : 0

FILE *infile;					//input file
FILE *outfile;					//output file
long int inleft;				//bytes left to read
long int bufsize;				//input/output buffer size
long int inread;				//bytes read from input file
long int byte_to_read;			//bytes to read from input file
unsigned char *inbuf; 			//input buffer
unsigned char *outbuf;			//output buffer
unsigned char cbuf[MOVLEN];		//circular buffer
long int incur = 0;				//input buffer cursor
long int outcur = 0;			//output buffer cursor
int a = 0;						//left index of cbuf
int b = 0;						//right index of cbuf


//writes a char into the out buffer. return new index
inline void wbuf(char c)
{
	outbuf[outcur] = c;
	outcur++;
	if(outcur == bufsize)
	{
		fwrite(outbuf, 1, bufsize, outfile);
		outcur = 0;
	}
}

//flushes all left bytes into the out file
inline void flushbuf()
{
	while(a != b)
	{
		wbuf(cbuf[a]);
		CBUFINC(a);
	}
	if(outcur)
		fwrite(outbuf, 1, outcur, outfile);
}

int main(int argc, char *argv[]){
	//open files
	infile = fopen("input.txt", "rb");
	outfile = fopen("output.txt", "wb");
	
	//get input file length
	fseek(infile, 0L, SEEK_END);
	inleft = ftell(infile);
	fseek(infile, 0L, SEEK_SET);
	
	//initializes buffers
	bufsize = MIN(inleft, BUFSIZE);
	inbuf = (unsigned char *) malloc(bufsize);	//input buffer
	outbuf = (unsigned char *) malloc(bufsize);	//output buffer
	
	register int i;
	register unsigned char peek;
	
	while(inleft)
	{
		byte_to_read = MIN(inleft, bufsize);
		inread = fread(inbuf, 1, byte_to_read, infile);
		inleft -= inread;
		for (i = 0; i < inread; i++)
		{
			peek = inbuf[i];
			cbuf[b] = peek;
			CBUFINC(b);
			if(b==a)
			{
				wbuf(cbuf[a]);
				CBUFINC(a);
			}
		}
	}
	
	flushbuf();
	return 0;
}
