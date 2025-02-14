#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define N 27
#define M 82
#define CLEANER printf("\033[H\033[J")

typedef struct termios interactive;

char** mapaloc(char** a);
void draw(char** a);
void scanDrow(char** a, char ch);
void game(char** map1, char** map2, char die, char life, char changer);
int check(char** map1, int i, int j, char life);
void mirror(char** a);
void check1(char** map1, char** map2, char die, char life);
void term();

int main() {
    char **map1 = NULL, **map2 = NULL;
    char die = '.';
    char life = '*';
    map1 = mapaloc(map1);
    map2 = mapaloc(map2);
    printf("Выбирайте с помощью цифр 1 2 3 4 5 карту и наслаждайтесь игрой!!!\n");
    printf("Регулировка скорости +/-\n q/Q - Выход из игры\n");
    char changer;
    scanf("%c", &changer);
    game(map1, map2, die, life, changer);
    free(map1);
    free(map2);
    return 0;
}

char** mapaloc(char** a) {
    a = malloc(M * N * sizeof(char) + N * sizeof(char*));
    char* ptr = (char*) (a + N);

    for (int i = 0; i < N; i++)
    a[i] = ptr + M * i;
    return a;
}

void draw(char** a) {
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < M - 1; j++) {
            printf("%c", a[i][j]);
        }
        printf("\n");
    }
}

void game(char** map1, char** map2, char die, char life, char changer) {
    term();
    int time = 100000;
    char step = changer;
while (step != 'q') {
        CLEANER;
        read(0, &step, 1);
        usleep(time);
        if (step == '+') {
        time = time * 0.5;
        if (time < 6250)
        time = 6250;}
        if (step == '-') {
            time = time * 1.5;
            if (time > 506250)
            time = 506250;}
        if (step == '1' || step == '2' || step == '3' || step == '4' || step == '5')
        scanDrow(map1, step);
        if (step == 'q')
        break;
        step = ' ';
        mirror(map1);
        check1(map1, map2, die, life);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j <  M; j++)
                map1[i][j] = map2[i][j];}
        draw(map1);}
}

void check1(char** map1, char** map2, char die, char life) {
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j <  M - 1; j++) {
            int checker = check(map1, i, j, life);
            if ( checker < 2)
            map2[i][j] = die;
            if (checker == 2)
            map2[i][j] = map1[i][j];
            if (checker == 3)
            map2[i][j] = life;
            if (checker > 3)
            map2[i][j] = die;}
    }
}

int check(char** map1, int i, int j, char life) {
    int counter = 0;
    if (map1[i - 1][j - 1] == life)
    counter++;

    if (map1[i - 1][j] == life)
    counter++;

    if (map1[i - 1][j + 1] == life)
    counter++;

    if (map1[i][j - 1] == life)
    counter++;

    if (map1[i][j + 1] == life)
    counter++;

    if (map1[i + 1][j - 1] == life)
    counter++;

    if (map1[i + 1][j] == life)
    counter++;

    if (map1[i + 1][j + 1] == life)
    counter++;

    return counter;
}

void mirror(char** a) {
    for (int j = 1; j < M - 1; j++) {
        a[N - 1][j] = a[1][j];
        a[0][j] = a[N - 2][j];}

    for (int i = 0; i < N; i++) {
        a[i][0] = a[i][M - 2];
        a[i][M - 1] = a[i][1];}
}

void term() {
    interactive ter;
    tcgetattr(0, &ter);
    ter.c_lflag &= ~(ECHO | ICANON);
    ter.c_cc[VTIME] = 0;
    ter.c_cc[VMIN] = 0;
    tcsetattr(0, TCSAFLUSH, &ter);
}

void scanDrow(char** a, char ch) {
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
