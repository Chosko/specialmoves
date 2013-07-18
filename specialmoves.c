#include <stdio.h>
#include <stdlib.h>

#define CHUNK_SIZE 32 * 1024
#define MOVE_SIZE 32
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

// ------------------------- TYPEDEFS -------------------------
//move
typedef
struct _move
{
	char n[MOVE_SIZE]; //name of the move
}
* move;

//list
typedef
struct _list_node
{
	struct _list_node *n; //next
	struct _list_node *p; //previous
	struct _tree_node *t; //reference to tree
}
*list;

//tree
typedef 
struct _tree_node
{
	unsigned char c; //char
	move m; //if is a move, points to a move, else NULL
	list l; //list of subtrees
}
*tree;

// ------------------------- PROTOTYPES -------------------------

//displays a tree
void display_tree(tree t, int depth);

//initializes a tree
void create_tree(tree *t);

//initializes a list
void create_list(list *l, unsigned char c);

//initializes a move
void create_move(move *m);

//destroys a list
void destroy_list(list l);

//destroys a move
void destroy_move(move m);

//destroys a tree
void destroy_tree(tree t);

//finds the given char in the next level of the tree, or returns NULL
tree find_next(tree t, unsigned char c);

//finds or create the given char in the next level of the tree
tree find_or_create_next(tree t, unsigned char c);

// ------------------------- FUNCTIONS --------------------------
void display_tree(tree t, int depth)
{
	if(t)
	{
		int i;
		list l = t->l;
		
		//indentation
		for (i = 0; i < depth; i++)
		{
			printf("  ");
		}
		
		//prints the node char
		printf("%c", t->c);
		
		//prints the special move
		if(t->m)
		{
			printf(" (%s)", t->m->n);
		}
		printf("\n");
		
		//prints children
		while(l)
		{
			display_tree(l->t, depth + 1);
			l = l->n;
		}
	}
}

//initializes a tree
void create_tree(tree *t)
{
	*t = (tree)malloc(sizeof(struct _tree_node));
	(*t)->c = 0;
	(*t)->l = NULL;
	(*t)->m = NULL;
}

//initializes a list
void create_list(list *l, unsigned char c)
{
	*l = (list)malloc(sizeof(struct _list_node));
	(*l)->n = NULL;
	(*l)->p = NULL;
	create_tree(&((*l)->t));
	(*l)->t->c = c;	
}

//initializes a move
void create_move(move *m)
{
	*m = (move)malloc(sizeof(struct _move));
	(*m)->n[0] = 0;
}

//destroys a list
void destroy_list(list l)
{
	if(l->n != NULL)
	{
		destroy_list(l->n);
	}
	if(l->t != NULL)
	{
		destroy_tree(l->t);
	}
	free(l);
}

//destroys a move
void destroy_move(move m)
{
	free(m);
}

//destroys a tree
void destroy_tree(tree t)
{
	if(t->m != NULL)
	{
		destroy_move(t->m);
	}
	if(t->l != NULL)
	{
		destroy_list(t->l);
	}
	free(t);
}

//finds the given char in the next level of the tree, or returns NULL
tree find_next(tree t, unsigned char c)
{
	while(1)
	{
		printf("not implemented \"find_next\" function\n");
	}
	return NULL;
}

//finds or create the given char in the next level of the tree
tree find_or_create_next(tree t, unsigned char c)
{
	//if empty tree (the root), create first node
	if(!(t->l))
	{
		create_list(&(t->l), c);
	}
	
	list l = t->l; 		//iterator
	list new;			//new node, if necessary
	unsigned char peek; //current reading char
	
	//loop. find or create!
	while(1)
	{
		peek = l->t->c;
		
		//looked char exceeded. create new node before and return
		if(peek > c)
		{
			create_list(&new, c);
			new->p = l->p;
			new->n = l;
			
			//if the peek is not the first node
			if(l->p)
			{
				l->p->n = new;
			}
			
			//if the peek is the first node, must update t->l
			else
			{
				t->l = new;
			}
			
			l->p = new;
			return new->t;
		}
		
		
		//looked char reached. return found node
		else if(peek == c)
		{
			return l->t;
		}
		
		//looked char not reached, try next
		else
		{
			//if next doesn't exists, create it
			if(!(l->n))
			{
				create_list(&new, c);
				new->p = l;
				l->n = new;
				return new->t;
			}
			
			//if next exists, iterate over next
			else
			{
				l = l->n;
			}
		}
	}
	return NULL;
}

// ------------------------- MAIN -------------------------

//The entry point of the application
int main(int argc, const char *argv[])
{
	//Var declaration
	int has_read;	//indicates if the loop has read something
	int input_bytes_left;	//the bytes left to reach the end of file
	int bytes_to_read;	//the bytes to read for each iteration
	int current_state = 0; //the FSM state
	unsigned char current_char; //the current char read
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
	
	int bufsize = MIN(CHUNK_SIZE, input_bytes_left);
	
	unsigned char input_buffer[MIN(CHUNK_SIZE, input_bytes_left)]; //input buffer
	unsigned char output_buffer[MIN(CHUNK_SIZE, input_bytes_left)]; //output buffer
	move current_move;
	
	tree root;
	create_tree(&root);
	tree iterator = root;
	
	int left_decl_counter = 0;
	
	//the I/O loop
	while(input_bytes_left > 0)
	{
		//the number of bytes to read
		bytes_to_read = MIN(input_bytes_left, bufsize);
		input_bytes_left -= bytes_to_read;
		
		//read from file
		has_read = fread(input_buffer, bytes_to_read, 1, fin);
		
		//read succesful
		if(has_read)
		{
			//for(i=0; i<input_buffer_count; i++)
			//{
			i=0;
			loop:
			
				current_char = input_buffer[i];
				
				//reading from input
				if(current_state == 2)
				{
					
				}
				
				//reading left of declarations
				else if(current_state == 0)
				{
					if(current_char == ':')
					{
						current_move->n[left_decl_counter] = 0;
						current_state = 1;
					}
					else if(current_char == '\n')
						current_state = 2;
					else
					{
						//reading first char of new move
						if(!left_decl_counter)
						{
							create_move(&current_move);
						}
						
						//write the char into the move name
						current_move->n[left_decl_counter] = current_char;
						left_decl_counter++;
					}
				}
				
				//reading right of declarations
				else
				{
					if(current_char == '\n')
					{
						current_state = 0;
						left_decl_counter = 0;
						iterator->m = current_move;
						iterator = root;
					}
					else
					{
						iterator = find_or_create_next(iterator, current_char);
					}
				}
				
				i++;
				if(i < bytes_to_read)
					goto loop;
			//}
			//write to file
			fwrite(input_buffer, bytes_to_read, 1, fout);
		}
	}
	
	//display_tree(root, 0);
	destroy_tree(root);

	return 0;
}
