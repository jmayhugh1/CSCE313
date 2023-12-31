/*
Example: this program fetch a thread exit status
*/

#include "../apue.h"
#include <pthread.h>

void *thr_fn1(void *arg)
{
    printf("thread 1 returning\n");
    return ((void *)1);
}
void *thr_fn2(void *arg)
{
    printf("thread 2 exiting\n");
    pthread_exit((void *)2);
}

int main(void)
{
    int err;
    pthread_t tid1, tid2;
    void *tret;

    // Create thread 1
    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        printf("can’t create thread 1");

    // Create thread 2
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0)
        printf("can’t create thread 2");

    // Wait for thread 1 to end and capture the exit status value in tret
    err = pthread_join(tid1, &tret);
    if (err != 0)
        printf("can’t join with thread 1");
    printf("thread 1 exit code %ld\n", (long)tret);

    // Wait for thread 2 to end and capture the exit status value in tret
    err = pthread_join(tid2, &tret);
    if (err != 0)
        printf("can’t join with thread 2");
    printf("thread 2 exit code %ld\n", (long)tret);
    exit(0);
}