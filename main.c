#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define BOARD_SIZE 8
#define TILE_SIZE 42
#define TILE_TYPES 5

const char tile_chars[TILE_TYPES] = {'@', '#', '$', '%', '&'};

char board[BOARD_SIZE][BOARD_SIZE];

int score = 0;
Vector2 grid_origin;

char random_tile();
void init_board();

int main(void)
{
    const int screen_width = 800;
    const int screen_height = 450;

    InitWindow(screen_width, screen_height, "Match3 - C");
    SetTargetFPS(60);
    srand(time(NULL));

    init_board();

    while(!WindowShouldClose())
    {
        // update game logic

        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                Rectangle rect = {
                    grid_origin.x + (i * TILE_SIZE),
                    grid_origin.y + (j * TILE_SIZE),
                    TILE_SIZE,
                    TILE_SIZE
                };

                DrawRectangleLinesEx(rect, 1, GRAY);

                Vector2 pos = {
                    i * TILE_SIZE + 12,
                    j * TILE_SIZE + 8
                };

                DrawTextEx(
                    GetFontDefault(),
                    TextFormat("%c", board[i][j]),
                    (Vector2) {rect.x + 12, rect.y + 8},
                    20, 1, WHITE
                );
            }
        }

        DrawText(TextFormat("SCORE: %d", score), 20, 20, 24, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

char random_tile()
{
    return tile_chars[rand() % TILE_TYPES];
}

void init_board()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = random_tile();
        }
    }

    int grid_width = BOARD_SIZE * TILE_SIZE;
    int grid_height = BOARD_SIZE * TILE_SIZE;

    grid_origin = (Vector2) {
        (GetScreenWidth() - grid_width) / 2,
        (GetScreenHeight() - grid_height) / 2
    };
}