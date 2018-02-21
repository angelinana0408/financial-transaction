/////////////////////////////////////////////////////////////////////////////
//	DevCom.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\ComPort.h"

#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	DVR FUNCTION(DVR) : DVR SET MODE
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	DVR FUNCTION(DVR) : DVR GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Status
CString	CDevCmn::fstrDVR_GetStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrDVR_GetStatus()");

	static CString  strSaveReturn("");
	CString			strReturn("");
	
	strReturn = m_pComPort->DvrGetStatus();
	
if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrDVR_GetStatus():return(%s)", strReturn);
}
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	DVR FUNCTION(DVR) : DVR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnDVR_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDVR_Initialize()");

	int		nReturn = FALSE;

	nReturn = m_pComPort->DvrInitialize();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDVR_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnDVR_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDVR_Deinitialize()");

	int		nReturn = FALSE;

	nReturn = m_pComPort->DvrDeinitialize();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDVR_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Send Data(Send Data, Send Delay Time)
int	CDevCmn::fnDVR_SendData(LPCTSTR szSendData, int nSendDelayTime)
{
HexDumpPack(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDVR_SendData(szSendData)", (LPSTR)szSendData, strlen(szSendData));

	int		nReturn = FALSE;
	BYTE	SendData[COMBUFFSIZE];
	int		nSendLength = MakePack((LPSTR)szSendData, SendData, strlen(szSendData));

	nReturn = m_pComPort->DvrWrite(SendData, nSendLength, nSendDelayTime);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDVR_SendData(szSendData, nSendDelayTime[%d]):return(%d)", nSendDelayTime, nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	ETC FUNCTION(SECURITY) : ETC SET MODE
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	ETC FUNCTION(ETC) : ETC GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Status
CString	CDevCmn::fstrETC_GetStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrETC_GetStatus()");

	static CString  strSaveReturn("");
	CString			strReturn("");
	
	strReturn = m_pComPort->EtcGetStatus();
	
if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrETC_GetStatus():return(%s)", strReturn);
}
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	ETC FUNCTION(ETC) : ETC OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnETC_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_Initialize()");

	int		nReturn = FALSE;

	nReturn = m_pComPort->EtcInitialize();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnETC_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_Deinitialize()");

	int		nReturn = FALSE;

	nReturn = m_pComPort->EtcDeinitialize();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Send Data(Send Data, Send Delay Time)
int	CDevCmn::fnETC_SendData(LPCTSTR szSendData, int nSendDelayTime)
{
HexDumpPack(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_SendData(szSendData)", (LPSTR)szSendData, strlen(szSendData));

	int		nReturn = FALSE;
	BYTE	SendData[COMBUFFSIZE];
	int		nSendLength = MakePack((LPSTR)szSendData, SendData, strlen(szSendData));

	nReturn = m_pComPort->EtcWrite(SendData, nSendLength, nSendDelayTime);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_SendData(szSendData, nSendDelayTime[%d]):return(%d)", nSendDelayTime, nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	RIC FUNCTION : RIC SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnRIC_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_RIC))
		nReturn = m_pComPort->RicClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnRIC_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	RIC FUNCTION : RIC GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrRIC_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_RIC))
		strReturn = m_pComPort->RicGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnRIC_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnRIC_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_RIC))
		nReturn = m_pComPort->RicGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Data Status
int	CDevCmn::fnRIC_GetDataStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnRIC_GetDataStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_RIC))
		nReturn = m_pComPort->RicGetDataStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_GetDataStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Ic Card Serial No
CString CDevCmn::fstrRIC_GetIcCardSerialNo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_GetIcCardSerialNo()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_RIC))
		strReturn = m_pComPort->RicGetIcCardSerialNo();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_GetIcCardSerialNo():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	RICWORK FUNCTION : RIC OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnRIC_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_Initialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	RicRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_RIC))
		nReturn = m_pComPort->RicInitialize();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnRIC_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_Deinitialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	RicRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_RIC))
		nReturn = m_pComPort->RicDeinitialize();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Rf Reset
int	CDevCmn::fnRIC_RfReset()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_RfReset()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	RicRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_RIC))
		nReturn = m_pComPort->RicRfReset();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_RfReset():return(%d)", nReturn);
	return nReturn;
}

// DetectCard
int	CDevCmn::fnRIC_DetectCard()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_DetectCard()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	RicRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_RIC))
		nReturn = m_pComPort->RicDetectCard();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_DetectCard():return(%d)", nReturn);
	return nReturn;
}

// Cpu Reset
int	CDevCmn::fnRIC_CpuReset()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_CpuReset()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	RicRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_RIC))
		nReturn = m_pComPort->RicCpuReset();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_CpuReset():return(%d)", nReturn);
	return nReturn;
}

// Select File
CString	CDevCmn::fstrRIC_SelectFile()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_SelectFile()");

	CString strReturn("");

	DeviceOperationFlag = TRUE;
	RicRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_RIC))
		strReturn = m_pComPort->RicSelectFile();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_SelectFile():return(%s)", strReturn);
	return strReturn;
}

// Verify PIN
int	CDevCmn::fnRIC_VerifyPIN(LPCTSTR szPinData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_VerifyPIN()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	RicRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_RIC))
		nReturn = m_pComPort->RicVerifyPIN(szPinData);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_VerifyPIN():return(%d)", nReturn);
	return nReturn;
}

// Read Balance
CString	CDevCmn::fstrRIC_ReadBalance()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_ReadBalance()");

	CString strReturn("");

	DeviceOperationFlag = TRUE;
	RicRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_RIC))
		strReturn = m_pComPort->RicReadBalance();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_ReadBalance():return(%s)", strReturn);
	return strReturn;
}

// Initialize For Load
CString	CDevCmn::fstrRIC_InitializeForLoad(LPCTSTR szAmount, LPCTSTR szTerminalID, int nKeyIndex)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_InitializeForLoad()");

	CString strReturn("");

	DeviceOperationFlag = TRUE;
	RicRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_RIC))
		strReturn = m_pComPort->RicInitializeForLoad(szAmount, szTerminalID, nKeyIndex);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_InitializeForLoad():return(%s)", strReturn);
	return strReturn;
}

// Load EP
CString	CDevCmn::fstrRIC_LoadEP(LPCTSTR szDate, LPCTSTR szTime, LPCTSTR szMac2)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_LoadEP()");

	CString strReturn("");

	DeviceOperationFlag = TRUE;
	RicRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_RIC))
		strReturn = m_pComPort->RicLoadEP(szDate, szTime, szMac2);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_LoadEP():return(%s)", strReturn);
	return strReturn;
}

// Get Reject Code
CString	CDevCmn::fstrRIC_GetRejectCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_GetRejectCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_RIC))
		strReturn = m_pComPort->RicGetRejectCode();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_GetRejectCode():return(%s)", strReturn);
	return strReturn;
}

// Entry Enable
int	CDevCmn::fnRIC_EntryEnable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_EntryEnable()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_RIC))
		nReturn = m_pComPort->RicEntryEnable();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnRIC_EntryEnable():return(%d)", nReturn);
	return nReturn;
}

// Entry Disable
int	CDevCmn::fnRIC_EntryDisable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRIC_EntryDisable()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_RIC))
		nReturn = m_pComPort->RicEntryDisable();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnRIC_EntryDisable():return(%d)", nReturn);
	return nReturn;
}

// Check Device Action
CString CDevCmn::fstrRIC_CheckDeviceAction(int nCheckTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_CheckDeviceAction()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_RIC))
		strReturn = m_pComPort->RicCheckDeviceAction(nCheckTime);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRIC_CheckDeviceAction():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	IRD FUNCTION : IRD SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnIRD_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pComPort->IrdClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnIRDC_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	IRD FUNCTION : IRD GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrIRD_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrIRD_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_IRD))
		strReturn = m_pComPort->IrdGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrIRD_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
// Get Device Status
int	CDevCmn::fnIRD_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnIRD_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pComPort->IrdGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Data Status
int	CDevCmn::fnIRD_GetDataStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnIRD_GetDataStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pComPort->IrdGetDataStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_GetDataStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Data
CString CDevCmn::fstrIRD_GetData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrIRD_GetData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_IRD))
		strReturn = m_pComPort->IrdGetData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrIRD_GetData():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	ETCWORK FUNCTION(ETC) : ETC OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnIRD_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_Initialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	IrdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pComPort->IrdInitialize();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnIRD_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_Deinitialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	IrdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pComPort->IrdDeinitialize();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Rf Reset
int	CDevCmn::fnIRD_Read()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_Read()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	IrdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pComPort->IrdRead();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_Read(...):return(%d)", nReturn);
	return nReturn;
}

// Entry Enable
int	CDevCmn::fnIRD_EntryEnable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_EntryEnable()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pComPort->IrdEntryEnable();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnIRD_EntryEnable():return(%d)", nReturn);
	return nReturn;
}

// Entry Disable
int	CDevCmn::fnIRD_EntryDisable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_EntryDisable()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pComPort->IrdEntryDisable();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnIRD_EntryDisable():return(%d)", nReturn);
	return nReturn;
}

// Check Device Action
CString CDevCmn::fstrIRD_CheckDeviceAction(int nCheckTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrIRD_CheckDeviceAction()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_IRD))
		strReturn = m_pComPort->IrdCheckDeviceAction(nCheckTime);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrIRD_CheckDeviceAction():return(%s)", strReturn);
	return strReturn;
}

