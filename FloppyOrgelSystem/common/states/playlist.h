#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

#include "../StackBasedFsm.h"

FsmState playlist(StackBasedFsm_t* fsm);
static FsmState startPlayBack(StackBasedFsm_t* fsm);
static FsmState playing(StackBasedFsm_t* fsm);
static FsmState playbackAborted(StackBasedFsm_t* fsm);
static FsmState playbackFinished(StackBasedFsm_t* fsm);

#endif // __PLAYLIST_H__

