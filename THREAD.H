// Listing 1 -- Thread.h

#if !defined(THREADS_INC)
#define THREADS_INC

//---------------------- Constants and Types -----------------------

const int THRDS_DEF_STACK		= 8192;		// default stack size

typedef void FNTHREADPROC (VOID * ulArg);	// thread procedure type
typedef FNTHREADPROC* PFNTHREADPROC;

//------------------------------ Class -----------------------------

class Thread {
public:
	Thread(	PFNTHREADPROC pfnThread,				// Constructor
			ULONG ulStack=THRDS_DEF_STACK);
		
	virtual ~Thread();								// Destructor

	virtual VOID Start (ULONG arg=0L);

	VOID	Stop()		{	DosKillThread(idThread); }

	VOID	Resume()	{	DosResumeThread(idThread); }

	VOID	Suspend()	{	DosSuspendThread(idThread); }

	TID	GetTID()		{ return idThread; }

private:
	ULONG			ulStackSize;
	TID				idThread;
	PFNTHREADPROC	pfnThreadProc;
};

//-------------------------- End of Header -------------------------
#endif

