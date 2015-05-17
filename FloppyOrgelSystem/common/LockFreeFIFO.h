#ifndef __LOCK_FREE_FIFO_H
#define __LOCK_FREE_FIFO_H

#define RING_BUFFER_SIZE 256
typedef struct {
  char ringBuffer[RING_BUFFER_SIZE];
  int wptr;
  int rptr;
} LockFreeFIFO_t;

int getRingBufferDistance(LockFreeFIFO_t* lff);
void writeToRingBuffer(LockFreeFIFO_t* lff, char b);
char readFromRingBuffer(LockFreeFIFO_t* lff);

#endif // __LOCK_FREE_FIFO_H
