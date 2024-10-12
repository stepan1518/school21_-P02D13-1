#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80

int **init_matrix(int n, int m);
void init_grid_from_file(int **grid);
void output_matrix(int **matrix, int n, int m);

int main() {
    int error = 0;

    if (error != 1) {
        int **grid = init_matrix(HEIGHT, WIDTH);
        init_grid_from_file(grid);

        if (error != 1) {
            output_matrix(grid, HEIGHT, WIDTH);
        }

        free(grid);
    } 

    return 0;
}

int **init_matrix(int n, int m) {
    int **matrix = malloc(n * sizeof(int *));
    for (int i = 0; i < m; i++) {
        matrix[i] = malloc(m * sizeof(int));
    }

    return matrix;
}

void output_matrix(int **matrix, int n, int m) {
    int amount;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d", matrix[i][j]);
            amount++;

            if (j != m - 1) printf(" ");
        }

        printf("\n");
    }

    printf("AMOUNT: %d", amount);
}


void init_grid_from_file(int **grid) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char ch;
            scanf("%c", &ch);
            if (ch == EOF) break; // replace break with smth else
            grid[i][j] = (ch == '1') ? 1 : 0;
        }
    }
}