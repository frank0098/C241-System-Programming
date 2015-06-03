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
    else { /* I am the child */
    while(1)
    {
        
        
        pid_t pid;
        char *cwd = NULL;
        pid = getpid();
        cwd=getcwd(NULL,0);
        printf("(pid=%d)%s$ ", pid, cwd);
        free(cwd);
        
        
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        read = getline(&line , &len, stdin);
        
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
        
        
        
        
        if(strcmp(p_ptr,"exit") == 0)
        {
            break;
        }
        
        else if(strcmp(p_ptr,"cd")==0)
        {
            
            int c_status;
            c_status = chdir(next);
        }
        
        else
        {
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
                    //return EXIT_SUCCESS;
                    
                }
                else
                {
                    
                    execlp("bash","bash","-c",line,(char*)0);
                    
                    perror("exec");
                }
            }
        }
        
        
        
        free(line);
    }
    
    
    }
    return 0;
}

