// Listing 9 -- PMThread.h

#if !defined(PMTHREAD_INC)
#define PMTHREAD_INC

//----------------------------- Includes ---------------------------

#include "QThread.h"

//----------------------------- defines ----------------------------

const ULONG PMTHRD_DEF_STACKSIZE 	= 8192;

//--------------------------- Public Types -------------------------
// Type for the procedure that is supplied to perform initialization
// and shutdown for the PM thread.  Usually this proc registers user
// classes and/or creates the main window or windows.

class PMThread;		// forward declaration

typedef VOID FNPROC (BOOL start, ULONG ulArg, PMThread* pmThrd);
typedef FNPROC* PFNPROC;

//------------------------------ Class -----------------------------

class PMThread : public QThread {
public:
	PMThread (	PFNPROC pfn,
				USHORT usQSize=0,
				ULONG ulStackSize=PMTHRD_DEF_STACKSIZE);
	~PMThread ();

	VOID Startup (ULONG ulArg);
	BOOL Shutdown(ULONG ulArg);

	VOID SendMsg(	ULONG objAddr,
					ULONG msg,
					MPARAM mp1, MPARAM mp2);
				
	BOOL GetMessage(QMSG & qmsg)
			{ return WinGetMsg(hab, &qmsg, NULLHANDLE, 0,0); }

	VOID DispatchMsg (QMSG & qmsg)
			{ WinDispatchMsg (hab, &qmsg); }

	HAB QueryHAB() { return hab; }
	HMQ QueryHMQ() { return hmq; }

private:
	HAB			hab;		// PM Anchor block handle
	HMQ			hmq;		// Message Queue handle
	PFNPROC		pfnProc;
};

//-------------------------- End of Header -------------------------
#endif
