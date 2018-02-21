/////////////////////////////////////////////////////////////////////////////
//	DevScr.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "CmnLibIn.h"
#include "ConstDef.h"

#include "nhmwiop.h"

#include "RearApCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	SCR FUNCTION(SCR) : SCR SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CRearApCtrl::fnSCR_ClearErrorCode()
{
	int		nReturn = FALSE;

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		nReturn = fnSFL_ClearErrorCode();
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		nReturn = fnSFL_ClearErrorCode();
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		nReturn = fnSTT_ClearErrorCode();
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		nReturn = fnSTT_ClearErrorCode();
	}

	return nReturn;
}

// Clear Key Data
int	CRearApCtrl::fnSCR_ClearKeyData()
{
	int		nReturn = FALSE;

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		nReturn = fnSFL_ClearKeyData();
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		nReturn = fnSFL_ClearKeyData();
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		nReturn = fnSTT_ClearKeyData();
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		nReturn = fnSTT_ClearKeyData();
	}

	return nReturn;
}

// Set Screen Data(Screen Data Name, Screen Data Value)
int	CRearApCtrl::fnSCR_SetScreenData(LPCTSTR szSetDataName, LPCTSTR szSetDataValue)
{
	int		nReturn = FALSE;

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		nReturn = fnSFL_SetScreenData(szSetDataName, szSetDataValue);
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		nReturn = fnSFL_SetScreenData(szSetDataName, szSetDataValue);
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		nReturn = fnSTT_SetScreenData(szSetDataName, szSetDataValue);
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		nReturn = fnSTT_SetScreenData(szSetDataName, szSetDataValue);
	}

	return nReturn;
}

// Set Display Data(Display Data Name, Display Data Value)
int	CRearApCtrl::fnSCR_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue)
{
	int		nReturn = FALSE;

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		nReturn = fnSFL_SetDisplayData(szSetDisplayDataName, szSetDisplayDataValue);
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		nReturn = fnSFL_SetDisplayData(szSetDisplayDataName, szSetDisplayDataValue);
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		nReturn = fnSTT_SetDisplayData(szSetDisplayDataName, szSetDisplayDataValue);
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		nReturn = fnSTT_SetDisplayData(szSetDisplayDataName, szSetDisplayDataValue);
	}

	return nReturn;
}

// Set Current Language Mode
int	CRearApCtrl::fnSCR_SetCurrentLangMode(int nLangMode)
{
	int		nReturn = FALSE;

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		nReturn = fnSFL_SetCurrentLangMode(nLangMode);
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		nReturn = fnSFL_SetCurrentLangMode(nLangMode);
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		nReturn = fnSTT_SetCurrentLangMode(nLangMode);
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		nReturn = fnSTT_SetCurrentLangMode(nLangMode);
	}

	return nReturn;
}
													
/////////////////////////////////////////////////////////////////////////////
//	SCR FUNCTION(SCR) : SCR GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CRearApCtrl::fstrSCR_GetErrorCode()
{
	CString strReturn("");

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		strReturn = fstrSFL_GetErrorCode();
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		strReturn = fstrSFL_GetErrorCode();
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		strReturn = fstrSTT_GetErrorCode();
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		strReturn = fstrSTT_GetErrorCode();
	}

	return strReturn;
}

// Get Device Status
int	CRearApCtrl::fnSCR_GetDeviceStatus()
{
	int		nReturn = FALSE;

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		nReturn = fnSFL_GetDeviceStatus();
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		nReturn = fnSFL_GetDeviceStatus();
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		nReturn = fnSTT_GetDeviceStatus();
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		nReturn = fnSTT_GetDeviceStatus();
	}

	return nReturn;
}

// Get Key Data
CString	CRearApCtrl::fstrSCR_GetKeyData()
{
	CString strReturn("");

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		strReturn = fstrSFL_GetKeyData();
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		strReturn = fstrSFL_GetKeyData();
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		strReturn = fstrSTT_GetKeyData();
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		strReturn = fstrSTT_GetKeyData();
	}

	return strReturn;
}

// Get Key String(Init Flag)
CString	CRearApCtrl::fstrSCR_GetKeyString(int nInitFlag)
{
	CString strReturn("");

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		strReturn = fstrSFL_GetKeyString(nInitFlag);
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		strReturn = fstrSFL_GetKeyString(nInitFlag);
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		strReturn = fstrSTT_GetKeyString(nInitFlag);
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		strReturn = fstrSTT_GetKeyString(nInitFlag);
	}

	return strReturn;
}

// Get Check Screen(Screen No, Language Mode)
CString	CRearApCtrl::fstrSCR_GetCheckScreen(int nScreenNo, int nLangMode)
{
	CString	strReturn("");

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		strReturn = fstrSFL_GetCheckScreen(nScreenNo, nLangMode);
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		strReturn = fstrSFL_GetCheckScreen(nScreenNo, nLangMode);
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		strReturn = fstrSTT_GetCheckScreen(nScreenNo, nLangMode);
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		strReturn = fstrSTT_GetCheckScreen(nScreenNo, nLangMode);
	}

	return strReturn;
}

// Get Current Screen No
int	CRearApCtrl::fnSCR_GetCurrentScreenNo()
{
	int		nReturn = FALSE;

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		nReturn = fnSFL_GetCurrentScreenNo();
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		nReturn = fnSFL_GetCurrentScreenNo();
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		nReturn = fnSTT_GetCurrentScreenNo();
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		nReturn = fnSTT_GetCurrentScreenNo();
	}

	return nReturn;
}

// Get Current Language Mode
int	CRearApCtrl::fnSCR_GetCurrentLangMode()
{
	int		nReturn = FALSE;

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		nReturn = fnSFL_GetCurrentLangMode();
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		nReturn = fnSFL_GetCurrentLangMode();
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		nReturn = fnSTT_GetCurrentLangMode();
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		nReturn = fnSTT_GetCurrentLangMode();
	}

	return nReturn;
}

// Get Key Numeric(Ret Length:0=Real Length)
CString	CRearApCtrl::fstrSCR_GetKeyNumeric(int nRetLength, int nInitFlag)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSCR_GetKeyNumeric(nRetLength[%d], nInitFlag)", nRetLength, nInitFlag);

	CString strReturn("");
	char	szTemp[256];
	CString strTemp("");

	if (nInitFlag)
		fnSCR_ClearKeyData();									

	strReturn = fstrSCR_GetKeyData();
	if (!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	// Character Skip
		;
	else
	if (nRetLength > 0)											
	{
		sprintf(szTemp, "%%%d.%dd%%s", nRetLength, nRetLength);	// Input Data Adjust
		strTemp.Format(szTemp, 0, strReturn);
		strReturn = strTemp.Right(nRetLength);
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSCR_GetKeyNumeric(...):return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SCR FUNCTION(SCR) : SCR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CRearApCtrl::fnSCR_Initialize()
{
	int		nReturn = FALSE;

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		nReturn = fnSFL_Initialize();
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		nReturn = fnSFL_Initialize();
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		nReturn = fnSTT_Initialize();
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		nReturn = fnSTT_Initialize();
	}

	return nReturn;
}

// Deinitialize
int	CRearApCtrl::fnSCR_Deinitialize()
{
	int		nReturn = FALSE;

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		nReturn = fnSFL_Deinitialize();
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		nReturn = fnSFL_Deinitialize();
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		nReturn = fnSTT_Deinitialize();
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		nReturn = fnSTT_Deinitialize();
	}

	return nReturn;
}

// Display Screen(Screen No, Input Time, Pin Input Mode, Card Data)
int	CRearApCtrl::fnSCR_DisplayScreen(int nScreenNo, int nInputSec, int nPinInputMode, LPCTSTR szCardData)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayScreen(nScreenNo[%d], nInputSec[%d], nPinInputMode[%d], szCardData[%s])", nScreenNo, nInputSec, nPinInputMode, szCardData);

	int		nReturn = FALSE;
	CString strTempTopName("");
	CString	strTempCheckName("");
	
	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		strTempCheckName= fstrSCR_GetCheckScreen(nScreenNo, Scr.CurrentLangMode);
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		strTempCheckName= fstrSCR_GetCheckScreen(nScreenNo, Scr.CurrentLangMode);
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		strTempCheckName= fstrSCR_GetCheckScreen(nScreenNo, Ttu.CurrentLangMode);
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		strTempCheckName= fstrSCR_GetCheckScreen(nScreenNo, Ttu.CurrentLangMode);
	}
	
	if (nInputSec > 0)											// Input Time
	{
		nReturn = fnSCR_SetDisplayData(SOP_TIME, Int2Asc(nInputSec));
	}
	
	if (Clerk.OpDevice == "NOT_SET")							// Numeric Pad : 2005.11.11
	{
		if (Clerk.MachineKind == "MCD3")
			fnSCR_DisplayImage(13, TRUE);
		else
			fnSCR_DisplayImage(13, FALSE);
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		fnSCR_DisplayImage(13, TRUE);
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		fnSCR_DisplayImage(13, FALSE);
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		fnSCR_DisplayImage(13, FALSE);
	}

	if (Clerk.MachineKind == "NANOCASH")						// Button Adjust : 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, TRUE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "MCD3")
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, TRUE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "TTW")
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, TRUE);
		fnSCR_DisplayImage(16, FALSE);
	}
	else
	if (Clerk.MachineKind == "MCD4")
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "TTW2")
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, FALSE);
	}
	else
	if (Clerk.MachineKind == "EATMGBA1")						// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "EATMULL1")						// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "EATMTTW")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "TTW2E")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "MCD4E")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "MX8000T")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "MX8000L")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "MX5600T")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "MX5600L")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		Scr.CurrentScreenNo = nScreenNo;						// Save
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		Scr.CurrentScreenNo = nScreenNo;						// Save
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		Ttu.CurrentScreenNo = nScreenNo;						// Save
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		Ttu.CurrentScreenNo = nScreenNo;						// Save
	}
	
	if (strTempCheckName == "")									// Screen Not Exist? => China Screen
		strTempCheckName = fstrSCR_GetCheckScreen(nScreenNo, CHN_MODE);
	if (strTempCheckName != "")									// Screen Exist? => Display
		nReturn = fnSCR_SetDisplayData(SOP_SCREEN, strTempCheckName);

	fnSCR_ClearKeyData();
		
	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		if (GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0)	// 2004.08.23
		{
			SetShareData("OP_COMMAND", "OP_PINMODE||" + Int2Asc(nPinInputMode));
			SetShareData("OP_RESULT", OP_DO);					// OP_PINMODE
			ClerkWaitResult();									// Clerk Wait Result
			Clerk.AtmOpMessage = "";							// Atm Op Message
			Clerk.UpdateMessage = FALSE;						// Update Message
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayScreen(...):return(%d)", nScreenNo, nInputSec, nReturn);
	return nReturn;
}

// Display Update(Screen No, Input Time, Pin Input Mode, Card Data)
int	CRearApCtrl::fnSCR_DisplayUpdate(int nScreenNo, int nInputSec, int nPinInputMode, LPCTSTR szCardData)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayUpdate(nScreenNo[%d], nInputSec[%d], nPinInputMode[%d], szCardData[%s])", nScreenNo, nInputSec, nPinInputMode, szCardData);

	int		nReturn = FALSE;

	if (Clerk.OpDevice == "NOT_SET")							// Numeric Pad : 2005.11.11
	{
		if (Clerk.MachineKind == "MCD3")
			fnSCR_DisplayImage(13, TRUE);
		else
			fnSCR_DisplayImage(13, FALSE);
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		fnSCR_DisplayImage(13, TRUE);
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		fnSCR_DisplayImage(13, FALSE);
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		fnSCR_DisplayImage(13, FALSE);
	}

	if (Clerk.MachineKind == "NANOCASH")						// Button Adjust : 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, TRUE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "MCD3")
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, TRUE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "TTW")
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, TRUE);
		fnSCR_DisplayImage(16, FALSE);
	}
	else
	if (Clerk.MachineKind == "MCD4")
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "TTW2")
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, FALSE);
	}
	else
	if (Clerk.MachineKind == "EATMGBA1")						// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "EATMULL1")						// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "EATMTTW")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "TTW2E")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "MCD4E")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "MX8000T")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "MX8000L")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "MX5600T")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}
	else
	if (Clerk.MachineKind == "MX5600L")							// 2005.11.11
	{
		fnSCR_DisplayImage(14, TRUE);
		fnSCR_DisplayImage(15, FALSE);
		fnSCR_DisplayImage(16, TRUE);
	}

	nReturn = fnSCR_SetDisplayData(SOP_UPDATE, SOP_UPDATE_SCREEN);
																// Screen Update
	fnSCR_ClearKeyData();
	
	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		if (GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0)	// 2004.08.23
		{
			SetShareData("OP_COMMAND", "OP_PINMODE||" + Int2Asc(nPinInputMode));
			SetShareData("OP_RESULT", OP_DO);					// OP_PINMODE
			ClerkWaitResult();									// Clerk Wait Result
			Clerk.AtmOpMessage = "";							// Atm Op Message
			Clerk.UpdateMessage = FALSE;						// Update Message
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayUpdate(...):return(%d)", nScreenNo, nInputSec, nReturn);
	return nReturn;
}

// Display Message()
int	CRearApCtrl::fnSCR_DisplayMessage()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayMessage()");

	int		nReturn = FALSE;

	nReturn = fnSCR_SetDisplayData(SOP_UPDATE, SOP_UPDATE_MESSAGE);

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayMessage():return(%d)", nReturn);
	return nReturn;
}


// Display Image(Image No, OnOff Flag)
int	CRearApCtrl::fnSCR_DisplayImage(int nImageNo, int nOnOffFlag)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayImage(nImageNo[%d], nOnOffFlag[%d])", nImageNo, nOnOffFlag);

	int		nReturn = FALSE;
	CString strTemp("");

	if (nImageNo > 0)
	{
		strTemp.Format("%s%d", SOP_BTNONOFF, nImageNo);
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), ((nOnOffFlag) ? "on" : "off"));
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayImage(...):return(%d)", nReturn);
	return nReturn;
}

// Display String(String No, String, Length)
int	CRearApCtrl::fnSCR_DisplayString(int nStringNo, LPCTSTR szString, int nInLength)
{
	int		nReturn = FALSE;
	char	szTemp[256];
	CString strTemp("");
	CString strTemp2("");

	sprintf(szTemp, "%%%d.%ds", nInLength, nInLength);			// Output Data Adjust
	strTemp2.Format(szTemp, szString);
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayString(nStringNo[%d], szString[%s], nInLength[%d])", nStringNo, strTemp2.GetBuffer(0), nInLength);

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SOP_STRING, nStringNo);
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayString(...):return(%d)", nReturn);
	return nReturn;
}

// Display String(String No, String)
int	CRearApCtrl::fnSCR_DisplayString(int nStringNo, LPCTSTR szString)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayString(nStringNo[%d], szString[%s])", nStringNo, szString);

	int		nReturn = FALSE;
	CString strTemp("");

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SOP_STRING, nStringNo);
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), szString);
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayString(...):return(%d)", nReturn);
	return nReturn;
}

// Display String Array(String No, String, Length)
int	CRearApCtrl::fnSCR_DisplayStringArray(int nStringNo, LPCTSTR szString, int nInLength)
{
	int		nReturn = FALSE;
	char	szTemp[256];
	CString strTemp("");
	CString strTemp2("");
	CStringArray strTempArray;

	sprintf(szTemp, "%%%d.%ds", nInLength, nInLength);			// Output Data Adjust
	strTemp2.Format(szTemp, szString);
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayString(nStringNo[%d], szString[%s], nInLength[%d])", nStringNo, strTemp2.GetBuffer(0), nInLength);

	if (nStringNo > 0)
	{
		SplitString(strTemp2, "\n", strTempArray);
		for (int i = 0; i < strTempArray.GetSize(); i++)
		{
			strTemp.Format("%s%d", SOP_STRING, nStringNo + i);
			nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTempArray[i].GetBuffer(0));
		}
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayString(...):return(%d)", nReturn);
	return nReturn;
}

// Display String (String No, String)
int	CRearApCtrl::fnSCR_DisplayStringArray(int nStringNo, LPCTSTR szString)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayString(nStringNo[%d], szString[%s])", nStringNo, szString);

	int		nReturn = FALSE;
	CString strTemp("");
	CStringArray strTempArray;

	if (nStringNo > 0)
	{
		strTemp.Format("%s", szString);							// 2004.11.05
		SplitString(strTemp, "\n", strTempArray);				// 2004.11.05
		for (int i = 0; i < strTempArray.GetSize(); i++)
		{
			strTemp.Format("%s%d", SOP_STRING, nStringNo + i);
			nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTempArray[i].GetBuffer(0));
		}
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayString(...):return(%d)", nReturn);
	return nReturn;
}

// Display Numeric(String No, Numeric, Length)
int	CRearApCtrl::fnSCR_DisplayNumeric(int nStringNo, LPCTSTR szNumeric, int nInLength)
{
	int		nReturn = FALSE;
	char	szTemp[256];
	CString strTemp("");
	CString strTemp2("");

	sprintf(szTemp, "%%%d.%ds", nInLength, nInLength);			// Output Data Adjust
	strTemp2.Format(szTemp, szNumeric);
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayNumeric(nStringNo[%d], szNumeric[%s], nInLength[%d])", nStringNo, strTemp2.GetBuffer(0), nInLength);

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SOP_STRING, nStringNo);
		strTemp2.Format("%d", Asc2Int(szNumeric, nInLength));
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayNumeric(...):return(%d)", nReturn);
	return nReturn;
}

// Display Numeric(String No, Numeric)
int	CRearApCtrl::fnSCR_DisplayNumeric(int nStringNo, LPCTSTR szNumeric)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayNumeric(nStringNo[%d], szNumeric[%s])", nStringNo, szNumeric);

	int		nReturn = FALSE;
	CString strTemp("");
	CString strTemp2("");

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SOP_STRING, nStringNo);
		strTemp2.Format("%d", Asc2Int(szNumeric, strlen(szNumeric)));
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayNumeric(...):return(%d)", nReturn);
	return nReturn;
}

// Display Numeric(String No, Numeric)
int	CRearApCtrl::fnSCR_DisplayNumeric(int nStringNo, int nNumeric)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayNumeric(nStringNo[%d], nNumeric[%d])", nStringNo, nNumeric);

	int		nReturn = FALSE;
	CString strTemp("");
	CString strTemp2("");

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SOP_STRING, nStringNo);
		strTemp2.Format("%d", nNumeric);
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayNumeric(...):return(%d)", nReturn);
	return nReturn;
}

// Display Amount(String No, Amount, Length, Mark, IsMinus)
int	CRearApCtrl::fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, int nInLength, char cMarkChar, int nIsMinus)
{
	int		nReturn = FALSE;
	char	szTemp[256];
	CString strTemp("");
	CString strTemp2("");

	sprintf(szTemp, "%%%d.%ds", nInLength, nInLength);			// Output Data Adjust
	strTemp2.Format(szTemp, szAmount);
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayAmount(nStringNo[%d], szAmount[%s], nInLength[%d], cMarkChar[%1.1c], nIsMinus[%d])", nStringNo, strTemp2.GetBuffer(0), nInLength, cMarkChar, nIsMinus);

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SOP_STRING, nStringNo);

		strTemp2 = Asc2Amt(strTemp2, strTemp2.GetLength(), 256, cMarkChar);
		strTemp2.TrimLeft();

		if (nIsMinus)
			strTemp2 = "-" + strTemp2;							// 2003.11.27

		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayAmount(...):return(%d)", nReturn);
	return nReturn;
}

// Display Amount(String No, Amount, Mark, IsMinus)
int	CRearApCtrl::fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, char cMarkChar, int nIsMinus)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayAmount(nStringNo[%d], szAmount[%s], cMarkChar[%1.1c], nIsMinus[%d])", nStringNo, szAmount, cMarkChar, nIsMinus);

	int		nReturn = FALSE;
	CString strTemp("");
	CString strTemp2("");

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SOP_STRING, nStringNo);

		strTemp2 = szAmount;
		
		strTemp2 = Asc2Amt(strTemp2, strTemp2.GetLength(), 256, cMarkChar);
		strTemp2.TrimLeft();

		if (nIsMinus)
			strTemp2 = "-" + strTemp2;							// 2003.11.27

		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayAmount(...):return(%d)", nReturn);
	return nReturn;
}

// Display Amount(String No, Amount, Mark, IsMinus)
int	CRearApCtrl::fnSCR_DisplayAmount(int nStringNo, int nAmount, char cMarkChar, int nIsMinus)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayAmount(nStringNo[%d], nAmount[%d], cMarkChar[%1.1c], nIsMinus[%d])", nStringNo, nAmount, cMarkChar, nIsMinus);

	int		nReturn = FALSE;
	CString strTemp("");
	CString strTemp2("");

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SOP_STRING, nStringNo);

		strTemp2.Format("%d", nAmount);

		strTemp2 = Asc2Amt(strTemp2, strTemp2.GetLength(), 256, cMarkChar);
		strTemp2.TrimLeft();

		if (nIsMinus)
			strTemp2 = "-" + strTemp2;							// 2003.11.27

		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_DisplayAmount(...):return(%d)", nReturn);
	return nReturn;
}

// Scan Device Action(Device Id, Check Time, Event Kind)
int	CRearApCtrl::fnSCR_ScanDeviceAction(int nDevId, int nScanTime, int nEventKind)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSCR_ScanDeviceAction(nDevId[%x], nScanTime[%d], nEventKind[%d])", nDevId, nScanTime, nEventKind);

	int		nReturn = FALSE;

	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		nReturn = m_pMwiOp->ScanDeviceAction(DEV_SOP, nScanTime, nEventKind);
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		nReturn = m_pMwiOp->ScanDeviceAction(DEV_SOP, nScanTime, nEventKind);
	}
	else
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_TSPLCHN"))
	{
		if (fnSTT_APGetReadAuto())
			nReturn = nDevId;
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		if (fnSTT_APGetReadAuto())
			nReturn = nDevId;
	}

if (nReturn)
{
	nReturn = nDevId;
	MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSCR_ScanDeviceAction(nDevId[%x], nScanTime[%d], nEventKind[%d]):return(%x)", nDevId, nScanTime, nEventKind, nReturn);
}
	return nReturn;
}

