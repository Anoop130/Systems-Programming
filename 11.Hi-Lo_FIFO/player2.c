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

// error checker
int checkError(int val, const char *msg) {
    if (val == -1) {
        if (errno == EEXIST) return val;
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

// random number generator
int randomNum(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int main() {
    int guess, response, localMin, localMax;
    int ref_p2_fd, p2_ref_fd;

    srand(time(NULL) + 1);

    checkError(mkfifo("p2_to_ref", S_IRWXU), "error mkfifo p2_to_ref");
    checkError(mkfifo("ref_to_p2", S_IRWXU), "error mkfifo ref_to_p2");

    checkError(p2_ref_fd = open("p2_to_ref", O_WRONLY), "error open p2_to_ref");
    checkError(ref_p2_fd = open("ref_to_p2", O_RDONLY), "error open ref_to_p2");

    while (1) {
        localMin = MIN;
        localMax = MAX;

        while (1) {
            guess = randomNum(localMin, localMax);
            printf("Player 2 Guess: %d\n", guess);

            checkError(write(p2_ref_fd, &guess, sizeof(guess)), "error writing player2 guess");
            checkError(read(ref_p2_fd, &response, sizeof(response)), "error reading referee response");

            if (response == 0) {
                printf("Round Over.\n");
                break;
            } else if (response == 1) {
                localMin = guess + 1;
            } else {
                localMax = guess - 1;
            }
        }
    }

    close(ref_p2_fd);
    close(p2_ref_fd);
}
