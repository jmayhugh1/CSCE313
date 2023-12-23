#include <vector> // vector, push_back, at
#include <string> // string
#include <iostream> // cin, getline
#include <fstream> // ofstream
#include <unistd.h> // getopt, exit, EXIT_FAILURE
#include <assert.h> // assert
#include <thread> // thread, join
#include <sstream> // stringstream
#include <stdio.h> // perror
#include <stdlib.h> // rand, srand
#include <time.h> // time
#include <cstring> // memset

#include "BoundedBuffer.h" // BoundedBuffer class

#define MAX_MSG_LEN 256

using namespace std;

/************** Helper Function Declarations **************/

void parse_column_names(vector<string>& _colnames, const string& _opt_input);
void write_to_file(const string& _filename, const string& _text, bool _first_input=false);

/************** Thread Function Definitions **************/

// "primary thread will be a UI data entry point"
void ui_thread_function(BoundedBuffer* bb) {
    // TODO: implement UI Thread Function
    //in an infinte loop, continuously prompt the user for inpt
    while(1){
        string input;
        printf("Enter data> ");
        getline(cin, input);
        if(input == "Exit"){
            bb->push((char*)input.c_str(), input.size());
            break;
        }
        bb->push((char*)input.c_str(), input.size());
       
    }
    //else push input to the buffer
}

// "second thread will be the data processing thread"
// "will open, write to, and close a csv file"
void data_thread_function(BoundedBuffer* bb, string filename, const vector<string>& colnames) {
    // TODO: implement Data Thread Function
    // (NOTE: use "write_to_file" function to write to file)
    // infintely loop,pop from bb
    int col = 0;
    while(1){
        char msg[MAX_MSG_LEN];
        int size = bb->pop(msg, MAX_MSG_LEN);
        string input = msg;
        if(input == "Exit"){
            break;
        }

        if (col % colnames.size() != colnames.size() - 1){
            input += ", ";
        }
        else{
            input += "\n";
        }
        col++;
            write_to_file(filename, input);
            input = "";
        memset(msg, '\0', MAX_MSG_LEN  );


    }
    // id its the exit message, break
    // else write to file
    // clear msg

}

/************** Main Function **************/

int main(int argc, char* argv[]) {

    // variables to be used throughout the program
    vector<string> colnames; // column names
    string fname; // filename
    BoundedBuffer* bb = new BoundedBuffer(3); // BoundedBuffer with cap of 3

    // read flags from input
    int opt;
    while ((opt = getopt(argc, argv, "c:f:")) != -1) {
        switch (opt) {
            case 'c': // parse col names into vector "colnames"
                parse_column_names(colnames, optarg);
                break;
            case 'f':
                fname = optarg;
                break;
            default: // invalid input, based on https://linux.die.net/man/3/getopt
                fprintf(stderr, "Usage: %s [-c colnames] [-f filename]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    // create file of fname, with column headers as colnames
    string colnames_str = "";
    for (int i = 0; i < colnames.size(); i++) {
        colnames_str += colnames.at(i);
        if (i != colnames.size() - 1)
            colnames_str += ", ";
    }
    colnames_str += "\n";
    
    write_to_file(fname, colnames_str, true);

    // TODO: instantiate ui and data threads
    thread ui_thread(ui_thread_function, bb);
    thread data_thread(data_thread_function, bb, fname, colnames);
    
    // TODO: join ui_thread
    ui_thread.join();


    // TODO: "Once the user has entered 'Exit', the main thread will
    // "send a signal through the message queue to stop the data thread"
    bb->push((char*)"Exit\0", 5);


    // TODO: join data thread
    data_thread.join();

    // CLEANUP: delete members on heap
    delete bb;

}

/************** Helper Function Definitions **************/

// function to parse column names into vector
// input: _colnames (vector of column name strings), _opt_input(input from optarg for -c)
void parse_column_names(vector<string>& _colnames, const string& _opt_input) {
    stringstream sstream(_opt_input);
    string tmp;
    while (sstream >> tmp) {
        _colnames.push_back(tmp);
    }
}

// function to append "text" to end of file
// input: filename (name of file), text (text to add to file), first_input (whether or not this is the first input of the file)
void write_to_file(const string& _filename, const string& _text, bool _first_input) {
    // based on https://stackoverflow.com/questions/26084885/appending-to-a-file-with-ofstream
    // open file to either append or clear file
    ofstream ofile;
    if (_first_input)
        ofile.open(_filename);
    else
        ofile.open(_filename, ofstream::app);
    if (!ofile.is_open()) {
        perror("ofstream open");
        exit(-1);
    }

    // sleep for a random period up to 5 seconds
    usleep(rand() % 5000);

    // add data to csv
    ofile << _text;

    // close file
    ofile.close();
}