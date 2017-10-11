#define BITS 32
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <fcntl.h>
#include <math.h> 
#include <bitset>
#include "hwtimer.h"
using namespace std;

void unweave(int i, int& x, int& y) {
	
	x = 0;
	y = 0;

	for(int k=0; k<BITS; k+=2){
		
		int maskx = 1 << k;
		int masky = 1 << (k+1);

		int masked_nx = i & maskx;
		int masked_ny = i & masky;

		int thexbit = masked_nx >> k;
		int theybit = masked_ny >> (k+1);

		x += thexbit*(int)pow((double)2, (k/2));
		y += theybit*(int)pow((double)2, (k/2));
	}

}

void MultA(int* C, int* A, int* B, int n) {
	
	if(n==2) { //base case
		C[0] += A[0]*B[0] + A[1]*B[2];
		C[1] += A[0]*B[1] + A[1]*B[3];
		C[2] += A[2]*B[0] + A[3]*B[2];
		C[3] += A[2]*B[1] + A[3]*B[3];
	} else {
		int sqn = n*n;
		MultA(C,             A,         B,         n/2);
		MultA(C+(sqn/4),     A,         B+(sqn/4), n/2);
		MultA(C+((3*sqn)/4), A+(sqn/2), B+(sqn/4), n/2);
		MultA(C+(sqn/2),     A+(sqn/2), B,         n/2);
		
		MultA(C+(sqn/2),     A+((3*sqn)/4), B+(sqn/2),     n/2);
		MultA(C+((3*sqn)/4), A+((3*sqn)/4), B+((3*sqn)/4), n/2);
		MultA(C+(sqn/4),     A+(sqn/4),     B+((3*sqn)/4), n/2);
		MultA(C,             A+(sqn/4),     B+(sqn/2),     n/2);
		return;
	}

}

void print(int* M, unsigned int n, ofstream & o) {

	int x, y;
	
	//create a new n by n matrix full of 0s
	int** matrix;
	
	// Allocate memory
  	matrix = new int*[n];
  	for (int i = 0; i < n; i++) {
    	matrix[i] = new int[n];
    }

	//for each index i in M, convert i into binary,
	//unweave this binary number into two binary
	//co-ordinates, set the value at (x,y) to the 
	//value of M at j
	int sqn = n*n;
	for(int j = 0; j < sqn; j++) {
		unweave(j, x, y);
		matrix[x][y] = M[j];
	}

	//print the resulting matrix
	for(int k = 0; k < n; k++) {
		o << "[ ";
		for(int l = 0; l < n; l++) {
			if(l == n-1) {
				o << matrix[l][k] << " ]\n";
			} else {
				o << matrix[l][k] << ", ";
			} 
		}
	}

	o << "\n\n";

	for (int x = 0; x < n; x++) {
    	delete [] matrix[x];
    	matrix[x] = 0;
	}
	delete [] matrix;
	matrix = 0;

}

int main(int argc, char* argv[]) {
		
		if (argc != 6) {
			cout << "Usage: parmatmult <num>" << endl;
			return 1;
		}
		
		ofstream testfile;
		testfile.open("sequential_tests.txt");
		
		hwtimer_t timer;
		initTimer(&timer);
		int i = 0;
		int j;
		int param;
		long time;
		
		while(i < 5) {
			
			j = i + 1;
			param = atoi(argv[j]);
		
			testfile << "Test run " << j << " on input of size " <<  param << ".\n" << endl;
			
			int p = param*param;
			int C[p], A[p], B[p];
			for(int q = 0; q < p; q ++) {
				C[q] = 0;
				A[q] = rand()%100;
				B[q] = rand()%100;
			}


			startTimer(&timer);
			MultA(C, A, B, param);
			stopTimer(&timer);
				
			time = getTimerNs(&timer);

			if(i < 3) {

				testfile << "Matrix A:\n\n";

				print(A, param, testfile);

				testfile << "Matrix B:\n\n";

				print(B, param, testfile);

				testfile << "Matrix C (A x B):\n\n";

				print(C, param, testfile);

			}
			
			testfile << "\nTotal time: " << time << "\n\n" << endl;
			
			initTimer(&timer);
			++i;
		
		}
		testfile.close();
		return 0;
		
};