#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <thread>
#include <mutex>
#include <sys/time.h>
#include <math.h>
#include <iostream>

#define NUM_OF_CORES 8
#define MAX_PRIME 1000000
using namespace std;

mutex m;
int found_primes = 0;
struct timeval start_time, end_time;

void timediff(struct timeval &start, struct timeval &end)
{
    int secs = (end.tv_sec * 1e6 + end.tv_usec - start.tv_sec * 1e6 - start.tv_usec) / (int)1e6;
    int usecs = (int)(end.tv_sec * 1e6 + end.tv_usec - start.tv_sec * 1e6 - start.tv_usec) % ((int)1e6);
    cout << "Took " << secs << "." << usecs << " seconds" << endl;
}

void do_primes(int threadid, int start, int end)
{
    int primes = 0;
    for (int num = start; num <= end; ++num)
    {
        int i;
        for (i = 2; (i <= num) && (num % i != 0); ++i)
            ;
        if (i == num)
            ++primes;
    }
    struct timeval tend;
    gettimeofday(&tend, 0);

    cout << "Thread [" << threadid << "] "
         << "Calculated " << primes << " in range [" << start << "," << end << "] ";
    timediff(start_time, tend);
    m.lock();
    found_primes += primes;
    m.unlock();
}

int main(int ac, char *av[])
{
    int maxprime = atoi(av[1]);

    int numthreads = atoi(av[2]);
    thread primethreads[numthreads];

    gettimeofday(&start_time, 0);

    int divisor = (int)ceil((double)maxprime / numthreads);
    int start = 1;
    for (int i = 0; i < numthreads; i++)
    {
        int end = min(start + divisor - 1, maxprime);
        primethreads[i] = thread(do_primes, i + 1, start, end);
        start += divisor;
    }

    for (int i = 0; i < numthreads; i++)
    {
        primethreads[i].join();
    }
    gettimeofday(&end_time, 0);
    cout << "Found a total of " << found_primes << " prime numbers in range [1," << MAX_PRIME << "]" << endl;
    timediff(start_time, end_time);
}
