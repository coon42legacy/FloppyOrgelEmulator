#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

#include "../StackBasedFsm.h"

void playlist(StackBasedFsm_t* pFsm, FsmState* state, void* pArgs);

//
static void startPlayBack(StackBasedFsm_t* pFsm, FsmState* state, void* pArgs);
static void playing(StackBasedFsm_t* pFsm, FsmState* state, void* pArgs);
static void playbackAborted(StackBasedFsm_t* pFsm, FsmState* state, void* pArgs);
static void playbackFinished(StackBasedFsm_t* pFsm, FsmState* state, void* pArgs);

#endif // __PLAYLIST_H__

