#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80
#define SPEED 300001

int init_grid_from_file(int grid[][WIDTH]);
void render_canvas(int grid[][WIDTH]);
int move_grid(int grid[][WIDTH]);
int neighbours_count(int grid[][WIDTH], int i_el, int j_el);
void change_speed(int *speed, char action);
void print_start_interface();
void print_end_interface(int flag);
int game(int grid[][WIDTH], int *speed);

int main() {
    initscr();
    noecho();
    raw();
    curs_set(0);

    int speed = SPEED;
    int grid[HEIGHT][WIDTH];

    int error = 0;

    error = init_grid_from_file(grid);
    if (error == 1) {
        printw("An error occured while opening a terminal.\n");
        refresh();
    } else {
        char action;
        while (1) {
            print_start_interface();
            action = getch();
            if (action == 'Q' || action == 'q' || action == 'S' || action == 's') break;
        }

        if (action == 'S' || action == 's') {
            int end;
            end = game(grid, &speed);
            nodelay(stdscr, FALSE);
            print_end_interface(end);
            getch();
        }
    }

    endwin();

    return 0;
}

void print_start_interface() {
    int start_row = HEIGHT / 3;
    int start_col = WIDTH / 5;

    mvprintw(start_row++, start_col, " GGG   AAAAA  M     M  EEEEE");
    mvprintw(start_row++, start_col, "G      A   A  MM   MM  E    ");
    mvprintw(start_row++, start_col, "G  GG  AAAAA  M M M M  EEEE ");
    mvprintw(start_row++, start_col, "G   G  A   A  M  M  M  E    ");
    mvprintw(start_row++, start_col, " GGG   A   A  M     M  EEEEE");

    start_col = WIDTH / 10;
    mvprintw(start_row++, start_col, " OOO   FFFFF    L       III   FFFFF  EEEEE");
    mvprintw(start_row++, start_col, "O   O  F        L        I    F      E    ");
    mvprintw(start_row++, start_col, "O   O  FFFF     L        I    FFFF   EEEE ");
    mvprintw(start_row++, start_col, "O   O  F        L        I    F      E    ");
    mvprintw(start_row++, start_col, " OOO   F        LLLLL   III   F      EEEEE");

    start_row += 4;
    mvprintw(start_row, start_col, "START (s)");
    start_col += 33;
    mvprintw(start_row, start_col, "QUIT (q)");

    refresh();
}

void print_end_interface(int end_reason) {
    clear();
    int start_row = HEIGHT / 3;
    int start_col = WIDTH / 5;

    mvprintw(start_row++, start_col, " GGG   AAAAA  M     M  EEEEE");
    mvprintw(start_row++, start_col, "G      A   A  MM   MM  E    ");
    mvprintw(start_row++, start_col, "G  GG  AAAAA  M M M M  EEEE ");
    mvprintw(start_row++, start_col, "G   G  A   A  M  M  M  E    ");
    mvprintw(start_row++, start_col, " GGG   A   A  M     M  EEEEE");

    mvprintw(start_row++, start_col, " OOO  V       V  EEEEE  RRRR");
    mvprintw(start_row++, start_col, "O   O  V     V   E      R  R");
    mvprintw(start_row++, start_col, "O   O   V   V    EEEE   RRRR");
    mvprintw(start_row++, start_col, "O   O    V V     E      R R ");
    mvprintw(start_row++, start_col, " OOO      V      EEEEE  R  R");

    start_row += 4;

    if (end_reason == -1) {
        mvprintw(start_row, start_col, "YOU ENDED THE GAME");
    } else if (end_reason == 2) {
        mvprintw(start_row, start_col, "THE POPULATION WILL NOT DEVELOP ANYMORE");
    } else {
        mvprintw(start_row, start_col, "THE POPULATION DIED");
    }

    refresh();
}

int game(int grid[][WIDTH], int *speed) {
    nodelay(stdscr, TRUE);

    int end_reason;

    while (1) {
        render_canvas(grid);

        char action = getch();
        if (action != ERR) change_speed(speed, action);

        int flag = move_grid(grid);
        if (flag != 0 || action == 'q') {
            end_reason = flag != 0 ? flag : -1;
            usleep(SPEED * 5);
            break;
        }

        usleep(*speed);
    }

    return end_reason;
}

int init_grid_from_file(int grid[][WIDTH]) {
    int error = 0;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int num;
            scanf("%d", &num);
            grid[i][j] = num;
        }
    }

    if (!freopen("/dev/tty", "rw", stdin)) error = 1;

    return error;
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

    printw("%s - slower\n%s - faster\n%s - quit\n", "z", "x", "q");

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

    int count = 0, flag = 2;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid_copy[i][j] == 1) count++;
            if (grid[i][j] != grid_copy[i][j]) flag = 0;
        }
    }

    if (count == 0) flag = 1;

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

    if (pos_j1 < 0) pos_j1 = WIDTH + pos_j1;

    for (int i = i_el - 1; i <= i_el + 1; i++) {
        int pos_i = i % HEIGHT;
        if (pos_i < 0) pos_i = HEIGHT + pos_i;

        count += grid[pos_i][pos_j1] + grid[pos_i][pos_j2];
    }

    count += grid[(i_el + 1) % HEIGHT][j_el];

    int pos_i = i_el - 1;
    if (pos_i < 0) pos_i = HEIGHT + pos_i;
    count += grid[pos_i][j_el];

    return count;
}

void change_speed(int *speed, char action) {
    if (action == 'z' && *speed < 600000) {
        *speed = *speed * 1.5;
    } else if (action == 'x' && *speed > 120000) {
        *speed = *speed * 0.75;
    }
}