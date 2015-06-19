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


int *core_status;
int max_core;
int max_round;
int total_tasks;
int total_response_time;
int total_waiting_time;
int total_turnaround_time;

typedef struct _job_t
{
  int job_id;
  int priority;
  int core_id;
  int round;
  int arrival_time;
  int start_time;
  int last_update_time;
  int run_time_already;
  int total_run_time;

}job_t;


int compare_fcfs(const void * a, const void * b)
{
  return ( ((job_t*)a)->arrival_time - ((job_t*)b)->arrival_time );
  //small--large
}

int compare_sjf(const void * a, const void * b)
{
  return ( (((job_t*)a)->total_run_time-((job_t*)a)->run_time_already) - (((job_t*)b)->total_run_time-((job_t*)b)->run_time_already) );
}


int compare_pri(const void * a, const void * b)
{
  return ( ((job_t*)a)->priority - ((job_t*)b)->priority );
  //small--large
}

int compare_rr(const void * a, const void * b)
{
  return ( ((job_t*)a)->round - ((job_t*)b)->round );
  //small--large
}

priqueue_t queue;

int preempt=0;

void scheduler_start_up(int cores, scheme_t scheme)
{
    total_tasks=0;
    total_waiting_time=0;
    total_turnaround_time=0;
    total_response_time=0;
    max_round=0;
    max_core = cores;
    core_status = malloc(cores*sizeof(int));
    for(int i=0;i<cores;i++)
      core_status[i]=-1;

    if(scheme == FCFS)
    priqueue_init(&queue, compare_fcfs);
    else if(scheme == PSJF)
    {
      preempt=1;
      priqueue_init(&queue, compare_sjf);
    }
    else if(scheme == SJF)
    {
      priqueue_init(&queue, compare_sjf);
    }
    else if(scheme == PPRI)
    {
      preempt=1;
      priqueue_init(&queue, compare_pri);
    }
    else if(scheme == PRI)
    {
      priqueue_init(&queue, compare_pri);
    }
    else if(scheme == RR)
    {
      priqueue_init(&queue, compare_rr);
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
  max_round++;
  total_tasks++;
  int status;

  job_t *new_job;
  new_job = malloc(sizeof(job_t));
  new_job->job_id = job_number;
  new_job->core_id = -1;
  new_job->priority = priority;
  new_job->round = max_round;
  new_job->arrival_time = time;
  new_job->last_update_time = -1;
  new_job->start_time = -10000000;
  new_job->run_time_already = 0;
  new_job->total_run_time = running_time;

  //update the running_time_already
  job_t *next_job;
  for(int i=0;i<priqueue_size(&queue);i++)
  {
    next_job= (job_t *)priqueue_at(&queue,i);
    if(next_job->core_id!=-1)
    next_job->run_time_already = next_job->run_time_already + time- next_job->last_update_time;
    next_job->last_update_time = time;
  }

  for(int i=0;i<max_core;i++) //If there are cores availble
  { 
      if(core_status[i]==-1)
      {

  
        new_job->core_id = i;
        new_job->last_update_time = time;
        new_job->start_time = time;
        status=priqueue_offer(&queue, new_job);
        core_status[i]=0;
        return i;
      }
  }

  int to_free_core;
  job_t *to_free_job;
  to_free_job=(job_t *)priqueue_at(&queue,max_core-1);
  to_free_core=to_free_job->core_id;
  
  status=priqueue_offer(&queue, new_job);
  if(preempt==1)
  {
    if(status < max_core)
    {
    to_free_job->core_id = -1;
    if(to_free_job->run_time_already==0)
      to_free_job->start_time=-1000;
    job_t *assign_job;
    assign_job = (job_t*) priqueue_at(&queue,status);
    assign_job->core_id = to_free_core;
    assign_job->last_update_time = time;
    assign_job->start_time = time;
    core_status[to_free_core]=0; 
    return to_free_core;
    }
    else
    return -1;   
  }
  else
  {
    return -1;
  }
  
  


  


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
  max_round--;
  job_t *next_job;
  for(int i=0;i<priqueue_size(&queue);i++)
  {
    next_job= (job_t *)priqueue_at(&queue,i);
    if(next_job->core_id!=-1)
    next_job->run_time_already = next_job->run_time_already + time- next_job->last_update_time;
    next_job->last_update_time = time;

  }

  int free_core;
  job_t *finished_job;  
  int index=0;
  for(index=0;index<priqueue_size(&queue);index++)
  {
    finished_job=(job_t *)priqueue_at(&queue, index);

    if(finished_job->job_id == job_number)
      break;
  }
  free_core = finished_job->core_id;
  core_status[free_core]=-1;
  for(int i=0;i<priqueue_size(&queue);i++)
  {
    next_job= (job_t *)priqueue_at(&queue,i);
    if(next_job->round>=finished_job->round)
      next_job->round--;

  }

  total_turnaround_time=total_turnaround_time+(time - finished_job->arrival_time);
  total_waiting_time=total_waiting_time+(time - finished_job->arrival_time -finished_job->total_run_time);
  total_response_time = total_response_time +(finished_job->start_time- finished_job->arrival_time);
  printf("##########responsetime#############%d\n",finished_job->start_time- finished_job->arrival_time );

  priqueue_remove_at(&queue,index);
  free(finished_job);

  int top_id;
  if(priqueue_size(&queue)>0)
  {
    
    for(int i=0;i<priqueue_size(&queue);i++)
    {
      next_job= (job_t *)priqueue_at(&queue,i);
      top_id = next_job->job_id;

      if(next_job->core_id == -1)
      {
        next_job->core_id=core_id;
        if(next_job->start_time<0)
          next_job->start_time=time;
        core_status[core_id]=0;
        return top_id;
      }

    }

    return -1;
    
  }
  else
  {

    return -1;
  }
  
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
  if(core_status[core_id]==-1)
    {
      printf("free cores exists\n");
      return -1;
    }
  job_t *next_job;
  if(max_core>=max_round)
  {
    printf("i can see ya\n");
    for(int i=0;i<priqueue_size(&queue);i++)
    {
      next_job = (job_t *)priqueue_at(&queue,i);
      if(next_job->core_id == core_id)
        return next_job->job_id;
    }
    printf("stderr,this is not happen\n");
    exit(-1);
  }

  printf("expeced here\n");
  int expired_job_index;
  int to_schedule_job_index;
  job_t *to_schedule_job;
  job_t *expired_job;
  
  for(int i=0;i<priqueue_size(&queue);i++)
  {
    next_job = (job_t *)priqueue_at(&queue,i);
    if(next_job->core_id==-1)
    {
      to_schedule_job_index = i;
      to_schedule_job=next_job;
      break;
    }
    
  }
  for(int i=0;i<priqueue_size(&queue);i++)
  {
    next_job= (job_t *)priqueue_at(&queue,i);

    printf("index is %d,round is %d\n",i,next_job->round );
    if(next_job->core_id!=-1)
    next_job->run_time_already = next_job->run_time_already + time- next_job->last_update_time;
    next_job->last_update_time = time;

    if(next_job->core_id==core_id)
      {
        next_job->core_id =-1;
        expired_job_index =i;
        expired_job=next_job;

      }

      next_job->round--;
    
  }


  priqueue_remove_at(&queue,expired_job_index);
  expired_job->round=max_round;
  expired_job->core_id=-1;
  if(to_schedule_job->start_time<0)
          to_schedule_job->start_time=time;
  to_schedule_job->core_id = core_id;
  priqueue_offer(&queue, expired_job);
  return to_schedule_job->job_id;
}


/**
  Returns the average waiting time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average waiting time of all jobs scheduled.
 */
float scheduler_average_waiting_time()
{
  return (float)(total_waiting_time)/(float)(total_tasks);
}


/**
  Returns the average turnaround time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average turnaround time of all jobs scheduled.
 */
float scheduler_average_turnaround_time()
{
  return (float)(total_turnaround_time)/(float)(total_tasks);
}


/**
  Returns the average response time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average response time of all jobs scheduled.
 */
float scheduler_average_response_time()
{
  return (float)(total_response_time)/(float)(total_tasks);
}


/**
  Free any memory associated with your scheduler.
 
  Assumptions:
    - This function will be the last function called in your library.
*/
void scheduler_clean_up()
{
  free(core_status);
  priqueue_destroy(&queue);

}


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
    int queue_size = priqueue_size(&queue);
    for(int i=0;i<queue_size;i++)
      printf("%d(%d) ", ((job_t *)priqueue_at(&queue, i))->job_id,((job_t *)priqueue_at(&queue, i))->core_id );
    printf("\n");
    for(int i=0;i<queue_size;i++)
      printf("%d #(%d %d)", ((job_t *)priqueue_at(&queue, i))->job_id,((job_t *)priqueue_at(&queue, i))->arrival_time,((job_t *)priqueue_at(&queue, i))->start_time);
    printf("\n");


    printf("core status\n");
    for(int i=0;i<max_core;i++)
      printf("core%d: %d ",i,core_status[i]);
     printf("\n");



}

