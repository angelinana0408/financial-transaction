/////////////////////////////////////////////////////////////////////////////
//	DevCmn.cpp : implementation file
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
//	SOP FUNCTION(SOP) : SOP SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Key Data
int	CRearApCtrl::fnSOP_ClearKeyData()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSOP_ClearKeyData()");

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->ScrClearKeyData();

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSOP_ClearKeyData():return(%d)", nReturn);
	return nReturn;
}

// Set Screen Data(Screen Data Name, Screen Data Value)
int	CRearApCtrl::fnSOP_SetScreenData(LPCTSTR szSetDataName, LPCTSTR szSetDataValue)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSOP_SetData(szSetDataName[%s], szSetDataValue[%s])", szSetDataName, szSetDataValue);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->ScrSetScreenData(szSetDataName, szSetDataValue);

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnSOP_SetData(...):return(%d)", nReturn);
	return nReturn;
}

// Set Display Data(Display Data Name, Display Data Value)
int	CRearApCtrl::fnSOP_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSOP_SetDisplayData(szSetDisplayDataName[%s], szSetDisplayDataValue[%s])", szSetDisplayDataName, szSetDisplayDataValue);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->ScrSetDisplayData(szSetDisplayDataName, szSetDisplayDataValue);

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSOP_SetDisplayData(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SOP FUNCTION(SOP) : SOP GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Key Data
CString	CRearApCtrl::fstrSOP_GetKeyData()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSOP_GetKeyData()");

	CString strReturn("");

	strReturn = m_pMwiOp->ScrGetKeyData(MID_DEVRSP_TIME);

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSOP_GetKeyData():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	TTU FUNCTION(TTU) : TTU SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CRearApCtrl::fnTTU_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_ClearErrorCode()");

	int		nReturn = FALSE;

	nReturn = RegSetStr(_REGKEY_DEVERROR, DEVNM_TTU, "");

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	TTU FUNCTION(TTU) : TTU GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CRearApCtrl::fstrTTU_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fstrTTU_GetErrorCode()");

	CString strReturn("");

	strReturn = RegGetStr(_REGKEY_DEVERROR, DEVNM_TTU, "0000000");		
	strReturn += "0000000";										// Error Code Adjust
	strReturn = strReturn.Left(7);

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fstrTTU_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CRearApCtrl::fnTTU_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnTTU_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	nReturn = m_pMwiOp->TtuGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Read Data
CString CRearApCtrl::fstrTTU_GetReadData()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fstrTTU_GetReadData()");

	CString strReturn("");

	strReturn = m_pMwiOp->TtuGetReadData();

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fstrTTU_GetReadData():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	TTU FUNCTION(TTU) : TTU OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CRearApCtrl::fnTTU_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_Initialize()");

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->TtuInitialize();

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CRearApCtrl::fnTTU_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_Deinitialize()");

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->TtuDeinitialize();

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Clear Screen Sync
int	CRearApCtrl::fnTTU_ClearScreenSync()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_ClearScreenSync()");

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->TtuClearScreenSync();

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_ClearScreenSync():return(%d)", nReturn);
	return nReturn;
}

// Set LED Sync
int	CRearApCtrl::fnTTU_SetLEDSync(int LEDNumber, LPCTSTR State)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_SetLEDSync(LEDNumber[%d], State[%s])", LEDNumber, State);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->TtuSetLEDSync(LEDNumber, State);

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_SetLEDSync(...):return(%d)", nReturn);
	return nReturn;
}

// Read At
int	CRearApCtrl::fnTTU_ReadAt(int x, int y, int NumberOpChars, LPCTSTR EchoMode, LPCTSTR EchoFormat, LPCTSTR InputType, int CursorOn, int Flush, int AutoEnd, int Timeout)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_ReadAt(x[%d], y[%d], NumberOpChars[%d], EchoMode[%s], EchoFormat[%s], InputType[%s], CursorOn[%d], Flush[%d], AutoEnd[%d], Timeout[%d])", x, y, NumberOpChars, EchoMode, EchoFormat, InputType, CursorOn, Flush, AutoEnd, Timeout);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->TtuReadAt(x, y, NumberOpChars, EchoMode, EchoFormat, InputType, CursorOn, Flush, AutoEnd, Timeout);

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_ReadAt(...):return(%d)", nReturn);
	return nReturn;
}

// Write At Sync
int	CRearApCtrl::fnTTU_WriteAtSync(int x, int y, LPCTSTR Format, LPCTSTR Text)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_WriteAtSync(x[%d], y[%d], Format[%s], Text[%s])", x, y, Format, Text);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->TtuWriteAtSync(x, y, Format, Text);

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_WriteAtSync(...):return(%d)", nReturn);
	return nReturn;
}

// Clear Area Sync
int	CRearApCtrl::fnTTU_ClearAreaSync(int x, int y, int Width, int Hight)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_ClearAreaSync(x[%d], y[%d], Width[%s], Hight[%s])", x, y, Width, Hight);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->TtuClearAreaSync(x, y, Width, Hight);

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_ClearAreaSync(...):return(%d)", nReturn);
	return nReturn;
}

// Cancel Read
int	CRearApCtrl::fnTTU_CancelRead()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_CancelRead()");

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->TtuCancelRead();

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnTTU_CancelRead():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	COMMON FUNCTION(CMN) : CMN SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Event Device(Device Id)
int	CRearApCtrl::fnCMN_ClearEventDevice(int nDevId)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnCMN_ClearEventDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->ClearEventDevice(nDevId);

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnCMN_ClearEventDevice(...):return(%x)", nReturn);
	return nReturn;
}

// Clear Status Device(Device Id)
int	CRearApCtrl::fnCMN_ClearStatusDevice(int nDevId)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnCMN_ClearStatusDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->ClearStatusDevice(nDevId);

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnCMN_ClearStatusDevice(...):return(%x)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	COMMON FUNCTION(CMN) : CMN GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Device(Device Id)
int	CRearApCtrl::fnCMN_GetErrorDevice(int nDevId)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnCMN_GetErrorDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->GetErrorDevice(nDevId);

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnCMN_GetErrorDevice(nDevId[%x]):return(%x)", nDevId, nReturn);
	return nReturn;
}

// Get Timeout Device(Device Id)
int	CRearApCtrl::fnCMN_GetTimeoutDevice(int nDevId)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnCMN_GetTimeoutDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->GetTimeoutDevice(nDevId);

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnCMN_GetTimeoutDevice(nDevId[%x]):return(%x)", nDevId, nReturn);
	return nReturn;
}

// Get Event Device(Device Id)
int	CRearApCtrl::fnCMN_GetEventDevice(int nDevId)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnCMN_GetEventDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->GetEventDevice(nDevId);

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnCMN_GetEventDevice(nDevId[%x]):return(%x)", nDevId, nReturn);
	return nReturn;
}

// Get Status Device(Device Id)
int	CRearApCtrl::fnCMN_GetStatusDevice(int nDevId)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnCMN_GetStatusDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->GetStatusDevice(nDevId);

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnCMN_GetStatusDevice(nDevId[%x]):return(%x)", nDevId, nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	COMMON FUNCTION(CMN) : CMN OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Check Device Action(Device Id, Check Time)
CString	CRearApCtrl::fstrCMN_CheckDeviceAction(int nDevId, int nCheckTime)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrCMN_CheckDeviceAction(nDevId[%x], nCheckTime[%d])", nDevId, nCheckTime);

	CString			strReturn("");

	strReturn = m_pMwiOp->CheckDeviceAction(nDevId, nCheckTime);

if (strReturn != CHK_NORMAL)
{
	MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fstrCMN_CheckDeviceAction(nDevId[%x], nCheckTime[%d]):return(%s)", nDevId, nCheckTime, strReturn);
}
	return strReturn;
}

// Scan Device Action(Device Id, Check Time, Event Kind)
int	CRearApCtrl::fnCMN_ScanDeviceAction(int nDevId, int nScanTime, int nEventKind)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnCMN_ScanDeviceAction(nDevId[%x], nScanTime[%d], nEventKind[%d])", nDevId, nScanTime, nEventKind);

	int		nReturn = FALSE;

	nReturn = m_pMwiOp->ScanDeviceAction(nDevId, nScanTime, nEventKind);

if (nReturn)
{
	MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnCMN_ScanDeviceAction(nDevId[%x], nScanTime[%d], nEventKind[%d]):return(%x)", nDevId, nScanTime, nEventKind, nReturn);
}
	return nReturn;
}

