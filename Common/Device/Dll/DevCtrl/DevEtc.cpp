/////////////////////////////////////////////////////////////////////////////
//	DevEtc.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\EtcWork.h"

#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	ETC FUNCTION(ETC) : ETC SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnETC_DVRClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRClearErrorCode()");

	int		nReturn = FALSE;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	NSM FUNCTION(ETC) : ETC GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrETC_DVRGetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrETC_DVRGetErrorCode()");

	CString strReturn("");

	strReturn += "0000000";										// Error Code Adjust
	strReturn = strReturn.Left(7);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrETC_DVRGetErrorCode():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
// Get Device Status
int	CDevCmn::fnETC_DVRGetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRGetDeviceStatus()");

	static	int		nSaveReturn = LINE_CLOSE;
	int				nReturn = LINE_CLOSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = LINE_OPEN;
	else
		nReturn = LINE_OPEN;

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRGetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
// Get Use Status
int	CDevCmn::fnETC_DVRGetUseStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRGetUseStatus()");

	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = FALSE;
	else
		nReturn = m_pEtcWork->GetUseStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRGetUseStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	ETCWORK FUNCTION(ETC) : ETC OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnETC_DVRInitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRInitialize()");

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pEtcWork->Initialize();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRInitialize():return(%d)", nReturn);
	return nReturn;
}

// InitializeIP
int	CDevCmn::fnETC_DVRInitializeIP(LPCTSTR szIpAddress, int nPortNumber)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRInitializeIP(szIpAddress[%s], nPortNumber[%d], nOpenSec[%d], nAsyncFlag[%d])", szIpAddress, nPortNumber);

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pEtcWork->InitializeIP(szIpAddress, nPortNumber);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRInitializeIP(...):return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnETC_DVRDeinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRDeinitialize()");

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pEtcWork->Deinitialize();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Send Data(Send Data, Wait Time)
int	CDevCmn::fnETC_DVRSendData(LPCTSTR szSendData)
{
HexDumpPack(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRSendData(szSendData)", (LPSTR)szSendData, strlen(szSendData));

	int		nReturn = FALSE;
	BYTE	SendData[NETBUFFSIZE];
	int		nSendLength = MakePack((LPSTR)szSendData, SendData, strlen(szSendData));

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
	{
		nReturn = TRUE;
HexDumpPack(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRSendData(szSendData)", (LPSTR)szSendData, strlen(szSendData));
	}
	else
		nReturn = m_pEtcWork->SendData(SendData, nSendLength);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnETC_DVRSendData(...):return(%d)", nReturn);
	return nReturn;
}

