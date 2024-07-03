#include <raylib.h>

#define NULL 0

#define T_WID 10
#define T_HEI 25
#define PIECE_COLORS_NUM 6

#define TETRIS_BACKGROUND_COLOR BLACK

#define FRAME_THICKNESS 10
#define P_SIZE 5

typedef bool piece[P_SIZE][P_SIZE];

typedef enum
{
    SQUARE,
    LINE,
    S,
    Z,
    L,
    J,
    T
} pieceType;

piece basicPieces[7] = {
    {{0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 1, 1, 0, 0},
     {0, 1, 1, 0, 0},
     {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0},
     {0, 1, 0, 0, 0},
     {0, 1, 0, 0, 0},
     {0, 1, 0, 0, 0},
     {0, 1, 0, 0, 0}},
    {{0, 0, 0, 0, 0},
     {0, 0, 1, 1, 0},
     {0, 1, 1, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0},
     {0, 1, 1, 0, 0},
     {0, 0, 1, 1, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 1, 0},
     {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 1, 1, 0, 0},
     {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 1, 1, 1, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 0, 0, 0}

    }};

Color piecesColors[PIECE_COLORS_NUM] = {MAGENTA, GREEN, RED, ORANGE, BLUE, YELLOW};

typedef struct
{
    piece p;
    int x;
    int y;
    Color c;
    pieceType type;
} pieceEntity;

void myMemcpy(void *dest, void *src, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        *((char *)dest + i) = *((char *)src + i);
    }
}

void myMemset(void *dest, int value, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        *((char *)dest + i) = value;
    }
}

void generateNewPiece(pieceEntity *fallingPiece)
{
    int res = GetRandomValue(0, 6);
    for (int i = 0; i < P_SIZE; i++)
    {
        for (int j = 0; j < P_SIZE; j++)
        {
            fallingPiece->p[i][j] = basicPieces[res][i][j];
        }
    }
    fallingPiece->type = res;
    res = GetRandomValue(0, 5);
    fallingPiece->x = T_WID / 2 - 1;
    fallingPiece->y = 0;
    fallingPiece->c = piecesColors[res];
}

bool checkLeftCollision(pieceEntity *fallingPiece, bool logicTable[T_WID][T_HEI])
{
    for (int i = 0; i < P_SIZE; i++)
    {
        for (int j = 0; j < P_SIZE; j++)
        {
            if (fallingPiece->p[i][j])
            {
                if (fallingPiece->x + i - 1 < 0 || logicTable[fallingPiece->x + i - 1][fallingPiece->y + j])
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkRightCollision(pieceEntity *fallingPiece, bool logicTable[T_WID][T_HEI])
{
    for (int i = 0; i < P_SIZE; i++)
    {
        for (int j = 0; j < P_SIZE; j++)
        {
            if (fallingPiece->p[i][j])
            {
                if (fallingPiece->x + i + 1 >= T_WID || logicTable[fallingPiece->x + i + 1][fallingPiece->y + j])
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkDownCollision(pieceEntity *fallingPiece, bool logicTable[T_WID][T_HEI])
{
    for (int i = 0; i < P_SIZE; i++)
    {
        for (int j = 0; j < P_SIZE; j++)
        {
            if (fallingPiece->p[i][j])
            {
                if (fallingPiece->y + j + 1 >= T_HEI || logicTable[fallingPiece->x + i][fallingPiece->y + j + 1])
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void logicTableAddPiece(pieceEntity *fallingPiece, bool logicTable[T_WID][T_HEI])
{
    for (int i = 0; i < P_SIZE; i++)
    {
        for (int j = 0; j < P_SIZE; j++)
        {
            if (fallingPiece->p[i][j])
            {
                logicTable[fallingPiece->x + i][fallingPiece->y + j] = true;
            }
        }
    }
}

void rotate90AntiClockwise(piece *a)
{
    // Consider all squares one by one
    for (int x = 0; x < P_SIZE / 2; x++)
    {
        // Consider elements in group
        // of 4 in current square
        for (int y = x; y < P_SIZE - x - 1; y++)
        {
            // Store current cell in
            // temp variable
            bool temp = (*a)[x][y];

            // Move values from right to top
            (*a)[x][y] = (*a)[y][P_SIZE - 1 - x];

            // Move values from bottom to right
            (*a)[y][P_SIZE - 1 - x] = (*a)[P_SIZE - 1 - x][P_SIZE - 1 - y];

            // Move values from left to bottom
            (*a)[P_SIZE - 1 - x][P_SIZE - 1 - y] = (*a)[P_SIZE - 1 - y][x];

            // Assign temp to left
            (*a)[P_SIZE - 1 - y][x] = temp;
        }
    }
}

void rotate90Clockwise(piece *a)
{
    for (int i = 0; i < P_SIZE / 2; i++)
    {
        for (int j = i; j < P_SIZE - i - 1; j++)
        {
            bool temp = (*a)[i][j];
            (*a)[i][j] = (*a)[P_SIZE - 1 - j][i];
            (*a)[P_SIZE - 1 - j][i] = (*a)[P_SIZE - 1 - i][P_SIZE - 1 - j];
            (*a)[P_SIZE - 1 - i][P_SIZE - 1 - j] = (*a)[j][P_SIZE - 1 - i];
            (*a)[j][P_SIZE - 1 - i] = temp;
        }
    }
}

bool checkIfPieceCanRotate(pieceEntity *fallingPiece, bool logicTable[T_WID][T_HEI], piece **refToRot)
{
    piece *temp = MemAlloc(sizeof(piece));
    myMemcpy(*temp, fallingPiece->p, sizeof(piece));

    if (fallingPiece->type == SQUARE)
    {
        return false;
    }
    if (fallingPiece->type == LINE || fallingPiece->type == S || fallingPiece->type == Z || fallingPiece->type == T)
    {
        rotate90Clockwise(temp);
    }
    else
    {
        rotate90AntiClockwise(temp);
    }

    for (int i = 0; i < P_SIZE; i++)
    {
        for (int j = 0; j < P_SIZE; j++)
        {
            if ((*temp)[i][j])
            {
                if (fallingPiece->x + i < 0 || fallingPiece->x + i >= T_WID || fallingPiece->y + j < 0 || fallingPiece->y + j >= T_HEI || logicTable[fallingPiece->x + i][fallingPiece->y + j])
                {
                    return false;
                }
            }
        }
    }
    *refToRot = temp;
    return true;
}

void pieceShouldDo(pieceEntity *fallingPiece, bool logicTable[T_WID][T_HEI], long *currentMillis, long *lastMillis, float currentLevel, bool *pieceFalling)
{

    // User input movement

    if (IsKeyDown(KEY_LEFT))
    {
        if (!checkLeftCollision(fallingPiece, logicTable))
        {
            fallingPiece->x--;
        }
        WaitTime(0.1);
        return;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        if (!checkRightCollision(fallingPiece, logicTable))
        {
            fallingPiece->x++;
        }
        WaitTime(0.1);
        return;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        if (!checkDownCollision(fallingPiece, logicTable))
        {
            fallingPiece->y++;
        }
        WaitTime(0.1);
        return;
    }
    if (IsKeyPressed(KEY_UP))
    {
        piece *temp = NULL;
        if (checkIfPieceCanRotate(fallingPiece, logicTable, &temp))
        {
            if (temp != NULL)
            {
                for (int i = 0; i < P_SIZE; i++)
                {
                    for (int j = 0; j < P_SIZE; j++)
                    {
                        fallingPiece->p[i][j] = (*temp)[i][j];
                    }
                }
                MemFree(temp);
            }
        }
        return;
    }
    if (IsKeyPressed(KEY_SPACE))
    {
        while (!checkDownCollision(fallingPiece, logicTable))
        {
            fallingPiece->y++;
        }
        logicTableAddPiece(fallingPiece, logicTable);
        *pieceFalling = false;
        return;
    }

    if (*currentMillis - *lastMillis > 100.0 / currentLevel)
    { // Automatic fall
        *lastMillis = *currentMillis;
        if (!checkDownCollision(fallingPiece, logicTable))
        {
            fallingPiece->y++;
        }
        else
        {
            logicTableAddPiece(fallingPiece, logicTable);
            *pieceFalling = false;
        }
        return;
    }
}

void drawPiece(pieceEntity *fallingPiece, int x, int y, int size, int tableXPosition, int tableYPosition)
{
    for (int i = 0; i < P_SIZE; i++)
    {
        for (int j = 0; j < P_SIZE; j++)
        {
            if (fallingPiece->p[i][j])
            {
                DrawRectangle(tableXPosition + (fallingPiece->x + i) * size, tableYPosition + (fallingPiece->y + j) * size, size, size, fallingPiece->c);
                DrawRectangleLines(tableXPosition + (fallingPiece->x + i) * size, tableYPosition + (fallingPiece->y + j) * size, size, size, (Color){255, 255, 255, 90});
            }
        }
    }
}

void drawPiceAtPos(pieceEntity *piece, int x, int y, int size)
{
    for (int i = 0; i < P_SIZE; i++)
    {
        for (int j = 0; j < P_SIZE; j++)
        {
            if (piece->p[i][j])
            {
                DrawRectangle(x + i * size, y + j * size, size, size, piece->c);
                DrawRectangleLines(x + i * size, y + j * size, size, size, (Color){255, 255, 255, 90});
            }
        }
    }
}

void checkCompleteLines(bool logicTable[T_WID][T_HEI], Color drawTable[T_WID][T_HEI], float *currentLevel)
{
    for (int i = 0; i < T_HEI; i++)
    {
        bool complete = true;
        for (int j = 0; j < T_WID; j++)
        {
            if (!logicTable[j][i])
            {
                complete = false;
                break;
            }
        }
        if (complete)
        {
            // Move all the lines above one line down, except the first line
            for (int k = i; k > 0; k--)
            {
                for (int j = 0; j < T_WID; j++)
                {
                    logicTable[j][k] = logicTable[j][k - 1];
                    drawTable[j][k] = drawTable[j][k - 1];
                }
            }
            for (int j = 0; j < T_WID; j++)
            {
                logicTable[j][0] = false;
                drawTable[j][0] = TETRIS_BACKGROUND_COLOR;
            }
            *currentLevel += 0.10;
        }
    }
}

void pressToStart(bool *randomSeeded, pieceEntity *nextPiece, pieceEntity *savedPiece)
{

    static int startValue = 41;

    DrawText("Press Screen to start", GetScreenWidth() / 2 - MeasureText("Press ENTER to start", 50) / 2, GetScreenHeight() / 2, 50, WHITE);
    startValue += GetMouseX() * GetMouseY() + GetMouseX() + GetMouseY();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {   
        *randomSeeded = true;
        SetRandomSeed(startValue);
        generateNewPiece(savedPiece);
        generateNewPiece(nextPiece);
    }
}

int main(void)
{
    const int FPS = 60;
    const Color loopBackgroundColor = ColorBrightness(GRAY, -0.7);
    const Color gameOverColor = ColorBrightness(RED, -0.7);
    

    int TETRIS_BLOCK_SIZE;
    int BLOCK_BORDER_SIZE;

    int screenWidth = 1200;
    int screenHeight = 600;
    float adjustFactor = (float)screenWidth / 1600.0;
    int FRAME_THICKNESS_ADJUSTED = FRAME_THICKNESS * adjustFactor;

    bool gameOver = false;
    bool pieceFalling = false;
    bool pieceSaved = false;
    bool randomSeeded = false;

    pieceEntity fallingPiece;
    pieceEntity savedPiece;
    pieceEntity nextPiece;

    float currentLevel = 1.0;
    int currentGameTime = 0;
    int piecesPlaced = 0;
    int lastPiecesPlaced = -1;
    long lastMillis = 0;

    // Reduce brightness of colors
    for (int i = 0; i < PIECE_COLORS_NUM; i++)
    {
        piecesColors[i] = ColorBrightness(piecesColors[i], -0.2);
    }

    if (screenWidth / T_WID < screenHeight / T_HEI)
    {
        TETRIS_BLOCK_SIZE = (screenWidth - 100 * adjustFactor) / T_WID;
    }
    else
    {
        TETRIS_BLOCK_SIZE = (screenHeight - 100 * adjustFactor) / T_HEI;
    }
    BLOCK_BORDER_SIZE = TETRIS_BLOCK_SIZE / 20;

    const int tableXPosition = screenWidth / 2 - T_WID * TETRIS_BLOCK_SIZE / 2;
    const int tableYPosition = screenHeight / 2 - T_HEI * TETRIS_BLOCK_SIZE / 2;

    Color drawTable[T_WID][T_HEI];
    for (int i = 0; i < T_WID; i++)
    {
        for (int j = 0; j < T_HEI; j++)
        {
            drawTable[i][j] = TETRIS_BACKGROUND_COLOR;
        }
    }

    bool logicTable[T_WID][T_HEI];
    for (int i = 0; i < T_WID; i++)
    {
        for (int j = 0; j < T_HEI; j++)
        {
            logicTable[i][j] = false;
        }
    }

    InitWindow(screenWidth, screenHeight, "Tetris");

    SetTargetFPS(FPS);
    long frames = 0;
    ClearBackground(BLACK);

    while (!WindowShouldClose())
    {
        if (!randomSeeded)
        {
            BeginDrawing();
            ClearBackground(loopBackgroundColor);
            pressToStart(&randomSeeded, &nextPiece, &savedPiece);
            EndDrawing();
            continue;
        }

        frames++;
        long current_millis = 200 / FPS * frames;

        if (gameOver)
        {
            BeginDrawing();
            ClearBackground(gameOverColor);
            DrawText("Game Over", screenWidth * 1 / 7, screenHeight / 2 - (240 * adjustFactor) / 2, 240 * adjustFactor, WHITE);
            DrawText("Press ENTER...", screenWidth * 1 / 3, screenHeight / 2 + (240 * adjustFactor + 40), 70 * adjustFactor, WHITE);
            if (IsKeyPressed(KEY_ENTER))
            {
                gameOver = false;
            }
            EndDrawing();
            continue;
        }

        if (!pieceFalling)
        {
            fallingPiece = nextPiece;
            generateNewPiece(&nextPiece);
            if (checkDownCollision(&fallingPiece, logicTable))
            {
                gameOver = true;
                pieceFalling = false;
                pieceSaved = false;
                piecesPlaced = 0;
                lastPiecesPlaced = -1;
                currentLevel = 1.0;
                for (int i = 0; i < T_WID; i++)
                {
                    for (int j = 0; j < T_HEI; j++)
                    {
                        logicTable[i][j] = false;
                        drawTable[i][j] = TETRIS_BACKGROUND_COLOR;
                    }
                }
                generateNewPiece(&nextPiece);
                generateNewPiece(&savedPiece);

                continue;
            }
            pieceFalling = true;
        }

        if (IsKeyPressed(KEY_C))
        {
            if (piecesPlaced != lastPiecesPlaced)
            {
                pieceEntity temp = fallingPiece;
                if (!pieceSaved)
                {
                    fallingPiece = nextPiece;
                    generateNewPiece(&nextPiece);
                }
                else
                    fallingPiece = savedPiece;
                temp.x = T_WID / 2 - 1;
                temp.y = 0;
                savedPiece = temp;
                pieceSaved = true;
                lastPiecesPlaced = piecesPlaced;
            }
        }

        pieceShouldDo(&fallingPiece, logicTable, &current_millis, &lastMillis, currentLevel, &pieceFalling);

        if (!pieceFalling)
        { // If the piece is not falling anymore
            for (int i = 0; i < P_SIZE; i++)
            {
                for (int j = 0; j < P_SIZE; j++)
                {
                    if (fallingPiece.p[i][j])
                    {
                        drawTable[fallingPiece.x + i][fallingPiece.y + j] = fallingPiece.c;
                        piecesPlaced++;
                    }
                }
            }
        }

        checkCompleteLines(logicTable, drawTable, &currentLevel);

        BeginDrawing();

        ClearBackground(loopBackgroundColor);

        // Draw drawTable
        for (int i = 0; i < T_WID; i++)
        {
            for (int j = 0; j < T_HEI; j++)
            {
                DrawRectangle(tableXPosition + i * TETRIS_BLOCK_SIZE,
                              tableYPosition + j * TETRIS_BLOCK_SIZE, TETRIS_BLOCK_SIZE,
                              TETRIS_BLOCK_SIZE, drawTable[i][j]);

                DrawRectangleLines(tableXPosition + i * TETRIS_BLOCK_SIZE,
                                   tableYPosition + j * TETRIS_BLOCK_SIZE, TETRIS_BLOCK_SIZE,
                                   TETRIS_BLOCK_SIZE, (Color){255, 255, 255, 50});
            }
        }

        // Draw falling piece
        if (pieceFalling)
            drawPiece(&fallingPiece,
                      TETRIS_BLOCK_SIZE, TETRIS_BLOCK_SIZE, TETRIS_BLOCK_SIZE,
                      tableXPosition, tableYPosition);

        // Endarken all the canvas pieces under the falling piece true values
        int trueValues[P_SIZE][2];
        myMemset(trueValues, -1, sizeof(trueValues));

        for (int i = 0; i < P_SIZE; i++)
        {
            for (int j = 0; j < P_SIZE; j++)
            {
                if (fallingPiece.p[i][j])
                {
                    trueValues[i][0] = fallingPiece.x + i;
                    trueValues[i][1] = fallingPiece.y + j;
                }
            }
        }

        for (int i = 0; i < P_SIZE; i++)
        {
            if (trueValues[i][0] != -1)
            {
                for (int y = trueValues[i][1]; y < T_HEI; y++)
                {
                    if (logicTable[trueValues[i][0]][y])
                    {
                        DrawRectangle(tableXPosition + trueValues[i][0] * TETRIS_BLOCK_SIZE,
                                      tableYPosition + y * TETRIS_BLOCK_SIZE, TETRIS_BLOCK_SIZE,
                                      TETRIS_BLOCK_SIZE, ColorBrightness(drawTable[trueValues[i][0]][y], -0.5));
                    }
                }
            }
        }

        // Draw level as "Score: level"
        DrawText(TextFormat("Score: %d", ((int)(currentLevel - 1) * 100) + piecesPlaced * 5), 10 * adjustFactor, screenHeight / 2, 90 * adjustFactor, WHITE);

        if (pieceSaved)
        {
            DrawText("Piece saved", screenWidth / 2 + TETRIS_BLOCK_SIZE * T_WID / 2 + 30 * adjustFactor, screenHeight / 2 + screenHeight / 15, 90 * adjustFactor, WHITE);
            Rectangle savedPieceFrame = {screenWidth / 2 + TETRIS_BLOCK_SIZE * (T_WID - 2) - FRAME_THICKNESS_ADJUSTED, screenHeight / 2 + screenHeight / 6 - FRAME_THICKNESS_ADJUSTED + TETRIS_BLOCK_SIZE,
                                         6 * TETRIS_BLOCK_SIZE + FRAME_THICKNESS_ADJUSTED * 3, 6 * TETRIS_BLOCK_SIZE + FRAME_THICKNESS_ADJUSTED * 3};
            DrawRectangleGradientV(screenWidth / 2 + TETRIS_BLOCK_SIZE * (T_WID - 2) - FRAME_THICKNESS_ADJUSTED, screenHeight / 2 + screenHeight / 6 - FRAME_THICKNESS_ADJUSTED + TETRIS_BLOCK_SIZE,
                                   6 * TETRIS_BLOCK_SIZE + FRAME_THICKNESS_ADJUSTED * 3, 6 * TETRIS_BLOCK_SIZE + FRAME_THICKNESS_ADJUSTED * 3, ColorBrightness(GREEN, -0.4), ColorBrightness(BLUE, -0.4));
            DrawRectangleLinesEx(savedPieceFrame, FRAME_THICKNESS_ADJUSTED, WHITE);
            drawPiceAtPos(&savedPiece, screenWidth / 2 + TETRIS_BLOCK_SIZE * T_WID, screenHeight / 2 + screenHeight / 6 + TETRIS_BLOCK_SIZE * 3, TETRIS_BLOCK_SIZE);
        }

        // Draw next piece
        DrawText("Next piece", screenWidth / 2 + TETRIS_BLOCK_SIZE * T_WID / 2 + 30 * adjustFactor, screenHeight / 2 - screenHeight * 2 / 5, 90 * adjustFactor, WHITE);
        Rectangle nextPieceFrame = {screenWidth / 2 + TETRIS_BLOCK_SIZE * (T_WID - 2) - FRAME_THICKNESS_ADJUSTED, screenHeight / 2 - screenHeight / 5 - FRAME_THICKNESS_ADJUSTED - TETRIS_BLOCK_SIZE * 2,
                                    6 * TETRIS_BLOCK_SIZE + FRAME_THICKNESS_ADJUSTED * 3, 6 * TETRIS_BLOCK_SIZE + FRAME_THICKNESS_ADJUSTED * 3};

        DrawRectangleGradientV(screenWidth / 2 + TETRIS_BLOCK_SIZE * (T_WID - 2) - FRAME_THICKNESS_ADJUSTED, screenHeight / 2 - screenHeight / 5 - FRAME_THICKNESS_ADJUSTED - TETRIS_BLOCK_SIZE * 2,
                               6 * TETRIS_BLOCK_SIZE + FRAME_THICKNESS_ADJUSTED * 3, 6 * TETRIS_BLOCK_SIZE + FRAME_THICKNESS_ADJUSTED * 3, ColorBrightness(GREEN, -0.4), ColorBrightness(BLUE, -0.4));
        DrawRectangleLinesEx(nextPieceFrame, FRAME_THICKNESS_ADJUSTED, WHITE);
        drawPiceAtPos(&nextPiece, screenWidth / 2 + TETRIS_BLOCK_SIZE * (T_WID-1), screenHeight / 2 - screenHeight / 5 - TETRIS_BLOCK_SIZE, TETRIS_BLOCK_SIZE);

        // Draw frame around the table
        Rectangle tableFrame = {tableXPosition - FRAME_THICKNESS_ADJUSTED, tableYPosition - FRAME_THICKNESS_ADJUSTED,
                                T_WID * TETRIS_BLOCK_SIZE + FRAME_THICKNESS_ADJUSTED * 2, T_HEI * TETRIS_BLOCK_SIZE + FRAME_THICKNESS_ADJUSTED * 2};
        DrawRectangleLinesEx(tableFrame, FRAME_THICKNESS_ADJUSTED, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}