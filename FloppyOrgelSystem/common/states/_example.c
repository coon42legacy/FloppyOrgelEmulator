#include "../StackBasedFsm.h"
#include "../../hal/hal_misc.h"

#include "_example.h"

//--------------------------------------------------------------------------------------------------------------
// Example state implementation
//
// This example shows, how to implement a state.
// - A state has always its own file, which consists of a collection of callbacks.
// - At the end of the file there is always an initialization function, which activates the state by setting
//   the callbacks.
// - The context struct can be used to store local variables. If this state gets called multiple times in a
//   state chain, an array of contexts can be used, to get an own context for each state.
//--------------------------------------------------------------------------------------------------------------

static struct {
  // put your local variables here!
  int some_local_variable;

} context;

static void onAction(StackBasedFsm_t* pFsm) {
  hal_printf("example::onAction()");

  // This function is called, when the player presses the action button on the game pad.
  // On the NES game pad this is the 'A' button.
}

static void onBack(StackBasedFsm_t* pFsm) {
  hal_printf("example::onBack()");

  // This function is called, when the player presses the back button on the Gamepad.
  // On the NES game pad this is the 'B' button.

  // In most cases you want to go to the previous screen. This is done by executing 'fsmPop(context.pFsm);'
  fsmPop(pFsm);
}

static void onEnter(StackBasedFsm_t* pFsm, void* pParams) {
  hal_printf("example::onEnter()");

  // This function is called, when the state is entered for the first time.
}

static void onReenter(StackBasedFsm_t* pFsm) {
  hal_printf("example::onReenter()");

  // This function is called, when the user pressed the back button on a higher state and lands back in this 
  // state again.
}

static void onLeaveState(StackBasedFsm_t* pFsm) {
  hal_printf("example::onLeaveState()");

  // This function is called, when the user leaves the current state. Either by going to a next state or by 
  // going back to the previous state.
}

static void onTick(StackBasedFsm_t* pFsm) {
  hal_printf("example::onTick()");

  // This function is called frequently as fast as possible, as long as this state is active. This state can be
  // used for time critical applications like animations, audio playback or advanced user interaction.
}

static void onDirection(StackBasedFsm_t* pFsm, bool south, bool north, bool west, bool east) {
  hal_printf("example::onDirection()");

  // This function is called, if the user presses one of the direction buttons on the game pad.
}

// Do not change the following implementation! Just change the function name to the name of your state and 
// copy the code unchanged and always as last function of the file:

void example(StackBasedFsm_t* pFsm, FsmState* pState, void* pArgs) {
  pState->onAction = onAction;
  pState->onBack = onBack;
  pState->onDirection = onDirection;
  pState->onEnterState = onEnter;
  pState->onReenterState = onReenter;
  pState->onLeaveState = onLeaveState;
  pState->onTick = onTick;

  pState->onEnterState(pFsm, pArgs);
}
