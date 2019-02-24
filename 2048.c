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
	for (int i = 0; i < 4; i++)
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
	int t;
	time_t ti;
	while (1)
	{
		srand((unsigned)time(&ti));
		int e = countEmpty(grid);
		t = rand() % e;
		int c = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (grid[i][j] == 0)
				{
					if (c++ == t)
					{
						grid[i][j] = value;
						return;
					}
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
					break;
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

void rotate(int n, int grid[][4])
{
	for (int r = 0; r < n; r++)
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

int onKeyPress(char c, int grid[][4])
{
	if (c == 'q' || c == 'q')
		return -1;
	else if (c == 'r' || c == 'R')
		return -2;

	int t = 0;
	if (c == 'h' || c == 'H' || c == 'a' || c == 'A')
	{
		t = move(grid);
	}
	else if (c == 'j' || c == 'J' || c == 's' || c == 'S')
	{
		rotate(3, grid);
		t = move(grid);
		rotate(1, grid);
	}
	else if (c == 'k' || c == 'K' || c == 'w' || c == 'W')
	{
		rotate(1, grid);
		t = move(grid);
		rotate(3, grid);
	}
	else if (c == 'l' || c == 'L' || c == 'd' || c == 'D')
	{
		rotate(2, grid);
		t = move(grid);
		rotate(2, grid);
	}
	return t;
}

int check(int grid[][4])
{
	int moveAvailable = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (grid[i][j] == 2048)
				return 9;
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

int initialize(int grid[][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			grid[i][j] = 0;
	}
	add(grid, 2);
	add(grid, 2);
	return 0;
}

void menu()
{
	system("clear");
	printf(" 2048 - Game\n * Use h-j-k-l / w-a-s-d keys to move the tiles.\n * When two tiles with the same number touch, they merge into one.\n\n");
	printf("\t      ^      \t\t      ^\n\t      k      \t\t      w\n\t< h       l >\t\t< a       d >\n\t      j      \t\t      s\n\t      v      \t\t      v\n\n");
	printf(" * Commands: \n\t r - Restart\n\t q - Exit\n");
	printf("Press 'Enter' key to continue.. ");
	char c;
	scanf("%c", &c);
}

int getBestScore(char* path){
	int i = 0;
	if(access( path, R_OK ) == 0){
		FILE* file = fopen (path, "r");
		fscanf (file, "%d", &i);    
		fclose (file); 
	}
	return i;
}

void setBestScore(char* path, int i){
	FILE* file = fopen (path, "w");
	fprintf (file, "%d", i);    
	fclose (file); 
}

void main()
{
	menu();
	int grid[4][4];
	int score = initialize(grid);
	int bestScore = getBestScore("data.txt");
	while (1)
	{
		system("clear");
		print(grid);

		printf("\n BEST SCORE: %d\n SCORE: %d\n ", bestScore, score);
		int t = onKeyPress(getchar(), grid);
		if (t == -1)
			break;
		else if(t==-2){
			score = initialize(grid);
			continue;
		}
		else
			score += t;

		if(score>bestScore) bestScore=score;
		t = check(grid);
		if (t == 9)
		{
			system("clear");
			print(grid);
			printf("\n BEST SCORE: %d\n Score: %d\n You Win !\n",bestScore, score);
			break;
		}
		else if (t == 0)
		{
			system("clear");
			print(grid);
			printf("\n BEST SCORE: %d\n Score: %d\n Game Over !\n",bestScore, score);
			break;
		}
		printf("\n");
	}
	setBestScore("data.txt",bestScore);
}
