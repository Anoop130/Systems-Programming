#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

// COMPILE THIS AS ./myRand binary

// Function to check for errors
int checkError(int val, const char *msg)
{
    if (val == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}


int randNum(int min, int max) {
    return rand() % (max - min + 1) + min;

}


int main(){
    char filename[16];
    int X = randNum(0,255);
    sprintf(filename, "data%d.dat", X);

    srand(time(NULL));  // seeding with current time

    // opening file
    int fd = checkError(open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644), "Error opening file");


    for (int i = 0; i < 60; i++){
        int num = randNum(0,100);
        checkError(write(fd, &num, sizeof(int)), "Error writing to file"); // writing to file
    }

    close(fd);

    exit(X);

}