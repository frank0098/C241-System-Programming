/** @file msort.c */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 200000

typedef struct seg_stack_struct
{
    
    int* seg[MAXSIZE];
    int stack_size;
    
}seg_stack_struct;

typedef struct thread_data_struct
{
    
    int* pointer;
    int seg_size;
    int seg_size_2;
    int merge_length;
    
}thread_data_struct;

int cmpfunc (const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}

void *qsort_func(void *input_data)
{
    thread_data_struct *thread_data;
    thread_data = (thread_data_struct*) input_data; 
    qsort(thread_data->pointer,thread_data->seg_size,sizeof(int),cmpfunc);
    printf("Sorted %d elements.\n",thread_data->seg_size );
    pthread_exit(NULL);
}

void *merge_sort(void *input_data)
{
    thread_data_struct *thread_data;
    thread_data = (thread_data_struct*) input_data;

    int seg_size,seg_size_2;
    seg_size = thread_data->seg_size;
    seg_size_2 = thread_data->seg_size_2;

    int *head_1;
    int *head_2;
    int *start_point;
    start_point = thread_data->pointer;
    head_1 = start_point;
    head_2 = head_1 + seg_size;
    
    int tmp_data[seg_size+seg_size_2];
    int duplicate_count = 0;
    int i;
    for(i=0; i<seg_size+seg_size_2;i++)
    {
        if(head_1 - start_point == seg_size && head_2 - start_point == seg_size_2 + seg_size)
        break;
        
        if(head_1 - start_point == seg_size)
        {
            tmp_data[i] = *head_2;
            head_2++;
        }
        else if(head_2 - start_point == seg_size_2 + seg_size)
        {
            tmp_data[i] = *head_1;
            head_1++;
        }
        else
        {
            if(*head_1 > *head_2)
            {
                tmp_data[i] = *head_2;
                head_2++;
            }
            else if(*head_1 < *head_2)
            {
                tmp_data[i] = *head_1;
                head_1++;
            }
            else
            {
                tmp_data[i] = *head_1;
                head_1++;
                head_2++;
                duplicate_count++;
            }
        }
        
    }
    thread_data->merge_length = i;
    int *replace;
    replace = start_point;
    for(int j=0;j<i;j++)
    {
        *replace = tmp_data[j];
        replace++;
    }
    fprintf(stderr, "Merged %d and %d elements with %d duplicates.\n", seg_size, seg_size_2, duplicate_count);
    pthread_exit(NULL);
}

void merge(int* start_point, int input_ct, int head_array[],int seg)
{   
    

    if(seg == 1)
    {
        for(int i=0;i<input_ct;i++)
        printf("%d\n",start_point[i] );
        return;
    }

    int tmp_seg;
    tmp_seg = seg/2;
    if(tmp_seg == 0)
    tmp_seg = 1;
    
    int values_per_segment;

    if (input_ct % seg == 0)
    values_per_segment = input_ct / seg;
    else
    values_per_segment = (input_ct / seg) + 1;
    
    //printf("count is %d value per seg%d\n",input_ct,values_per_segment );
    
    pthread_t threads[tmp_seg];
    thread_data_struct *record[tmp_seg];
    int *head_pointer;
    head_pointer = start_point;
    int rc;
    
    for(int i=0; i<tmp_seg; i++)
    {
        thread_data_struct* thread_data;
        thread_data = malloc(sizeof(thread_data_struct));
        record[i] = thread_data;
        thread_data->pointer = head_pointer;
        thread_data->seg_size = head_array[2*i];
        thread_data->seg_size_2 = head_array[2*i+1];
        //printf("the size is %d\n",thread_data->seg_size_2 );
        rc=pthread_create(&threads[i],NULL,merge_sort,(void*) thread_data);
        
        if(rc)
        {
            printf("thread creation fails!,error id %d\n",rc);
            exit(-1);
        }
        head_pointer = head_pointer + thread_data->seg_size + thread_data->seg_size_2;
    }
    
    for(int j=0;j<tmp_seg;j++)
    {
        pthread_join(threads[j],NULL);
    }
    
    int *last_seg;
    int last_seg_size;
    last_seg_size = head_array[seg-1];
    last_seg = start_point + input_ct - head_array[seg-1];

    for(int t=0;t<tmp_seg;t++)
    {
        //printf("seg_size %d,segsize_2 %d,mergelen %d\n",record[t]->seg_size, record[t]->seg_size_2 ,record[t]->merge_length);
        input_ct = input_ct - (record[t]->seg_size + record[t]->seg_size_2 - record[t]->merge_length) ;
    }

    int tmp_data[input_ct];
    head_pointer = start_point;
    int seg_count=0;
    int tmp_size=0;

    //printf("input_ct%d,segment count%d\n",input_ct,seg );
    for(int k=0;k<input_ct;k++)
    {
        
        tmp_size++;
        tmp_data[k] = *head_pointer;
        head_pointer++;
        
            if(record[seg_count]->merge_length == tmp_size)
            {
                if(seg_count < tmp_seg-1)
                {
                    seg_count++;
                    head_pointer = record[seg_count]->pointer;
                    tmp_size=0;
                }
                else
                {
                    head_pointer = last_seg;
                    tmp_size = 0;
                }
            }
    }

    int new_seg;
    if(seg%2==0)
        new_seg = seg/2;
    else
        new_seg = seg/2+1;

    int new_head_array[new_seg];
    for(int i=0;i<new_seg;i++)
    {
        if(i == new_seg-1 && seg%2!=0)
            new_head_array[i] = last_seg_size;
        else
            new_head_array[i] = record[i]->merge_length;

    }

    seg=new_seg;

    for(int t=0;t<tmp_seg;t++)
    {
        free(record[t]);
    }
    head_pointer = start_point;
    for(int i=0;i<input_ct;i++)
    {
        *head_pointer = tmp_data[i];
        head_pointer++;
    }

    merge(start_point,input_ct,new_head_array,seg);
    
}

int main(int argc, char **argv)
{
    
    //*filename = "/Users/Frank/Desktop/CS241-System-Programming/mp4/example1.txt";
    
    printf("seg fault\n");
    if(argc != 3)
    {
        fprintf(stderr, "Wrong Input Parameter\n");
        return 1;
    }
    
    int segment_count;
    
    segment_count = atoi(argv[1]);
    
    if(segment_count <= 0)
    {
        fprintf(stderr, "Too Few Inputs\n");
        return 1;
    }
    if(segment_count > MAXSIZE)
    {
        fprintf(stderr, "Holy! That's too big\n");
        return 1;
    }
    char *filename;
    filename = argv[2];
    
    FILE *file = fopen(filename,"r");
    if(file == NULL)
    {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    size_t buffer_size = 200;
    char *buffer = malloc(buffer_size * sizeof(char));
    int input_ct = 0;
    int *input_data;
    
    int max_number = MAXSIZE;
    input_data = malloc(sizeof(int) * max_number);

    while( getline(&buffer, &buffer_size, file) != -1)
    {
        
        input_data[input_ct] = atoi(buffer);
        input_ct++;
        //printf("%d: %s", input_ct, buffer);
        if(input_ct >= MAXSIZE)
        {
        fprintf(stderr, "Holy! That's file is too big\n");
        fclose(file);
        free(buffer);
        free(input_data);
        return 1;
        }
        
    }
    fflush(stdout);
    fclose(file);
    free(buffer);
    
    
    int values_per_segment; /**< Maximum number of values per segment. */
    
    
    values_per_segment = (input_ct / segment_count);
    
    int* input_data_ptr;
    input_data_ptr = input_data;
    
    seg_stack_struct seg_stack;
    seg_stack.stack_size = segment_count;
    for(int i=0;i<segment_count;i++)
    {
        seg_stack.seg[i] = input_data_ptr;
        input_data_ptr = input_data_ptr + values_per_segment;
    }
    

    //QSORT
    pthread_t threads[segment_count];
    
    int sort_elements;
    int rc;
    
    thread_data_struct* record[segment_count];
    for(int j=0;j<segment_count;j++)
    {
        sort_elements = values_per_segment;
        if(j == segment_count -1)
        sort_elements = input_ct - (segment_count-1)*values_per_segment;
        
        
        thread_data_struct* thread_data;
        thread_data = malloc(sizeof(thread_data_struct));
        thread_data->pointer = seg_stack.seg[j];
        thread_data->seg_size = sort_elements;
        record[j] = thread_data;
        
        rc = pthread_create(&threads[j],NULL,qsort_func,(void*)thread_data);
        
        if(rc)
        {
            printf("thread creation fails!,error id %d\n",rc);
            exit(-1);
        }
        
    }
    
    for(int t=0;t<segment_count;t++)
    {
        pthread_join(threads[t],NULL);
    }
    
    for(int t=0;t<segment_count;t++)
    {
        free(record[t]);
    }

    int head_array[segment_count];
    
    for(int i=0;i<segment_count;i++)
    {
        head_array[i]=values_per_segment;
        if(i == segment_count -1)
            head_array[i]=input_ct - (segment_count-1)*values_per_segment;
    }

    merge(input_data,input_ct,head_array,segment_count);

    free(input_data);
    
    return 0;
}
