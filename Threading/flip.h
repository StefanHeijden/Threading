/* 
 * Operating Systems {2INC0} Practical Assignment
 * Threaded application
 *
 * Joris Geurts
 * 
 */

#ifndef _FLIP_H_
#define _FLIP_H_

#include "uint128.h"
 
/**
 * NROF_PIECES: size of the board; number of pieces to be flipped
 */
#define NROF_PIECES			3

/**
 * NROF_THREADS: number of threads that can be run in parallel
 * (value must be between 1 and ... (until you run out of system resources))
 */
#define NROF_THREADS		10

// create a bitmask where bit at position n is set
#define BITMASK(n)          (((uint128_t) 1) << (n))

// check if bit n in v is set
#define BIT_IS_SET(v,n)     (((v) & BITMASK(n)) == BITMASK(n))

// set bit n in v
#define BIT_SET(v,n)        ((v) =  (v) |  BITMASK(n))

// clear bit n in v
#define BIT_CLEAR(v,n)      ((v) =  (v) & ~BITMASK(n))

/**
 * buffer[]: datastructure of the pieces; each piece is represented by one bit
 */
static uint128_t			buffer [(NROF_PIECES/128) + 1];

#endif
