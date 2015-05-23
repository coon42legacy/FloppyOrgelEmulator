#include "../hal/hal_misc.h" // TODO: remove after debug!
#include "LockFreeFIFO.h"

int getRingBufferDistance(LockFreeFIFO_t* lff) {
  return lff->rptr > lff->wptr ? lff->rptr - lff->wptr : lff->rptr - lff->wptr + RING_BUFFER_SIZE;
}

void writeToRingBuffer(LockFreeFIFO_t* lff, char b) {
  if (getRingBufferDistance(lff) == 1) {
    hal_printfWarning("Ring buffer overflow!\n\r"); // TODO: remove after debug!
    return;
  }

  lff->ringBuffer[lff->wptr] = b;
  lff->wptr = (lff->wptr + 1) % RING_BUFFER_SIZE;
}

char readFromRingBuffer(LockFreeFIFO_t* lff) {
  if (getRingBufferDistance(lff) == RING_BUFFER_SIZE) {
    hal_printfWarning("Ring buffer underflow!\n\r"); // TODO: remove after debug!
    return 0;
  }

  char ret = lff->ringBuffer[lff->rptr];
  lff->rptr = (lff->rptr + 1) % RING_BUFFER_SIZE;
  return ret;
}
