// your PA3 BoundedBuffer.cpp code here
#include "BoundedBuffer.h"
#include <iostream>
#include <vector>
#include <mutex>

using namespace std;


BoundedBuffer::BoundedBuffer (int _cap) : cap(_cap) {
    // modify as needed
    // this jawns gonna be empty
}

BoundedBuffer::~BoundedBuffer () {
    // modify as needed
}

void BoundedBuffer::push (char* msg, int size) {
    // 1. Convert the incoming byte sequence given by msg and size into a vector<char>
    // use one of the vector's constructors
    std::vector<char> v(msg, msg + size);

    // 2. Wait until there is room in the queue (i.e., queue lengh is less than cap)
        // wait for slots_avail
        unique_lock<mutex> ul(m);
        slots_avail.wait(ul, [this] {return (int)q.size() < (int)cap;});
    // 3. Then push the vector at the end of the queue
    q.push(v);
    // 4. Wake up threads that were waiting for push
    ul.unlock();
    //  // notify data_avail
    data_avail.notify_one();
}

int BoundedBuffer::pop (char* msg, int size) {
    // 1. Wait until the queue has at least 1 item
        // wait for data_avail
        unique_lock<mutex> ul(m);
        data_avail.wait(ul, [this] {return q.size() > 0;});
    // 2. Pop the front item of the queue. The popped item is a vector<char>
    vector<char> v = q.front();
    q.pop();



    ul.unlock();
    // 3. Convert the popped vector<char> into a char*, copy that into msg; assert that the vector<char>'s length is <= size
    //      use the vector's data() method
    assert((int) v.size() <= (int) size);
    char* data = v.data();
    memcpy(msg, data, v.size());
    // 4. Wake up threads that were waiting for pop
    //      notify slots_avail
    slots_avail.notify_one();
    // 5. Return the vector's length to the caller so that they know how many bytes were popped
    return v.size();
}

size_t BoundedBuffer::size () {
    return q.size();
}