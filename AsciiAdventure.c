/* started to learn memory management using c and low level Threads .*/
// add an enemy using threads, implement the haze feature 
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

#define ROWS 10
#define COLS 10

typedef struct PLAYER{
    int i;
    int j;
}player;

typedef struct GOAL {
    int i;
    int j;
}goal;

typedef struct ENEMY {
    int i;
    int j;
}enemy;

int found(int pi, int pj, int gi, int gj) {
    return (gi == pi && gj == pj) ? 1 : 0;
}

int caught(int pi, int pj, int ei, int ej) {
    return (ei == pi && ej == pj) ? 1 : 0;
}

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

void printMaze(cell** cells, int pi, int pj, int gi, int gj, int ei, int ej) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(cells[i][j].top && i == 0) printf(" __");
            else if(i == 0) printf("   ");
        }
        printf("\n");
        for(int j = 0; j < COLS; j++) {

            if(cells[i][j].left &&  j==0) printf("|");
            else if(j == 0) printf(" ");
            if(pi == i && pj == j) printf("@ ");
            else if(gi == i && gj == j) printf("$ ");
            else if(ei == i && ej == j) printf("& ");
            else if(cells[i][j].bottom) printf("__");
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

int pi;
int pj;
int gi;
int gj;

typedef struct Args {
    int ei;
    int ej;
} args;

void* moveEnemy(void* arg) {
    args* a = (args*)arg;
    while(!found(pi,pj,gi,gj)) {
        if(caught(pi,pj,a->ei,a->ej)) {
            system("cls");
            sleep(1);
            printf("You were caught!");
            exit(0);
        }
        sleep(1);
        int d = rand() % 4;
        switch(d){
            case 0:
                if(a->ei > 0 && !cells[a->ei - 1][a->ej].bottom) a->ei -= 1;
                break;
            case 3:
                 if(a->ei < ROWS - 1 && !cells[a->ei][a->ej].bottom) a->ei += 1;
                break;
            case 1:
                if(a->ej > 0 && !cells[a->ei][a->ej - 1].right) a->ej -= 1;
                break;
            case 2:
                if(a->ej < COLS -1 && !cells[a->ei][a->ej].right) a->ej += 1;
                break;
        }
        system("cls");
        printMaze(cells,pi,pj,gi,gj,a->ei,a->ej);
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


    goal* g = malloc(sizeof(goal));
    g->i = 0;
    g->j = 0;

    player* p = malloc(sizeof(player));
    p->i = ROWS-1;
    p->j = COLS/2;
 
    enemy* e = malloc(sizeof(enemy));
    e->i = 0;
    e->j = COLS - 1;

    pi = p->i;
    pj = p->j;
    gi = g->i;
    gj = g->j;
    createMaze(cells,ROWS-1,COLS/2);

    args* a = malloc(sizeof(args));
    a->ei = e->i;
    a->ej = e->i;

    pthread_t new_t; 
    pthread_create(&new_t, NULL, moveEnemy, a);

    while(!found(pi,pj,gi,gj) && !caught(pi,pj,a->ei,a->ej)) {
        printMaze(cells,pi,pj,gi,gj,a->ei,a->ej);
        printf("\n");
        char d = readDirection();
        switch(d){
            case 'w':
                if(pi > 0 && !cells[pi - 1][pj].bottom) pi -= 1;
                break;
            case 's':
                 if(pi < ROWS - 1 && !cells[pi][pj].bottom) pi += 1;
                break;
            case 'a':
                if(pj > 0 && !cells[pi][pj - 1].right) pj -= 1;
                break;
            case 'd':
                if(pj < COLS -1 && !cells[pi][pj].right) pj += 1;
                break;
        }
        system("cls");
        if(found(pi,pj,gi,gj)) printf("gaymamba");
    }
    if(found(pi,pj,gi,gj)) {
        system("cls");
        sleep(1);
        printf("\nYou Found the Booty!");
    }

    pthread_join(new_t, NULL);
   return 0;
}