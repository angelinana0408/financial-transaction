/////////////////////////////////////////////////////////////////////////////
//	DevSfl.cpp : implementation file
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
//	SFL FUNCTION(SFL) : SFL SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CRearApCtrl::fnSFL_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSFL_ClearErrorCode()");

	int		nReturn = FALSE;

	nReturn = RegSetStr(_REGKEY_DEVERROR, DEVNM_SOP, "");

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSFL_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Clear Key Data
int	CRearApCtrl::fnSFL_ClearKeyData()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_ClearKeyData()");

	int		nReturn = FALSE;

	nReturn = fnSOP_ClearKeyData();

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_ClearKeyData():return(%d)", nReturn);
	return nReturn;
}

// Set Screen Data(Screen Data Name, Screen Data Value)
int	CRearApCtrl::fnSFL_SetScreenData(LPCTSTR szSetDataName, LPCTSTR szSetDataValue)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_SetScreenData(szSetDataName[%s], szSetDataValue[%s])", szSetDataName, szSetDataValue);

	int		nReturn = FALSE;

	nReturn = fnSOP_SetScreenData(szSetDataName, szSetDataValue);

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_SetScreenData(...):return(%d)", nReturn);
	return nReturn;
}

// Set Display Data(Display Data Name, Display Data Value)
int	CRearApCtrl::fnSFL_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_SetDisplayData(szSetDisplayDataName[%s], szSetDisplayDataValue[%s])", szSetDisplayDataName, szSetDisplayDataValue);

	int		nReturn = FALSE;

	nReturn = fnSOP_SetDisplayData(szSetDisplayDataName, szSetDisplayDataValue);

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_SetDisplayData(...):return(%d)", nReturn);
	return nReturn;
}

// Set Current Language Mode
int	CRearApCtrl::fnSFL_SetCurrentLangMode(int nLangMode)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSFL_SetCurrentLangMode(nLangMode[%d])", nLangMode);

	int		nReturn = FALSE;

	Scr.CurrentLangMode = nLangMode;
	nReturn = nLangMode;

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSFL_SetCurrentLangMode(...):return(%d)", nReturn);
	return nReturn;
}
													
/////////////////////////////////////////////////////////////////////////////
//	SFL FUNCTION(SFL) : SFL GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CRearApCtrl::fstrSFL_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fstrSFL_GetErrorCode()");

	CString strReturn("");

	strReturn = RegGetStr(_REGKEY_DEVERROR, DEVNM_SOP, "0000000");		
	strReturn += "0000000";										// Error Code Adjust
	strReturn = strReturn.Left(7);

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fstrSFL_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CRearApCtrl::fnSFL_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	nReturn = NORMAL;

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSFL_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Key Data
CString	CRearApCtrl::fstrSFL_GetKeyData()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSFL_GetKeyData()");

	CString strReturn("");

	strReturn = fstrSOP_GetKeyData();

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSFL_GetKeyData():return(%s)", strReturn);
	return strReturn;
}

// Get Key String(Init Flag)
CString	CRearApCtrl::fstrSFL_GetKeyString(int nInitFlag)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSFL_GetKeyString(nInitFlag[%d])", nInitFlag);

	CString strReturn("");

	if (nInitFlag)
		fnSFL_ClearKeyData();									

	strReturn = fstrSFL_GetKeyData();

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSFL_GetKeyString(...):return(%s)", strReturn);
	return strReturn;
}

// Get Check Screen(Screen No, Language Mode)
CString	CRearApCtrl::fstrSFL_GetCheckScreen(int nScreenNo, int nLangMode)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSFL_GetCheckScreen(nScreenNo[%d], nLangMode[%d])", nScreenNo, nLangMode);

	CString	strReturn("");
	CFileStatus	FileStatus;

	if (nScreenNo > 0)
	{
		if (nLangMode == CHN_MODE)	
			strReturn.Format("%sT%sC%3.3d.swf", _SCR_DIR, Clerk.ScrBankName.GetBuffer(0), nScreenNo);
		else							
		if (nLangMode == ENG_MODE)	
			strReturn.Format("%sT%sE%3.3d.swf", _SCR_DIR, Clerk.ScrBankName.GetBuffer(0), nScreenNo);
		else 
		if (nLangMode == KOR_MODE)	
			strReturn.Format("%sT%sK%3.3d.swf", _SCR_DIR, Clerk.ScrBankName.GetBuffer(0), nScreenNo);
		else 
		if (nLangMode == JPN_MODE)	
			strReturn.Format("%sT%sJ%3.3d.swf", _SCR_DIR, Clerk.ScrBankName.GetBuffer(0), nScreenNo);
		else 
			strReturn.Format("%sT%sC%3.3d.swf", _SCR_DIR, Clerk.ScrBankName.GetBuffer(0), nScreenNo);

		if (!CFile::GetStatus(strReturn.GetBuffer(0), FileStatus))	
			strReturn = "";										// File Not Found
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fstrSFL_GetCheckScreen(...):return(%s)", strReturn);
	return strReturn;
}

// Get Current Screen No
int	CRearApCtrl::fnSFL_GetCurrentScreenNo()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_GetCurrentScreenNo()");

	int		nReturn = FALSE;

	nReturn = Scr.CurrentScreenNo;								

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_GetCurrentScreenNo():return(%d)", nReturn);
	return nReturn;
}

// Get Current Language Mode
int	CRearApCtrl::fnSFL_GetCurrentLangMode()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_GetCurrentLangMode()");

	int		nReturn = FALSE;

	nReturn = Scr.CurrentLangMode;								

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_GetCurrentLangMode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SFL FUNCTION(SFL) : SFL OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CRearApCtrl::fnSFL_Initialize()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_Initialize()");

	int		nReturn = FALSE;

	nReturn = NORMAL;

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CRearApCtrl::fnSFL_Deinitialize()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_Deinitialize()");

	int		nReturn = FALSE;

	nReturn = NORMAL;

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSFL_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

