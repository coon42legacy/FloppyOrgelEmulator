#ifndef __LIVEMODE_H__
#define __LIVEMODE_H__

#include "../StackBasedFsm.h"

FsmState liveMode(StackBasedFsm_t* fsm);
FsmState liveReceiving(StackBasedFsm_t* fsm);

#endif // __LIVEMODE_H__

