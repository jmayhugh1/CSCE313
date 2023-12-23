#include <iostream>

#include <condition_variable>
#include <mutex>
#include <thread>



// include additional necessary headers
std::mutex mut;
std::condition_variable cv;
bool flag = true;


void query(int cnt) {
    // Should print: the print number (starting from 0), "SYN", and the three dots "..."
for(int i = 0; i < cnt; i++)
{
    std::unique_lock<std::mutex> lock(mut);
    cv.wait(lock, []  {return flag;});
    


    std::cout << i << " SYN ... ";



    flag = false;


    cv.notify_all();

}
}

void response(int cnt) {
    // Should print "ACK"
    for(int i = 0; i < cnt; i++)
    {


        std::unique_lock<std::mutex> lock(mut);
        cv.wait(lock,   []    {return !flag;});


        
        std::cout << "ACK" << std::endl;
        flag = true;
        cv.notify_all();
    }
}

int main(int argc, char** argv) {
    if (argc < 2) 
    {


        printf("Usage: ./handshake <count>\n");
        exit(1);
    }

    /**
     * Steps to follow:
     * 1. Get the "count" from cmd args
     * 2. Create necessary synchronization primitive(s)
     * 3. Create two threads, one for "SYN" and the other for "ACK"
     * 4. Provide the threads with necessary args
     * 5. Update the "query" and "response" functions to synchronize the output
    */
    int cnt = atoi(argv[1]);
    // create necessary synchronization primitive(s)
    std::thread t1(query, cnt);
    std::thread t2(response, cnt);
    t1.join();
    t2.join();



    return 0;
}