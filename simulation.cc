//======================================================================
//  PROJECT:         CS 340, Hamilton College, Rick Decker
//  FILE:            simulation.cc
//  PURPOSE:         Test harness for the Controller class
//  VERSION:         2.0
//  UPDATE HISTORY:  version 2.0      10/19/11
//                   version 1.0      04/11/09
//======================================================================

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <math.h>
#include "controller.h"

using namespace std;

const long REQUESTS = 5000000L;

// Generates a uniform random int in the range [min, max)
int uniform(int min, int max) {
   return int(min + (double(rand()) / RAND_MAX) * (max - min));
}

int main() {
   int cylinders, window;
   cout << "Cylinders = ";
   cin >> cylinders;
   cout << "Window size = ";
   cin >> window;

   vector<int> initial_requests(window);

   // Seed the random number generator.
   srand(time(0)); rand(); rand();

   // Build the initial request vector
   for (int i = 0; i < window; i++) {
      initial_requests[i] = uniform(0, cylinders);
   }

   // Construct a new Controller instance.
   Controller c(cylinders, window, initial_requests);

   // Run the controller, keeping a running total of time used.
   double total_time = 0.0;
   for (long i = 0; i < REQUESTS; i++) {
      total_time += c.run(uniform(0, cylinders));
   }

   // Print the average number of cylinders traversed per request.
   cout << "--------------------------------------------------------" << endl;
   cout << "With " << cylinders << " cylinders and window size = " << window;
   cout << ", " << endl << "the average time to service " << REQUESTS / 1000;
   cout << "K requests was " << fixed << setprecision(2);
   cout << total_time / REQUESTS << endl;
   cout << "--------------------------------------------------------" << endl;

   return EXIT_SUCCESS;
}
