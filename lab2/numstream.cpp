#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <sys/wait.h>
using namespace std;

int PID;
int MAXIMUM_BUFFER=100;

void child(string message) {
    cout << "CHILD[" << PID << "]: " << message << endl;
}

void parent(string message) {
    cout << "PARENT[" << PID << "]: " << message << endl;
}


int main(void){

    int nbytes;
    int fd_parent_to_child[2];
    int fd_child_to_parent[2];

    // create pipes for inter-process communication
    if (pipe(fd_parent_to_child) || pipe(fd_child_to_parent)) {
        fprintf(stderr, "Pipe failed.\n"); 
        return EXIT_FAILURE;
    }

    pid_t pid = fork(); // used to distinguish parent from child
    PID = getpid(); // returns process id

    if (pid == 0) 
    {
        /* Child Process */ 
        child("Created.");
        child("Enter a signed byte value (-128 to 127).");

        signed int input,answer,i=0;
        signed char bytes[MAXIMUM_BUFFER];

        while (1) 
        {
            cin >> input;

            // check if input representable in a signed byte
            if (-128 <= input && input <= 127) {
                bytes[i] = input; i++; 
                
                if (input == -1) break;

            } else {
                child("Number not representable in a signed byte.");
            }
        }
        child("Ending input stream...");

        for (int j = 0; j < i; j++) {
            printf("CHILD[%d]: Sending: %hhx\n", PID, bytes[j]);
        }

        close(fd_child_to_parent[0]); // close read end of child_to_parent
        write(fd_child_to_parent[1], &bytes, i); // write to child_to_parent
        close(fd_child_to_parent[1]); // close write end of child_to_parent

        close(fd_parent_to_child[1]); // close the write end of parent_to_child
        read(fd_parent_to_child[0], &answer, sizeof(answer)); // read parent_to_child
        close(fd_parent_to_child[0]); // close the read end of parent_to_child

        printf("CHILD[%d]: Parent returned Sum: %d\n", PID, answer);
        child("Terminating...");
        exit(0);
    }
    else if (pid > 0) 
    {
        /* Parent Process */
        parent("Begins."); 
        parent("Awaiting bytes from child...");

        signed char bytes[MAXIMUM_BUFFER];
        signed int nbytes=0,sum=1;//offset -1

        close(fd_child_to_parent[1]); // close write end of child_to_parent
        nbytes = read(fd_child_to_parent[0], &bytes, sizeof(bytes));
        close(fd_child_to_parent[0]); // close read end of child_to_parent

        parent("Recieved...");
        for (int j = 0; j < nbytes; j++) {
            printf("%hhx ", bytes[j]);
            sum+=bytes[j];
        }

        printf("\nPARENT[%d]: Sending Sum: %d to CHILD[%d]...\n", PID, sum, pid);

        close(fd_parent_to_child[0]); // close read end of parent_to_child
        write(fd_parent_to_child[1], &sum, sizeof(sum)); // write to parent_to_child
        close(fd_parent_to_child[1]); // close write end of parent_to child
        
        printf("PARENT[%d]: Waiting for CHILD[%d] to terminate.\n", PID, pid);
        wait(NULL); // wait for child to terminate
        parent("Terminating...");
        exit(0);
    }
    else 
    {
        /* Fork failure */
        perror("Fork failed.");
        exit(0);
    }
}