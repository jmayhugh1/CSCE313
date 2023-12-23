/***
 * Timing updates from Thomas Cousins Spring 2023
*/

#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <ctime>
#include <random>

using namespace std;


// Total number of books available (the shared resource)
int books_available = 100;

// Mutex for locking
mutex mut;


void print_checkout(int i, int quantity) {
    printf("Student %d checked out %d books\n", i, quantity);
}

void print_checkin(int i, int quantity) {
    printf("Student %d checked in %d books\n", i, quantity);
}

// Conditional variable for the allocating threads to wait upon
condition_variable checkout_ready;

/* TODO:
    Use mut and checkout_ready for synchronization, and call print_checkout() while owning lock
*/
void books_checkout(int threadId, int quantity)
{
    unique_lock<mutex> lock(mut );

    //wait until there are enough books available
    checkout_ready.wait(lock,   [& ]{return books_available > quantity;});
    books_available -= quantity;

    //rint the checkout message
    print_checkout(threadId, quantity);
    lock.unlock(); //unlock the mutex


}

/* TODO:
    Use mut and checkout_ready for synchronization, and call print_checkin() while owning lock
*/
void books_checkin(int threadId, int quantity)
{
    unique_lock<mutex> lock(mut);

    //Return the books
    books_available += quantity;
    lock.unlock(); //unlock the mutex
    print_checkin(threadId, quantity);

    //notify the waiting threads
    checkout_ready.notify_one();
}

/**
 * thread function that takes in a thread id and the number of books to checkout and return.
 * the manager function is in charge of checkouts and checkins of books for the threads
 * @param threadId thread id.
 * @param quantity number of books to be allocated.
 */
void manager(int threadId, int quantity)
{
    books_checkout(threadId, quantity);

    int64_t sleep_ms = (rand() % 2000) + 1; // sleep upto 2 seconds
    this_thread::sleep_for(chrono::milliseconds(sleep_ms)); // keep the books for some time

    books_checkin(threadId, quantity);
}
int main()
{
    // The number of books to checkout by eah thread
    int cart[6] = {90, 90, 40, 50, 30, 5};
    srand(time(NULL));


    // Creating an array of threads
    thread *student_threads = new thread[6];

    /* TODO: Create the six student threads that will use the function (manager) for a respective number of books (cart[i]) */
    for (int i = 0; i < 6; i++)
    {   //create the threads
        student_threads[i] = thread(manager, i, cart[i]);
    }




    /* TODO: join all the threads before exiting */
    for (int i = 0; i < 6; i++)
    {


        
        student_threads[i].join(); // join the threads
    }

    delete[] student_threads;
    return 0;
}