#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define N 25
#define M 80
#define CLEANER printf("\033[H\033[J")

typedef struct termios interactive;


char** map_allocate();
void draw_map(char** map);
void load_map(char** a, char ch);


void game(char** map1, char** map2, char die, char life, char speed);
int check(char** map, int i, int j, char life);
void next_gen(char** map1, char** map2, char die, char life);
void mirror(char** map);
void term();


int main() {
    char **map1 = map_allocate();
    char **map2 = map_allocate();
    char die = '.';
    char life = '*';
    
    printf("Выберите карту 1-5 ;)\n");
    printf("Скорость регулируется +/-\nq/Q - Выход из игры\n");
    
    char speed;
    scanf("%c", &speed);
    
    game(map1, map2, die, life, speed);

    free(map1);
    free(map2);
    return 0;
}


char** map_allocate() {
    char** map = malloc(M * N * sizeof(char) + N * sizeof(char*));
    char* ptr = (char*) (map + N);
    
    for (int i = 0; i < N; i++) {
        map[i] = ptr + M * i;
    }
    
    return map;
}


void draw_map(char** map) {
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < M - 1; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}


void load_map(char** a, char ch) {
    FILE*file;
    if (ch == '1')
    file = fopen("map1.txt", "r+");
    else if (ch == '2')
    file = fopen("map2.txt", "r+");
    else if (ch == '3')
    file = fopen("map3.txt", "r+");
    else if (ch == '4')
    file = fopen("map4.txt", "r+");
    else if (ch == '5')
    file = fopen("map5.txt", "r+");

    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < M - 1; j++) {
            a[i][j] = fgetc(file);
            if (a[i][j] == '\n')
            a[i][j] = fgetc(file);}
    }
}


void game(char** map1, char** map2, char die, char life, char speed) {
    term();
    
    int time_delay = 100000; 
    char user_input = speed;

    while (user_input != 'q') {
        CLEANER;
        
        read(0, &user_input, 1);
        usleep(time_delay);

        
        if (user_input == '+') {
            time_delay = time_delay * 0.5;
            if (time_delay < 6250) time_delay = 6250;
        }
        if (user_input == '-') {
            time_delay = time_delay * 1.5;
            if (time_delay > 506250) time_delay = 506250;
        }

        
        if (user_input >= '1' && user_input <= '5') {
            load_map(map1, user_input);
        }

        
        if (user_input == 'q') {
            break;
        }

        
        mirror(map1);
        next_gen(map1, map2, die, life);

        
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                map1[i][j] = map2[i][j];
            }
        }

        
        draw_map(map1);
    }
}


int check(char** map, int i, int j, char life) {
    int count = 0;
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue;
            if (map[i + di][j + dj] == life) {
                count++;
            }
        }
    }
    return count;
}


void next_gen(char** map1, char** map2, char die, char life) {
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < M - 1; j++) {
            int life_neighbors = check(map1, i, j, life);
            if (life_neighbors < 2 || life_neighbors > 3) {
                map2[i][j] = die;
            } else if (life_neighbors == 2) {
                map2[i][j] = map1[i][j];
            } else if (life_neighbors == 3) {
                map2[i][j] = life;
            }
        }
    }
}


void mirror(char** map) {
    for (int j = 1; j < M - 1; j++) {
        map[N - 1][j] = map[1][j];
        map[0][j] = map[N - 2][j];
    }

    for (int i = 0; i < N; i++) {
        map[i][0] = map[i][M - 2];
        map[i][M - 1] = map[i][1];
    }
}


void term() {
    interactive term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ECHO | ICANON);
    term.c_cc[VTIME] = 0;
    term.c_cc[VMIN] = 0;
    tcsetattr(0, TCSAFLUSH, &term);
}
