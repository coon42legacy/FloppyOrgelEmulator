#ifndef __LIVEMODE_H__
#define __LIVEMODE_H__

#include "../StackBasedFsm.h"

void liveMode(FsmState* state, void* pArgs);
void liveReceiving(FsmState* state, void* pArgs);

#endif // __LIVEMODE_H__

