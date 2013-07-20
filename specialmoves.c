#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024 * 1024 
#define MOVLEN 128
#define NAMELEN 32
#define LULEN 128
#define LF 10
#define COLON 58
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

typedef struct
{
	unsigned char name[NAMELEN];	//the name of the move
	unsigned char seq[MOVLEN];	//the key sequence of the move
	unsigned char *status;
	int length;			//the length of the move
	int count;			//counter for lookup search
} move;

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

move lu[LULEN];					//lookup array
int searches[LULEN];			//search indices
int searchcount = 0;			//current search count

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
	unsigned char tmp1 = 0;
	unsigned char tmp2 = 0;
	unsigned char curmove = 0;
	
	
	while(inleft)
	{	
		//reads from file
		byte_to_read = MIN(inleft, bufsize);
		inread = fread(inbuf, 1, byte_to_read, infile);
		inleft -= inread;
		
		//while input buffer not empty
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
			
			//reading right side of declaration
			else if(status == 1)
			{
				if(peek == LF)
				{
					status = 2;
					lu[curmove].seq[tmp2] = 0;
					lu[curmove].length = tmp2;
				}
				else
				{	
					if(tmp1)
					{
						tmp1=0;
						tmp2=0;
						curmove = peek;
						while(lu[curmove].status)
							curmove++;
						lu[curmove].status = "taken";
						strcpy(lu[curmove].name, lu[0].name);
					}
					lu[curmove].seq[tmp2] = peek;
					tmp2++;
				}
			}
			
			//reading left side of declaration
			else
			{
				if(peek == COLON)
				{
					status = 1;
					lu[0].name[tmp1] = 0;
					tmp2 = 0;
				}
				else if(peek == LF)
					status = 0;
				else
				{
					lu[0].name[tmp1] = peek;
					tmp1++;
				}
			}
		}
	}
	
	flushbuf();
//	for(i=LULEN; i--; )
//	{
//		printf("%d - %s:%s\n", i, lu[i].name, lu[i].seq);
//	}
	return 0;
}
