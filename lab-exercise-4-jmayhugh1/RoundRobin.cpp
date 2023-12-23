#include "RoundRobin.h"
# include <iostream>
using namespace std;
/*
This is a constructor for RoundRobin Scheduler, you should use the extractProcessInfo function first
to load process information to process_info and then sort process by arrival time;

Also initialize time_quantum
*/
// vector stores incoming processes

RoundRobin::RoundRobin(string file, int time_quantum) : time_quantum(time_quantum) {
	extractProcessInfo(file);
	
	sort(processVec.begin(), processVec.end(), [](Process p1, Process p2) {
		return p1.get_arrival_time() < p2.get_arrival_time();
	});
	
	

}

// Schedule tasks based on RoundRobin Rule
// the jobs are put in the order the arrived
// Make sure you print out the information like we put in the document



void RoundRobin::schedule_tasks() {
	int time_quantum = get_time_quantum();
    size_t proc_complete = 0; 
    int stime = 0;      
    size_t pindex = 0;  
      
    
    processQ.push(processVec[pindex]); // Push the first process into the queue
    while (proc_complete != processVec.size()) {
		
		
		 // While all processes are not proc_complete
        if (processQ.empty()) 
		{
            print(stime, -1, false);
            stime++;

            for (size_t i =0; i < processVec.size(); i++)
			 { // Check if any processes arrive at the current time
                if (processVec[i].get_arrival_time() == stime)
				 {
                    processQ.push(processVec[i]);
                    pindex++;
                }
            }
            continue;
        }
        Process cur = processQ.front(); // Get the first process in the queue
       
	   
	    processQ.pop();
        int time = min(cur.get_remaining_time(), time_quantum); // Get the time to run the process

        if (time == 0) {
          	  cur.Run(0);
           	 proc_complete++; // Increment the number of proc_complete processes
            print(stime, cur.getPid(), cur.is_Completed()); // Print the process information
            for (size_t i = 0; i < processVec.size(); i++) {
                if (processVec[i].get_arrival_time() == stime && processVec[i].getPid() != cur.getPid()) {
                    processQ.push(processVec[i]);
                    pindex++;
                }
            }

            continue;
        }

        for (int i = 0; i < time; i++) { // Run the process for the time
  
            print(stime, cur.getPid(), cur.is_Completed());
            cur.Run(1); 
			// Increment the system time
            stime++;

            for (size_t i = 0; i < processVec.size(); i++) 
			{
                if (processVec[i].get_arrival_time() == stime) {
                                       pindex++;

				    processQ.push(processVec[i]);
                }
            }
        }
		/*
		if (!cur.is_Completed()) {
			 proc_complete++;
            print(stime, cur.getPid(), cur.is_Completed());
           
        } 
		*/
        // Check if complete
        if (cur.is_Completed()) {
			 proc_complete++;
            print(stime, cur.getPid(), cur.is_Completed());
           
        } 
		else 
		{
            processQ.push(cur);
            pindex++;
        }
    }
}

/*************************** 
ALL FUNCTIONS UNDER THIS LINE ARE COMPLETED FOR YOU
You can modify them if you'd like, though :)
***************************/


// Default constructor
RoundRobin::RoundRobin() {
	time_quantum = 0; //time permitted to run
}

// Time quantum setter
void RoundRobin::set_time_quantum(int quantum) {
	this->time_quantum = quantum;
}

// Time quantum getter
int RoundRobin::get_time_quantum() {
	return time_quantum;
}

// Print function for outputting system time as part of the schedule tasks function
void RoundRobin::print(int stime, int pid, bool isComplete){
	string s_pid = pid == -1 ? "NOP" : to_string(pid);
	cout << "System Time [" << stime << "].........Process[PID=" << s_pid << "] ";
	if (isComplete)
		cout << "finished its job!" << endl;
	else
		cout << "is Running" << endl;
}

// Read a process file to extract process information
// All content goes to proces_info vector
void RoundRobin::extractProcessInfo(string file){
	// open file
	ifstream processFile (file);
	if (!processFile.is_open()) {
		perror("could not open file");
		exit(1);
	}

	// read contents and populate process_info vector
	string curr_line, temp_num;
	int curr_pid, curr_arrival_time, curr_burst_time;
	while (getline(processFile, curr_line)) {
		// use string stream to seperate by comma
		stringstream ss(curr_line);
		getline(ss, temp_num, ',');
		curr_pid = stoi(temp_num);
		getline(ss, temp_num, ',');
		curr_arrival_time = stoi(temp_num);
		getline(ss, temp_num, ',');
		curr_burst_time = stoi(temp_num);
		Process p(curr_pid, curr_arrival_time, curr_burst_time);

		processVec.push_back(p);
	}

	// close file
	processFile.close();
}
