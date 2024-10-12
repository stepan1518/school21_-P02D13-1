#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define HEIGHT 25
#define WIDTH 80

void init_grid_from_file(int grid[][WIDTH]);
void render_canvas(int grid[][WIDTH]);
int move_grid(int grid[][WIDTH]);
int neighbours_count(int grid[][WIDTH], int i_el, int j_el);
void change_speed(int* speed, char action);

int main() {
    initscr();
    nodelay(stdscr, TRUE);
    noecho();
    
    int speed = 300001;
    int grid[HEIGHT][WIDTH];

    init_grid_from_file(grid);

    while (1) {
        render_canvas(grid);
        char action = getch();
        if (action != ERR) {
            change_speed(&speed, action);
        } 
        if (action == 'q') {
            break;
        }
        int flag = move_grid(grid);
        if (flag == 0) {
            break;
        }
        usleep(speed);
    }

    endwin();
    return 0;
}


void init_grid_from_file(int grid[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int num;
            scanf("%d", &num);
            grid[i][j] = num;
        }
    }
    freopen("/dev/tty", "rw", stdin);
}

void render_canvas(int grid[][WIDTH]) {
    clear();    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j])
                printw("%c", 'O');
            else
                printw("%c", '-');
        }
        printw("\n");
    }
    refresh();
}

int move_grid(int grid[][WIDTH]) {
    int grid_copy[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int count = neighbours_count(grid, i, j);
            if (count == 3) {
                grid_copy[i][j] = 1;
            } else if (count == 2)
                grid_copy[i][j] = grid[i][j];
            else
                grid_copy[i][j] = 0;
        }
    }
    int count = 0;
    int flag = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid_copy[i][j] == 1) count++;
            if (grid[i][j] != grid_copy[i][j]) {
                flag = 1;
            }
        }
    }
    if (count == 0) {
        flag = 0;
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = grid_copy[i][j];
        }
    }
    return flag;
}

int neighbours_count(int grid[][WIDTH], int i_el, int j_el) {
    int count = 0;
    int pos_j1 = j_el - 1, pos_j2 = (j_el + 1) % WIDTH;

    if (pos_j1 < 0)
        pos_j1 = WIDTH + pos_j1;

    for (int i = i_el - 1; i <= i_el + 1; i++) {
        int pos_i = i % HEIGHT;
        if (pos_i < 0)
            pos_i = HEIGHT + pos_i;

        count += grid[pos_i][pos_j1] + grid[pos_i][pos_j2];
    }

    count += grid[(i_el + 1) % HEIGHT][j_el];

    int pos_i = i_el - 1;
    if (pos_i < 0)
        pos_i = HEIGHT + pos_i;
    count += grid[pos_i][j_el];

    return count;
}

void change_speed(int* speed, char action) {
    if (action == 'z' && *speed < 600000) {
        *speed = *speed * 1.5;
    } else if (action == 'x' && *speed > 120000) {
        *speed = *speed * 0.75;
    }
}