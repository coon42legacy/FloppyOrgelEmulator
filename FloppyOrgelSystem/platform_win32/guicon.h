#ifndef __GUICON_H__
#define __GUICON_H__
#ifdef _DEBUG
  void redirectIOToConsole();
#else
  #define redirectIOToConsole() ((void)0)
#endif

#endif

/* End of File */