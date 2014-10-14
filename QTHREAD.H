// Listing 3 -- QThread.h

#if !defined(QTHREAD_INC)
#define QTHREAD_INC

//----------------------------- Includes ---------------------------

#include "Thread.h"

//----------------------------- defines ----------------------------

const int QTHRD_DEF_QSIZE		= 0L;

//------------------------------ Types -----------------------------

class QThread;		// forward declaration

typedef VOID FNQTHPROC (QThread *, ULONG);	// QThread Procedure type
typedef FNQTHPROC * PFNQTHPROC;

//------------------------------ Class -----------------------------

class QThread : public Thread {
public:
	QThread (	ULONG ulQueueSize=QTHRD_DEF_QSIZE,		// Constructor
				ULONG ulStackSize=THRDS_DEF_STACK);
	~QThread();											// Destructor

			VOID	Start(ULONG ulArg=0L);
		
	virtual VOID	SendMsg(ULONG objAddr,
							ULONG msg,
							MPARAM mp1, MPARAM mp2) = 0;

protected:
	virtual VOID	MsgLoop();

	virtual BOOL	GetMessage(QMSG & qmsg) = 0;	// pure virtual
	virtual VOID	DispatchMsg (QMSG & qmsg) = 0;	// pure virtual

	virtual VOID 	Startup(ULONG ulArg) = 0;		// pure virtual
	virtual BOOL	Shutdown(ULONG ulArg) = 0;		// pure virtual

	ULONG	ulQSize;

	ULONG ulParam;	// initial argument passed in when thread is started

private:
	static VOID threadProc(QThread*);		// static thread procedure
};

//-------------------------- End of Header -------------------------
#endif
