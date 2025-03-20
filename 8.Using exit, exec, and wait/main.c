#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

// function to check for errors
int checkError(int val, const char *msg) {
    if (val == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

// child process function
void child(void) {
    char *path = "./myRand";  // matches this with compiled myRand binary
    char *args[] = {"myRand", NULL};
    execv(path, args);

    perror("Error during execv");
    exit(EXIT_FAILURE);
}

int main() {

    int status;
    char filename[16];
    int numbers[60];

    // fork
    pid_t pid = checkError(fork(), "Error during fork");

    // child process
    if (pid == 0) {  
        child();
        exit(EXIT_SUCCESS);
    }

    // parent process waiting for child 
    checkError(wait(&status), "Error during wait");


    int X = WEXITSTATUS(status); // converting status to actual value
    sprintf(filename, "data%d.dat", X);
    printf("Opening file: %s\n", filename);

    // openin file for reading
    int fd = checkError(open(filename, O_RDONLY), "Error while opening file");

    // Read 60 integers from file
    checkError(read(fd, numbers, sizeof(numbers)), "Error reading file");
    checkError(close(fd), "Error closing file");

    // displaying numbers
    int sum = 0;
    printf("Numbers:\n");
    for (int i = 0; i < 60; i++) {
        printf("%d ", numbers[i]);
        sum += numbers[i];
    }
    printf("\n");

    // average
    double avg = (double)sum / 60;
    printf("\nAverage: %.2f\n", avg);

    // unlink data file
    checkError(unlink(filename), "Error during unlinking");

    exit(EXIT_SUCCESS);
}
