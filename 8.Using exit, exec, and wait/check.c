#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
// check whether myRand wrote values correctly
// Function to check for errors
int checkError(int val, const char *msg) {
    if (val == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

int main() {
    int fd, num;
    
    // Open the file for reading
    fd = checkError(open("data.dat", O_RDONLY), "Error opening file");

    printf("Reading numbers from data.dat:\n");

    // Read integers one by one and print them
    while (read(fd, &num, sizeof(int)) == sizeof(int)) {
        printf("%d ", num);
    }

    printf("\n");
    
    // Close the file
    close(fd);

    return 0;
}
