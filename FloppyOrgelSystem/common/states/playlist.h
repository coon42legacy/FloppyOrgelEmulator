#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

#include "../StackBasedFsm.h"

void playlist(FsmState* state, void* pArgs);
static void startPlayBack(FsmState* state, void* pArgs);
static void playing(FsmState* state, void* pArgs);
static void playbackAborted(FsmState* state, void* pArgs);
static void playbackFinished(FsmState* state, void* pArgs);

#endif // __PLAYLIST_H__

