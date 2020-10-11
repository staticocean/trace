
#ifndef __PICOC_WIN_H__
#define __PICOC_WIN_H__

#include <lib/picoc/picoc.h>

#ifdef DEBUGGER
static int gEnableDebugger = true;
#else
static int gEnableDebugger = false;
#endif

#endif /* __PICOC_WIN_H__ */