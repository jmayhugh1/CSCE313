/****************
LE2: Introduction to Unnamed Pipes
****************/
#include <unistd.h> // pipe, fork, dup2, execvp, close
using namespace std;

int main () {
    // lists all the files in the root directory in the long format
    char* cmd1[] = {(char*) "ls", (char*) "-al", (char*) "/", nullptr};
    // translates all input from lowercase to uppercase
    char* cmd2[] = {(char*) "tr", (char*) "a-z", (char*) "A-Z", nullptr};
    //save original stdin and stdout
    int original_stdin = dup(0);
    int original_stdout = dup(1);
    // TODO: add functionality
    // Create pipe
    int fd[2]; //file descriptor identifies each end of the pipe
    pipe(fd); //returns pair of file descriptors
    // STDIN_FILENO == 0
    // STDOUT_FILENO  == 1
    // Create child to run first command
    pid_t pid1 = fork();
    if (pid1 == 0){ //child
        // In child, redirect output to write end of pipe
        dup2(fd[1],STDOUT_FILENO); //standard output (STDOUT_FILENO) is redirected to the write end of the pipe using dup2
        
        // Close the read end of the pipe on the child side.
        close(fd[0]);
        // In child, execute the command;
        execvp(cmd1[0], cmd1);
    }
    else { //parent
        // Create another child to run second command
        pid1 = fork();
        if (pid1 == 0){ //child

        // In child, redirect input to the read end of the pipe
        dup2(fd[0], STDIN_FILENO);
        // Close the write end of the pipe on the child side.
        close(fd[1]);
        // Execute the second command.
        execvp(cmd2[0], cmd2);
    }
    else { //parent
    
        // Reset the input and output file descriptors of the parent.
        dup2(original_stdin, STDIN_FILENO);
        dup2(original_stdout, STDOUT_FILENO);

        //overrite in/out with whaat was saved
        close(fd[0]);
        close(fd[1]);
    }
    }
}
