#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define MIN 1
#define MAX 100

int target;

// error checkker
int checkError(int val, const char *msg) {
    if (val == -1) {
        if (errno == EEXIST) return val;
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

int randomNum(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int printMsg(const char *str) {
    return write(STDOUT_FILENO, str, strlen(str));
}

int checkGuess(int guess) {
    if (guess == target) return 0;
    else if (guess < target) return 1;
    else return -1;
}

void delay(int n) {
    struct timespec request = {.tv_sec = n, .tv_nsec = 0}, remain;
    while (nanosleep(&request, &remain) == -1 && errno == EINTR) {
        request = remain;
    }
}

int main() {
    int guess1, guess2, result1, result2, score1 = 0, score2 = 0;
    int ref_p1_fd, ref_p2_fd, p1_ref_fd, p2_ref_fd;

    srand(time(NULL));

    checkError(mkfifo("ref_to_p1", S_IRWXU), "error mkfifo ref_to_p1");
    checkError(mkfifo("ref_to_p2", S_IRWXU), "error mkfifo ref_to_p2");
    checkError(mkfifo("p1_to_ref", S_IRWXU), "error mkfifo p1_to_ref");
    checkError(mkfifo("p2_to_ref", S_IRWXU), "error mkfifo p2_to_ref");

    checkError(p1_ref_fd = open("p1_to_ref", O_RDONLY), "error open p1_to_ref");
    checkError(p2_ref_fd = open("p2_to_ref", O_RDONLY), "error open p2_to_ref");
    checkError(ref_p1_fd = open("ref_to_p1", O_WRONLY), "error open ref_to_p1");
    checkError(ref_p2_fd = open("ref_to_p2", O_WRONLY), "error open ref_to_p2");

    printMsg("\nNUMBER GUESSING GAME!\n");

    for (int i = 0; i < 10; i++) {
        printf("\n\nROUND %d\n", i + 1);
        printf("Current Scores: Player 1: %d, Player 2: %d\n", score1, score2);

        target = randomNum(MIN, MAX);
        printf("\nTarget number: %d\n", target);

        while (1) {
            delay(1);

            checkError(read(p1_ref_fd, &guess1, sizeof(int)), "error reading player1 guess");
            checkError(read(p2_ref_fd, &guess2, sizeof(int)), "error reading player2 guess");

            printf("\nPlayer 1's guess: %d\n", guess1);
            printf("Player 2's guess: %d\n", guess2);

            result1 = checkGuess(guess1);
            result2 = checkGuess(guess2);

            if (result1 == 0 || result2 == 0) {
                result1 = result2 = 0;
            }

            checkError(write(ref_p1_fd, &result1, sizeof(int)), "wrror writing player1 guess");
            checkError(write(ref_p2_fd, &result2, sizeof(int)), "error writing player2 guess");

            if (result1 == 0 && result2 == 0) {
                if (guess1 == target) {
                    score1++;
                    printMsg("\nPlayer 1 wins the round!\n");
                }
                if (guess2 == target) {
                    score2++;
                    printMsg("\nPlayer 2 wins the round!\n");
                }
                break;
            }
        }
    }

    close(ref_p1_fd);
    close(ref_p2_fd);
    close(p1_ref_fd);
    close(p2_ref_fd);

    printMsg("\nGAME OVER!\n\n");
    printf("Final Scores: Player 1: %d, Player 2: %d\n", score1, score2);

    unlink("ref_to_p1");
    unlink("ref_to_p2");
    unlink("p1_to_ref");
    unlink("p2_to_ref");
}
