// Listing 11 -- Sample.cpp

//------------------------- Includes ------------------------------

#define INCL_DOS
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include "MsgThrd.h"

//-------------------------- defines ------------------------------

const USHORT NUMTHREADS				= 10;

const ULONG MSGID_STAMP_MESSAGE		= MSG_THRD_USER;
const ULONG MSGID_DISPLAY			= MSGID_STAMP_MESSAGE + 1;

//--------------------------- types -------------------------------

struct instanceData {
	ULONG		id;
	MsgThread*	pNxtQ;
	MsgThread*	pLastQ;
};

static instanceData thrdData[NUMTHREADS];

//---------------------- local prototypes -------------------------

VOID threadProc (ULONG objAddr, ULONG msgID,
					  MPARAM mp1, MPARAM mp2, ULONG ulParam);
VOID lastThreadProc (ULONG objAddr, ULONG msgID,
							MPARAM mp1, MPARAM mp2, ULONG ulParam);

//--------------------------- code --------------------------------

void main ()
{
	HEV	hevDone;
	MsgThread	lastThrd(lastThreadProc);
	
	DosCreateEventSem(NULL, &hevDone, 0L, FALSE);
	lastThrd.Start((ULONG)hevDone);

	//------------------------------------------
	// Create Threads
	MsgThread*		msgThrd[NUMTHREADS];
	USHORT			i;
	for (i=0; i < NUMTHREADS; i++)
		msgThrd[i] = new MsgThread(threadProc);

	//------------------------------------------
	// Start the threads
	ULONG        idStamp = 1L;
	MsgThread*   pNxtQ;
	for (i=0; i < NUMTHREADS; i++) {
		if (i < (NUMTHREADS-1))
			pNxtQ = msgThrd[i+1];
		else
			pNxtQ = msgThrd[0];
		thrdData[i].id = idStamp;
		thrdData[i].pNxtQ = pNxtQ;
		thrdData[i].pLastQ = &lastThrd;
		msgThrd[i]->Start((ULONG)(&thrdData[i]));
		idStamp = idStamp << 1;
	}

	//------------------------------------------
	// Send a message to each of the threads
	for (i=0; i < NUMTHREADS; i++) {
		msgThrd[i]->SendMsg((ULONG)NULL, MSGID_STAMP_MESSAGE,
						 0L, (MPARAM)thrdData[i].id);
	}

	// Wait for all the message threads to terminate
	DosWaitEventSem(hevDone, SEM_INDEFINITE_WAIT);
	printf ("Done waiting for output... destroying threads\n");

	for (i=0; i < NUMTHREADS; i++)
		delete(msgThrd[i]);
	exit(0);
}

VOID lastThreadProc (ULONG objAddr, ULONG msgID,
							MPARAM mp1, MPARAM mp2, ULONG ulParam)
{
	static  USHORT		msgCount;
	static  HEV			hevDone;

	switch (msgID) {
		case MSG_THRD_STARTUP:
			msgCount = NUMTHREADS;
			hevDone = (HEV)mp1;
			break;
		case MSGID_DISPLAY:
			printf ("Message from %8lx value: %8lx\n",(ULONG)mp2, (ULONG)mp1);
			msgCount--;
			if (msgCount == 0)
				DosPostEventSem(hevDone);
			break;
		default:
			break;
	}
}

VOID threadProc (ULONG objAddr, ULONG msgID,
						MPARAM mp1, MPARAM mp2, ULONG ulParam)
{
	instanceData*  pData = (instanceData*)ulParam;

	switch (msgID) {
		case MSG_THRD_STARTUP:
		case MSG_THRD_SHUTDOWN:
			break;
		case MSGID_STAMP_MESSAGE:
			if ((ULONG)mp1 & pData->id) {
				pData->pLastQ->SendMsg(objAddr, MSGID_DISPLAY, mp1, mp2);
			} else {
				mp1 = (MPARAM)((ULONG)mp1 | pData->id);
				pData->pNxtQ->SendMsg(objAddr, MSGID_STAMP_MESSAGE, mp1, mp2);
			}
			break;
		default:
			break;
	}
}

//------------------------ end of file ----------------------------
