#include <vector>
#include <unistd.h>
#include <iostream>
#include <algorithm>

#include <sys/wait.h>
#include <signal.h>



void execChildProcess(int signal) {
    if (signal == SIGINT) {
        char* arguments[] = {(char*)"./p1-helper", nullptr};
        execvp(arguments[0], arguments);
    }
}

int main(int argc, char** argv) {
    int n = 1, opt;
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
        case 'n':
            n = atoi(optarg);
            break;
        }
    }

    

    /*
    1. TODO: fork n child processes and run p1-helper on each using execvp
        > note: we need to synchronize the child processes to print in the desired order
        > note: sleep(..) does not guarantee desired synchronization
        > note: check "raise" system call
        > note: check "WUNTRACED" flag for "waitpid"
    */


    std::vector<pid_t> pid_list; // vector to store pids



for (int i = 0; i < n; i ++) { // fork n child processes
        pid_t pid = fork();

        if (pid == 0) {
            signal(SIGINT, execChildProcess); // register signal handler
            pause(); // wait for signal
            
        }
        else if (pid > 0) {
            pid_list.push_back(pid); // add pid to vector
        }
    }
    
    /* 
    2. TODO: print children pids 
    */
       
       std::sort(pid_list.begin(), pid_list.end());

    for (size_t j = 0; j < pid_list.size(); j++) {
        if (j < pid_list.size() - 1) {
            std::cout << pid_list[j] << " ";
        }
        else 
        {
            std::cout << pid_list[j];
        }
    }
    std::cout << std::endl;


    fflush(stdout);             // DO NOT REMOVE: ensures the first line prints all pids

    /* 
    3. TODO: signal children with the reverse order of pids 
        > note: take a look at "kill" system call 
    */




   for (int i = pid_list.size() - 1; i >= 0; --i) 
   {
        kill(pid_list[i], SIGINT);


        int STATUS;
        
        waitpid(pid_list[i], &STATUS, WUNTRACED);
   }
    
    printf("Parent: exiting\n");




    return 0;
}