#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

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


int main() {
    int guess, response, min, max;
    int ref_p1_fd, p1_ref_fd;

    checkError(mkfifo("p1_to_ref", S_IRWXU), "error mkfifo p1_to_ref");
    checkError(mkfifo("ref_to_p1", S_IRWXU), "error mkfifo ref_to_p1");

    checkError(p1_ref_fd = open("p1_to_ref", O_WRONLY), "error open p1_to_ref");
    checkError(ref_p1_fd = open("ref_to_p1", O_RDONLY), "error open ref_to_p1");

    while (1) {
        min = MIN;
        max = MAX;

        while (1) {
            guess = (min + max) / 2;
            printf("Player 1 Guess: %d\n", guess);

            checkError(write(p1_ref_fd, &guess, sizeof(guess)), "error writing player1 guess");
            checkError(read(ref_p1_fd, &response, sizeof(response)), "error reading referee response");

            if (response == 0) {
                printf("Round Over.\n");
                break;
            } else if (response == 1) {
                min = guess + 1;
            } else {
                max = guess - 1;
            }
        }
    }

    close(ref_p1_fd);
    close(p1_ref_fd);
}
