The pipe is one of a number of IPC techniques available in Unix systems programming.
A pipe is a unidirectional data channel that connects two processes created in a parent-child relationship.

For this program we will be using a pipe to capture a series of integers printed to the console from  one 
program and computing their sum and average with another program. The data will be delivered via a pipe.

The basic outline for this program is as follow ... 

Create a pipe
Call fork
If you are in the child, call the function child
If you are in the parent, call the function parent
Exit the program
The child should do the following ...

Close the read side of the pipe
Dup the write side of the pipe onto standard output
Run "rand -c 50 -m 200 -M 500" using exec

The parent should do the following ...

Close the write side of the pipe
Loop forever
Read an integer from the pipe
If the read returns a 0 or a -1, break the loop
Print the value read to standard out
Add the value read to the sum
Increment the number of values read by 1
Print the total of the values read
Print the average of the values read
 

I have included the .c file for the rand program. 

rand.cDownload rand.c

Compile it as follows:

gcc rand.c -o rand
