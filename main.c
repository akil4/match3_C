#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define BOARD_SIZE 8
#define TILE_SIZE 42
#define TILE_TYPES 5
#define SCORE_FONT_SIZE 32

const char tile_chars[TILE_TYPES] = {'@', '#', '$', '%', '&'};

char board[BOARD_SIZE][BOARD_SIZE];

int score = 0;
Vector2 grid_origin;
Vector2 selected_tile = {-1, -1};

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
    Vector2 mouse = {0, 0};

    while(!WindowShouldClose())
    {
        // update game logic
        mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            int x = (mouse.x - grid_origin.x) / TILE_SIZE;
            int y = (mouse.y - grid_origin.y) / TILE_SIZE;
            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
            {
                selected_tile = (Vector2){x, y};
            }
        }

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

        // select tile
        if (selected_tile.x >= 0)
        {
            DrawRectangleLinesEx((Rectangle) {
                grid_origin.x + (selected_tile.x * TILE_SIZE),
                grid_origin.y + (selected_tile.y * TILE_SIZE),
                TILE_SIZE,
                TILE_SIZE
            }, 2, YELLOW);
        }

        DrawTextEx(
            GetFontDefault(),
            TextFormat("SCORE: %d", score),
            (Vector2) {
                20, 20
            },
            SCORE_FONT_SIZE,
            1.0f,
            RED
        );

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