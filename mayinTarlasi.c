#include <stdio.h>
//#include <stdlib.h>
//#include <iostream>

#define SIZE_OF_TABLE 8
#define NUMBER_OF_MINES 7
#define WIN 1
#define LOSE -1
#define GO 0
#define CONTINUE_GAME 'y'

typedef enum {false, true} bool;

//Global variables
int subTable[SIZE_OF_TABLE+2][SIZE_OF_TABLE+2];
char table[SIZE_OF_TABLE+2][SIZE_OF_TABLE+2];
int nb_mines;

//Functions
void createSubTable();
void initializeTable();
void printSubTable();
void printTable();
void placeMines();
int verifyWin();	//win or not?
void explore(int x, int y);
int openPoint(int x, int y);
void startGame();

int main(int argc, char** argv) {
	
	char again = CONTINUE_GAME, action;
	int x, y;
	int flagCount;
	int state ;
	
	startGame();

	state = GO;
	flagCount = 0;
	

	do
	{

		printTable();
		printf("\nEnter: ");
		scanf("%c,%d,%d", &action, &x, &y);
		printf("\n");
		
		if ('c' == action)
		{
			state = openPoint(x,y);
		}
		 
		else if ('d' == action)
		{			
			if(NUMBER_OF_MINES > flagCount)
			{
				flagCount++;
				table[x][y] = 'D';
			}
			
			state = verifyWin();
		}
		
		else if ('e' == action)
		{
			if(0 < flagCount)
			{
				flagCount--;
				table[x][y] = '#';
			}
		}

		if(LOSE == state)
		{
			printf("\n	You lost the game!\n\n");
			printSubTable();
			fflush(stdin);
			printf("\nDo you want to play again? (y or n): ");
			scanf("%c", &again);
		}
		
		else if(WIN == state)
		{
			printf("\n	You won the game!\n\n");
			printSubTable();
			fflush(stdin);
			printf("\nDo you want to play again? (y or n): ");
			scanf("%c", &again);
		}	
		
		if (WIN == state || LOSE == state)
		{
			if(CONTINUE_GAME == again)
			{
				state = GO;
				flagCount = 0;
				startGame();
			}
		}
		
	} while(CONTINUE_GAME == again);
	
	printf("\n\nSee you next time!");
	return 0;
}

void startGame()
{
	printf("\nMayin tarlasi oyunu basliyooor!\n\n");

	createSubTable();
	initializeTable();
	
	printf("c: Bir nokta acar\nd: Verilen noktaya bayrak koyar\ne: varsa verilen noktadan bayragi kaldirir.\n\n");
	printf("Lutfen yanda verilen formatta girdi veriniz: c,x,y veya d,x,y veya e,x,y\n");
	printf("(x ve y bir noktayi temsil etmektedir.)\n\n");
}

void createSubTable()
{
	int i, j, nbMine;
	
	for (i = 0; i < SIZE_OF_TABLE+2; i++)
	{
		for (j = 0; j < SIZE_OF_TABLE+2; j++)
		{
			subTable[i][j] = 0;
		}
	}
	
	//set first and last row and column to 1
	for (j = 0; j < SIZE_OF_TABLE+2; j++)
	{
		subTable[0][j] = 1;
		subTable[SIZE_OF_TABLE+1][j] = 1;
		subTable[j][0] = 1;
		subTable[j][SIZE_OF_TABLE+1] = 1;
	}

	placeMines();
}

void initializeTable()
{
	int i, j;
	
	for (i = 0; i < SIZE_OF_TABLE+2; i++)
	{
		for (j = 0; j < SIZE_OF_TABLE+2; j++)
		{
			table[i][j] = '#';
		}
	}
	
	//set first and last row and column to 1
	for (j = 0; j < SIZE_OF_TABLE+2; j++)
	{
		table[0][j] = '*';
		table[SIZE_OF_TABLE+1][j] = '*';
		table[j][0] = '*';
		table[j][SIZE_OF_TABLE+1] = '*';
	}
}

void printSubTable()
{
	int i, j;
	for (i = 0; i < SIZE_OF_TABLE; i++)
	{
		for (j = 0; j < SIZE_OF_TABLE; j++)
		{
			printf("%2d ", subTable[i+1][j+1]);
		}
		printf("\n");
	}	
}

void printTable()
{
	int i, j;
	
	for (i = 0; i < SIZE_OF_TABLE; i++)
	{
		printf("%3d", i+1);
	}
	printf("\n");
	
	for (i = 0; i < SIZE_OF_TABLE; i++)
	{
		printf("---");
	}
	printf("\n");
	
	//print table
	for (i = 0; i < SIZE_OF_TABLE; i++)
	{
		for (j = 0; j < SIZE_OF_TABLE; j++)
		{
			printf("%3c", table[i+1][j+1]);
		}
		printf("%2c %d ", '|', i+1);
		printf("\n");
	}
	
	//print end of table
	for (i = 0; i < SIZE_OF_TABLE; i++)
	{
		printf("---");
	}
	
}

void placeMines()
{
	int i, j, x, y;
	int k, p;
	nb_mines = NUMBER_OF_MINES;
	
	srand(time(NULL));
	
	for (i = 0; i < nb_mines; i++)
	{
		x = 1 + rand() % SIZE_OF_TABLE;
		y = 1 + rand() % SIZE_OF_TABLE;
		
		if (-1 == subTable[x][y])
		{
			i--;
		}
		subTable[x][y] = -1;
		for (k = -1; k < 2; k++)
		{
			for (p = -1; p < 2; p++)
			{
				//kendisi bombaysa
				if (-1 == subTable[x][y])	
				{
					//çevresinde bomba olmayan noktalari 1 artir
					if (-1 != subTable[x+k][y+p])	
					{
						subTable[x+k][y+p] += 1;
					}
				}	
			}
		}
	}
}

int openPoint(int x, int y)
{
	int val = GO;
	
	if ((1 <=x && SIZE_OF_TABLE >= x) && (1 <=y && SIZE_OF_TABLE >= y))
	{
		if(-1 == subTable[x][y])
		{
			val = LOSE;
		}
		
		else if(0 < subTable[x][y])
		{
			table[x][y] = subTable[x][y] + '0';
			verifyWin();
		}
		
		else
		{
			explore(x, y);
		}
		
	}
	
	return val;
}

int verifyWin()
{
	int i, j;
	int trueFlag = 0;
	int status;
	
	for (i = 1; i<=SIZE_OF_TABLE; i++)
	{
		for (j = 1; j<=SIZE_OF_TABLE; j++)
		{
			if ('D' == table[i][j])
			{
				if (-1 == subTable[i][j])
				{
					trueFlag++;
				}
			}
		}
	}
	
	if (NUMBER_OF_MINES == trueFlag)
	{
		status = WIN;
	}
	else
	{
		status = GO;
	}
	
	return status;
}

void explore(int x, int y)
{
	int i, j;
	
	table[x][y] = subTable[x][y] + '0';
	
	for (i =-1; i<2; i++)
	{
		for (j =-1; j<2; j++)
		{
			if((0 < subTable[x+i][y+j]) && ('#' == (table[x+i][y+j])))
			{
				table[x+i][y+j] = subTable[x+i][y+j] + '0';
			}
			
			else if((0 == subTable[x+i][y+j]) && ('#' == (table[x+i][y+j])))
			{
				 explore(x+i, y+j);
			}
				
		}
	}
}
