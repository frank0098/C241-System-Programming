/** @file parmake.c */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <semaphore.h>
#include <sys/types.h>
#include "parser.h"
#include "queue.h"
#include "rule.h"


queue_t *rule_list;
queue_t *work_list;
pthread_mutex_t m_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t w_lock = PTHREAD_MUTEX_INITIALIZER;
int error_flag;
sem_t sitems;

void new_target_func (char *target)
{
	char* target_name = strdup(target);
	// target_name = malloc(sizeof(strlen(target)+1));
	// strcpy(target_name,target);
	rule_t* new_rule;
	new_rule = malloc(sizeof(rule_t));
	rule_init(new_rule);
	new_rule->target = target_name;
	queue_enqueue(rule_list,new_rule);
}

void new_dependency_func (char *target, char *dependency)
{
	char* dependency_name = strdup(dependency);
	// dependency_name = malloc(sizeof(strlen(dependency)+1));
	// strcpy(dependency_name,dependency);
	int rule_list_size;
	rule_list_size = queue_size(rule_list);
	rule_t* find_rule;
	for(int i=0; i<rule_list_size; i++)
	{
		find_rule = (rule_t*) queue_at(rule_list,i);
		if(strcmp(find_rule->target, target) == 0)
		{
			queue_enqueue(find_rule->deps,dependency_name);
			return;
		}
	}
	fprintf(stderr, "dep error\n");
	exit(1);
}
void new_command_func (char *target, char *command)
{

	char* new_command_name = strdup(command);
	// new_command_name = malloc(sizeof(strlen(command)+1));
	// strcpy(new_command_name,command);
	int rule_list_size;
	rule_list_size = queue_size(rule_list);
	rule_t* find_rule;
	for(int i=0; i<rule_list_size; i++)
	{
		find_rule = (rule_t*) queue_at(rule_list,i);
		if(strcmp(find_rule->target, target) == 0)
		{
			queue_enqueue(find_rule->commands,new_command_name);
			return;
		}
	}
	fprintf(stderr, "command error\n");
	exit(1);

}

void cleanup()
{
	int rule_list_size;
	rule_t* find_rule;
	rule_list_size = queue_size(rule_list);
	for(int i=0; i<rule_list_size; i++)
	{
		find_rule = (rule_t*) queue_at(rule_list,i);
		free(find_rule->target);
		char* tmp_char;
		for(int j=0; j<queue_size(find_rule->deps);j++)
		{
			tmp_char = (char*) queue_at(find_rule->deps,j);
			free(tmp_char);
		}
		for(int j=0; j<queue_size(find_rule->commands);j++)
		{
			tmp_char = (char*) queue_at(find_rule->commands,j);
			free(tmp_char);
		}

		rule_destroy(find_rule);
		free(find_rule);
	}

	queue_destroy(rule_list);

	queue_destroy(work_list);

	free(rule_list);

	free(work_list);
}

void show_rule_list()
{
	int rule_list_size;
	rule_t* find_rule;
	rule_list_size = queue_size(rule_list);
	for(int i=0; i<rule_list_size; i++)
	{
		find_rule = (rule_t*) queue_at(rule_list,i);
		printf("target%d: %s\n",i,find_rule->target );
		char* tmp_char;
		for(int j=0; j<queue_size(find_rule->deps);j++)
		{
			tmp_char = (char*) queue_at(find_rule->deps,j);
			printf("dependency%d: %s\n",j,tmp_char );
		}
		for(int j=0; j<queue_size(find_rule->commands);j++)
		{
			tmp_char = (char*) queue_at(find_rule->commands,j);
			printf("commands%d: %s\n",j,tmp_char );
		}

	}
	printf("\n");

}
void *parmake_worker()
{
	
	while(1)
	{

		if(queue_size(rule_list) == 0 && queue_size(work_list) == 0)
			break;

		rule_t* find_rule;
		char* next_command;
		next_command = NULL;
		pthread_mutex_lock(&m_lock);

		printf("the tid is %d\n", (unsigned int)pthread_self());
		show_rule_list();

		int index = 0;
		for(index=0;index<queue_size(rule_list);index++)
		{
			find_rule = (rule_t*)queue_at(rule_list,index);
			if(queue_size(find_rule->deps) == 0)
				{
					next_command = (char*) queue_dequeue(find_rule->commands);
					break;
				}
		}
		pthread_mutex_unlock(&m_lock);

		if(next_command != NULL)
		{	
			//printf("enqueue: %s\n\n",next_command );
			pthread_mutex_lock(&w_lock);
			queue_enqueue(work_list,next_command);
			sem_post(&sitems);
			pthread_mutex_unlock(&w_lock);

			pthread_mutex_lock(&m_lock);

			rule_t* tmp_rule;
			char* tmp;

			if(queue_size(find_rule->commands) == 0)
			{
				

				for(int i=0;i<queue_size(rule_list);i++)
				{
					tmp_rule = (rule_t*)queue_at(rule_list,i);

					for(int j=0;j<queue_size(tmp_rule->deps);j++)
					{
						if(strcmp(find_rule->target,(char*)queue_at(tmp_rule->deps,j))==0)
							{
								tmp = (char*) queue_remove_at(tmp_rule->deps,j);
								free(tmp);
							}
					}
				}

				tmp_rule = (rule_t*) queue_remove_at(rule_list,index);
				rule_destroy(tmp_rule);
				free(tmp_rule->target);
				free(tmp_rule);
			}
			pthread_mutex_unlock(&m_lock);
		}

		printf("the semaphore is %d\n", sitems);
		sem_wait(&sitems);
		pthread_mutex_lock(&w_lock);
		if(error_flag==1)
			{
				exit(1);
				break;
			}
		next_command = queue_dequeue(work_list);
		
		if(next_command != NULL)
		{
			int status;
			printf("Next Command is %s\n",next_command );
			printf("\n");
			status = system(next_command);
			free(next_command);
			if(status !=0 )
				{
					printf("parmake: ***Error\n");
					error_flag = 1;
					//exit(1);
				}
		}
		else
		{
			printf("wtf???the tid is %d\n", (unsigned int)pthread_self());
		}

		pthread_mutex_unlock(&w_lock);

	}
	

}
/**
 * Entry point to parmake.
 */
int main(int argc, char **argv)
{
	/*
	------PART1-------
	*/
	extern char *optarg;
	extern int optind;
	int c,err=0;
	int frflag=0;
	int jrflag=0;
	int threads_no=1;
	char* fname;
	char* jname;
	fname = NULL;
	jname = NULL;
	char** target;

	static char usage[] = "parmake [ -f makefile ] [ -j threads ] [ targets ]\n";
	while ((c = getopt(argc, argv, "f:j:")) != -1)
	switch (c) {
		case 'f':
		if (frflag == 1) {
		printf("warning:  -f is set multiple times\n");
		}
		frflag = 1;
		fname = optarg;
		break;
		case 'j':
		if (jrflag == 1) {
		printf("warning:  -j is set multiple times\n");
		}
		jrflag = 1;
		jname = optarg;
		break;
		case '?':
		err = 1;
		break;

	}
	printf("argc is %d ,optind %d\n",argc,optind );

	
	if(err)
		{
			fprintf(stderr, usage, argv[0]);
			exit(-1);
		}
	else if(jname!=NULL)
		{
			threads_no = atoi(jname);  //---------------wtf
			if(threads_no == 0)
			{
				fprintf(stderr, usage, argv[0]);
				exit(-1);
			}
		}
	else if(optind > argc -1 )
		{
			fprintf(stderr, usage, argv[0]);
			exit(-1);
		}
	if (fname == NULL)
	{
        if (access("makefile", R_OK) == 0)
        	{
        		fname = "makefile";
        	}
        else if (access("Makefile",R_OK) == 0)
        	{
        		fname = "Makefile";
        	}
        else return -1;
    }

	/*
	------PART2-------
	*/
	target = argv+optind;
	rule_list = malloc(sizeof(queue_t));

	queue_init(rule_list);

	work_list = malloc(sizeof(queue_t));
	queue_init(work_list);

	parser_parse_makefile(fname,argv+optind,new_target_func,new_dependency_func,new_command_func);

	//show_rule_list();

	/*
	------PART3-------
	*/

	error_flag = 0;
	sem_init(&sitems,0,0);
	pthread_t threads[threads_no];
	int rc;
	for(int i=0; i<threads_no; i++)
	{

		rc=pthread_create(&threads[i],NULL,parmake_worker,NULL);
		if(rc)
        {
            printf("thread creation fails!,error id %d\n",rc);
            exit(-1);
        }
	}
	for(int j=0;j<threads_no;j++)
    {
        pthread_join(threads[j],NULL);
    }

	cleanup();

	return 0; 
}
