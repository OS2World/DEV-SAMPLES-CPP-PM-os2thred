// Listing 5 -- MsgThrd.h

#if !defined(MSGTHREAD_INC)
#define MSGTHREAD_INC

//----------------------------- Includes ---------------------------

#include "QThread.h"
#include "MsgQ.h"

//----------------------------- defines ----------------------------

const USHORT MSG_DEF_QSIZE		= 10;

//----------------------------------------------------
// The following two values are reserved messages ID's.
// All MsgThread's must be prepared to receive them.
// All other message ID's are user defined.

const ULONG MSG_THRD_SHUTDOWN	= 0;  // Received during shutdown
const ULONG MSG_THRD_STARTUP	= 1;  // Received at startup

const ULONG MSG_THRD_USER		= 2;  // First user defined msg ID

//------------------------------ Types -----------------------------

typedef VOID FNMSGTHRDPROC (ULONG objAddr, 
                            ULONG msgID, MPARAM mp1, MPARAM mp2, 
							ULONG ulParam);
						
typedef FNMSGTHRDPROC* PFNMSGTHRDPROC;

//------------------------------ Class -----------------------------

class MsgThread : public QThread {
public:
	MsgThread (	PFNMSGTHRDPROC pfn, 
				USHORT usQSize=MSG_DEF_QSIZE,
				ULONG ulStack=THRDS_DEF_STACK);
	~MsgThread ();
	
	VOID SendMsg (ULONG objAddr, ULONG msgID, MPARAM mp1, MPARAM mp2)
			{ pMsgQ->PostMsg(objAddr, msgID, mp1, mp2); }

protected:
	BOOL GetMessage (QMSG & qmsg) 
			{ return pMsgQ->WaitMsg(qmsg); }
	
	VOID DispatchMsg (QMSG & qmsg) 
			{ pfnMsg((ULONG)qmsg.hwnd,qmsg.msg,qmsg.mp1,qmsg.mp2,ulParam); }
			
	VOID Startup (ULONG ulArg)
			{ pfnMsg((ULONG)this, MSG_THRD_STARTUP, 
				     (MPARAM)ulArg, (MPARAM)NULL,ulArg); }
			
	BOOL Shutdown(ULONG ulArg);
		
private:
	MsgQueue*		pMsgQ;		// pointer to msg queue
	PFNMSGTHRDPROC	pfnMsg;		// pointer to client thread proc

};

//-------------------------- End of Header -------------------------
#endif
