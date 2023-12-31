/*
Approximate Pi using the Leibniz formula
*/

#include <thread>
#include <iostream>
#include <vector>
#include <chrono>
#include <mutex>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#define NUM_THREADS 2 // adjust this to change the number of threads

#define PAD 8 // prevent false sharing

static long long int num_steps = 1000000000;
double step;

// add synchronization primitive(s) here
std::mutex mtx;
// Do not modify this function
inline void single_sum_thread(int id, int num_threads, double sum[NUM_THREADS][PAD])
{
    double x;
    sum[id][0] = 0.0;
    for (long long int i = id; i < num_steps; i = i + num_threads)
    {
        x = (i + 0.5) * step;
        sum[id][0] += 4.0 / (1.0 + x * x);
    }
}

// TODO
inline void pi_sum_thread(int i, double sum[NUM_THREADS][PAD], double *pi)
{
            // // add code here
            // for (i = 0, pi = 0.0; i < nthreads; i++)
            // {
            //     pi += sum[i][0] * step;
            //     sleep(1); // simulate a long running task
            // }
            mtx.lock();
          *  pi += sum[i][0] * step;
            mtx.unlock();



    sleep(1); // DO NOT REMOVE THIS
}
