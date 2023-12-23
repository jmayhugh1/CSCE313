#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#define MAX_MESSAGE 256

long long unsigned int hash(int seed, char* buf, int nbytes) {
    long long unsigned int H = seed; 
    for (int i = 0; i < nbytes; ++i) 
        H = H * 2 * buf[i] + 1;
    return H;
}

int main(int argc, char** argv) {
	if (argc != 2) {
        return 1;
    }

	// TODO: create pipe 
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }




    int pid = fork();
    
    
    if (pid == 0) {
        // TODO: read from parent
        char buf[MAX_MESSAGE];
       
        int bytes = read(pipefd[0], buf, MAX_MESSAGE);
        close(pipefd[0]);
        // TODO: compute hash 
        long long unsigned int h = hash(getpid(), buf, bytes);


        // TODO: send hash to parent 
        
        bytes = write(pipefd[1], &h, sizeof(&h));
        


    }
    else {
        // TODO: write to child 
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]);
        wait(nullptr);

        
        // TODO: get hash from child 
        long long unsigned int hrecv;
        read(pipefd[0], &hrecv, sizeof(hrecv));
        close(pipefd[0]);
        
        // TODO: calculate hash on parent side
        long long unsigned int h = hash(pid, argv[1], strlen(argv[1]));

		
		// print hashes; DO NOT change
        printf("%llX\n", h);
        printf("%llX\n", hrecv);
    }

    return 0;
}