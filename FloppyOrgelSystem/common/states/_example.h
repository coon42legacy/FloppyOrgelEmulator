#ifndef __EXAMPLESTATE_H__
#define __EXAMPLESTATE_H__

typedef struct {
  int someInt;
  int* pReturnValue;
} ExampleParams;

void example(StackBasedFsm_t* pFsm, FsmState* pState);

#endif // __EXAMPLESTATE_H__