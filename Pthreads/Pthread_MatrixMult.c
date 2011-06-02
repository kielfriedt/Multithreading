/*
 *	Copyright (C) 2011  Kiel Friedt
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 Matrix multiplier using NxN matrices
 First attempt at POSIX threads. I produces the correct results and creates threads.
 */

#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <sys/time.h>

//size of matrix 
#define DIM 50

double a[DIM][DIM], b[DIM][DIM], c[DIM][DIM];

//globals for errors and timing
extern int errno;
struct timeval start, end;

//Struct for threads
typedef struct
{
	int             id;
	int             ProcessNum;
	int             dim;
	double	(*a)[DIM][DIM],(*b)[DIM][DIM],(*c)[DIM][DIM];
}               parm;


using namespace std;

//Prototyping
void MM(int ThreadNum, int ProcessNum, int n, double a[DIM][DIM], double b[DIM][DIM], double c[DIM][DIM]);
void * Worker(void *arg);

void MM(int ThreadNum, int ProcessNum, int n, double a[DIM][DIM], double b[DIM][DIM], double c[DIM][DIM])
{
	//looping for matrix mult using specific thread
	int i = ThreadNum;
	while (i<n) {
		//Matrix mult
#pragma omp parallel for
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				c[i][j] += a[i][k] * b[k][j];
		i+=ProcessNum;
	}
}

//Worker is used between the creation of threads and diving of the matrix.
void * Worker(void *arg)
{
	parm *p = (parm *) arg;
	MM(p->id, p->ProcessNum, p->dim, *(p->a), *(p->b), *(p->c));
	return NULL;
}


double MatMult(int n)
{
	int             ProcessNum, ThreadNum;
	double          t1, t2;
	// thread structs
	pthread_t      *threads;
	pthread_attr_t  pthread_custom_attr;
	parm           *arg;
	
	//create threads
	threads = (pthread_t *) malloc(n * sizeof(pthread_t));
	//erroring in thread creation
	if ( threads == NULL ){
		puts("Threads malloc failed");
		puts(strerror(errno));
	}
	else
	{
		pthread_attr_init(&pthread_custom_attr);
		arg=(parm *)malloc(sizeof(parm)*n);
		if (arg == NULL ){
			puts("arg malloc failed");
			puts(strerror(errno));
		}
		else
		{
            // reset the clock start timing
			gettimeofday(&start, NULL);
			
			//assign threads
            for (int i = 0; i < n; i++)
            {
                arg[i].id = i;
                arg[i].ProcessNum = n;
                arg[i].dim = DIM;
                arg[i].a = &a;
                arg[i].b = &b;
                arg[i].c = &c;
                pthread_create(&threads[i], &pthread_custom_attr, Worker, (void *)(arg+i));
            }
			
			//joining threads
            for (int i = 0; i < n; i++)
                pthread_join(threads[i], NULL);
			
			//stop timing
			gettimeofday(&end, NULL);
			
		}
		free(arg);
	}
	//return timing
	return (((end.tv_sec  - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000.0) + 0.5);
}


int main(){
	//hold data points
	//int threadnum[12];
	//double timing[12];
	
	//create matrix
	for (int i = 0; i < DIM; i++)
		for (int j = 0; j < DIM; j++)
		{
			a[i][j] = i * j;
			b[i][j] = i * j;
		}
	
	//holds time in msec
	double timesec = 0.0;
	cout << "Matrix Dimension: " << DIM<<"X"<<DIM<<"\n";
	//loop though # of threads
	for(int x = 12; x>0; x--)
	{
		timesec = MatMult(x);
		
		//store values for fast printout
		//threadnum[x-1] = x;
		//timing[x-1] = (timesec/1000.0);
		
		cout <<  "# of threads: " << x << " Time: " << (timesec/1000.0) <<" Seconds \n";
	}
	return 0;
}
