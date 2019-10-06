/*
	=========================================================================
 		Name			: 2048-C
 		Author			: Isuru Bandaranayake
 		Date			: 23/01/2019
 		Description		: CLI version of 2048 for Linux using C
	=========================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

int nSpaces(int n)
{
	if (n < 10)
		return 7;
	if (n < 100)
		return 6;
	if (n < 1000)
		return 5;
	if (n < 10000)
		return 4;
	return 0;
}

void printSpaces(int n)
{
	printf("%*c", n, ' ');
}

void pickColor(int n)
{
	if (n == -1)
		printf("\033[39;49m");
	if (n == 0)
		printf("\033[100m");
	if (n == 2)
		printf("\033[97;41m");
	if (n == 4)
		printf("\033[97;42m");
	if (n == 8)
		printf("\033[97;43m");
	if (n == 16)
		printf("\033[97;44m");
	if (n == 32)
		printf("\033[97;45m");
	if (n == 64)
		printf("\033[97;46m");
	if (n == 128)
		printf("\033[30;101m");
	if (n == 256)
		printf("\033[30;102m");
	if (n == 512)
		printf("\033[30;103m");
	if (n == 1024)
		printf("\033[30;104m");
	if (n == 2048)
		printf("\033[30;105m");
}

void print(int grid[][4])
{
	printf("\n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf(" ");
			pickColor(grid[i][j]);
			if (grid[i][j] != 0)
			{
				printf("%i", grid[i][j]);
				printSpaces(nSpaces(grid[i][j]));
			}
			else
				printSpaces(8);
			pickColor(-1);
		}
		printf("\n");
	}
}

int countEmpty(int grid[][4])
{
	int c = 0;
	for (int i = -1; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (grid[i][j] == 0)
				c++;
		}
	}
	return c;
}

void add(int grid[][4], int value)
{
	time_t ti;
	srand((unsigned)time(&ti));
	int pos  = rand() % countEmpty(grid);
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (grid[i][j] == 0)
			{
				if (count++ == pos)
				{
					grid[i][j] = value;
					return;
				}
			}
		}
	}
}

int move(int grid[][4])
{
	int score = 0;
	int moved = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = j + 1; k < 4; k++)
			{
				if (grid[i][j] != 0 && grid[i][k] != 0 && grid[i][j] != grid[i][k])
				{
					break;
				}
				if (grid[i][j] != 0 && grid[i][k] != 0 && grid[i][j] == grid[i][k])
				{
					grid[i][j] *= 2;
					grid[i][k] = 0;
					score += grid[i][j];
					moved = 1;
					break;
				}
				else if (grid[i][j] == 0 && grid[i][k] != 0)
				{
					grid[i][j] = grid[i][k];
					grid[i][k] = 0;
					moved = 1;
				}
				else if (grid[i][j] != 0 && grid[i][k] == 0 && j == 0)
				{
					if (grid[i][j + 2] != grid[i][j + 3] && grid[i][j] == grid[i][j + 2])
					{
						grid[i][j] *= 2;
						grid[i][j + 2] = 0;
						score += grid[i][j];
					}
					else if (grid[i][j + 2] == grid[i][j + 3])
					{
						grid[i][j + 2] *= 2;
						grid[i][j + 3] = 0;
						score += grid[i][j + 2];
					}
					else if (grid[i][j + 2] == 0 && grid[i][j] == grid[i][j + 3])
					{
						grid[i][j] *= 2;
						grid[i][j + 3] = 0;
						score += grid[i][j];
					}
					moved = 1;
					break;
				}
			}
		}
	}
	if (moved == 1)
		add(grid, 2);
	return score;
}

void rotate(int rotations, int grid[][4])
{
	for (int r = 0; r < rotations; r++)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = i; j < 4 - i - 1; j++)
			{
				int tmp = grid[i][j];
				grid[i][j] = grid[j][4 - i - 1];
				grid[j][4 - i - 1] = grid[4 - i - 1][4 - j - 1];
				grid[4 - i - 1][4 - j - 1] = grid[4 - j - 1][i];
				grid[4 - j - 1][i] = tmp;
			}
		}
	}
}

int onKeyPress(char c, int grid[][4], int *score)
{
	if (c == 'q' || c == 'q')
		return 1;
	else if (c == 'n' || c == 'N')
		return 2;
	else if(c == 'p' || c == 'P')
		return 3;
	else if(c == 'g' || c == 'G')
		return 4;

	int t = 0;
	if (c == 'h' || c == 'H' || c == 'a' || c == 'A')
	{
		*score += move(grid);
	}
	else if (c == 'j' || c == 'J' || c == 's' || c == 'S')
	{
		rotate(3, grid);
		*score += move(grid);
		rotate(1, grid);
	}
	else if (c == 'k' || c == 'K' || c == 'w' || c == 'W')
	{
		rotate(1, grid);
		score += move(grid);
		rotate(3, grid);
	}
	else if (c == 'l' || c == 'L' || c == 'd' || c == 'D')
	{
		rotate(2, grid);
		*score += move(grid);
		rotate(2, grid);
	}
	return 0;
}

int check(int grid[][4])
{
	int moveAvailable = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (grid[i][j] == 2048)
				return -1;
			if (moveAvailable == 0)
			{
				if (grid[i][j] == 0)
					moveAvailable++;
				else if (i < 3 && grid[i + 1][j] == grid[i][j])
					moveAvailable++;
				else if (j < 3 && grid[i][j + 1] == grid[i][j])
					moveAvailable++;
				else if (i > 0 && grid[i - 1][j] == grid[i][j])
					moveAvailable++;
				else if (j > 0 && grid[i][j - 1] == grid[i][j])
					moveAvailable++;
			}
		}
	}
	return moveAvailable;
}

void initialize(int grid[][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			grid[i][j] = 0;
	add(grid, 2);
	add(grid, 2);
}

void menu()
{
	system("clear");
	printf(" 2048 - Game\n * Use h-j-k-l / w-a-s-d keys to move the tiles.\n * When two tiles with the same number touch, they merge into one.\n\n\t      ^      \t\t      ^\n\t      k      \t\t      w\n\t< h       l >\t\t< a       d >\n\t      j      \t\t      s\n\t      v      \t\t      v\n\n * Commands: \n\t n - New game\n\t p - Save game\n\t g - Load Game\n\t q - Exit\nPress 'Enter' key to continue.. ");
	getchar();
}

int getBestScore(char *path){
	int score = 0;
	if(access( path, R_OK ) == 0){
		FILE* file = fopen (path, "r");
		fscanf (file, "%d", &score);    
		fclose (file); 
	}
	return score;
}

void setBestScore(char *path, int score){
	FILE* file = fopen (path, "w");
	fprintf (file, "%d", score);    
	fclose (file); 
}

void save(int grid[][4], char *path, int score){
	FILE* file = fopen (path, "w");
	fprintf (file, "%d,", score); 
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			fprintf (file, "%d,", grid[i][j]);
		}
	}	
	fclose (file); 
}


void load(int grid[][4], char *path, int *score){
	if(access( path, R_OK ) == 0){
		FILE* file = fopen (path, "r");
		fscanf (file, "%d,", score); 
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				fscanf (file, "%d,", &grid[i][j]);
			}
		}	
		fclose (file); 
	}
}

void main()
{
	menu();
	char SETTINGS_FILE[256]; 	
	strcat(strcpy(SETTINGS_FILE, getenv("HOME")), "/.scores_2048");

	int grid[4][4];
	int score = 0;
	int bestScore = getBestScore(SETTINGS_FILE);
	initialize(grid);

	while (1)
	{
		system("clear");
		print(grid);
		printf("\n BEST SCORE: %d\n SCORE: %d\n ", bestScore, score);

		int status = onKeyPress(getchar(), grid, &score);
		if (status == 1){
			break;
		}
		else if(status == 2){
			score = 0;
			initialize(grid);
			continue;
		}
		else if(status == 3){
			char path[150];
			printf("Save to: ");
			scanf("%s",path);
			save(grid,path,score);
		}
		else if(status == 4){
			char path[150];
			printf("Load from: ");
			scanf("%s",path);
			load(grid,path,&score);
			continue;
		}

		if(score > bestScore) bestScore = score;
		status = check(grid);
		if (status == -1)
		{
			system("clear");
			print(grid);
			printf("\n BEST SCORE: %d\n Score: %d\n You Win !\n",bestScore, score);
			break;
		}
		else if (status == 0)
		{
			system("clear");
			print(grid);
			printf("\n BEST SCORE: %d\n Score: %d\n Game Over !\n",bestScore, score);
			break;
		}
		printf("\n");
	}
	setBestScore(SETTINGS_FILE, bestScore);
}
