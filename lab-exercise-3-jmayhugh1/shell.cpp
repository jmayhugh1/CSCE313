/****************
LE2: Basic Shell
****************/

#include <unistd.h>      // pipe, fork, dup2, execvp, close
#include <sys/wait.h>    // wait
#include "Tokenizer.h"
#include <iostream>

using namespace std;

int main() {
    // TODO: insert lab exercise 2 main code here

    // Lists all the files in the root directory in the long format
    
    
    // Save original stdin and stdout
    int original_stdin = dup(0);
    int original_stdout = dup(1);

    // Input
    
    string input;
    while (true) {
        cout << "Provide commands: ";
        getline(cin, input);
        if (input == "exit") {
            break;
        }
        Tokenizer token(input);

        // For each command in token.commands
        // TODO: add functionality

        for (size_t i = 0; i < token.commands.size(); i++) {
            
            int fd[2];  // File descriptor identifies each end of the pipe
            pipe(fd);    // Returns a pair of file descriptors
            // Create child to run the first command
            pid_t pid1 = fork();
            if (pid1 == 0) { // Child
                // In child, redirect output to write end of the pipe
                if ( i < token.commands.size() - 1 ) {
                    dup2(fd[1], STDOUT_FILENO);
                }
                // Standard output (STDOUT_FILENO) is redirected to the write end of the pipe using dup2
                
                // Close the read end of the pipe on the child side
                close(fd[0]);
                //vector of args
                vector<char*> argc;
                for (auto const& arg : token.commands[i]->args) {
                    argc.push_back(const_cast<char*>(arg.c_str()));
                }
                argc.push_back(nullptr);
                // In child, execute the command
                execvp(argc[0], argc.data());
            } else { // Parent
                //redirect the shell(parents) input to the read end of the pipe
                dup2(fd[0], STDIN_FILENO);
                // Close the write end of the pipe on the parent side
                close(fd[1]);
                // Wait for the second child to finish
                if (i == token.commands.size() - 1) {
                    waitpid(pid1, nullptr, 0);
                }
            }

        }
                dup2(original_stdin, 0);
                dup2(original_stdout, 1);

    }
}
