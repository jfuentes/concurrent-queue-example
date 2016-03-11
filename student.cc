#include "mpmc_bounded_queue.h" // concurrent queue is provided with the example
#include <iostream>
#include <sys/time.h>

#ifndef GEANT_STUDENT_EX
#define GEANT_STUDENT_EX
//g++ student.cc -o student -O3 -pthread -std=c++11

namespace GeantEx {
  using size_t = std::size_t;

  /** Record structure has to be used "as is" */
  struct Record {
    size_t   id_;      /** student id */
    size_t   math_;    /** grade in math */
    size_t   phys_;    /** grade in physics */
    size_t   chem_;    /** grade in physics */
    size_t   comp_sc_; /** grade in computing science */
  };

  /** Generate a random student record, to be called concurrently
  using the signature below */
  void GenerateRecord(Record &rec) {
    // Task to generate random record such that:
    //   rec.id_ has to be generated incrementally, starting from 0
    //   all grades have to be generated randomly between 1 and 10
    // A checksum of the id's of all generated records has to be computed
    // 
  }
  
  /** Processing of a record, to be called concurrently */
  void ProcessRecord(Record const &rec, float average) {
    // Task to compute the average of all grades for a record
    // The average has to be computed only if the grade in computing science
    // is higher than 7, otherwise can be scored as zero.
    // A checksum of the id's of all processed records has to be computed
  }
} // GeantEx

//______________________________________________________________________________
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

//______________________________________________________________________________
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

//______________________________________________________________________________
int main(int argc, char *argv[]) {
// Main should take a single argument, which is the max number of worker threads
// used at a given time.
  using namespace GeantEx;
  constexpr size_t nrecords = 1000000;  // number of records to be processed
  // The processing pipeline should be:
  // - parallel generation of records, computing the id checksum
  size_t checksum_ref = 0;
  // - parallel processing of records, returning the record id with the best
  //   average. The checksum of all processed records has to be computed and 
  // compared with the the checksum of generated ones. Processing of records
  // cannot be done in the same step as generation of records, it has to be logically
  // formalized as a separate task.
  // The final result has to print out the winning record with best average, the
  // 2 checksums, the number of threads used and the CPU and real time used.
  size_t checksum = 0;
  double cpu0  = get_cpu_time();  
  double rt0 = get_wall_time(); 
  // ... Your code here
  double cpu1  = get_cpu_time();  
  double rt1 = get_wall_time(); 

  std::cout << "run time: " << rt1-rt0 << "   cpu time: " << cpu1-cpu0 
            << "  checksum: " << checksum << " ref: " << checksum_ref << std::endl; 
  return 0;
}  
  
#endif
