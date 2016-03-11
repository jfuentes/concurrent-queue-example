#include "mpmc_bounded_queue.h" // concurrent queue is provided with the example
#include <iostream>
#include <sys/time.h>
#include <atomic>

#ifndef GEANT_STUDENT_EX
#define GEANT_STUDENT_EX
//g++ student.cc -o student -O3 -pthread -std=c++11

namespace GeantEx {
  using size_t = std::size_t;

  //used for id assignment
  std::atomic<size_t> current_id;
  //global checksum
  std::atomic<size_t> global_checksum;
  //best average so far
  std::atomic<float> best_average;
  //id best record so far
  std::atomic<size_t> best_id;
  // total number of records
  size_t num_records;



  /** Record structure has to be used "as is" */
  struct Record {
    size_t   id_;      /** student id */
    size_t   math_;    /** grade in math */
    size_t   phys_;    /** grade in physics */
    size_t   chem_;    /** grade in physics */
    size_t   comp_sc_; /** grade in computing science */
  };

  //global concurrent queue
  mpmc_bounded_queue<Record> queue;

  /** Generate a random student record, to be called concurrently
  using the signature below */
  void GenerateRecord(Record &rec) {
    // Task to generate random record such that:
    //   rec.id_ has to be generated incrementally, starting from 0
    //   all grades have to be generated randomly between 1 and 10
    // A checksum of the id's of all generated records has to be computed
    //
    size_t temp;
    while(1) {
         temp=current_id.load(std::memory_order_relaxed);
         if (current_id.compare_exchange_weak(temp, temp + 1, std::memory_order_relaxed))
            break;
   }
   rec.id_ = temp;
   rec.math_ = rand()%11;
   rec.phys_ = rand()%11;
   rec.chem_ = rand()%11;
   rec.comp_sc_ = rand()%11;
   global_checksum.fetch_add(rec.id_);
  }

  /** Processing of a record, to be called concurrently */
  void ProcessRecord(Record const &rec, float &average) {
    // Task to compute the average of all grades for a record
    // The average has to be computed only if the grade in computing science
    // is higher than 7, otherwise can be scored as zero.
    // A checksum of the id's of all processed records has to be computed
    if(rec.comp_sc_>7)
         average = (rec.math + rec.phys_ + rec.chem_ + rec.comp_sc_)/4;
    else
         average = 0;
    global_checksum.fetch_add(rec.id_);
  }


int GenerateRecordThread(){
   while(current_id.load(std::memory_order_relaxed) < num_records){
         Record rec;
         GenerateRecord(rec);
         queue.enqueue(rec);
   }
   return 0;
}

int ProcessRecordThread(Record &result){
   float max_avg, temp;
   Record max_record;
   while(queue.size()>0){
         Record rec = queue.dequeue();
         ProcessRecord(rec, temp);
         if(temp>max_avg){
            max_record = rec;
            result = rec;
         }
   }
   return 0;
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

  size_t num_threads = atoi(argv[1]);


  //create the queue of records, array of results and threads
  queue = mpmc_bounded_queue(nrecords);
  Record results= new Record[num_threads];
  thrd_t threads[num_threads];

  //record generation step
  for(size_t i=0; i<num_threads; i++){
     thrd_create(&threads[i], &GenerateRecordThread);
  }

  for(size_t i=0; i<num_threads; i++){
    thrd_join(&threads[i]);
  }


  //record processing step
  for(size_t i=0; i<num_threads; i++){
     thrd_create(&threads[i], &ProcessRecordThread, &results[i]);
  }

  for(size_t i=0; i<num_threads; i++){
    thrd_join(&threads[i]);
  }


  double cpu1  = get_cpu_time();
  double rt1 = get_wall_time();

  std::cout << "run time: " << rt1-rt0 << "   cpu time: " << cpu1-cpu0
            << "  checksum: " << checksum << " ref: " << checksum_ref << std::endl;
  return 0;
}

#endif
