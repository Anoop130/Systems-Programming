Each program is simulating a guessing game between a referee and two players. The game will work as follows:

    Each player tells the referee they are ready to play
    The referee chooses a random number between 1 and 100 (inclusive)
    Tell the players to make their guess.
    Each player hands the referee their guess (this information isn’t shared between players).
    The referee then tells each player whether the guess is less then, greater than, or equal to the referee’s number. If no player has guessed the correct number, repeat steps 3 – 5.

Each player uses a different strategy to make their guess; each strategy has the following steps in common:

    Player 1 and Player 2 both maintain a lower bound and an upper bound for their guesses. The lower bound starts at 0; the upper bound starts at 100.
    The players make their guess based on these boundaries.
    Each player will then adjust their bounds based on the information provided by the referee.
        A player will set the lower bound to the current guess if that guess was lower than the referee’s value.
        A player will set the upper bound to the current guess if that guess was larger than the referee’s value.
        Both players reset the upper and lower bounds if one or both players guess correctly.

Each player’s step 2 is different. Player 1 always chooses the average of the lower and upper bounds as the next guess; Player 2 always chooses a random number between the upper and lower bounds as the next guess.
Each program will play the game 10 times.
What follows are the details I feel are necessary to help you complete each option.
You will need some global variables to save the shared data and manage the state of the game. I used the following global variables:

    guess[] stores each thread’s guess
    cmp[] stores the referee’s response to each thread
        < 0 means guess < target
        0 means guess = target
        > 0 means guess > target
    rdy[] stores the various flags need for the condition variables
        0 means the condition variable hasn't been signaled
        1 means the condition variable has been signaled
    mtx[] stores the mutexes
    cnd[] stores the condition variables

Thread 1 handles Player 1’s behavior; Thread 2 handles Player 2’s behavior. Both threads share a common structure.

    Loop forever
        Set min to 0 and max to 100
        Lock mutex 3
        Using a while loop, wait on condition variable 3.
        In my code base this was implemented as a while(!rdy) cond_wait() loop. The following loop is for thread 1
        while (rdy[2] == 0) pthread_cond_wait(&cnd[2], &mtx[2]);
        (note: both players are waiting on the cnd[2], mtx[2] pairing)
        Unlock mutex 3 and set the rdy flag you were looping over to 0
        Loop forever
            Calculate the child’s guess using the proper method (See below)
            Store that guess in the proper global variable.
            Lock the mutex associated with the thread’s guess (See below)
            Using a while loop, wait on the associated condition variable (See below)
            Unlock the proper mutex (See below)
            Once this condition variable has been signaled, the value of the referee’s response should be stored in the cmp global. Using the proper element of the cmp global do the following:
                If cmp < 0, set min to the current guess
                If cmp > 0, set max to the current guess
                If cmp is 0, break.

I suggest using the following values for Thread 1:

    use mtx[0] as the thread's mutex for its guess
    use cnd[0] and rdy[0] as your condition variable and associated state
    use guess[0] and cmp[0] to store thread 1's guess and the referee's response

Thread 1 computes its guess by taking the average of min and max
I suggest using the following values for Thread 2:

    use mtx[1] as the thread's mutex for its guess
    use cnd[1] and rdy[1] as your condition variable and associated state
    use guess[1] and cmp[1] to store thread 2's guess and the referee's response

Thread 2 computes its guess by generating a random number between min and max.
Remember to seed the random number generator when Thread 2 starts; you might also need to generate a few initial random numbers so that Thread 2 isn’t always generating the same random numbers as the referee.
The Referee thread should follow the outline below:

    Loop from 0 to 10
        Lock mutex 3
        Generate the target for this game. Recall this is a random number between 1 and 100 (inclusive).
        Set the two rdy flags associated with condition variable 3 to 1. I suggest using rdy[2] and rdy[3].
        Broadcast condition variable 3 and unlock mutex 3
        Print the game statistics (which game you are in, the current scores, etc)
        Loop forever
            Sleep for 1 second
            Lock mutexes 1 and 2
            Set the rdy flags for each mutex-condition variable pairing to 0.
            I suggest using rdy[0] for mutex1-condition variable 1 and rdy[1] for mutex2-condition variable 2.
            Set each cmp[0] to guess[0] - target and cmp[1] to guess[1] - target
            Set both rdy[0] and rdy[1] to 1.
            Broadcast condition variables 1 and 2 and unlock mutexes 1 and 2
            If either thread made a correct guess, break. A thread made a correct guess if cmp is 0. Make certain to set both cmp's to 0 in this case.
    Print the final game statistics

The main function should follow the outline below:

    Seed the random number generator
    Set the rdy flags to 0
    Spawn Threads 1, 2, and the Referee.
    Join the Referee Thread

