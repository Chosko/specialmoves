#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024 * 1024
#define MOVLEN 128
#define LF 10
#define COLON 58
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

FILE *infile;					//input file
FILE *outfile;					//output file
long int inleft;				//bytes left to read
long int bufsize;				//input/output buffer size
long int inread;				//bytes read from input file
long int byte_to_read;			//bytes to read from input file

unsigned char *inbuf; 			//input buffer
unsigned char *incur;			//input buffer cursor
unsigned char *inmax;			//input buffer max ptr value

unsigned char *outbuf;			//output buffer
unsigned char *outcur;			//output buffer cursor
unsigned char *outmax;			//output buffer max ptr value

unsigned char cbuf[MOVLEN];		//circular buffer
unsigned char *cleft;			//left cursor of cbuf
unsigned char *cright;			//right cursor of cbuf
unsigned char *cmax;			//max ptr value for cbuf


//writes a char into the out buffer. return new index
inline void wbuf(char c)
{
	*outcur = c;
	outcur++;
	if(outcur > outmax)
	{
		fwrite(outbuf, 1, bufsize, outfile);
		outcur = outbuf;
	}
}

//circular increments cbuf cursor
inline void cbufinc(unsigned char *cursor)
{
	cursor++;
	if(cursor > cmax)
		cursor = cbuf;
}

//flushes all left bytes into the out file
inline void flushbuf()
{
	while(cleft != cright)
	{
		wbuf(*cleft);
		cbufinc(cleft);
	}
	fwrite(outbuf, 1, outcur - outbuf, outfile);
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
	incur = inbuf;
	inmax = inbuf + bufsize-1;
	
	outbuf = (unsigned char *) malloc(bufsize);	//output buffer
	outcur = outbuf;
	outmax = outbuf + bufsize-1;
	
	cleft = cbuf;
	cright = cbuf;
	
	register int i;
	register unsigned char peek;
	register int status = 1;
	
	while(inleft)
	{
		byte_to_read = MIN(inleft, bufsize);
		inread = fread(inbuf, 1, byte_to_read, infile);
		inleft -= inread;
		for (i = 0; i < inread; i++)
		{
			peek = inbuf[i];
			
			//reading user input
			if(!status)
			{
				*cright = peek;
				cbufinc(cright);
				if(cleft==cright)
				{
					wbuf(*cleft);
					cbufinc(cleft);
				}
			}
			
			//reading left side of declaration
			else if(status == 1)
			{
				if(peek == LF)
					status = 2;
			}
			
			//reading right side of declaration
			else
			{
				if(peek == COLON)
					status = 1;
				else if(peek == LF)
					status = 0;
			}
		}
	}
	
	flushbuf();
	return 0;
}
