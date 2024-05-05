#include <raylib.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define T_WID 10
#define T_HEI 20
#define PIECE_COLORS_NUM 6

#define TETRIS_BACKGROUND_COLOR BLACK

typedef bool piece[4][4];

piece basicO = {
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
};

piece basicI = {
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0}
};
piece basicS = {
    {0,0,0,0},
    {0,1,1,0},
    {1,1,0,0},
    {0,0,0,0}
};
piece basicZ = {
    {0,0,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,0,0,0}
};
piece basicL = {
    {0,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
};
piece basicJ = {
    {0,1,0,0},
    {0,1,0,0},
    {1,1,0,0},
    {0,0,0,0}
};
piece basicT = {
    {0,0,0,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,0,0,0}
};

piece basicPieces[7] = {
    {
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    {
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0}
    },
    {
        {0,0,0,0},
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0}
    },
    {
        {0,0,0,0},
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    {
        {0,1,0,0},
        {0,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    {
        {0,1,0,0},
        {0,1,0,0},
        {1,1,0,0},
        {0,0,0,0}
    },
    {
        {0,0,0,0},
        {1,1,1,0},
        {0,1,0,0},
        {0,0,0,0}
    
    }
};

Color piecesColors[PIECE_COLORS_NUM] = {MAGENTA, GREEN, RED, ORANGE, BLUE, YELLOW};




typedef struct{
    piece p;
    int x;
    int y;
    Color c;
} pieceEntity;

void generateNewPiece(pieceEntity * fallingPiece){
    int res = rand() % 7;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            fallingPiece->p[i][j] = basicPieces[res][i][j];
        }
    }
    res = rand() % PIECE_COLORS_NUM;
    fallingPiece->x = T_WID/2 - 1;
    fallingPiece->y = 0;
    fallingPiece->c = piecesColors[res];
}

bool checkLeftCollision(pieceEntity * fallingPiece, bool logicTable[T_WID][T_HEI]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(fallingPiece->p[i][j]){
                if(fallingPiece->x + i - 1 < 0 || logicTable[fallingPiece->x + i - 1][fallingPiece->y + j]){
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkRightCollision(pieceEntity * fallingPiece, bool logicTable[T_WID][T_HEI]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(fallingPiece->p[i][j]){
                if(fallingPiece->x + i + 1 >= T_WID || logicTable[fallingPiece->x + i + 1][fallingPiece->y + j]){
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkDownCollision(pieceEntity * fallingPiece, bool logicTable[T_WID][T_HEI]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(fallingPiece->p[i][j]){
                if(fallingPiece->y + j + 1 >= T_HEI || logicTable[fallingPiece->x + i][fallingPiece->y + j + 1]){
                    return true;
                }
            }
        }
    }
    return false;
}

void logicTableAddPiece(pieceEntity * fallingPiece, bool logicTable[T_WID][T_HEI]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(fallingPiece->p[i][j]){
                logicTable[fallingPiece->x + i][fallingPiece->y + j] = true;
            }
        }
    }
}

piece * rotatePieceEntity(pieceEntity* pe) {

    piece * temp = malloc(sizeof(piece));

    if(temp == NULL){
        printf("Error allocating memory\n");
        exit(1);
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            (*temp)[j][3 - i] = pe->p[i][j];
        }
    }
    
    return temp;
}

bool checkIfPieceCanRotate(pieceEntity * fallingPiece, bool logicTable[T_WID][T_HEI], piece ** refToRot){
    piece * temp = rotatePieceEntity(fallingPiece);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if((*temp)[i][j]){
                if(fallingPiece->x + i < 0 || fallingPiece->x + i >= T_WID || fallingPiece->y + j < 0 || fallingPiece->y + j >= T_HEI || logicTable[fallingPiece->x + i][fallingPiece->y + j]){
                    return false;
                }
            }
        }
    }
    *refToRot = temp;
    return true;
}


void pieceShouldDo(pieceEntity * fallingPiece, bool logicTable[T_WID][T_HEI], clock_t * currentMillis, clock_t * lastMillis, float currentLevel, bool * pieceFalling){
    if(*currentMillis - *lastMillis > 100.0/currentLevel){ // Automatic fall
        *lastMillis = *currentMillis;
        if(!checkDownCollision(fallingPiece, logicTable)){
            fallingPiece->y++;
        } else {
            logicTableAddPiece(fallingPiece, logicTable);
            *pieceFalling = false;
        }
        return;
    }

    // User input movement

    if(IsKeyPressed(KEY_LEFT)){
        if(!checkLeftCollision(fallingPiece, logicTable)){
            fallingPiece->x--;
        }
    }
    if(IsKeyPressed(KEY_RIGHT)){
        if(!checkRightCollision(fallingPiece, logicTable)){
            fallingPiece->x++;
        }
    }
    if(IsKeyPressed(KEY_DOWN)){
        if(!checkDownCollision(fallingPiece, logicTable)){
            fallingPiece->y++;
        }
    }
    if(IsKeyPressed(KEY_UP)){
        piece * temp = NULL;
        if(checkIfPieceCanRotate(fallingPiece, logicTable, &temp)){
            if(temp != NULL){
                for(int i = 0; i < 4; i++){
                    for(int j = 0; j < 4; j++){
                        fallingPiece->p[i][j] = (*temp)[i][j];
                    }
                }

                free(temp);
            } else {
                printf("Error rotating piece\n");
            }
        }
    }

}

void drawPiece(pieceEntity * fallingPiece, int x, int y, int size, int tableXPosition, int tableYPosition){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(fallingPiece->p[i][j]){
                DrawRectangle(tableXPosition + (fallingPiece->x + i)*size, tableYPosition + (fallingPiece->y + j)*size, size, size, fallingPiece->c);
                DrawRectangleLines(tableXPosition + (fallingPiece->x + i)*size, tableYPosition + (fallingPiece->y + j)*size, size, size, (Color){255,255,255,90});
            }
        }
    }
}

void checkCompleteLines(bool logicTable[T_WID][T_HEI], Color drawTable[T_WID][T_HEI], float * currentLevel){
    for(int i = 0; i < T_HEI; i++){
        bool complete = true;
        for(int j = 0; j < T_WID; j++){
            if(!logicTable[j][i]){
                complete = false;
                break;
            }
        }
        if(complete){
            // Move all the lines above one line down, except the first line
            for(int k = i; k > 0; k--){
                for(int j = 0; j < T_WID; j++){
                    logicTable[j][k] = logicTable[j][k-1];
                    drawTable[j][k] = drawTable[j][k-1];
                }
            }
            for(int j = 0; j < T_WID; j++){
                logicTable[j][0] = false;
                drawTable[j][0] = TETRIS_BACKGROUND_COLOR;
            }
            *currentLevel += 0.5;

        }
    }
}



int main(void)
{   
    const int screenWidth = 1600;
    const int screenHeight = 900;

    int TETRIS_BLOCK_SIZE;
    int BLOCK_BORDER_SIZE;

    bool gameOver = false;
    bool pieceFalling = false;

    pieceEntity fallingPiece;

    float currentLevel = 1.0;
    int currentGameTime = 0;
    clock_t lastMillis = 0;

    // Reduce brightness of colors
    for(int i = 0; i < PIECE_COLORS_NUM; i++){
        piecesColors[i] = ColorBrightness(piecesColors[i], 0.5);
    }

    srand(time(NULL));

    if(screenWidth / T_WID < screenHeight / T_HEI){
        TETRIS_BLOCK_SIZE = (screenWidth-100) / T_WID;
    } else {
        TETRIS_BLOCK_SIZE = (screenHeight-100) / T_HEI;
    }
    BLOCK_BORDER_SIZE = TETRIS_BLOCK_SIZE / 20;

    const int tableXPosition = screenWidth/2 - T_WID*TETRIS_BLOCK_SIZE/2;
    const int tableYPosition = screenHeight/2 - T_HEI*TETRIS_BLOCK_SIZE/2;

    Color drawTable[T_WID][T_HEI];
    for(int i = 0; i < T_WID; i++){
        for(int j = 0; j < T_HEI; j++){
            drawTable[i][j] = TETRIS_BACKGROUND_COLOR;
        }
    }

    bool logicTable[T_WID][T_HEI];
    for(int i = 0; i < T_WID; i++){
        for(int j = 0; j < T_HEI; j++){
            logicTable[i][j] = false;
        }
    }

    InitWindow(screenWidth, screenHeight, "Tetris");

    SetTargetFPS(144);

    ClearBackground(BLACK);

    while (!WindowShouldClose()){

        clock_t current_millis = clock()/(CLOCKS_PER_SEC/1000);

        if(!pieceFalling){
            generateNewPiece(&fallingPiece);
            if(checkDownCollision(&fallingPiece, logicTable)){
                gameOver = true;
                break;
            }
            pieceFalling = true;
        }

        pieceShouldDo(&fallingPiece, logicTable, &current_millis, &lastMillis, currentLevel, &pieceFalling);
        
        if(!pieceFalling){ // If the piece is not falling anymore
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    if(fallingPiece.p[i][j]){
                        drawTable[fallingPiece.x + i][fallingPiece.y + j] = fallingPiece.c;
                    }
                }
            }
        }

        checkCompleteLines(logicTable, drawTable, &currentLevel);

        BeginDrawing();

        ClearBackground(GRAY);

        // Draw drawTable
        for(int i = 0; i < T_WID; i++){
            for(int j = 0; j < T_HEI; j++){
                DrawRectangle(tableXPosition + i*TETRIS_BLOCK_SIZE,
                            tableYPosition + j*TETRIS_BLOCK_SIZE, TETRIS_BLOCK_SIZE,
                            TETRIS_BLOCK_SIZE, drawTable[i][j]);

                DrawRectangleLines(tableXPosition + i*TETRIS_BLOCK_SIZE,
                                tableYPosition + j*TETRIS_BLOCK_SIZE, TETRIS_BLOCK_SIZE,
                                TETRIS_BLOCK_SIZE, (Color){255,255,255,50});
            }
        }

        // Draw falling piece
        if(pieceFalling)
            drawPiece(&fallingPiece,
                    TETRIS_BLOCK_SIZE, TETRIS_BLOCK_SIZE, TETRIS_BLOCK_SIZE,
                    tableXPosition, tableYPosition);
        
        // Draw piece coordinates
        DrawText(TextFormat("X: %d Y: %d", fallingPiece.x, fallingPiece.y), 10, 10, 20, WHITE);
        // Draw if piece is falling
        DrawText(TextFormat("Piece falling: %s", pieceFalling ? "true" : "false"), 10, 30, 20, WHITE);
        
        EndDrawing();
    }


    CloseWindow();

    return 0;

}