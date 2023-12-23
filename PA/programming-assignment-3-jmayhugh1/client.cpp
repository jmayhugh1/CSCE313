

#include "FIFORequestChannel.h"
#include <thread>
#include "BoundedBuffer.h"
#include "common.h"
#include "Histogram.h"
#include "HistogramCollection.h"
#include <fstream>
#include <iostream>

#include <sys/time.h>
#include <sys/wait.h>


using namespace std;
// ecgno to use for datamsgs
#define EGCNO 1
struct patient_struct // struct to store patient data, this is needed in order to store the data in the buffer
{int p_no; double Double_data;};



//patient thread funciton needs to take three arguments, one to the bounded byfferm thread count and the patient number

void patient_thread_function(BoundedBuffer *buffer, int thread_Count, int pn)
{ 
    /* What will the patient threads do?
    - create datamsgs
    - push datamsgs to request buffer
    - increment time
    
    */
    //data message object message is created with the default number from the file
// fot loop[ eill tun thread count times, for each patient thread and psh a message inro buffer and incrmeenting message.seconds y .004

    datamsg message(pn, 0.0, EGCNO); 
    for (int i = 0; i < thread_Count; i++) {
        buffer-> push((char* )&message,  sizeof( message)); 
        message.seconds += 0.004;       
    }
}

//needs to take four arguments, a pointer to the bounded buffer request buffer, a file name, the maxmessage size and the file size
// inside the functio sevetal variable need to be declard
void file_thread_function(BoundedBuffer *request_buffer, string file_name, int m, int filesize) 
{
    printf("File name: %s\n", file_name.c_str());
    // need to create a filemsg object message with offest and m as arguments, copying message and file_name into a biffer
    int offset = 0;  int length = sizeof(filemsg) + file_name.size() + 1; int n = floor(filesize / m); 
for (int i = 0; i < n; i++)
            {
                    filemsg message(offset, m); 
                char char_buffer[MAX_MESSAGE];       

                /**copy file message into temp buffer
                    * copy file name into temp buffer
                    * push to request buffer
                    *
                */
                memcpy(char_buffer, &message, sizeof(filemsg));
                strcpy(char_buffer + sizeof(filemsg), file_name.c_str());

                //pushinf the the char buffer into request_buffer, then increasing offset by the maxmessage

                request_buffer->push(char_buffer, length);
                 offset = offset +  m; 
            }
    //need to handle remainder
    if ((filesize % m) != 0) {


        // remainder of whats left in the thing
        filemsg remainder(offset, filesize - offset);
        char char_buffer[MAX_MESSAGE]; 
    // copy remainder into temp buffer

        /**copy file message into temp buffer
                    * copy file name into temp buffer
                    * push to request buffer
                    *
                */
        memcpy(char_buffer, &remainder, sizeof(filemsg));         
        strcpy(sizeof(filemsg) + char_buffer, file_name.c_str()); 
    // push to request buffer
        // to request buffer'



        request_buffer->push(char_buffer, length); }
    
}
void worker_thread_function(BoundedBuffer *response_buffer, BoundedBuffer *request_buffer, FIFORequestChannel *channel, int m)
{
    // Functionality of the worker threads
    double temp_data_msg = 0;
    MESSAGE_TYPE *message;

    // needed to be vector of chars, not a char array
    std::vector<char> sendTo(m); // Dynamically allocated buffers 
    std::vector<char> recieveFrom(m);
    while (true)
    {   

        message = (MESSAGE_TYPE *)sendTo.data(); 

        request_buffer->pop(sendTo.data(), sendTo.size()); 
        if (*message == FILE_MSG){
            //creating a pointer of type filemsg, done when you know the underlying datatype
            filemsg *filemessage = (filemsg *)sendTo.data(); 
            //casting the memory location of the filemessage to a string,
            //tihs is done to extraxt the the string data the follows the filemsg in memory
            std::string tempstring = (char *)(filemessage + 1); 
            //calculating the size of the file
            //adding the size of the string tempstring to the size of the filemsg and 1 to account for the null character
            int filesize = tempstring.size() + sizeof(filemsg) + 1;
            
            std::string filename = (char *)(filemessage + 1); 
            //printf("File name: %s\n", filename.c_str());
            //prepening the filename with the received directory
            filename = "received/" + filename; 
            //sending the filemsg to the server
            channel->cwrite(sendTo.data(), filesize);
            //creating a buffer to store the data received from the server
            channel->cread(recieveFrom.data(), m);

            FILE *file_out = fopen(filename.c_str(), "r+");
            /*
            *this line is setting the position of the file pointer 
            *file_out from the beginning of the file by filemessage->
            *offset bytes.
            */
            // a bunch of file stuff I dont undersatnd
            fseek(file_out, filemessage->offset, SEEK_SET);
            fwrite(recieveFrom.data(), 1, filemessage->length, file_out); 
            fclose(file_out);
        }
        else if (*message == QUIT_MSG)
                    {

                        channel->cwrite(message, sizeof(QUIT_MSG));
                        break;
                    }

        else
        {
            // send datamsg to server
            channel->cwrite(sendTo.data(), sizeof(datamsg));

            // read from server, store in temp_data_msg, then push to response buffer
            channel->cread(&temp_data_msg, sizeof(double));

            // create patient struct
            patient_struct patient_info = {((datamsg *)sendTo.data())->person, temp_data_msg};


            // push to response buffer
            response_buffer->push((char *)&patient_info, sizeof(patient_info)); 
        }
        
    }
}


void histogram_thread_function(HistogramCollection *hc, BoundedBuffer *reponse_buffer)
{  while (true) {
//buffer with the same size as patient struct, used to store patient data
        char buffer[sizeof(patient_struct)]; 

        // calls the pop meethod in order to remove data from the bufferfer, data is tored in buffer and the maximum 
        // amount od data removed is the size of the patient struct
        reponse_buffer->pop(buffer, sizeof(patient_struct)); // patient data
        // creating a pointer patient_info of type patient struct, this is done to access the data in buf
        patient_struct *patient_info = (patient_struct *)buffer;
        // checks of the patient number and double data are -1, if so then break out of the loop

        if ( (patient_info->Double_data == -1.0)     &&    (patient_info->p_no == -1)){break;}
        else
            hc->update(patient_info->p_no, patient_info->Double_data);
    }}




int main(int argc, char *argv[])
{
    int n = 1000;        // default number of requests per "patient"
    int p = 10;          // number of patients [1:,15:]
    int w = 100;         // default number of worker threads
    int h = 20;          // default number of histogram threads
    int b = 20;          // default capacity of the request buffer (should be changed)
    int m = MAX_MESSAGE; // default capacity of the message buffer
    string f = "";       // name of file to be transferred

    // read arguments
    int opt;
    while ((opt = getopt(argc, argv, "n:p:w:h:b:m:f:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            n = atoi(optarg);
            break;
        case 'p':
            p = atoi(optarg);
            break;
        case 'w':
            w = atoi(optarg);
            break;
        case 'h':
            h = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 'm':
            m = atoi(optarg);
            break;
        case 'f':
            f = optarg;
            break;
        }
    }

    // fork and exec the server
    int pid = fork();
    if (pid == 0)
    {
        execl("./server", "./server", "-m", (char *)to_string(m).c_str(), nullptr);
    }

    // initialize overhead (including the control channel)
    FIFORequestChannel *chan = new FIFORequestChannel("control", FIFORequestChannel::CLIENT_SIDE);
    BoundedBuffer request_buffer(b);
    BoundedBuffer response_buffer(b);
    HistogramCollection hc;

    // making histograms and adding to collection
    for (int i = 0; i < p; i++)
    {
        Histogram *h = new Histogram(10, -2.0, 2.0);
        hc.add(h);
    }

    // record start time
    struct timeval start, end;
    gettimeofday(&start, 0);
     /* create all threads here */
    //if data:
        // - create p patient threads(store them in producer array)
        // - create w worker threads (store them in worker array)
        //      - create a new channel for each worker thread (store them in FIFO array)
        // - create h histogram threads(store them in histogram array)
    //if file:
        // - create 1 file thread (store it in producer array)
        // - create w worker threads (store them in worker array)
        //      - create  w channels (store FIFP array)
    //if data:
        // - create p patient threads
    // if file:
        // - create 1 file thread
    //
    //
    //create w worker threads
    //      - create w channels
    //
    //if data:
        // - create h histogram threads


    /* join all threads here */
    // iterate over all thread arrays and join them
        // -order is important produucers bevore consumers
    // declares arrayas and vectors of threads used to handle dile operations. histogram collection and patient data
    // pricessing
    thread file_thread[1];


        vector<thread> histogram_threads(h);

    vector<thread> patient_threads(n);



    // cehcks if the client is requestiong a file from this server, if its not a file,
    // it creates p patient threads and h histrogram threads, each patient thread is assigned the function patient_thread_function
    // and each histogram thread is assigned the function histogram_thread_function


    if (f.length() == 0)
    {
        for (int i = 0; i < p; i++)
        {patient_threads[i] = thread(patient_thread_function, &request_buffer, n, i + 1);}
        for (int i = 0; i < h; i++){histogram_threads[i] = thread(histogram_thread_function, &hc, &response_buffer);}
    }
    // if f is not empty it means that the client is requesting a file from the server, in this case it opens a file with the name recieved/ + f
    // and prepares to write to it
    // 
    else {


    string filename = "received/" + f;

       FILE *file_out;



        file_out = fopen(filename.c_str(), "w"); 
        /* create file thread here
        - create file message
        - send file message to server
        - get file size from server
        - create file thread
        - join file thread

        */

        //  creates a file message object with oddest and length at 0
        // it copies the file message into a buffer and then copies the file name into the buffer
        // it the sends the buffer to the server ehich respinds witht the size if the requested file, which is stored
        // in getsuze
        filemsg file_message(0, 0); 


        /* need to process the file mesage object and retieve the file name and size
        *from the server
        */
        int file_leng =  sizeof(filemsg) + (1 + f.size());
         char char_buffer[MAX_MESSAGE];    
        memcpy(char_buffer, &file_message, sizeof(filemsg));        
        strcpy(char_buffer + sizeof(filemsg), f.c_str()); 
        chan->cwrite(char_buffer, file_leng);                   
         size_t file_size; // file size
        chan->cread(&file_size, sizeof(size_t));
        // file thread is created ti handle the file transfer
        file_thread[0] = thread(file_thread_function, &request_buffer, f, m,  file_size); // threading
        fclose(file_out);  // close file



        
    }



    //declare a vector of pointers to FIFORequestChannels
    
    // an array od std:Lthread objects and a worker threaf
    
    thread worker_message[MAX_MESSAGE];

    vector<FIFORequestChannel *> channels;
    
    
    //creates w worker thredsa and each worker threads is assifned the functino
    // worker thread function that was declared above
    // a new channels is created for each worker threads and stored in the channel vector
    // a new FIFORequestChannel is created for each worker thread
for (int i = 0; i < w; i++){
         MESSAGE_TYPE messagenew = NEWCHANNEL_MSG;
        char buffer[2048];
        chan->cwrite(&messagenew, sizeof(MESSAGE_TYPE));
        chan->cread(buffer, 2048);
        FIFORequestChannel *c = new FIFORequestChannel(buffer, FIFORequestChannel::CLIENT_SIDE); // create new channel
        worker_message[i] = thread(worker_thread_function, &response_buffer, &request_buffer, c, m);
        channels.push_back(c);
    
    }
    if (f.size() == 0) {//checks if the f is empty, if is it it waits for all the patient threds to finish, 
    //

        for (int i = 0; i < p; i++){patient_threads.at(i).join();}
    }
    else {
        // if not empty is it wasits for all the fule threads to finish
        file_thread[0].join();

    }
    //sends a quit message to all the worker threads to signal them to dinish, then it waits for all the worker threds to finsh

    for (int i = 0; i < w; i++) {
        datamsg d(0, 0.0, 0);
        d.mtype = QUIT_MSG;
        request_buffer.push((char *)&d, sizeof(datamsg));
    }for (int i = 0; i < w; i++){
        worker_message[i].join();
    }

    //checls again ig g is emopty, if is it sepdsn a special patient data that has 
    // impossible valus in toder for it to signal all the histrgram threads to finish, 
    // then it waits for all the histogram threads to finish
if (f.size() == 0) {
for (int i = 0; i < h; i++)
{
    patient_struct patient_info; 
 // impossible values to determine if qut
patient_info.p_no = -1;

patient_info.Double_data = -1.0;
    response_buffer.push((char *)&patient_info, sizeof(patient_struct));
        }
        for (int i = 0; i < h; i++)
        {
            histogram_threads.at(i).join();
            }
    }
//need to clear up all the chabbels in the channel vector`
    for (auto c : channels)
    {
        delete c;
    }
    // record end time
    gettimeofday(&end, 0);

    // print the results
    if (f == "")
    {
        hc.print();
    }
    int secs = ((1e6 * end.tv_sec - 1e6 * start.tv_sec) + (end.tv_usec - start.tv_usec)) / ((int)1e6);
    int usecs = (int)((1e6 * end.tv_sec - 1e6 * start.tv_sec) + (end.tv_usec - start.tv_usec)) % ((int)1e6);
    std::cout << "Took " << secs << " seconds and " << usecs << " micro seconds" << endl;

    // quit and close control channel
    MESSAGE_TYPE q = QUIT_MSG;
    chan->cwrite((char *)&q, sizeof(MESSAGE_TYPE));
    std::cout << "All Done!" << endl;
    delete chan;

    // wait for server to exit
    wait(nullptr);
}