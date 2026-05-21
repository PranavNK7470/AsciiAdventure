/* started to learn memory management using c and low level Threads .*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>

#define ROWS 5
#define COLS 5

typedef struct MC {
    int i;
    int j;
}mc;

char readDirection() {
    int direction;
    scanf("%c",&direction);
    if(direction == 'w' || direction == 's' || direction == 'a' || direction == 'd') return direction;
}

typedef struct Cell {
    int left;
    int right;
    int bottom;
    int top;
    int* walls;
} cell;
cell** cells;

void printMaze(cell** cells) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(cells[i][j].top && i == 0) printf(" __");
            else if(i == 0) printf("   ");
        }
        printf("\n");
        for(int j = 0; j < COLS; j++) {

            if(cells[i][j].left &&  j==0) printf("|");
            else if(j == 0) printf(" ");
            if(cells[i][j].bottom) printf("__");
            else printf("  ");
            if(cells[i][j].right) printf("|");
            else printf(" ");
        }
    }
}

int visited[ROWS][COLS];
int findNeighbour(int r, int c) {
    int neighbours[4];
    neighbours[0] = (r > 0) && !visited[r-1][c];
    neighbours[1] = (c > 0) && !visited[r][c-1];
    neighbours[2] = (c <  COLS -1) && !visited[r][c+1];
    neighbours[3] = (r < ROWS-1) &&!visited[r+1][c];

    int random = rand() % 4;
    for(int i = 0; i < 4; i++) {
        if(neighbours[random]){
            return random;
        } 
            
        else {
            random = (random + 1) % 4;
        }
    }
    return -1;
}

void createMaze(cell** cells, int i, int j) {
    int r ;
    visited[i][j] = 1;
    while((r = findNeighbour(i,j)) != -1) {
        switch(r) {
            case 0:
                if(i != 0){
                    cells[i-1][j].bottom = 0;
                    createMaze(cells,i-1,j);
                }
            break;
            case 1:
                if(j != 0){
                    cells[i][j-1].right = 0;
                    createMaze(cells,i,j-1);
                }
            break;
            case 2:
                if(j < COLS - 1) {
                    cells[i][j].right = 0;
                    createMaze(cells,i,j+1);
                }
            break;
            case 3:
                if(i < ROWS - 1){
                    cells[i][j].bottom = 0;
                    createMaze(cells,i+1,j);
                }
            break;
        }
    }
}

int main() {
    srand(time(0));
    cells = malloc(ROWS * sizeof(cell*) );
    for(int i=0; i<ROWS; i++) {
        cells[i] = malloc(COLS * sizeof(cell));
    }

    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            cells[i][j].top = 1;
            cells[i][j].bottom = 1;
            cells[i][j].left = 1;
            cells[i][j].right = 1;
        }
    }

    createMaze(cells,ROWS-1,COLS/2);
    printMaze(cells);

    return 0;
}