// Listing 12 -- PMApp.c   Presentation Manager sample program)

#define INCL_WIN
#define INCL_DOS

#include <os2.h>
#include "PMThread.h"

//----------------------------- Statics -------------------------------

static CHAR		szClientClass[]="PMApp";
static HWND		hwndFrame, hwndClient;
static ULONG	flFrameFlags = FCF_SYSMENU  | FCF_TITLEBAR | FCF_MINMAX
						| FCF_TASKLIST | FCF_SHELLPOSITION | FCF_SIZEBORDER;

//------------------------- Local Prototypes --------------------------

// client window procedure prototype
MRESULT EXPENTRY ClientWinProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// procedure to setup and shutdown the pm thread -- passed to PMThread
VOID setupProc (BOOL starting, ULONG ulArg, PMThread* pPMThrd);

//------------------------------- Code --------------------------------

void main (void)
{
	HEV	hevDone;
	DosCreateEventSem(NULL, &hevDone, 0L, FALSE);

	// Create and start the PM Thread
	PMThread pmThrd((PFNPROC)setupProc);
	pmThrd.Start((ULONG)hevDone);

	// Wait for PM thread to finish
	DosWaitEventSem(hevDone, SEM_INDEFINITE_WAIT);
	DosExit (EXIT_PROCESS, 0L);
}

VOID setupProc (BOOL starting, ULONG ulArg, PMThread* pPMThrd)
{
	// If the starting parameter is TRUE, then we are initializing the
	// PM Thread, otherwise we are shutting it down.
	if (starting) {
		WinRegisterClass (pPMThrd->QueryHAB(),   // Anchor block
							szClientClass,    // Class Name
							ClientWinProc,    // Window procedure
							CS_SIZEREDRAW,    // Style bits
							0);               // Extra space to reserve

		hwndFrame=WinCreateStdWindow (
				HWND_DESKTOP,               // Parent window handle
				WS_VISIBLE,                 // Style of frame window
				&flFrameFlags,              // Pointer to control data
				szClientClass,              // Registered Class name
				"PM App based on PM Msg Thread",   // Title bar text
				0L,                         // Style of Client window
				(HMODULE)NULL,              // module resource id
				0L,               			 // ID of resources (icons and menus)
				&hwndClient);               // pointer to client window handle

	} else { // not starting
		WinDestroyWindow (hwndFrame);
		DosPostEventSem((HEV)ulArg);  // signal main thread that we're done
	}
}

MRESULT EXPENTRY ClientWinProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
HPS             hps;
RECTL           rcl;

	 switch (msg) {
		case WM_PAINT:
			hps = WinBeginPaint(hwnd, NULLHANDLE, &rcl);
			WinFillRect (hps, &rcl, CLR_BACKGROUND);
			WinQueryWindowRect (hwnd, &rcl);
			WinDrawText (hps, 11, "Hello World", &rcl,
				CLR_BLUE, CLR_BACKGROUND, DT_CENTER | DT_VCENTER);
			WinEndPaint (hps);
			return 0;
	 }
    return WinDefWindowProc (hwnd, msg, mp1, mp2);
}
