/////////////////////////////////////////////////////////////////////////////
//	DevApm.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\Profile.h"
#include "..\..\H\Dll\Network.h"

#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	MAIN FUNCTION(APM) : APM SET MODE
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	MAIN FUNCTION(APM) : APM GET MODE
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	MAIN FUNCTION(APM) : APM OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Atm Mode Ctrl
int CDevCmn::fnAPM_AtmModeCtrl()
{
	static	HWND hAviWnd = ::FindWindow(NULL, "AviPlayer");
	static	int	nSaveSoundStatus = ON;
	int			nTempSoundStatus = ON;

	if (!OpenKey)
		nTempSoundStatus = OFF;
	else
		nTempSoundStatus = ON;

	switch (AtmStatus) 
	{
		case ATM_INIT:											// Initial Mode
			break;
		
		case ATM_ERROR: 										// Error Mode
			break;

		case ATM_CLERK: 										// Cleark Mode
			break;

		case ATM_READY:											// Ready Mode
			break;

		case ATM_CUSTOM:										// Custom Mode
			break;

		case ATM_TRAN:											// Transaction Mode
			if ((AtmStatus != AtmStatusSave)	||
				(AtmStatusReEntryFlag))
				nTempSoundStatus = OFF;
			break;

		case ATM_DOWN:											// Down Mode
			break;

		default:
			break;
	}
	
	if (nTempSoundStatus != nSaveSoundStatus)
	{
		nSaveSoundStatus = nTempSoundStatus;
		if (Asc2Int(RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMPLAYMOVIE)))
		{
			if (nTempSoundStatus == ON)
			{
				if (GetShareData("OP_PLAYMOVIE").CompareNoCase("OFF") == 0)
					;
				else
				if (hAviWnd)
					::PostMessage(hAviWnd, WUM_SOUND_ON, 0, 0);	// 2008.09.22
			}
			else
			{
				if (hAviWnd)
					::PostMessage(hAviWnd, WUM_SOUND_OFF, 0, 0);// 2008.09.22
			}
		}
	}

	return TRUE;
}

// Atm Host Control
int CDevCmn::fnAPM_AtmHostCtrl()
{
	return TRUE;
}

// Clerk Procedure
int CDevCmn::fnAPM_ClerkProc()
{
	return TRUE;
}

