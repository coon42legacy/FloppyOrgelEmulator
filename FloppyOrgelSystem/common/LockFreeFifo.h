#ifndef __LOCK_FREE_FIFO_H
#define __LOCK_FREE_FIFO_H

#include <stdbool.h>
#include "config.h"


typedef struct {
  char ringBuffer[RING_BUFFER_SIZE];
  int wptr;
  int rptr;
} LockFreeFIFO_t;

bool ringBufferDataAvailable(LockFreeFIFO_t* lff);
void writeToRingBuffer(LockFreeFIFO_t* lff, char b);
char readFromRingBuffer(LockFreeFIFO_t* lff);

#endif // __LOCK_FREE_FIFO_H
