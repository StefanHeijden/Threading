/* 
 * Operating Systems {2INCO} Practical Assignment
 * Threaded Application
 *
 * Stefan van der Heijden 0910541
 * Sonya Zarkova 1034611
 *
 * Grading:
 * Students who hand in clean code that fully satisfies the minimum requirements will get an 8. 
 * Extra steps can lead to higher marks because we want students to take the initiative. 
 * Extra steps can be, for example, in the form of measurements added to your code, a formal 
 * analysis of deadlock freeness etc.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>          // for perror()
#include <pthread.h>

#include "uint128.h"
#include "flip.h"

int currentNoOfThreads = 0;
pthread_t my_threads[NROF_THREADS];


// Print for each bit whether it is flipped or not
// Used for testing
void printBits(){
	for(int i = 1; i <= NROF_PIECES; i = i + 1) {
		if (BIT_IS_SET (buffer[(i/128) + 1], i)) {
			printf ("bit %d is set\n", i);
		}else {
			printf ("bit %d is not set\n", i);
		}
	}
}


// Print all integers whose bit is set to 1
void printOutput(){
	for(int i = 1; i <= NROF_PIECES; i = i + 1) {
		if (BIT_IS_SET (buffer[(i/128) + 1], i)) {
			printf("%d\n",i); 
		}
	}
}


// Thread TO DO <----------------------------------
static void * 
my_thread (void * arg) {
    int *   argi; 
    int     i;      
    int *   rtnval;
    
    argi = (int *) arg;     // proper casting before dereferencing (could also be done in one statement)
    i = *argi;              // get the integer value of the pointer
    free (arg);             // we retrieved the integer value, so now the pointer can be deleted
    
    printf ("        %lx: thread started; parameter=%d\n", pthread_self(), i);
    
    sleep (1);
    
    // a return value to be given back to the calling main-thread
    rtnval = malloc (sizeof (int)); // will be freed by the parent-thread
    *rtnval = 42;           // assign an arbitrary value...
    return (rtnval);        // you can also use pthread_exit(rtnval);
}


// Create thread for current piece --> thread_test()
void createThread(int piece){
	//printf ("Created thread for %d\n", piece);
	// Create thread for current piece
	// TO DO <-----
	// Then try flip all bits
	flipAllBits(piece);
	//printBits(); // TEST <-----
	// Close thread
	// TO DO <-----
	// Lower number of current Threads
	currentNoOfThreads = currentNoOfThreads - 1;
	//printf ("Closed thread for %d\n", piece);
}


// Flip all bits for ceratin multiplier --> thread_mutex_test()
void flipAllBits(int piece){
	int pointer = 1;
	int currentPosition = piece;
	// For element in the list that has not been flipped:
	while(currentPosition <= NROF_PIECES){
		// Check mutexlock for piece
		// TO DO <-----
		// If free, lock it
		// TO DO <-----
		// then flip bit --> flitbit(piece)
		flipBit(currentPosition);
		// Then free lock
		// TO DO <-----
		// Then remove element from list
		// TO DO <-----
		// If not then go to next element in the list
		pointer = pointer + 1;
		currentPosition = pointer * piece;
	}
}


// Flip bit of given piece --> bit_test()
void flipBit(int currentPosition){
	if(BIT_IS_SET(buffer[(currentPosition/128) + 1], currentPosition % 128)){
		BIT_CLEAR(buffer[(currentPosition/128) + 1], currentPosition % 128);
	}else{
		BIT_SET(buffer[(currentPosition/128) + 1], currentPosition % 128);
	}
}


// TODO: start threads to flip the pieces and output the results
// (see thread_test() and thread_mutex_test() how to use threads and mutexes,
//  see bit_test() how to manipulate bits in a large integer)
int main (void)
{
	// Output the start
	//printBits();
	int currentPiece = 1;
	// Create thread for each piece
	while(currentPiece <= NROF_PIECES){
		// Create thread for current piece
		// but only if there are more threads available
		if(currentNoOfThreads < NROF_THREADS) {
			currentNoOfThreads = currentNoOfThreads + 1;
			createThread(currentPiece);
			currentPiece = currentPiece + 1;
		}else{
			// Wait for a thread then
			// To do wait for thread
			currentNoOfThreads = currentNoOfThreads + 1;
			createThread(currentPiece);
			currentPiece = currentPiece + 1;
		}
	}
	// Output the results
	printOutput();
    return (0);
}

