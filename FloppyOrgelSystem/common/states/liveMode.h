#ifndef __LIVEMODE_H__
#define __LIVEMODE_H__

#include "../StackBasedFsm.h"

void liveMode(StackBasedFsm_t* pFsm, FsmState* state, void* pArgs);
void liveReceiving(StackBasedFsm_t* pFsm, FsmState* state, void* pArgs);

#endif // __LIVEMODE_H__

