/* Instuctrion:
    *Must compile and run in zeus w/ command line.
    *README with how to compile and run. (starting after log in)
    *Report that breifly describes the project and gives the RESULTS as well as answering any questions

    Discrete-Time event Simulator (FCFS) CPU Scheduling Algorithm to assess the impact of different workloads on the following performance metrics:
      * Average turnaround time of processes
      * Total throughput (number of processes done per unit time)
      * Average CPU utilization
      * Average number of processes in the Ready Queue

    Simulation:
      * Generate a sequence of processes
      * Generate arrival time
          * Poisson Distribution -> Exponentially Distributed interarrival times
          * Lambda will vary to simulate different work loads
      * Generate requested service time
          * Exonential Distribution, fixed average
      * STOP after 10,000 processes complete, then output metrics
      * Events that occur cause the simulator to update its current state
          * Events kept in a priority queue (sorted linked list) called Event Queue that describes future events and is kept sorted by the time of each event
          * Keep a clock variable that represents the current time
          * Initialization creates first event -> first process (time obtained by 0 plus a generated interarrival time) and adds it to Event Queue
          * Additional events may be added to the Event Queue, we know when an event will finish and if there is an event in the queue, so we can schedule an event and place it in the queue
          * Time hops between events so update simulator clock accordingly
      * Ready Queue is NOT the Event Queue, keep number of processes in the Ready Queue as part of the system state
      * Slides "A Simple Example" in the "Discrete-Time Event Simulation" should be helpful
      * May need auxiliary variables/codes to keep log records, in order to obtain the four itemized metrics at the end of each run.

    Runs:
      * takes 2 command line arguments, average arrival rate and average service time. (10->30pps, 0.04 seconds)

    Plots:
      * include a brief description of the results and show a single plot for each one of the above metrics with lambda as the x-axis (21 data points). Include a brief description/interpretation of the plots.


  z = random number b/w 0 and 1
  x = -(1/lambda)*ln(z)
  average waiting proccesses, multiply how many waiting by time interval, divide by time
*/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>


//      Struct Process
struct process {
  int id;
  float arrivalTime;
  float serviceTime;
  float remainingServiceTime;
  int priority;
};
//      Struct Event
struct event {
  std::string type; //ARR or DEP
  float time;
  event* nextEvent;
  process* p;

  //constructor
  event(std::string e_type, int t) {
    type = e_type;
    time = t;
  }
};

//      Globals
bool isIdle;
int currTime = 0;
int readyQueueCount = 0;
event* eq_head;
int numProcesses = 0;
float avgTurnaround = 0;
float totThroughput = 0;
float avgCpuUtil = 0;
float avgNumProcs = 0;
double serviceTime;
int lambda;
float x;


//      Schedule an Event
void Schedule_event(std::string e_type, float t, event* head) {
  std::cout << "Event Scheduled : " << numProcesses << std::endl;//debug
  event* e = new event(e_type,t);//create event
  e->type = e_type;e->time = t;
  if (eq_head == NULL) { //queue is empty, put on front
    eq_head = e;
  } else {
    event* temp = eq_head;
    while (temp->nextEvent != NULL) { //traverse to end
      temp = temp->nextEvent;
    }
    temp->nextEvent = e; //place new node in back of queue
  }
}
//      Initialize Simulation
void Init(int lambda) {
  std::cout << "Initializing..." << std::endl;
  x = rand()/RAND_MAX; //generate a number b/w 0 and 1
  currTime = 0;
  isIdle = true;
  eq_head = NULL; //Event Queue Head
  float t = currTime + (-(1/lambda)*log(x)); //Generate Interarrival time
  Schedule_event("ARR",t,eq_head);
}
//      Arrival Event Handler
void Arr_handler(event* e) {
  if (isIdle == true) { //CPU available
    isIdle = false;
    float s = (-serviceTime) * log(((double)rand()/(double)RAND_MAX));
    Schedule_event("DEP",currTime + s,e); //add to Event Queue
  } else { //CPU not available
    readyQueueCount++;
    float a = (-(1/lambda)*log(x));
    Schedule_event("ARR",currTime + a,e); //add to Event Queue
  } 
}
//      Departure Event Handler
void Dep_handler(event* e) {
  numProcesses++;
  if (readyQueueCount == 0) {
    isIdle = true;
  } else {
    readyQueueCount--;
    float s = (-serviceTime) * log(((double)rand()/(double)RAND_MAX));
    Schedule_event("DEP",currTime + s,e);
  }
}
//      Get Next Event From Queue
event* Get_next_event () {
  return eq_head;
}
//      Delete Event From Queue
void Delete_event (event* e) {
  event* temp = eq_head;
  eq_head = temp->nextEvent;
}



//      Run the Simulation
void Run() { //run through all procs
  std::cout << "Running" << std::endl;
  while (numProcesses <= 10000) {
    event* e = Get_next_event();
    //record the time
    //record interval length
    //record states
    currTime = currTime + (e->time);
    if (e->type == "ARR") {
      std::cout << "arrival event" << std::endl;
      //record arrival time
      Arr_handler(e);
    } else if (e->type == "DEP") {
      std::cout << "departure event" << std::endl;
      //record arrival time
      Dep_handler(e);
    }
    Delete_event(e);
  }
}

  int main(int argc, char*argv[]) {
    //grabs first argument, lambda, and stores it as an int
    lambda = std::atoi(argv[1]); //Average Arrival Rate
    //grabs second argument, average service time, and stores it as a double
    serviceTime = std::atof(argv[2]); //Average Service Time
    srand((unsigned)time(NULL)); //allows for rng

    Init(lambda);
    Run();

    std::cout << "Discrete-Time Event Simulator (FCFS) for 10,000 processes with arrival rate of lambda = "  + std::to_string(lambda) + " and average service time of " +  std::to_string(serviceTime) + "\n";
  
    std::cout << "Average turnaround time: " + std::to_string(avgTurnaround) << std::endl;
    std::cout << "Total throughput: " + std::to_string(totThroughput) << std::endl;
    std::cout << "Average CPU utilization: " + std::to_string(avgCpuUtil) << std::endl;
    std::cout << "Average number of processes in the Ready Queue: " + std::to_string(avgNumProcs) << std::endl;
    return 0;
  }
//};
