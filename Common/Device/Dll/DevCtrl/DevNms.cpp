/////////////////////////////////////////////////////////////////////////////
//	DevNms.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\NmsWork.h"

#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	NMS FUNCTION(NMS) : NMS SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnNMS_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNMS_ClearErrorCode()");

	int		nReturn = FALSE;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNMS_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	NSM FUNCTION(NMS) : NMS GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrNMS_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrNMS_GetErrorCode()");

	CString strReturn("");

	strReturn += "0000000";										// Error Code Adjust
	strReturn = strReturn.Left(7);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrNMS_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
// Get Device Status
int	CDevCmn::fnNMS_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNMS_GetDeviceStatus()");

	static	int		nSaveReturn = LINE_CLOSE;
	int				nReturn = LINE_CLOSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = LINE_OPEN;
	else
		nReturn = LINE_OPEN;

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNMS_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
// Get Use Status
int	CDevCmn::fnNMS_GetUseStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNMS_GetUseStatus()");

	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = FALSE;
	else
		nReturn = m_pNmsWork->GetUseStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNMS_GetUseStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	NMSWORK FUNCTION(NMS) : NMS OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnNMS_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNMS_Initialize()");

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pNmsWork->Initialize();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNMS_Initialize():return(%d)", nReturn);
	return nReturn;
}

// InitializeIP
int	CDevCmn::fnNMS_InitializeIP(LPCTSTR szIpAddress, int nPortNumber)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNMS_InitializeIP(szIpAddress[%s], nPortNumber[%d], nOpenSec[%d], nAsyncFlag[%d])", szIpAddress, nPortNumber);

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pNmsWork->InitializeIP(szIpAddress, nPortNumber);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNMS_InitializeIP(...):return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnNMS_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNMS_Deinitialize()");

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pNmsWork->Deinitialize();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNMS_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Send Data(Send Data, Wait Time)
int	CDevCmn::fnNMS_SendData(LPCTSTR szSendData)
{
HexDumpPack(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNMS_SendData(szSendData)", (LPSTR)szSendData, strlen(szSendData));

	int		nReturn = FALSE;
	BYTE	SendData[NETBUFFSIZE];
	int		nSendLength = MakePack((LPSTR)szSendData, SendData, strlen(szSendData));

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
	{
		nReturn = TRUE;
HexDumpPack(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNMS_SendData(szSendData)", (LPSTR)szSendData, strlen(szSendData));
	}
	else
		nReturn = m_pNmsWork->SendData(SendData, nSendLength);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNMS_SendData(...):return(%d)", nReturn);
	return nReturn;
}

