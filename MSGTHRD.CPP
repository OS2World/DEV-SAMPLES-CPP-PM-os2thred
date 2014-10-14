// Listing 6 -- MsgThrd.cpp

//------------------------- Includes ------------------------------

#define INCL_WIN
#define INCL_DOS

#include <os2.h>
#include "MsgThrd.h"

//--------------------------- code --------------------------------

MsgThread::MsgThread (	PFNMSGTHRDPROC pfn, 
						USHORT usQSize, ULONG ulStack)
						: QThread(usQSize,ulStack),
						  pfnMsg(pfn)
{
	pMsgQ = new MsgQueue(usQSize);
}

MsgThread::~MsgThread ()
{
	delete(pMsgQ);
}

BOOL MsgThread::Shutdown(ULONG ulArg)
{
	pfnMsg((ULONG)NULL, MSG_THRD_SHUTDOWN, 0L, 0L, ulArg);
	return TRUE;
}

//------------------------ end of file ----------------------------
