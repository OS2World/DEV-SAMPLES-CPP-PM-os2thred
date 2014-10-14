// Listing 4 -- QThread.cpp

//------------------------- Includes ------------------------------

#define INCL_WIN
#define INCL_DOS

#include <os2.h>
#include "QThread.h"

//--------------------------- code --------------------------------

QThread::QThread (	ULONG ulQueueSize,
							ULONG ulStack)
					:	Thread((PFNTHREADPROC)this->threadProc, ulStack),
						ulQSize(ulQueueSize)
{}

QThread::~QThread() 
{}

VOID  QThread::Start(ULONG ulArg)
{
	ulParam = ulArg;
	this -> Thread::Start((ULONG)this);
}

VOID QThread::MsgLoop()
{
	QMSG  qmsg;
	while (this -> GetMessage(qmsg))
		this -> DispatchMsg(qmsg);
}

VOID QThread::threadProc (QThread* pQThrd)
{
	pQThrd->Startup(pQThrd->ulParam);
	pQThrd->MsgLoop();
	pQThrd->Shutdown(pQThrd->ulParam);
}

//------------------------ end of file ----------------------------