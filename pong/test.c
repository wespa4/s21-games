#include <stdio.h>

// Функция отрисовки игрового поля
void drawField(int w, int h, int ball_x, int ball_y, int ri_1, int rstart_1, int ri_2, int rstart_2, int scring_x, int scring_y, int player1, int player2) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if ((x == ri_1 && (y == rstart_1 - 1 || y == rstart_1 || y == rstart_1 + 1)) || 
                (x == ri_2 && (y == rstart_2 - 1 || y == rstart_2 || y == rstart_2 + 1))) {
                printf("|");  // Отрисовка ракеток
            } else if (x == ball_x && y == ball_y) {
                printf("o");  // Отрисовка мяча
            } else if ((y == 0 && x != 0 && x != w) || (y == h - 1 && x != 0 && x != w - 1)) {
                printf("-");  // Отрисовка верхней и нижней границ
            } else if ((x == 0 && y != 0 && y != h) || 
                       (x == w - 1 && y != 0 && y != h - 1 && y != 3) || 
                       (x == w / 2 && y != 3) || 
                       (x == w - 3 && y == 3)) {
                printf("|");  // Отрисовка боковых границ
            } else if (x == w / 2 - 1 && y == 3) {
                printf("|");  // Отрисовка центра
            } else if (x == scring_x && y == scring_y) {
                printf("%02d", player1);  // Очки первого игрока
            } else if (x == (w - scring_x - 2) && y == scring_y) {
                printf("%02d", player2);  // Очки второго игрока
            } else {
                printf(" ");  // Заполнение пробелами
            }
        }
        printf("\n");
    }
}

int main() {
    int w = 80;
    int h = 25;
    int ball_start_x = w / 2;
    int ball_start_y = h / 2;
    int ball_y = ball_start_y;
    int ball_x = ball_start_x;
    int scring_x = 33;
    int scring_y = 3;
    int ri_1 = 1;
    int ri_2 = w - 2;  // положение ракетки по оси x
    int rstart_1 = 12;
    int rstart_2 = 12;  // положение центра ракетки по оси y
    int vector_x = -1;
    int vector_y = 1;
    int win = 21;
    int player1 = 0;
    int player2 = 0;
    char key;

    while (1) {
        drawField(w, h, ball_x, ball_y, ri_1, rstart_1, ri_2, rstart_2, scring_x, scring_y, player1, player2);

        // Логика движения мяча
        if (ball_y == 1 || ball_y == h - 2) {
            vector_y = -vector_y;  // смена траектории по оси Y при столкновении с верхней или нижней гранью
        }
        if (ball_x == ri_1 + 1 && (ball_y == rstart_1 - 1 || ball_y == rstart_1 || ball_y == rstart_1 + 1)) {
            vector_x = -vector_x;  // отражение от левой ракетки
        }
        if (ball_x == ri_2 - 1 && (ball_y == rstart_2 - 1 || ball_y == rstart_2 || ball_y == rstart_2 + 1)) {
            vector_x = -vector_x;  // отражение от правой ракетки
        }
        ball_y += vector_y;  // смещение по оси Y
        ball_x += vector_x;  // смещение по оси X

        // Обработка очков
        if (ball_x < 2) {  // Мяч ушел за левую границу
            player2++;
            ball_x = ball_start_x;  // Сброс мяча в центр
            ball_y = ball_start_y;
        }
        if (ball_x > w - 1) {  // Мяч ушел за правую границу
            player1++;
            ball_x = ball_start_x;  // Сброс мяча в центр
            ball_y = ball_start_y;
        }
        if (player1 == win) {
            printf("Победил первый игрок\n");
            break;
        } else if (player2 == win) {
            printf("Победил второй игрок\n");
            break;
        }

        // Логика движения ракеток
        key = getchar();
        if (key == 'a') {
            if (rstart_1 - 2 != 0)
                rstart_1--;
        }
        if (key == 'z') {
            if (rstart_1 + 2 != h - 1)
                rstart_1++;
        }
        if (key == 'k') {
            if (rstart_2 - 2 != 0)
                rstart_2--;
        }
        if (key == 'm') {
            if (rstart_2 + 2 != h - 1)
                rstart_2++;
        }
        if (key == 'q') {
            return -1;
        }

        printf("\33[0d\33[2J");
    }

    return 0;
}