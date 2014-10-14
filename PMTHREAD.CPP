// Listing 10 -- PMThread.cpp

//------------------------- Includes ------------------------------

#define INCL_WIN
#define INCL_DOS

#include <os2.h>
#include "PMThread.h"

//--------------------------- code --------------------------------

PMThread::PMThread (PFNPROC pfn, 
							USHORT usQSize,
							ULONG ulStackSize)
					:	QThread (usQSize, ulStackSize),
						pfnProc(pfn)
{}

PMThread::~PMThread()
{}


VOID PMThread::Startup(ULONG ulArg)
{
	hab = WinInitialize(0);
	hmq = WinCreateMsgQueue (hab, ulQSize);
	pfnProc(TRUE, ulArg, this);
}


BOOL PMThread::Shutdown(ULONG ulArg)
{
	pfnProc(FALSE, ulArg, this);
	WinDestroyMsgQueue(hmq);
	WinTerminate(hab);
	return TRUE;
}


VOID PMThread::SendMsg(	ULONG objAddr,
								ULONG msg,
								MPARAM mp1, MPARAM mp2)
{
	if (objAddr)
		WinPostMsg ((HWND)objAddr, msg, mp1, mp2);
	else
		WinPostQueueMsg (hmq, msg, mp1, mp2);
}

//------------------------ end of file ----------------------------
