/** @file part1.c */

/*
 * Machine Problem #1
 * CS 241
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "mp1-functions.h"

/**
 * (Edit this function to print out the ten "Illinois" lines in mp1-functions.c in order.)
 */
int main()
{

	first_step(81);

	int t2=132;
	int *p2;
	p2=&t2;
	second_step(p2);

	int r3[15];
	 r3[0]=8942;
	void *p3;
	void *q3;
	q3=&r3;
	p3=&q3;
	double_step(p3);

	int *p4;
	p4=0;
	strange_step (p4);

	void *p5;
	char empty5[5];
	empty5[3]=0;
	p5=&empty5;
	//xxx v=&empty[3];
	//printf("the number is %c\n",v[3]);
	//printf("the number is %d\n",((char *)v)[3]);
	empty_step(p5);

	char empty6[5];
	empty6[3]='u';
	void *p6;
	void *q6;
	q6=&empty6;
	p6=q6;
	two_step(p6,q6);

	char x7=0;
	char y7=2;
	char z7=4;
	three_step(x7,y7,z7);

	void *p8;
	char empty8[4];
	empty8[1]=0;
	empty8[2]=8;
	empty8[3]=16;
	p8=&empty8;
	step_step_step(p8,p8,p8);

	void *p9;
	int q9=1;
	p9=&q9;
	it_may_be_odd(p9,q9);

	// void* p10;
	// void* q10;
	// void* r10;
	// char empty10[1];
	// int t10=1049;
	// empty10[0]=1;
	// p10=&empty10;
	// q10=&t10;
	// r10=&q10;
	// the_end(q10,q10);
	// char q=t10;
	// printf("the char is %d\n",((char *)q10)[0]);
	char temp[4] = {0,0,0,0};
	temp[0] = 1;
	temp[1] = 2;
	the_end(temp,temp);
	// int * ten_one; 
	// int * ten_two; 
	
	// ten_two = malloc(sizeof(int));
	
	// ten_one = ten_two; 
	
	// *ten_two = 2047; 
	
	// printf("the char is %d\n",((char *)ten_two)[0]);
	// the_end(ten_two, ten_one);	
	// free(ten_two);
	








	return 0;
}
