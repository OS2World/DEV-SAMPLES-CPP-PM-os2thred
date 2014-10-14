// Listing 8 -- MsgQ.cpp

//------------------------- Includes ------------------------------

#define INCL_WIN
#define INCL_DOS

#include <os2.h>
#include "MsgQ.h"

//-------------------------- defines ------------------------------

const USHORT MQ_INCREMENT   = 5;

//--------------------------- code --------------------------------

MsgQueue::MsgQueue (USHORT usQSz) : usQSize(usQSz),
									Front(0), Rear(0)
{
	msgs = new QMSG[usQSize];
	DosCreateMutexSem (NULL, &hmtx, DC_SEM_SHARED, FALSE);
	DosCreateEventSem (NULL, &hevItmRdy, DC_SEM_SHARED, FALSE);
}

MsgQueue::~MsgQueue()
{
	DosCloseEventSem (hevItmRdy);
	DosCloseMutexSem (hmtx);
	delete msgs;
}

VOID MsgQueue::PostMsg (ULONG hobj, ULONG msg, MPARAM mp1, MPARAM mp2)
{
	DosRequestMutexSem (hmtx, SEM_INDEFINITE_WAIT);
	QPut(hobj, msg, mp1, mp2);
	DosReleaseMutexSem (hmtx);
	DosPostEventSem (hevItmRdy);	// wake up whoever is waiting for msgs
}


BOOL MsgQueue::WaitMsg(QMSG & qmsg)
{
	ULONG ulNPosts;
	
	DosWaitEventSem (hevItmRdy, SEM_INDEFINITE_WAIT);
	DosRequestMutexSem (hmtx, SEM_INDEFINITE_WAIT);
	QGet (qmsg);
	if (QEmpty())
		DosResetEventSem (hevItmRdy, &ulNPosts);
	DosReleaseMutexSem (hmtx);
	return (qmsg.msg);
}

BOOL MsgQueue::QEmpty()
{
	return (Front == Rear);
}

VOID MsgQueue::QPut(ULONG hobj, ULONG msg, MPARAM mp1,MPARAM mp2)
{
	USHORT	usNxtR, usNQSize, idxF, i;
	QMSG	*p;
	
	msgs[Rear].hwnd = (HWND)hobj;
	msgs[Rear].msg = msg;
	msgs[Rear].mp1 = mp1;
	msgs[Rear].mp2 = mp2;

	// If queue has filled up, then reallocate a larger queue
	// and transfer the contents to the new queue
	usNxtR = (Rear+1) % usQSize;
	if (usNxtR == Front) {
		usNQSize = usQSize + MQ_INCREMENT;
		p = new QMSG[usNQSize];
		idxF = Front;
		for (i=0; i < usQSize; i++) {
			p[i] = msgs[idxF++];
			if (idxF == usQSize)
				idxF = 0;
		}
		Front = 0;
		Rear = usQSize;
		delete msgs;
		usQSize = usNQSize;
		msgs = p;
	} else 
		Rear = usNxtR;
}


VOID MsgQueue::QGet (QMSG & qmsg)
{
	qmsg.hwnd = msgs[Front].hwnd;
	qmsg.msg = msgs[Front].msg;
	qmsg.mp1 = msgs[Front].mp1;
	qmsg.mp2 = msgs[Front].mp2;
	Front = (++Front % usQSize);
}

//------------------------ end of file ----------------------------

