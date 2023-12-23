#include <iostream>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <unistd.h>
using namespace std;
#define MAX_THREADS 100

#define BURGER 0
#define FRIES 1
const char* type_names[] = {"BURGER", "FRIES"};
#define pii pair<int, int>

int k;
int orders = 0;
sem_t sem_orders[2];
mutex mtx;
//wait count
int sem_wait_count[2] = {0, 0};
// Do not change
void process_order() {
    sleep(2);
}

void place_order(int type) {
    /**post incrmenting wait decrmenting
     *  Add logic for synchronization before order processing
     *  Check if already k orders in process;  if orders >= k
     *     if true -> wait (print waiting) -> lock mutex -> print order 
     *     otherwise place this order (print order)
     *  Use type_names[type] to print the order type
     sem_wait(&sem_orders[type]);


     else:
        increment the order count
     */
    sem_wait_count[type]++;
    if (orders >= k){

            
            mtx.lock();
            cout << "Waiting to order " << type_names[type] << endl;
            mtx.unlock();
            sem_wait(&sem_orders[type]);
        
            //sem_wait_count[type]--;
    }

    orders++;
            
    mtx.lock();
    cout << "Ordering " << type_names[type] << endl;
    mtx.unlock();
    
     

    process_order();        // Do not remove, simulates preparation


    //case 1: burgers
    //sempost(&sem_orders[0])
    //modify wait count
    //case 2: fries
    //sempost(&sem_orders[1])
    //modify wait count
    mtx.lock();
    orders--;
    if(type == BURGER){
        sem_wait_count[BURGER]--;
        //sem_post(&sem_orders[BURGER]);
        if (sem_wait_count[BURGER] > 0){
            sem_post(&sem_orders[BURGER]);
        }
        else if (sem_wait_count[FRIES] > 0){
            sem_post(&sem_orders[FRIES]);
        
        }
    }
    else if(type == FRIES){
        sem_wait_count[FRIES]--;
        //sem_post(&sem_orders[FRIES]);
        if(sem_wait_count[FRIES] > 0){
            sem_post(&sem_orders[FRIES]);
        }
        else if (sem_wait_count[BURGER] > 0) {
        sem_post(&sem_orders[BURGER]);
        }
    }
    mtx.unlock();


    /**
     *  Add logic for synchronization after order processed
     *  Allow next order of the same type to proceed if there is any waiting; if not, allow the other type to proceed.
     * 
     */
}

int main() {
    // Initialize necessary variables, semaphores etc.
    //sem_orders[2] initialized to 0
    sem_init(&sem_orders[0], 0, 0);
    sem_init(&sem_orders[1], 0, 0);
    // wait count initialized to 0

    // Read data: done for you, do not change
    pii incoming[MAX_THREADS];
    int _type, _arrival;
    int t;
    cin >> k;
    cin >> t;
    for (int i = 0; i < t; ++i) {
        cin >> _type >> _arrival;
        incoming[i].first = _type;
        incoming[i].second = _arrival;
    }

    // Create threads: done for you, do not change
    thread* threads[MAX_THREADS];
    for (int i = 0; i < t; ++i) {
        _type = incoming[i].first;
        threads[i] = new thread(place_order, _type);
        if (i < t - 1) {
            int _sleep = incoming[i + 1].second - incoming[i].second;
            sleep(_sleep);
        }
    }

    // Join threads: done for you, do not change
    for (int i = 0; i < t; ++i) {
        threads[i]->join();
        delete threads[i];
    }
    return 0;
}