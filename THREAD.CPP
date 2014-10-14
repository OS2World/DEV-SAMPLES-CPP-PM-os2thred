// Listing 2 -- Thread.cpp

//------------------------- Includes ------------------------------

#define INCL_DOS

#include <os2.h>
#include <Process.h>
#include "Thread.h"

//--------------------------- code --------------------------------

Thread::Thread(PFNTHREADPROC pfnThread, ULONG ulStack)
{
	ulStackSize = ulStack;
	pfnThreadProc = pfnThread;
}

Thread::~Thread() {}	// empty implementation

VOID Thread::Start (ULONG arg)
{
	idThread = _beginthread(pfnThreadProc, ulStackSize, (void*)arg);
}

//------------------------ end of file ----------------------------
