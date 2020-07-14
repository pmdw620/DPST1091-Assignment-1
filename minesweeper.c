//
//  minesweeper.c
//  DPST1091 Assignment1
//
//  Created by Ming Pan on 14/7/20.
//  Copyright © 2020 Ming Pan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#define VISIBLE_SAFE 0
#define HIDDEN_SAFE 1
#define HIDDEN_MINE 2
#define SIZE 8
#define REVEAL_SIZE 3

int revealSquare(int gameMatrix[SIZE][SIZE], int row, int col);
int revealRadial(int gameMatrix[SIZE][SIZE], int row, int col);
int manhanttamDistance(int x1, int y1, int x2, int y2);
int inBound(int x, int y);
void gameInitial(int gameMatrix[SIZE][SIZE], int *n);
void printMatrix(const int gameMatrix[SIZE][SIZE], int gameMode);
void stageTest(int gameMatrix[SIZE][SIZE]);
void printFailMatrix(int gameMatrix[SIZE][SIZE]);
int detectRow(const int gameMatrix[SIZE][SIZE], int row);
int detectColumn(const int gameMatrix[SIZE][SIZE], int col);
int detectSqure(const int gameMatrix[SIZE][SIZE], int row, int col, int size);
int gameContinue(int gameMatrix[SIZE][SIZE]);
int min(int a, int b);
int max(int a, int b);

int main(int argc, const char * argv[]) {
    
    int gameMatrix[SIZE][SIZE];                 // game matrix
    int nMines = 0;                             // number of mines
    
    gameInitial(gameMatrix, &nMines);
    stageTest(gameMatrix);
    return 0;
}

// initial the game
void gameInitial(int gameMatrix[SIZE][SIZE], int *n){
    /* Initial the game matrix */
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            gameMatrix[i][j] = HIDDEN_SAFE;
        }
    }
    
    printf("Welcome to minesweeper!\n");
    /* get the configuration */
    printf("How many mines? ");
    scanf("%d", n);
    printf("Enter pairs:\n");
    
    /* get & set mines locations */
    for(int i=0; i<(*n); i++){
        int row, col;
        scanf("%d %d", &row, &col);
        
        /* if the number out of bounds, ignore */
        if(row >= SIZE || col >= SIZE)
            continue;
        
        /* otherwiese, set the mine to the location */
        gameMatrix[row][col] = HIDDEN_MINE;
    }
    printf("Game Started\n");
}

/* function that print the matrix (show mines location) */
void printMatrix(const int gameMatrix[SIZE][SIZE], int gameMode){
    if(gameMode == 0){
        for(int i=0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                printf("%d", gameMatrix[i][j]);
                if(j<SIZE-1)
                    printf(" ");
            }
            printf("\n");
        }
    } else{
        printf("..\n\\/\n");
        printf("    00 01 02 03 04 05 06 07\n");
        printf("   -------------------------\n");
        for(int i=0; i<SIZE; i++){
            printf("0%d |", i);
            for(int j=0; j<SIZE; j++){
                if(gameMatrix[i][j] == VISIBLE_SAFE){
                    int count = detectSqure(gameMatrix, i, j, REVEAL_SIZE);
                    if(count == 0)
                        printf("  ");
                    else
                        printf("%02d", count);
                } else{
                    printf("##");
                }
                if(j<SIZE-1)
                    printf(" ");
            }
            printf("|\n");
        }
        printf("   -------------------------\n");
    }
}

/* Stage Test */
void stageTest(int gameMatrix[SIZE][SIZE]){
    /* use a while loop to do the test */
    int hintUsed = 0;
    int gameMode = 0;       /* 0->debugMode, 1->gameMode*/
    
    while(gameContinue(gameMatrix)){
        printMatrix(gameMatrix, gameMode);
        int cmd = -1;
        fflush(stdin);
        /* break the loop if we reach Ctrl + D */
        if(scanf("%d", &cmd) != 1){
            printf("Game Over\n");
            
            break;
        }
        if(hintUsed >= 3 && (cmd == 1 || cmd == 2 || cmd == 3)){
            printf("Help already used\n");
        } else if(cmd == 1){
            int row;
            scanf("%d", &row);
            int count = detectRow(gameMatrix, row);
            printf("There are %d mine(s) in row %d\n", count, row);
            hintUsed++;
        } else if(cmd == 2){
            int col;
            scanf("%d", &col);
            int count = detectColumn(gameMatrix, col);
            printf("There are %d mine(s) in column %d\n", count, col);
            hintUsed++;
        } else if(cmd == 3){
            int row, col, size;
            scanf("%d %d %d", &row, &col, &size);
            int count = detectSqure(gameMatrix, row, col, size);
            printf("There are %d mine(s) in the square centered at row %d, column %d of size %d\n", count, row, col, size);
            hintUsed++;
        } else if(cmd == 4){
            int row, col;
            scanf("%d %d", &row, &col);
            int result = revealSquare(gameMatrix, row, col);
            if(result == -1){
                printf("Game Over\n");
                printFailMatrix(gameMatrix);
                return;
            }
        } else if(cmd == 5){
            gameMode = 1;
            printf("Gameplay mode activated\n");
        } else if(cmd == 6){
            gameMode = 0;
            printf("Debug mode activated\n");
        } else if(cmd == 7){
            int row, col;
            scanf("%d %d", &row, &col);
            int result = revealRadial(gameMatrix, row, col);
            if(result == -1){
                printf("Game Over\n");
                printFailMatrix(gameMatrix);
                return;
            }
        }else{
            printf("Invalid Command.\n");
        }
    }
    printf("Game Won!\n");
}

/* detect how many mines in a row */
int detectRow(const int gameMatrix[SIZE][SIZE], int row){
    int count = 0;
    for(int i=0; i<SIZE; i++){
        if(gameMatrix[row][i] == HIDDEN_MINE)
            count++;
    }
    return count;
}

/* detect how many mines in a column */
int detectColumn(const int gameMatrix[SIZE][SIZE], int col){
    int count = 0;
    for(int i=0; i<SIZE; i++){
        if(gameMatrix[i][col] == HIDDEN_MINE)
            count++;
    }
    return count;
}

/* detect how many ines in a square */
int detectSqure(const int gameMatrix[SIZE][SIZE], int row, int col, int size){
    int count = 0;
    int startRow = max(0, row-(size-1)/2);
    int endRow = min(SIZE, row+(size-1)/2+1);
    int startCol = max(0, col-(size-1)/2);
    int endCol = min(SIZE, col+(size-1)/2+1);
    for(int i=startRow; i < endRow; i++){
        for(int j=startCol; j < endCol; j++){
            if(gameMatrix[i][j] == HIDDEN_MINE){
                count++;
            }
        }
    }
    return count;
}


// if the selected square has adjacent mines then only reveal that square. Otherwise reveal all adjacent squares.
int revealSquare(int gameMatrix[SIZE][SIZE], int row, int col){
    /* if there is a mine in selected square, game is over */
    if(gameMatrix[row][col] == HIDDEN_MINE)
        return -1;
    
    gameMatrix[row][col] = VISIBLE_SAFE;
    if(detectSqure(gameMatrix, row, col, REVEAL_SIZE) == 0){
        int startRow = max(0, row-(REVEAL_SIZE-1)/2);
        int endRow = min(SIZE, row+(REVEAL_SIZE-1)/2+1);
        int startCol = max(0, col-(REVEAL_SIZE-1)/2);
        int endCol = min(SIZE, col+(REVEAL_SIZE-1)/2+1);
        for(int i=startRow; i<endRow; i++){
            for(int j=startCol; j<endCol; j++){
                gameMatrix[i][j] = VISIBLE_SAFE;
            }
        }
    }
    
    return 1;
}

// if the selected square has adjacent mines, then only reveal that square. Otherwise, reveal lines out from the square in 45 degree increments. A line stops after a square on the edge, or a square adjacent to a mine is reached
int revealRadial(int gameMatrix[SIZE][SIZE], int row, int col){
    gameMatrix[row][col] = VISIBLE_SAFE;
    if(detectSqure(gameMatrix, row, col, REVEAL_SIZE) == 0){
        int startRow = max(0, row-(REVEAL_SIZE-1)/2);
        int endRow = min(SIZE, row+(REVEAL_SIZE-1)/2+1);
        int startCol = max(0, col-(REVEAL_SIZE-1)/2);
        int endCol = min(SIZE, col+(REVEAL_SIZE-1)/2+1);
        for(int i=startRow; i<endRow; i++){
            for(int j=startCol; j<endCol; j++){
                gameMatrix[i][j] = VISIBLE_SAFE;
                if(manhanttamDistance(i, j, row, col) == 0)
                    continue;
                /* detect by radial */
                int rowDiff = i-row;
                int colDiff = j-col;
                int x = i+rowDiff, y = j+colDiff;
                while(1){
                    if(inBound(x,y) == 1 && detectSqure(gameMatrix,x, y, REVEAL_SIZE) == 0){
                        gameMatrix[x][y] = VISIBLE_SAFE;
                        x+=rowDiff;
                        y+=colDiff;
                    }else{
                        break;
                    }
                }
            }
        }
    }
    return 1;
}

/* use flag to mark the game status, 0 represents game won,
 and 1 represents game is not completed yet */
int gameContinue(int gameMatrix[SIZE][SIZE]){
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            if(gameMatrix[i][j] == HIDDEN_SAFE)
                return 1;
        }
    }
    return 0;
}

void printFailMatrix(int gameMatrix[SIZE][SIZE]){
    printf("xx\n/\\\n");
    printf("    00 01 02 03 04 05 06 07\n");
    printf("   -------------------------\n");
    for(int i=0; i<SIZE; i++){
        printf("0%d |", i);
        for(int j=0; j<SIZE; j++){
            if(gameMatrix[i][j] == VISIBLE_SAFE){
                int count = detectSqure(gameMatrix, i, j, REVEAL_SIZE);
                if(count == 0)
                    printf("  ");
                else
                    printf("%02d", count);
            } else if(gameMatrix[i][j] == HIDDEN_MINE){
                printf("()");
            } else{
                printf("##");
            }
            if(j<SIZE-1)
                printf(" ");
        }
        printf("|\n");
    }
    printf("   -------------------------\n");
}

int manhanttamDistance(int x1, int y1, int x2, int y2){
    return abs(x1-x2) + abs(y1-y2);
}

int min(int a, int b){
    if(a<b)
        return a;
    else
        return b;
}

int max(int a, int b){
    if(a>b)
        return a;
    else
        return b;
}

int inBound(int x, int y){
    if(x>=0 && y >=0 && x<=SIZE-1 && y<=SIZE-1)
        return 1;
    return 0;
}
