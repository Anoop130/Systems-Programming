#include <stdio.h>
#include <stdlib.h>  
#include <unistd.h>   
#include <sys/wait.h>   
#include <string.h>     

// please create an executable for rand.c first
// the code uses execvp to run ./rand with the necessary cmd line arguments


// error checker
int checkError(int val, const char* msg) {
    if (val == -1) {
     perror(msg);
     exit(EXIT_FAILURE);
    }
    return val;
   }

// child function
int childFunction(int fd[2]){
    // printf("This is the child");
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);

    // running compiled rand.c
    char *args[] = {"./rand", "-c", "50", "-m", "200", "-M", "500", NULL};
    execvp("./rand", args);
    perror("Execvp failed");

    close(fd[1]);
    exit(EXIT_SUCCESS);

}

// parent function
int parentFunction(int fd[2]){
    // printf("This is the parent");
    close(fd[1]);

    int val;
    int counter = 0;
    int sum = 0;
    ssize_t bytesRead;

    // looping till EOF
    while (1) {
        bytesRead = read(fd[0], &val, sizeof(int));
        if (bytesRead <= 0) {
            if (bytesRead == 0)
                printf("Exiting Pipe\n");
            else
                perror("read failed");
            break;
        }
        printf("%d\n", val);
        sum += val;
        counter++;
    }

    // Outputs
    printf("Total values read: %d\n", counter);
    printf("Average of values read: %d\n", sum/counter);

    close(fd[0]);
    wait(NULL);
    exit(EXIT_SUCCESS);
}

int main(){
    int fd[2]; // read and write for pipe
    pid_t pid;

    // Creating pipe
    checkError(pipe(fd), "Error creating pipe");
    printf("Created pipes with read fd: %d and write fd: %d\n", fd[0], fd[1]);

    // forking
    pid = checkError(fork(), "Error forking");
    if (pid == 0){
        childFunction(fd);
    }

    if (pid > 0){
        parentFunction(fd);
    }

    exit(EXIT_SUCCESS);
}
