/** @file shell.c */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "log.h"

/**
* Starting point for shell.
*/
int main()
{
    
    pid_t child = fork();
    if (child == -1) return EXIT_FAILURE;
    if (child)
    { /* I have a child! */
    int status;
    waitpid(child , &status ,0);
    return EXIT_SUCCESS;
    
    }
    else 
    { /* I am the child */
    
    struct _log_t *my_log;
    my_log = malloc(sizeof(log_t));
    log_init(my_log);
    
    while(1)
    {
        printf("loop...\n\n");
        //Current pid and working directory
        pid_t pid;
        char *cwd = NULL;
        cwd = malloc(1000);
        pid = getpid();
        strcpy(cwd,getcwd(NULL,0));
        printf("(pid=%d)%s$ ", pid, cwd);
        free(cwd);
        
        //read the input
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        read = getline(&line , &len, stdin);
        
        //input = string + next
        char string[read];
        char next[read];
        char *p_ptr;
        p_ptr = string;
        int i=0;
        for(i=0;i<read-1;i++)
        {
            
            if(line[i] == ' ' || (int)line[i] == 10)
            {
                string[i]='\0';
                for(int j=0;j<read-i-2;j++)
                {
                    next[j] = line[i+j+1];
                }
                next[read-i-2]= '\0';
                
                break;
            }
            string[i] = line[i];
            
        }
        string[read - 1] ='\0';
        char new_string[read-1];
        for(int i=0;i<read-1;i++)
        {
            new_string[i]=line[i];
        }
        new_string[read-1]='\0';

        
        
        
        if(strcmp(p_ptr,"exit") == 0)
        {

            free(line);
            break;
        }
        
        else if(strcmp(p_ptr,"cd")==0)
        {
            
            int c_status;
            c_status = chdir(next);
            if(c_status == -1)
            {
                printf("%s:No such file or directory\n", next);
            }
            
            log_append(my_log,new_string);
            
            
        }
        
        else if(strcmp(p_ptr,"!#") ==0)
        {
            for(int i=0;i<my_log->size;i++)
            {
                printf("%s\n",my_log->pointer[i] );
            }
        }
        
        else
        {
            char *exec_string;
            exec_string = string;
            char query_string[read-2];
            
            if(new_string[0] == '!')
            {
                for(int i=0;i < read - 1; i++)
                query_string[i]=new_string[i+1];
                
                exec_string = log_search(my_log,query_string);
                if(exec_string == NULL)
                printf("No Match\n");
                else
                printf("%s matches %s\n", query_string, exec_string);
            }
            printf("point 1\n");
            if(exec_string != NULL)
            {
                printf("point 2\n");
                if(exec_string[0]=='c' && exec_string[1]=='d' && exec_string[2]==' ')
                {
                    printf("i am here!!!\n");
                    char* new_next;
                    new_next = &exec_string[3];
                    
                    int c_status;
                    c_status = chdir(new_next);
                    if(c_status == -1)
                    {
                        printf("%s:No such file or directory\n", new_next);
                    }
                    
                    log_append(my_log,exec_string);   
                }

                else
                {
                    printf("not cd!\n");
                    log_append(my_log,exec_string);
                    
                    pid_t new_child = fork();
                    if(new_child == -1)
                    {
                        return EXIT_FAILURE;
                    }
                    else
                    {
                        if(new_child)
                        {
                            int status;
                            waitpid(new_child , &status ,0);
                            printf("Command executed by pid=%d\n", new_child);
                            
                        }
                        else
                        {
                            
                            execlp("bash","bash","-c",exec_string,(char*)0);
                            perror("exec");
                        }
                    }
                    
                }
            }
            
            
        }

        
        
        free(line);
        
    }

            
    
    log_destroy(my_log);
    
    free(my_log);
    
    
    }
    return 0;
}

