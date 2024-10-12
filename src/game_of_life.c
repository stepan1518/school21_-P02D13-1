#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80

int **init_matrix(int n, int m);
int init_grid_from_file(int **grid, char *filename);
void output_matrix(int **matrix, int n, int m);

int main(int argc, char *argv[]) {
    int error = 0;

    if (argc != 2) {
        printf("usage: %s <filename>\n", argv[0]);
        error = 1;
    } else if (access((argv[1]), 0) != 0) {
        error = 1;
        printf("file %s does not exist\n", argv[1]);
    }

    if (error != 1) {
        int **grid = init_matrix(HEIGHT, WIDTH);
        error = init_grid_from_file(grid, argv[1]);

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
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d", matrix[i][j]);

            if (j != m - 1) printf(" ");
        }

        printf("\n");
    }
}


int init_grid_from_file(int **grid, char *filename) {
    int error = 0;

    FILE *file = fopen(filename, "r"); 
    if (!file) {
        printf("couldn't open file %s", filename);
        error = 1;
    }

    if (error != 0) {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                int ch = fgetc(file);
                if (ch == EOF) break; // replace break with smth else
                grid[i][j] = (ch == '1') ? 1 : 0;
            }

            fgetc(file);
        }
    }
    
    fclose(file);

    return error;
}