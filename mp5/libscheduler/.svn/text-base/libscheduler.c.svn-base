/** @file libscheduler.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libscheduler.h"
#include "../libpriqueue/libpriqueue.h"


/**
  Stores information making up a job to be scheduled including any statistics.

  You may need to define some global variables or a struct to store your job queue elements. 
*/

/*
  Questions: 
    4. RR completely fucked
    5.
*/
typedef struct _job_t
{
  int job_number; 
  int toa; 
  int running_time; 
  int priority; 
  int start_time; 
  int head; 
  int time_remaining;
  int pause; 

} job_t;

//GLOBAL VARIABLES
scheme_t current_scheme; 
priqueue_t q1;
float time_waiting = 0; 
float turn_around_time = 0; 
float response_time = 0; 
int num_of_jobs; 
int start1 = 1; 
int start_time = -1;
int end1 = 1; 
int quantum;
int start; 
int end;  
  int x = 0; 


//COMPARE FUNCTIONS
int compare_fcfs(const void* a, const void* b) {
  return (((job_t*)a)->toa - ((job_t*)b)->toa);
}
int compare_sjf(const void* a, const void* b) {
  if ((((job_t*) b) -> head) == 1) {return 1;}
  return (((job_t*)a)->running_time - ((job_t*)b)->running_time);
}
int compare_psjf(const void* a, const void* b) {
  return (((job_t*)a)->time_remaining - ((job_t*)b)->time_remaining);
}
int compare_pri(const void* a, const void* b) {
  if ((((job_t*) b) -> head) == 1) {return 1;}
  return (((job_t*)a)->priority - ((job_t*)b)->priority);
}
int compare_ppri(const void* a, const void* b) {
  return (((job_t*)a)->priority - ((job_t*)b)->priority);
}
int compare_rr(const void* a, const void* b) {
    return 1;
}

/**
  Initalizes the scheduler.
 
  Assumptions:
    - You may assume this will be the first scheduler function called.
    - You may assume this function will be called once once.
    - You may assume that cores is a positive, non-zero number.
    - You may assume that scheme is a valid scheduling scheme.

  @param cores the number of cores that is available by the scheduler. These cores will be known as core(id=0), core(id=1), ..., core(id=cores-1).
  @param scheme  the scheduling scheme that should be used. This value will be one of the six enum values of scheme_t
*/
void scheduler_start_up(int cores, scheme_t scheme)
{
  if (scheme == FCFS){
    current_scheme = FCFS; 
    priqueue_init(&q1, compare_fcfs); 
  }
  else if (scheme == SJF){
    current_scheme = SJF; 
    priqueue_init(&q1, compare_sjf); 
  }
  else if(scheme == PSJF){
    current_scheme = PSJF; 
    priqueue_init(&q1, compare_psjf); 
  }
  else if (scheme == PRI){
    current_scheme = PRI; 
    priqueue_init(&q1, compare_pri);
  }
  else if(scheme == PPRI){
    current_scheme = PPRI; 
    priqueue_init(&q1, compare_ppri);
  }
  else {
    current_scheme = RR; 
    priqueue_init(&q1, compare_rr);
  }

}

/**
  Called when a new job arrives.
 
  If multiple cores are idle, the job should be assigned to the core with the
  lowest id.
  If the job arriving should be scheduled to run during the next
  time cycle, return the zero-based index of the core the job should be
  scheduled on. If another job is already running on the core specified,
  this will preempt the currently running job.
  Assumptions:
    - You may assume that every job wil have a unique arrival time.

  @param job_number a globally unique identification number of the job arriving.
  @param time the current time of the simulator.
  @param running_time the total number of time units this job will run before it will be finished.
  @param priority the priority of the job. (The lower the value, the higher the priority.)
  @return index of core job should be scheduled on
  @return -1 if no scheduling changes should be made. 
 
 */
int scheduler_new_job(int job_number, int time, int running_time, int priority)
{
  // BOOKKEEPING
  job_t * new_job = malloc(sizeof(job_t));
  new_job -> job_number = job_number; 
  new_job -> toa = time; 
  new_job -> running_time = running_time; 
  new_job -> priority = priority; 
  new_job -> time_remaining = running_time;
  new_job -> start_time = -1; 
  new_job -> head = 0; 
  num_of_jobs++; 

  job_t * peeker; 
  peeker = priqueue_peek(&q1); 

  // FOR NON PREEMPTIVE....
  if (current_scheme == FCFS || current_scheme == SJF || current_scheme == PRI){
    priqueue_offer(&q1, new_job); 

    //scheduler_show_queue(); 

    if (peeker == NULL){
      new_job -> head = 1;
      new_job -> start_time = time; 
      return 0; 
    }
    else {
      return -1; 
    }
  }
  else if (current_scheme == PSJF){
    // FOR PREEMPTIVE 
    if (peeker != NULL){
      peeker -> time_remaining = (peeker -> time_remaining) - (time - peeker-> pause); 
    }

    priqueue_offer(&q1, new_job); 

    if (peeker == NULL){
      // the core number 
      new_job -> start_time = time; 
      new_job -> head = 1;
      new_job -> pause = time;  
      return 0; 
    }
    else if (peeker != NULL && (peeker -> time_remaining > running_time)){
      new_job -> start_time = time; 
      new_job -> pause = time; 
      new_job -> head = 1; 
      // ODD CASE 
      if (peeker -> start_time == time){
        peeker -> start_time = -1; 
      }
      return 0; 
    }
    else {
      return -1; 
    }
  }
  else if (current_scheme == PPRI){
    priqueue_offer(&q1, new_job); 

    if (peeker == NULL){
      // the core number 
      new_job -> start_time = time; 
      new_job -> pause = time;  
      return 0; 
    }
    else if (peeker != NULL && (peeker -> priority > priority)){
      new_job -> start_time = time; 
      new_job -> pause = time; 
      new_job -> head = 1; 
      // ODD CASE 
      if (peeker -> start_time == time){
        peeker -> start_time = -1; 
      }
      return 0; 
    }
    else {
      return -1; 
    }    
  }
  else{
    //if (start1 ==1){
    //  start = time;
    //  start1 =0; 
    //}
    priqueue_offer(&q1, new_job);
    if (peeker == NULL){
      new_job -> start_time = time; x++;
      return 0;
    }
    return -1;    
  } 
  //return 69; 
}


/**
  Called when a job has completed execution.
 
  The core_id, job_number and time parameters are provided for convenience. You may be able to calculate the values with your own data structure.
  If any job should be scheduled to run on the core free'd up by the
  finished job, return the job_number of the job that should be scheduled to
  run on core core_id.
 
  @param core_id the zero-based index of the core where the job was located.
  @param job_number a globally unique identification number of the job.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled to run on core core_id
  @return -1 if core should remain idle.
 */
int scheduler_job_finished(int core_id, int job_number, int time)
{
  job_t * poll = priqueue_poll(&q1); 

  time_waiting += ((time - poll -> toa) - poll-> running_time); 
  turn_around_time += time - poll-> toa; 
  response_time += (poll -> start_time - (poll-> toa)); 

  free(poll); 

  job_t * peek = priqueue_peek(&q1); 

  // FOR NON PREEMPTIVE STUFF 
  if (current_scheme == FCFS || current_scheme == SJF || current_scheme == PRI){
    if (peek == NULL){
      return -1; 
    }
    else {
      peek -> head = 1; 
      peek -> start_time = time; 
      return peek -> job_number; 
    }
  }
  else if(current_scheme == PSJF){
    if (peek == NULL){
      return -1; 
    }
    else{
      peek -> pause = time; 
      peek -> head = 1; 
      if (peek -> start_time == -1){
        peek -> start_time = time;
      }
      return peek -> job_number; 
    }
  }
  else if (current_scheme == PPRI){
    if (peek == NULL){
      return -1; 
    }
    else{
      peek -> pause = time; 
      peek -> head = 1; 
      if (peek -> start_time == -1){
        peek -> start_time = time;
      }
      return peek -> job_number; 
    }
  }
  else {
    if (peek == NULL){return -1;}
    else {
      if (peek ->start_time == -1){peek -> start_time = time;}      
      return peek -> job_number; 
    }
  }
  //return 69; 
}


/**
  When the scheme is set to RR, called when the quantum timer has expired
  on a core.
 
  If any job should be scheduled to run on the core free'd up by
  the quantum expiration, return the job_number of the job that should be
  scheduled to run on core core_id.

  @param core_id the zero-based index of the core where the quantum has expired.
  @param time the current time of the simulator. 
  @return job_number of the job that should be scheduled on core cord_id
  @return -1 if core should remain idle
 */

int scheduler_quantum_expired(int core_id, int time)
{
  //if (end1 ==1){
  //    end = time;
  //    printf("YELL THIS IS THE VALUE OF END %d\n", end);
  //    printf("YELL THIS IS THE VALUE OF START %d\n", start);
     
  //    end1 =0;
  //    quantum = end- start; 
  //}
  //printf("###################THIS IS THE QUANTUM######################### %d\n", quantum);
  job_t * peek = priqueue_poll(&q1); 

  //if (core_id == 0){
    if (peek == NULL){return -1;}
    else {
      //peek -> time_remaining = peek -> time_remaining - quantum; 
      //peek = priqueue_poll(&q1);
      priqueue_offer(&q1, peek);
      peek = priqueue_peek(&q1);; 
      if (peek == NULL){return -1;}
      else{
        if (peek->start_time == -1){peek -> start_time = time;x++;}
        return peek -> job_number;}
    }


  //}
  return -1;
}


/**
  Returns the average waiting time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average waiting time of all jobs scheduled.
 */
float scheduler_average_waiting_time()
{
  return ((float)(time_waiting))/num_of_jobs;
}


/**
  Returns the average turnaround time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average turnaround time of all jobs scheduled.
 */
float scheduler_average_turnaround_time()
{
  return ((float)(turn_around_time))/num_of_jobs;
}


/**
  Returns the average response time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average response time of all jobs scheduled.
 */
float scheduler_average_response_time()
{return ((float)(response_time))/num_of_jobs;}


/**
  Free any memory associated with your scheduler.
 
  Assumptions:
    - This function will be the last function called in your library.
*/
void scheduler_clean_up(){}


/**
  This function may print out any debugging information you choose. This
  function will be called by the simulator after every call the simulator
  makes to your scheduler.
  In our provided output, we have implemented this function to list the jobs in the order they are to be scheduled. Furthermore, we have also listed 
  the current state of the job (either running on a given core or idle). For example, if we have a non-preemptive algorithm and job(id=4) has began 
  running, job(id=2) arrives with a higher priority, and job(id=1) arrives with a lower priority, the output in our sample output will be:

    2(-1) 4(0) 1(-1)  
  
  This function is not required and will not be graded. You may leave it
  blank if you do not find it useful.
 */
void scheduler_show_queue()
{
  int i; 
  for (i = 0; i < priqueue_size(&q1); i++){
    job_t * temp = priqueue_at(&q1, i); 
    printf("%d: running_time: %d head: %d\n ",temp->job_number, temp->running_time, temp->head);
  } 

  printf("The number of jobs %d the value of x %d", num_of_jobs, x); 

}

