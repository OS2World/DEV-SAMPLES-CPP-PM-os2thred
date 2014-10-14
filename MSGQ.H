// Listing 7 -- Msgq.h

#if !defined(MSGQUEUE_INC)
#define MSGQUEUE_INC

//-------------------------- defines -------------------------------

const USHORT MQ_DEF_QSIZE   = 10;

//------------------------------ Class -----------------------------

class MsgQueue {
public:
	MsgQueue (USHORT usQSz=MQ_DEF_QSIZE);
	~MsgQueue ();
	
	//--------------------------------------------------------------
	// This method blocks until it acquires the mutual exclusion 
	// semaphore for the queue. It then calls the private 
	// method QPut to add the message to the queue.
	VOID PostMsg (ULONG hobj, ULONG msg, MPARAM mp1, MPARAM mp2);

	//--------------------------------------------------------------
	// This method blocks until a message is available on the queue.
	// It then obtains the necessary mutual exclusion semaphores
	// before calling the private method QGet.
	BOOL WaitMsg(QMSG & qmsg);

private:
	BOOL	QEmpty();		// returns TRUE if queue is empty

	//--------------------------------------------------------------------
	// This function puts a message in the queue.  This function is
	// private because it assumes that the proper mutual exclusion 
	// semaphores have already been acquired.
	// If the queue is full it will automatically grow, so it 
	// cannot overflow until memory is exhausted.
	VOID	QPut(	ULONG hobj,  // hwnd or object handle
					ULONG msg,		// msg ID
					MPARAM mp1,		// parameter 1
					MPARAM mp2);	// parameter 2

	//--------------------------------------------------------------------
	// This function extracts a waiting message from the queue and fills
	// the QMSG structure.  This is a private function because it does 
	// no mutual exclusion and assumes a msg is indeed waiting at the 
	// Front of the queue (it returns whatever is there, valid or not).
	// This function does not block.
	VOID	QGet (QMSG & pqmsg);

	HEV		hevItmRdy;		// Even semaphore to indicate item ready
	HMTX	hmtx;			// Mutual exclusion semaphore
	USHORT	Front, Rear;	// Queue pointers
	USHORT	usQSize;		// Maximum number of queue entries
	QMSG	*msgs;			// Array of QMSG structures
};

//-------------------------- End of Header -------------------------
#endif
