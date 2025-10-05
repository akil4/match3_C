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
bool matched[BOARD_SIZE][BOARD_SIZE] = {0};
float fall_offset[BOARD_SIZE][BOARD_SIZE] = {0};

float fall_speed = 8.0f;
int score = 0;
Vector2 grid_origin;
Vector2 selected_tile = {-1, -1};

char random_tile();
void init_board();
bool find_matches();
void resolve_matches();

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

        if (find_matches())
        {
            resolve_matches();
        }

        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (fall_offset[i][j] > 0)
                {
                    fall_offset[i][j] -= fall_speed;
                    if (fall_offset[i][j] < 0)
                    {
                        fall_offset[i][j] = 0;
                    }
                }
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

                if (board[i][j] != ' ')
                {
                    DrawTextEx(
                    GetFontDefault(),
                    TextFormat("%c", board[i][j]),
                    (Vector2) {rect.x + 12, rect.y + 8 - fall_offset[i][j]},
                    20, 
                    1, 
                    matched[i][j] ? GREEN : WHITE
                    );
                }
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

bool find_matches()
{
    bool found = false;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            matched[i][j] = false;
        }
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 2; j++)
        {
            char t = board[i][j];
            if (t == board[i][j + 1] && 
                t == board[i][j + 2])
                {
                    matched[i][j] = matched[i][j + 1] = matched[i][j + 2] = true;
                    // update scoreboard
                    score += 10;
                    found = true;
                }
        }
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 2; j++)
        {
            char t = board[j][i];
            if (t == board[j + 1][i] &&
                t == board[j + 2][i])
                {
                    matched[j][i] = matched[j + 1][i] = matched[j + 2][i] = true;
                    score += 10;
                    found = true;
                }
        }
    }

    return found;
}

void resolve_matches()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int write_j = BOARD_SIZE - 1;
        for (int j = BOARD_SIZE - 1; j >= 0; j--)
        {
            if (!matched[j][i])
            {
                if (j != write_j)
                {
                    board[write_j][i] = board[j][i];
                    fall_offset[write_j][i] = (write_j - j) * TILE_SIZE;
                    board[j][i] = ' ';
                }
                write_j--;
            }
        }

        // fill new tiles
        while (write_j >= 0)
        {
            board[write_j][i] = random_tile();
            fall_offset[write_j][i] = (write_j + 1) * TILE_SIZE;
            write_j--;
        }
    }
}