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
#include <unistd.h>

#include "uint128.h"
#include "flip.h"

int currentNoOfThreads = 0;
int lastThread = 0;
int threadToWaitFor = 0;
pthread_t my_threads[NROF_THREADS];
pthread_mutex_t lock;

// create a bitmask where bit at position n is set
#define BITMASK(n)          (((uint128_t) 1) << (n))

// check if bit n in v is set
#define BIT_IS_SET(v,n)     (((v) & BITMASK(n)) == BITMASK(n))

// set bit n in v
#define BIT_SET(v,n)        ((v) =  (v) |  BITMASK(n))

// clear bit n in v
#define BIT_CLEAR(v,n)      ((v) =  (v) & ~BITMASK(n))


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
		if (!BIT_IS_SET (buffer[(i/128) + 1], i)) {
			printf("%d\n",i); 
		}
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


// Flip all bits for ceratin multiplier --> thread_mutex_test()
static void * 
flipAllBits(void * arg){
	int *   argi; 
    int     piece;      
    
    argi = (int *) arg;     // proper casting before dereferencing (could also be done in one statement)
    piece = *argi;          // get the integer value of the pointer
    free (arg);             // we retrieved the integer value, so now the pointer can be deleted
    
    
	int pointer = 1;
	int currentPosition = piece;
	// For element in the list that has not been flipped:
	while(currentPosition <= NROF_PIECES){
		// Check mutexlock for piece
		//printf("Flip bit: %d, %d\n", piece, currentPosition);
		pthread_mutex_lock(&lock);
		// then flip bit --> flitbit(piece)
		flipBit(currentPosition);
		// Then free lock
		pthread_mutex_unlock(&lock);
		// Then go to the next one
		pointer = pointer + 1;
		currentPosition = pointer * piece;
	}
    return (NULL);        // you can also use pthread_exit(rtnval);
}


// Create thread for current piece --> thread_test()
void createThread(int piece){
	int *       parameter;
    
    // parameter to be handed over to the thread
    parameter = malloc (sizeof (int));  // memory will be freed by the child-thread
    *parameter = piece;        // assign value of current piece
    
	// Create thread for current piece and try flip all bits
	pthread_create (&my_threads[lastThread], NULL, flipAllBits, parameter);
	
	// Increase number of current Threads
	currentNoOfThreads = currentNoOfThreads + 1;
}


// Wait for a thread
void waitForThread(){
	// Join the next thread
	pthread_join (my_threads[threadToWaitFor], NULL);  
	
	// Lower number of current Threads
	currentNoOfThreads = currentNoOfThreads - 1;
	
	// Obtain next thread to wait for
	threadToWaitFor = threadToWaitFor + 1;
	if(threadToWaitFor >= NROF_THREADS || 
				threadToWaitFor > currentNoOfThreads) {
		threadToWaitFor = 0;
	}
}


// Wait for all threads to terminate
void waitForAllThread(){
	for(int i = 0; i < currentNoOfThreads; i = i + 1){
		pthread_join (my_threads[i], NULL);  
	}
}


// Main thread
int main (void)
{
	// Create mutex lock
	if (pthread_mutex_init(&lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1;
    } 
    
	int currentPiece = 2;
	// Create threads for all pieces < NROF_PIECES < NROF_THREADS
	while(currentPiece <= NROF_PIECES && currentNoOfThreads < NROF_THREADS){
		lastThread = currentNoOfThreads;
		createThread(currentPiece);
		currentPiece = currentPiece + 1;
	}
	// Create thread for each remaining piece, each time a thread is 
	// released
	while(currentPiece <= NROF_PIECES){
		// Wait for a thread to terminate and get its ID
		lastThread = threadToWaitFor;
		waitForThread();
		// Then create a single thread for next piece
		createThread(currentPiece);
		currentPiece = currentPiece + 1;
	}
	waitForAllThread();
	pthread_mutex_destroy(&lock);
	// Output the results
	printOutput();
    return (0);
}

