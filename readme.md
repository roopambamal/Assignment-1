### CrackedMe Solution
This is the solution to Assignment 1 in INF-3201 by Jonathan L. Claudius
The assignment is about implementing a solution to CrackMe, and add a parallel running component using MPI.
MPI allows a program to synchronize using messages in a multi-process environment.
A process can be a program running on a different processor core locally or remote.

## Install & Run
Make sure that normal C compile tools (gcc) and MPI is installed to your system.

To compile a single-thread local program, navigate to the code directory. Run the command without quotations:
"gcc main-seq.c crackme.o -o main"

Should optimization be a desire, add the "-O3" command.

The program can then be runned by typing "./main" without quotations.
By default, the program will use a string length of 2 and an empty string to start.
To use a longer string length run main with a number like: "./main 5".

For MPI compilation, use mpicc. The command will be:
"mpicc main.c crackme.o -o mainMPI"
Again, optimization can be added with "-O3"

To run MPI manually, use mpirun. On the cluster, use the given run.sh and follow the instructions on how to use that.
To use run.sh, use the command "sh run.sh".

## Credits
* StackOverflow
* Wikipedia
* Google
* Etc.