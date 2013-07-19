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

//displays all searches in given list
void display_searches(list l, unsigned char next_verse);

//initializes a tree
void create_tree(tree *t);

//initializes a list
void create_list(list *l, unsigned char c);

//initializes a list for search
void create_search(list *l);

//initializes a move
void create_move(move *m);

//destroys a list
void destroy_list(list l);

//destroys a move
void destroy_move(move m);

//destroys a tree
void destroy_tree(tree t);

//finds the given char in the next level of the tree. returns NULL if not found
tree find_next(tree t, unsigned char c);

//finds or create the given char in the next level of the tree
tree find_or_create_next(tree t, unsigned char c);

// ------------------------- FUNCTIONS --------------------------
//displays a tree
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
		if(t->c)
			printf("%c", t->c);
		else
			printf("root");
		
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

//displays all searches in given list
void display_searches(list l, unsigned char next_verse)
{
	if(!l)
		printf("X\n");
	else
	{
		printf("[");
		if(l->t)
		{
			if(l->t->c)
				printf("%c", l->t->c);
			else
				printf("root");
		}
		printf(next_verse ? "]->" : "]<-");
		display_searches(next_verse ? l->n : l->p, next_verse);
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

//initializes a list for search
void create_search_list(list *l)
{
	*l = (list)malloc(sizeof(struct _list_node));
	(*l)->n = NULL;
	(*l)->p = NULL;
	(*l)->t = NULL;
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

//finds the given char in the next level of the tree. returns NULL if not found
tree find_next(tree t, unsigned char c)
{
	//if leaf, return NULL
	if(!(t->l))
		return NULL;
	
	//if node, search into all children (only one depth level)
	list l = t->l;		//iterator
	unsigned char peek;	//current reading char
	
	//loop. find!
	while(1)
	{
		peek = l->t->c;
		
		//looked char exceeded. return null
		if(peek > c)
		{
			return NULL;
		}
		
		
		//looked char reached. return found node
		else if(peek == c)
		{
			return l->t;
		}
		
		//looked char not reached, try next
		else
		{
			//if next doesn't exists, char not found. return null
			if(!(l->n))
			{
				return NULL;
			}
			
			//if next exists, iterate over next
			else
			{
				l = l->n;
			}
		}
	}
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
	int has_read;				//indicates if the loop has read something
	int input_bytes_left;		//the bytes left to reach the end of file
	int bytes_to_read;			//the bytes to read for each iteration
	int current_state = 0; 		//the FSM state
	unsigned char current_char; //the current char read
	int i;						//counter
	int j;  					//counter
	int max_move_length = 0;	//the max length of all moves
	int cur_move_length = 0;	//the length of the current move

	//open input file
	FILE *fin;
	fin = fopen(".\\large\\input.txt", "rb");
	
	//open output file
	FILE *fout;
	fout = fopen(".\\output.txt", "wb");
	
	//files not found
	if(!fin)
	{
		printf("input.txt file not found");
		return 0;
	}
	if(!fout)
	{
		printf("output.txt file not created");
		return 0;
	}
	
	fseek(fin, 0, SEEK_END); // seek to end of file
	input_bytes_left = ftell(fin);
	fseek(fin, 0, SEEK_SET);
	
	int bufsize = MIN(CHUNK_SIZE, input_bytes_left);
	
	unsigned char input_buffer[MIN(CHUNK_SIZE, input_bytes_left)]; //input buffer
	unsigned char output_buffer[MIN(CHUNK_SIZE, input_bytes_left)]; //output buffer
	move current_move;
	
	tree root; 					//the root of the parse tree
	tree available_searches;	//the available search lists
	tree current_searches;		//the current used search lists
	list current_search;		//iterator for search lists
	list tmp_search;			//a temporary ptr
	create_tree(&root);
	create_tree(&available_searches);
	create_tree(&current_searches);
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
					//no active current searches. Set the first
					if(!current_searches->l)
					{
						current_searches->l = available_searches->l;
						available_searches->l = available_searches->l->n;
						if(available_searches->l)
							available_searches->l->p = NULL;
						current_searches->l->n = NULL;
					}
					
					//already active current searches. request for another.
					else
					{	
						current_searches->l->p = available_searches->l;
						available_searches->l = available_searches->l->n;
						if(available_searches->l)
							available_searches->l->p = NULL;
						current_searches->l->p->n = current_searches->l;
						current_searches->l = current_searches->l->p;
					}
					
					//initializes new search
					current_search = current_searches->l;
					current_search->t = root;
					
					//performs search operations
					while(current_search)
					{
						//try to match into next level
						current_search->t = find_next(current_search->t, current_char);
						
						//matched
						if(current_search->t)
						{
							//move matched. must reset all searches
							if(current_search->t->m)
							{
								//TODO: save current move
								
								//go until the end of the searches
								while(current_search->n)
								{
									current_search->t = root;
									current_search = current_search->n;
								}
								
								//append current_search to available_searche
								current_search->n = available_searches->l;
								if(available_searches->l)
									available_searches->l->p = current_search;
								available_searches->l = current_searches->l;
								current_searches->l = NULL;
								current_search = NULL;
							}
							
							//node reached. must go on with next search
							else
							{
								current_search = current_search->n;
							}
						}
							
						//not matched. delete current_search and go on with next search
						else
						{
							tmp_search = current_search->n;
							
							if(current_search->n)
								current_search->n->p = current_search->p;
							if(current_search->p)
								current_search->p->n = current_search->n;
							
							//current_search was the first in the list. Must update the list with the new first.
							else
								current_searches->l = current_search->n;
							
							current_search->p = NULL;
							current_search->n = available_searches->l;
							if(current_search->n)
								current_search->n->p = current_search;
							available_searches->l = current_search;
							current_search->t = root;
							current_search = tmp_search;
						}
					}
				}
				
				//reading left of declarations
				else if(current_state == 0)
				{
					if(current_char == ':')
					{
						current_move->n[left_decl_counter] = 0;
						current_state = 1;
						cur_move_length = 0;
					}
					else if(current_char == '\n')
					{
						current_state = 2;
												
						//preparing for read the user input. initialize search resources
						list new_search;
						for (j = 0; j <= max_move_length; j++)
						{
							create_search_list(&new_search);
							new_search->t = root;
							if(j!=0)
							{
								new_search->n = available_searches->l;
								available_searches->l->p = new_search;
							}
							available_searches->l = new_search;
						}
					}
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
						if(cur_move_length > max_move_length)
							max_move_length = cur_move_length;
					}
					else
					{
						iterator = find_or_create_next(iterator, current_char);
						cur_move_length++;
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
	//destroy_list(available_searches);
	//destroy_list(current_searches);

	return 0;
}
