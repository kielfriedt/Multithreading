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
 This is my first attempt at OpenMP. I maybe be implemented wrong but it does produce additional threads.
 
 function call format:
 MatSolve(double A[], double B[], int n)
 A,B = double array
 n = # of unknowns
 
 This does not implement pivoting. AKA no zero elements in the diagonal
 
 Prints out systems solutions. This may be correct or not.... Not my fault. 
 
 Oh and it might be numericall unstable in some rare cases... 
 
 "Software is like sex: it's better when it's free."
 --Linus Torvalds
 */

#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <new>
#include <omp.h>
#include <sys/time.h>

using namespace std;
/*
 3x3
 double a[] = {2,1,3,2,6,8,6,8,18};
 double b[] = {1,3,5};
 */

//time structs
struct timeval start, end;

double* MatSolve(double A[], double B[], int n)
{
	int j = 0,k = 0,i = 0, counter = 0;
	double x = 0, sum = 0;
	
	//create A,B matrix using parameters
	double** a = new double*[n];
	double* b = new double[n];
	for ( int j = 0; j < n; j++ )
		a[j] = new double[n];
	
	
#pragma omp parallel sections
	{
#pragma omp parallel for private(k)
		//fill martices
		for (j = 0; j < n; j++){
			b[j] = B[j];
			for (k = 0; k < n; k++)
				a[j][k] = A[j+k+counter];
			counter+=(n-1);
		}
		
		// convert to upper triangular form
#pragma omp parallel for private(i)
		for (k=0;k<n-1;k++){
			if ( fabs(a[k][k])>=1.e-6)
			{
#pragma omp parallel for private(j)
				for (i=k+1;i<n;i++)
				{
					x = a[i][k]/a[k][k];
					for (j=k+1;j<n;j++)
						a[i][j] = a[i][j] -a[k][j]*x;
					b[i] = b[i] - b[k]*x;
				}
			}
			else
			{
				//Zero element found in diagonal
				cout << "zero pivot element found.\n";
				exit(1);
			}
			
		}
		// backwards subsitution 
		b[n-1]=b[n-1]/a[n-1][n-1];
#pragma omp parallel for private(j)
		for (i = n-2; i >= 0; i--)
		{
			sum = b[i];
			for (j = i+1; j < n; j++)
				sum = sum - a[i][j]*b[j];
			b[i] = sum/a[i][i];
		}
	}
	//deleted memory and return b as S  Crazy huh???
	for(j = 0; j < n; j++)
		delete[] a[j];
	delete[] a;
	a = NULL;
	return b;
}

int main()
{
	//change this to number of systems
	int NumElements = 200;
	
	//created to store answer matrix
	double *s = new double[NumElements]; 
	
	cout.precision(16);
	
	for(int i = 0; i<200; i++){
		// reset the clock start timing
		gettimeofday(&start, NULL);
		
		s = MatSolve(a, b, NumElements);
		
		//stop timing
		gettimeofday(&end, NULL);
		
		//time in msecs
		double finishingtime = (((end.tv_sec  - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000.0) + 0.5);
		cout << "Time: " << (finishingtime/1000)<< "\n"; // convert to seconds
		
		cout << "S: \n"; 
		for (int x = 0; x<NumElements; x++) 
			cout<< s[x]<<"\n";
	}
	return 0;
}