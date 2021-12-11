#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define GRID_SIDE_EXT 12
#define GRID_MIN_IDX 1
#define GRID_MAX_IDX 10
#define GRID_AREA_EXT (GRID_SIDE_EXT*GRID_SIDE_EXT)
#define LINE_BUF_LEN 16

typedef int grid_t[GRID_SIDE_EXT][GRID_SIDE_EXT];

void print_grid(grid_t grid)
{
    for (int i = GRID_MIN_IDX; i <= GRID_MAX_IDX; ++i) {
        for (int j = GRID_MIN_IDX; j <= GRID_MAX_IDX; ++j) {
            putchar(grid[i][j] + '0');
        }
        putchar('\n');
    }
}

int step(grid_t grid)
{
    int *flat = (int*)grid;
    for (int i = 0; i < GRID_AREA_EXT; ++i)
        ++flat[i];

    int flashes = 0;
    bool had_niner = false;
    do {
        had_niner = false;
        for (int i = GRID_MIN_IDX; i <= GRID_MAX_IDX; ++i) {
            for (int j = GRID_MIN_IDX; j <= GRID_MAX_IDX; ++j) {
                if (grid[i][j] > 9) {
                    if (grid[i-1][j-1] != 0) ++grid[i-1][j-1];
                    if (grid[i-1][j] != 0)   ++grid[i-1][j];
                    if (grid[i-1][j+1] != 0) ++grid[i-1][j+1];
                    if (grid[i][j-1] != 0)   ++grid[i][j-1];
                    grid[i][j] = 0;
                    if (grid[i][j+1] != 0)   ++grid[i][j+1];
                    if (grid[i+1][j-1] != 0) ++grid[i+1][j-1];
                    if (grid[i+1][j] != 0)   ++grid[i+1][j];
                    if (grid[i+1][j+1] != 0) ++grid[i+1][j+1];
                    had_niner = true;
                    ++flashes;
                }
            }
        }
    } while (had_niner);
    return flashes;
}

int main()
{
    grid_t grid = {{0}};
    char line[LINE_BUF_LEN];

    for (int i = GRID_MIN_IDX; i <= GRID_MAX_IDX && fgets(line, LINE_BUF_LEN, stdin); ++i) {
        for (int j = GRID_MIN_IDX; j <= GRID_MAX_IDX && line[j-1]; ++j) {
            grid[i][j] = line[j-1] - '0';
        }
    }

    int i = 0;
    while (1) {
        if (step(grid) == 100) {
            break;
        }
        ++i;
    }

    printf("%d\n", i+1);
    return 0;
}
