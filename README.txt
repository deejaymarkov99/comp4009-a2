COMP 4001 Assignment 2

"The goal of this assignment is to write a CILK++ program to multiply two matrices. You should implement the "no temp array" version shown below, as discussed in class. That is, recursive "MultA" calls should not allocate new temporary arrays. 

http://images.slideplayer.com/25/7698907/slides/slide_24.jpg

Make sure you implement the "shuffled row major order / z-order" (described here: http://en.wikipedia.org/wiki/Z-order_curve) of matrices discussed in class, and thereby avoid copying of sub-matrices by passing all submatrices as "pointer,length" values. Note that, all generated input/output matrices are to be displayed in standard row-wise order, so you will need to write a function to print a Z-order matrix (this will come in handy for debugging as well)."

All code by Damjan Markovic, based on cilk_template code provided.

The code seg-faults when attempting to perform MultA on a square matrix with size 1024 or greater. I suspect this to have
something to do with the way I implemented the "unweave" function. When I change the "BITS" definition to be 64 it also seg-faults.

I ran the code on my machine which has only 2 cores. 

The time it took to run the code for n = 512 sequentially was 384813493 ns, in parallel 911528929 ns.

So the speed up runnning on 2 cores appears to be:

S(2) = 384813493/911528929 = 0.42216267718

This is horrendous. A parallel slow-down in fact. In order to get some sort of a speed up I would need to run this code on a machine with at least 4 cores. As is I am overloading the memory of each CPU.

I split the sequential and parallel programs into 2 seperate .cpp files (parmatmult.cpp and seqmatmult.cpp respectively).

To compile:

Execute the following commands,

g++ parmatmult.cpp -O3 -fcilkplus -o par
g++ seqmatmult.cpp -O3 -o seq


To run:

To achieve the results I got in parallel_tests1.txt and sequential_tests1.txt these are the run commands I executed,

./par 4 8 16 256 512
./seq 4 8 16 256 512

So you pass 5 parameters to the executables. The first 3 are for small values of n which can be realistically
written to a file, the last 2 are for large values of n, n is assumed to be a power of 2.



