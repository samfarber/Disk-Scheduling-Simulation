#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <deque>
#include <limits>

using namespace std;

class Controller {
public:
   Controller(double cylinders, int window, vector<int> initial_requests);
   int run(int new_request);

private:
   int current_cylinder;

   // DATA STRUCTURES NEEDED
   deque<int> requests;
   bool snap_back;
   bool bottom;

   // HELPER FUNCTIONS
   deque<int>::iterator FCFS_run();
   deque<int>::iterator SST_run();
   deque<int>::iterator CSCAN_run();
   deque<int>::iterator elevator_run();
};

//==========================================================================

Controller::Controller(double cylinders, int window, vector<int> initial_requests) {
   // INITIALIZE YOUR DATA STRUCTURES AND PLACE THE initial_requests
   // ONTO YOUR LOCAL QUEUE (which will be capable of storing no more
   // than window requests). SET current_cylinder TO cylinders / 2.

  requests.resize(window);
	for (size_t i = 0; i < initial_requests.size(); i++) {
		requests.push_back(initial_requests[i]);
    if (requests.size() == window)
      break;
  }

  current_cylinder = cylinders/2;
  snap_back = false;
  bottom = true;
}

/***********************************************************************
   Satisfy a single r/w request (perhaps not the one requested).
***********************************************************************/
int Controller::run(int new_request) {
   double time;

   // SELECT A CYLINDER, c, TO SERVICE, BASED ON YOUR SCHEDULING ALGORITHM
   deque<int>::iterator loc = CSCAN_run(); // Change based on preferred algo
   int c = *loc;

   if (snap_back)
     time = 0.7071 * sqrt(current_cylinder);

   else {
     time = sqrt(abs(c - current_cylinder));
     current_cylinder = c;

     // REMOVE c FROM YOUR LOCAL QUEUE
     // PLACE new_request ON YOUR LOCAL QUEUE
     requests.erase(loc);
     requests.push_back(new_request);
   }

   return time;
}

deque<int>::iterator Controller::FCFS_run() {
	deque<int>::iterator c = requests.begin();
	return c; // Returns the value that's been in requests the longest
}

deque<int>::iterator Controller::SST_run() {
	deque<int>::iterator iter = requests.begin();
	deque<int>::iterator closest_loc = requests.begin();
	int closest = std::numeric_limits<int>::max();

	while (iter != requests.end()) {
		if (abs(current_cylinder - *iter) < closest) {
      // Is closest to the current head position
			closest = abs(current_cylinder - *iter);
			closest_loc = iter;
		}
		iter++;
	}

	return closest_loc;
}

deque<int>::iterator Controller::CSCAN_run() {
  snap_back = false;
  deque<int>::iterator iter = requests.begin();
  deque<int>::iterator closest_loc = requests.begin();
	int closest = std::numeric_limits<int>::max();

  while (iter != requests.end()) {
    int next_closest = *iter - current_cylinder;
    if (next_closest > 0 && next_closest < closest) {
      // Gets next highest cylinder
      closest = next_closest;
      closest_loc = iter;
    }

    iter++;
  }

  if (closest == std::numeric_limits<int>::max()) {
    // If there was no next highest cylinder, then snap back
    snap_back = true;
    current_cylinder = requests.front();
  }

  return iter;
}

deque<int>::iterator Controller::elevator_run() {
  deque<int>::iterator iter = requests.begin();
  deque<int>::iterator closest_loc = requests.begin();
  int closest = std::numeric_limits<int>::max();

  while (iter != requests.end()) {
    if (bottom) { // Going up
      if (*iter > current_cylinder && *iter - current_cylinder < closest) {
        closest = *iter - current_cylinder;
        closest_loc = iter;
      }
    }
    else { // Going down
      if (*iter < current_cylinder && current_cylinder - *iter < closest) {
        closest = current_cylinder - *iter;
        closest_loc = iter;
      }
    }

    iter++;
  }
  if (closest == std::numeric_limits<int>::max())
    bottom = !bottom; // Reached the end, either top or bottom

  return closest_loc;
}


#endif
