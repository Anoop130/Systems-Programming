/* Write a multiple concurrent process program that does the following
   1. Displays "ascii modern art" composed of a series of 
      randomly generated blocks of 2 or more repeating characters

   The program should perform this task in the following way:
   1. Create a semaphore set of size 2 -- use IPC_PRIVATE as your key
      a. Initialize the semaphore representing the child to available
      b. Initialize the semaphore representing the parent to in use
   2. Create a segment of shared memory -- use IPC_PRIVATE as your key

   3. Create a child process that:
      a. Attaches the segment of shared memory
      b. Seeds the random number generator 
      c. Reserves the child semaphore
      d. Generates and stores the total number of blocks to generate. 
         This should be between 10 and 20
      c. For each block, generate and store the following values:
         1. The length of the block (i.e. the number of characters to display)
            This should be between 2 and 10
         2. The character that comprises the block.
            This should be between 'a' and 'z'
      d. Release the parent semaphore
      e. Reserve the child semaphore
      f. Detach the shared memory segment
      g. Release the parent semaphore

   4. Create a parent process that:
      a. Attaches the segment of shared memory
      b. Seeds the random number generator
      c. Reserve the parent semaphore 
      d. Generate a random width for the ASCII art. 
         This should be between 10 and 15
      e. Using the data stored in the shared memory segment, output an image
         Use the following tips: 
         1. One value stored in the segment should represent the number
            of (length, character) pairings. For each (length, character)
            pairing, you should output length instances of the given 
            character. This means if the pairing was (3, 'b'), you would
            output "bbb".
         2. The random image has basically been encoded use run-length
            encoding (RLE); RLE doesn't including the location of new lines.
            The location of new lines is determined by the random width 
            generated in step d. Each time you output width total characters,
            output a new line.
      f. Release the child semaphore
      g. Reserve the parent semaphore
      h. Detach the shared memory segment

   5. Delete the semaphore
   6. Delete the shared memory
   
   -- You can use/should use the binary semaphore protocol introduced in class
 */

/* Problem 3 -- List the inlcude files you need for this program. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include "semun.h"
#include "binary_sem.h"


// error checker
int checkError(int val, const char* msg) {
   if (val == -1) {
    perror(msg);
    exit(EXIT_FAILURE);
   }
   return val;
  }


/* Problem 4 -- remember to declare a structure that represents the data
   stored in the shared memory */

#define MAX_BLOCKS 20
typedef struct {
   int num_blocks;
   int len_block[MAX_BLOCKS];
   char chars[MAX_BLOCKS];
} sharedData;
   

/* Problem 5 -- create a function to handle the code for the child. 
   Be certain to pass this function the id values for both the semaphore 
   and the shared memory segment */

void child_process(int semid, int shmid) {
    sharedData* data = (sharedData*)shmat(shmid, NULL, 0);

    if (data == (void*)-1) {
        perror("child shmat failed");
        exit(1);
    }

    srand(time(NULL) ^ getpid());
    reserveSem(semid, 0); // Reserve child

    data->num_blocks = (rand() % 11) + 10; // 10 to 20
    
    for (int i = 0; i < data->num_blocks; i++) {
        data->len_block[i] = (rand() % 9) + 2; // 2 to 10
        data->chars[i] = 'a' + (rand() % 26);
    }

    releaseSem(semid, 1); // Release parent
    reserveSem(semid, 0); // Wait for parent again

    shmdt(data);
    releaseSem(semid, 1); // Final release for parent
}




/* Problem 6 -- create a function to handle the code for the parent. 
   Be certain to pass this function the id values for both the semaphore 
   and the shared memory segment */


void parent_process(int semid, int shmid) {
    sharedData* data = (sharedData*)shmat(shmid, NULL, 0);
    if (data == (void*)-1) {
        perror("parent shmat failed");
        exit(1);
    }

    srand(time(NULL) ^ getpid());
    reserveSem(semid, 1); // Wait for child

    int width = (rand() % 6) + 10; // 10 to 15
    int count = 0;

    printf("\n--- ASCII MODERN ART ---\n\n");
    for (int i = 0; i < data->num_blocks; i++) {
        for (int j = 0; j < data->len_block[i]; j++) {
            putchar(data->chars[i]);
            count++;
            if (count % width == 0) putchar('\n');
        }
    }
    if (count % width != 0) putchar('\n');

    releaseSem(semid, 0); // Signal child to cleanup
    reserveSem(semid, 1); // Wait for child to finish

    shmdt(data);
}



/* Problem 7 -- implement function main */

int main(int argc, char *argv[])
{

   int semid = checkError(semget(IPC_PRIVATE, 2, IPC_CREAT | 0666), "semget failed");

   // Initialize child semaphore (index 0) to available (1)
   if (initSemAvailable(semid, 0) == -1) {
       perror("Failed to initialize child semaphore");
       exit(EXIT_FAILURE);
   }
   
   // Initialize parent semaphore (index 1) to in-use (0)
   if (initSemInUse(semid, 1) == -1) {
      perror("Failed to initialize parent semaphore");
       exit(EXIT_FAILURE);
   }
   

   int shmid = checkError(shmget(IPC_PRIVATE, sizeof(sharedData), IPC_CREAT | 0666), "shmget failed");
   pid_t pid = checkError(fork(), "fork faield");

   if (pid == 0) {
      child_process(semid, shmid); 
      exit(0);
   } else {
      parent_process(semid, shmid);
      wait(NULL); // Wait for child
   }


   if (semctl(semid, 0, IPC_RMID) == -1) {
      perror("semctl(IPC_RMID) failed");
   }

   if (shmctl(shmid, IPC_RMID, NULL) == -1) {
      perror("shmctl(IPC_RMID) failed");
   }
  
}
