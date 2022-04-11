#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <main.cpp>

class Mycode {
//Functions

void Schedule_event(std::string e_type, float t, event* head) { //schedule event
  //create event
  event* e = new event(e_type,t);
  //insert event into linked list Event Queue
  if (eq_head == NULL) { //list is empty, put on front
    eq_head = e;
  } else {
    event* currEvent = eq_head;
    while (currEvent != NULL) { //traverse to end
      currEvent = currEvent->nextEvent;
    }
    currEvent = e;
    e->nextEvent = NULL;
  }
  numProcesses++;
}

void Init(int lambda) { //initialize event
  x = rand()/RAND_MAX; //generate a number b/w 0 and 1
  currTime = 0;
  isIdle = true;
  eq_head = NULL; //Event Queue Head
  float t = currTime + (-(1/lambda)*log(x)); //Generate Interarrival time
  Schedule_event("ARR",t,eq_head);
}

void Arr_handler(event* e) { //handle arrival event
  if (isIdle == true) { //CPU available
    isIdle = false;
    float s = (-serviceTime) * log(((double)rand()/(double)RAND_MAX));
    Schedule_event("DEP",currTime + s,e); 
  } else { //CPU not available
    readyQueueCount++;
  } //add to Event Queue
  float a = (-(1/lambda)*log(x));
  Schedule_event("ARR",currTime + a,e);
}

void Dep_handler(event* e) { //handle departure event
  if (readyQueueCount == 0) {
    isIdle = true;
  } else {
    readyQueueCount--;
    float s = (-serviceTime) * log(((double)rand()/(double)RAND_MAX));
    Schedule_event("DEP",currTime + s,e);
  }
}

event* Get_next_event () {
  return eq_head;
}

void Delete_event (event* e) {
  eq_head = eq_head->nextEvent;
}




void Run() { //run through all procs
  while (numProcesses <= 10000) {
    event* e = Get_next_event();
    //record the time
    //record interval length
    //record states
    currTime = currTime + (e->time);
    if (e->type == "ARR") {
      //record arrival time
      Arr_handler(e);
    } else if (e->type == "DEP") {
      //record arrival time
      Dep_handler(e);
    }
    Delete_event(e);
  }
}
};