#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nSpaces(int n){
	if(n<10) return 7;
	if(n<100) return 6;
	if(n<1000) return 5;
	if(n<10000) return 4;
	return 0;
}

void printSpaces(int n){
	printf("%*c", n, ' ');
}

void pickColor(int n){
	if(n==-1) printf("\033[39;49m");
	if(n==0) printf("\033[100m");
	if(n==2) printf("\033[97;41m");
	if(n==4) printf("\033[97;42m");
	if(n==8) printf("\033[97;43m");
	if(n==16) printf("\033[97;44m");
	if(n==32) printf("\033[97;45m");
	if(n==64) printf("\033[97;46m");
	if(n==128) printf("\033[30;101m");
	if(n==256) printf("\033[30;102m");
	if(n==512) printf("\033[30;103m");
	if(n==1024) printf("\033[30;104m");
	if(n==2048) printf("\033[30;105m");
}

void print(int grid[][4]){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			pickColor(grid[i][j]);
			if(grid[i][j]!=0){
				printf("%i",grid[i][j]);
				printSpaces(nSpaces(grid[i][j]));
			}else
				printSpaces(8);
			pickColor(-1);
			printf(" ");
		}
		printf("\n");
	}
}

int countEmpty(int grid[][4]){
	int c =0;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(grid[i][j]==0)
				c++;
		}
	}
	return c;
}

void add(int grid[][4],int value){
	int t;
	time_t ti;
	while(1){
		srand((unsigned) time(&ti));
		int e =countEmpty(grid);
		t = rand() % e;
		int c=0;
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				if(grid[i][j]==0){
					if(c++==t){
						grid[i][j]=value;
						return;
					}
				}
		}
	}
	}
}

int move(int grid[][4]){
	int moved = 0;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			for(int k=j+1;k<4;k++){
				if(grid[i][j]!=0 && grid[i][k]!=0 && grid[i][j]!=grid[i][k])
					break;
				if(grid[i][j]!=0 && grid[i][k]!=0 && grid[i][j]==grid[i][k]){
					grid[i][j]*=2;
					grid[i][k]=0;
					moved=1;
					break;
				}else if(grid[i][j]==0 && grid[i][k]!=0){
					grid[i][j]=grid[i][k];
					grid[i][k]=0;
					moved=1;
				}else if(grid[i][j]!=0 && grid[i][k]==0 && j==0){
					if(grid[i][j+2]!=grid[i][j+3] && grid[i][j]==grid[i][j+2]){
						grid[i][j]*=2;
						grid[i][j+2]=0;
					}else if(grid[i][j+2]==grid[i][j+3]){
						grid[i][j+2]*=2;
						grid[i][j+3]=0;
					}else if(grid[i][j+2]==0 && grid[i][j]==grid[i][j+3]){
						grid[i][j]*=2;
						grid[i][j+3]=0;
					}
					moved=1;
					break;
				}
			}
		}
	}
	return moved;
}

void rotate(int n,int grid[][4]){
	for(int r = 0;r<n;r++){
		for (int i=0; i<2; i++) {
			for (int j=i; j<4-i-1; j++) {
				int tmp = grid[i][j];
				grid[i][j] = grid[j][4-i-1];
				grid[j][4-i-1] = grid[4-i-1][4-j-1];
				grid[4-i-1][4-j-1] = grid[4-j-1][i];
				grid[4-j-1][i] = tmp;
			}
		}
	}
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
int onKey(char c,int grid[][4]){
	if(c=='x')
		return 9;
	int t=0;
	if(c=='h' || c=='H'){
		t=move(grid);
	}else if(c=='j' || c=='J'){
		rotate(3,grid);
		t=move(grid);
		rotate(1,grid);
	}else if(c=='k' || c=='K'){
		rotate(1,grid);
		t=move(grid);
		rotate(3,grid);
	}else if(c=='l' || c=='L'){
		rotate(2,grid);
		t=move(grid);
		rotate(2,grid);
	}
	if(t==1) add(grid,2);
	return t;
}

int check(int grid[][4]){
	int t[4][4];
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(grid[i][j]==2048)
				return 9;
			t[i][j]=grid[i][j];
		}
	}
	return onKey('a',t)+onKey('s',t)+onKey('d',t)+onKey('w',t);
}

void initialize(int grid[][4]){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++)
			grid[i][j]=0;
	}
	add(grid,2);
	add(grid,2);
}

void main(){
	int grid[4][4];
	initialize(grid);
	while(1){
		system("clear");
		print(grid);
		printf("\nEnter A(Left) | S(Down) | W(Up) | D(Right) | x(Exit): ");

		if(onKey(getchar(),grid)==9)
			break;
		
		int t = check(grid);
		if(t==9){
			system("clear");
			print(grid);
			printf("You Win !\n");
			break;
		}else if(t==0){
			system("clear");
			print(grid);
			printf("Game Over !\n");
			break;
		}	
		printf("\n");
	}
}