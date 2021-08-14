#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
int items=0;
int score=0;  
int power=0; 
int powerCounter=0; 
char ico[5] = "0123";

void printBoard(int length, int width, int** board)
{
	for(int k = 0; k < length; k++)
	{
		for (int l = 0; l < width; l++)
		{
			if (k==0||k==length-1||l==0||l==width-1) printf("*"); 
			else if (!board[k][l]) printf(" ");
			else printf("%c", board[k][l]);
		} printf("\n");
	}
}

void placeBuilding(int x, int y, int** board)
{
	/*a building is always a 4-by-6 block of dashes
	except for a “door” built with ampersands in the middle of it. 
	In the case of a building, the provided coordinates locate its top-left. 
	------
	------
	--&&--
	--&&--
	*/

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (i >= 2 && (j == 2 || j==3)) board[x+i][y+j] = '&';
			else board[x+i][y+j] = '-';
		}	
	}
}

void destroy(int row, int** arr)
{
	for (int i = 0; i < row; i++)
		free(arr[i]);
	free(arr);
}

char* removeSpace (char* source, char* dest)
{
	//reduce all the white space and transfer 
	char* tok = NULL; 
	int index = 0; 
	tok = strtok(source, " ");

	while (tok)
	{
		dest[index] = *tok;
		tok = strtok(NULL," ");
		index++; 
	}

	return dest; 
}

void determineIcons(char* source)
{
	char* tok = NULL; 
	tok = strtok(source, " ");
	ico[0]=*tok;
	tok = strtok(NULL, " "); 
	ico[1]=*tok; 
	tok = strtok(NULL, " ");
	ico[2] = atoi(tok); 
	tok = strtok(NULL, " "); 
	ico[3] = atoi(tok); 

}

void parseItem (char* line, int** board)
{
	char* c = NULL; 
	int x,y = 0; 

	c = strtok(line, " "); //item
	x = atoi(strtok(NULL, " ")); //position
	y = atoi(strtok(NULL, " "));
	
	switch(*c)
	{
		case 'B':
			placeBuilding(x+1, y+1, board);
			break; 
		case 'E':
			board[x+1][y+1] = 'X'; 
			break; 
		case 'I':
			board[x+1][y+1] = '$'; 
			items++; 
			break; 
		case 'P':
			board[x+1][y+1] = '*'; 
			break; 
		default:
			board[x][y] = 0; 
	}

}
int checkPos(int oldX, int oldY, int newX, int newY, int** board)
{
	char c = board[newX][newY]; 
	if (c == '$')
	{
		score++;
		items--; 
		if (items == 0) return 2; 
	}  
	if (c == '*') 
	{
		board[oldX][oldY] = ico[1];
		power = 1;
		powerCounter = 6;  
	}
	if (c == 'X') 
	{
		if (power) score++; 
		else
		{
			board[oldX][oldY] = '@'; 
			return 1;
		}	 
	}
	return 0; 
}
int updatePosition (int posX, int posY, int length, int width, int** board, char* position)
{
	if (power && !powerCounter) 
	{
		board[posX][posY] = ico[0];
		power=0; 
	}
	if (powerCounter) powerCounter--;

	int end = 0; 
	//TODO: check if new pos is valid IN A NEW HELPER FUNCTION
	if (strcmp(position, "right")==0)
	{
		if (posY+1 < width-1 && board[posX][posY+1] != '-')
		{ 
			end = checkPos(posX, posY, posX, posY+1, board);
			board[posX][posY+1] = board[posX][posY]; 
			board[posX][posY] = 0; 
			if (end == 1) return -1; //killed by enemy 
			if (end == 2) return -2; //won game by collecting items
			return posY+1;
		} return posY;
	}
	if (strcmp(position, "left")==0)
	{
		if (posY-1 > 0 && board[posX][posY-1] != '-')
		{
			end = checkPos(posX, posY, posX, posY-1, board);
			board[posX][posY-1] = board[posX][posY]; 
			board[posX][posY] = 0; 
			if (end == 1) return -1; //killed by enemy 
			if (end == 2) return -2; //won game by collecting items
			return posY-1; 
		} return posY; 
	}
	if (strcmp(position, "up")==0)
	{
		if (posX-1 > 0 && board[posX-1][posY] != '-' && board[posX-1][posY] != '&')
		{
			end = checkPos(posX, posY, posX-1, posY, board);
			board[posX-1][posY] = board[posX][posY];
			board[posX][posY] = 0; 
			if (end == 1) return -1; //killed by enemy 
			if (end == 2) return -2; //won game by collecting items
			return posX-1;
		} return posX; 
	}
	if (strcmp(position, "down")==0)
	{
		if (posX+1 < length-1 && board[posX+1][posY] != '-')
		{
			end = checkPos(posX, posY, posX+1, posY, board);
			board[posX+1][posY] = board[posX][posY];
			board[posX][posY] = 0; 
			if (end == 1) return -1; //killed by enemy 
			if (end == 2) return -2; //won game by collecting items
			return posX+1; 
		} return posX; 
	}
	return 0; 
}

int playGame(const char* filename)
{ 
	char lines[100][100]; 
	FILE* fp = fopen(filename,"r"); //open a file up to read 
	if (!fp) return 0; 

	int i = 0; //number of lines in the file 
	while(fgets(lines[i],100,fp))
	{
		//printf("lines[%d] = %s\n", i,lines[i]);
		i++; 
	} 
	fclose(fp);
	//parse length and width  
	int length = atoi(strtok(lines[0]," ")); 
	int width = atoi(strtok(NULL, " "));

	//parse lines[1] for keys and remove all spaces 
	char keys[6] = "adwsq"; //default keys 
	if (strcmp(lines[1],"\n")!=0) 
	{
		//printf("before keys: %s, lines: %s\n", keys, lines[1]);
		removeSpace(lines[1], keys); 
	}
	//printf("keys %s\n", keys);

	//create a board calloc of size length x width
	int** board = (int**) (calloc(length+2,sizeof(int*)));
	for (int j = 0; j < length+2; j++)
		board[j]= (int*) (calloc(width+2, sizeof(int))); 

	for (int m = 0; m < length+2; m++)
		for (int n = 0; n < width+2; n++)
			board[m][n] = ' ';

	//parse lines[2] for icons + positions and remove all spaces 
	//enter position of default icon inside calloc
	
	determineIcons(lines[2]);
	int x = ico[2]+1; 
	int y = ico[3]+1; 
	board[x][y]= ico[0]; //start position
	//printf("x %d y %d\n", x,y);
	//printf("icons: %c %c %d %d \n", ico[0],ico[1],ico[2],ico[3]);

	//TODO: while loop through the rest and set values into board 
	int index = 3; 
	while (index < i)
	{
		//printf("lines[%d]:%s\n", index, lines[index]);
		parseItem(lines[index], board); //finds the first character of item and places it on board with position
		index++; 
	}

	//TODO: while loop ask for input and change positions + update/print board
	char input = 0; 
	do
	{
		printf("Score: %d\nItems remaining: %d\n", score, items);
		printBoard(length+2, width+2,board);
		if(x == -1 || y == -1) 
		{
			printf("You have died.\n");
			break; 
		}
		printf("Enter input: ");
		scanf(" %c", &input); 

		if (input == keys[4])
			printf("You have quit.\n"); 
		else if (input == keys[0])
		{
			y=updatePosition(x, y, length+2, width+2, board, "left"); 
		}
		else if (input == keys[1])
		{
			y=updatePosition(x,y,length+2,width+2,board,"right");
		}
		else if (input == keys[2])
		{
			x=updatePosition(x,y,length+2,width+2,board,"up");
		}
		else if (input == keys[3])
		{
			x=updatePosition(x,y,length+2,width+2,board,"down");
		}

		if(x == -2 || y == -2) 
		{
			printf("Congratulations! You have won.\n");
			break; 
		}


	}while(input!=keys[4]);
	
	printf("Final score: %d\n", score);
	//free all callocs at the end 
	destroy(length+2, board);
	return i; 

}
//return 0 if filename is null
//return 1 once game ends 
int startGame(const char* filename)
{ 
	if (!filename) return 0; 
	
	int retval = 0;
	retval = playGame(filename);
	//printf("lines in file: %d\n",retval);
	if (!retval) return 0; 
	return 1; 
}
/*
int main(int argc, char* argv[])
{
	if (argc == 1) 
	{
		startGame("game1.txt");
		return 0; 
	}
	switch(*argv[1])
	{
		case '2':
			startGame("game2.txt"); 
			break; 
		case '3':
			startGame("game3.txt");
			break; 
		case '4':
			startGame("game4.txt");
			break; 
		case '5':
			startGame("game5.txt");
			break; 
		default: 
			startGame("game1.txt"); 
	} return 0; 

}*/
