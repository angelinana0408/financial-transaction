/////////////////////////////////////////////////////////////////////////////
//	DevNet.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\Network.h"

#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	NETWORK FUNCTION(NET) : NET SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnNET_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_ClearErrorCode()");

	int		nReturn = FALSE;

	nReturn = RegSetStr(_REGKEY_DEVERROR, DEVNM_NET, "");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	NETWORK FUNCTION(NET) : NET GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrNET_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrNET_GetErrorCode()");

	CString strReturn("");

	strReturn = RegGetStr(_REGKEY_DEVERROR, DEVNM_NET, "0000000");		
	strReturn += "0000000";										// Error Code Adjust
	strReturn = strReturn.Left(7);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrNET_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
// Get Device Status
int	CDevCmn::fnNET_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_GetDeviceStatus()");

	static	int		nSaveReturn = LINE_CLOSE;
	int				nReturn = LINE_CLOSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = LINE_OPEN;
	else
		nReturn = m_pNetWork->GetStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
// Get Device Status2
int	CDevCmn::fnNET_GetDeviceStatus2()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_GetDeviceStatus2()");

	static	int		nSaveReturn = LINE_CLOSE;
	int				nReturn = LINE_CLOSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = LINE_OPEN;
	else
		nReturn = m_pNetWork->GetStatus2();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_GetDeviceStatus2():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
// Get Device Status3
int	CDevCmn::fnNET_GetDeviceStatus3()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_GetStatus3()");

	static	int		nSaveReturn = LINE_CLOSE;
	int				nReturn = LINE_CLOSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = LINE_OPEN;
	else
		nReturn = m_pNetWork->GetStatus3();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_GetStatus3():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	NETWORK FUNCTION(NET) : NET OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnNET_Initialize(int nOpenSec, int nAsyncFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Initialize(nOpenSec[%d], nAsyncFlag[%d])", nOpenSec, nAsyncFlag);

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pNetWork->LineOpen(nOpenSec, nAsyncFlag);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Initialize(...):return(%d)", nReturn);
	return nReturn;
}

// InitializeIP
int	CDevCmn::fnNET_InitializeIP(LPCTSTR szIpAddress, int nPortNumber, int nOpenSec, int nAsyncFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_InitializeIP(szIpAddress[%s], nPortNumber[%d], nOpenSec[%d], nAsyncFlag[%d])", szIpAddress, nPortNumber, nOpenSec, nAsyncFlag);

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pNetWork->LineOpenIP(szIpAddress, nPortNumber, nOpenSec, nAsyncFlag);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_InitializeIP(...):return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnNET_Deinitialize(int nCloseSec, int nAsyncFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Deinitialize(nCloseSec[%d], nAsyncFlag[%d])", nCloseSec, nAsyncFlag);

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pNetWork->LineClose(nCloseSec, nAsyncFlag);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Deinitialize(...):return(%d)", nReturn);
	return nReturn;
}

// Send Data(Send Data, Wait Time)
int	CDevCmn::fnNET_SendData(LPCTSTR szSendData, int nSendSec)
{
HexDumpPack(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_SendData(szSendData)", (LPSTR)szSendData, strlen(szSendData));

	int		nReturn = FALSE;
	BYTE	SendData[NETBUFFSIZE];
	int		nSendLength = MakePack((LPSTR)szSendData, SendData, strlen(szSendData));

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
	{
		nReturn = TRUE;
HexDumpPack(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_SendData(szSendData)", (LPSTR)szSendData, strlen(szSendData));
	}
	else
		nReturn = m_pNetWork->SendData(SendData, nSendLength, nSendSec);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_SendData(...):return(%d)", nReturn);
	return nReturn;
}

// Recv Data(Recv Data, Wait Time)
int CDevCmn::fnNET_RecvData(CString& strRecvData, int nRecvSec)
{
if (nRecvSec)
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_RecvData(strRecvData, nRecvSec[%d])", nRecvSec);

	int		nReturn = FALSE;
	BYTE	RecvData[NETBUFFSIZE];
	int		nRecvLength = 0;
	
	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
	{
		nReturn = TRUE;
		strRecvData = "";
	}
	else
	{
		nReturn = m_pNetWork->RecvData(RecvData, &nRecvLength, nRecvSec);
		strRecvData = MakeUnPack(RecvData, nRecvLength);
	}

if (strRecvData.GetLength())
{
	HexDumpPack(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_RecvData(...):return(strRecvData)", strRecvData.GetBuffer(0), strRecvData.GetLength());
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_RecvData(...):return(%d)", nReturn);
}

	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
// Initialize2
int	CDevCmn::fnNET_Initialize2(int nOpenSec, int nAsyncFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Initialize2(nOpenSec[%d], nAsyncFlag[%d])", nOpenSec, nAsyncFlag);

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pNetWork->LineOpen2(nOpenSec, nAsyncFlag);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Initialize2(...):return(%d)", nReturn);
	return nReturn;
}

// Deinitialize2
int	CDevCmn::fnNET_Deinitialize2(int nCloseSec, int nAsyncFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Deinitialize2(nCloseSec[%d], nAsyncFlag[%d])", nCloseSec, nAsyncFlag);

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pNetWork->LineClose2(nCloseSec, nAsyncFlag);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Deinitialize2(...):return(%d)", nReturn);
	return nReturn;
}

// Send Data2(Send Data, Wait Time)
int	CDevCmn::fnNET_SendData2(LPCTSTR szSendData, int nSendSec)
{
HexDumpPack(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_SendData2(szSendData)", (LPSTR)szSendData, strlen(szSendData));

	int		nReturn = FALSE;
	BYTE	SendData[NETBUFFSIZE];
	int		nSendLength = MakePack((LPSTR)szSendData, SendData, strlen(szSendData));

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pNetWork->SendData2(SendData, nSendLength, nSendSec);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_SendData2(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
// Initialize3
int	CDevCmn::fnNET_Initialize3(int nOpenSec, int nAsyncFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Initialize3(nOpenSec[%d], nAsyncFlag[%d])", nOpenSec, nAsyncFlag);

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pNetWork->LineOpen3(nOpenSec, nAsyncFlag);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Initialize3(...):return(%d)", nReturn);
	return nReturn;
}

// Deinitialize3
int	CDevCmn::fnNET_Deinitialize3(int nCloseSec, int nAsyncFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Deinitialize3(nCloseSec[%d], nAsyncFlag[%d])", nCloseSec, nAsyncFlag);

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pNetWork->LineClose3(nCloseSec, nAsyncFlag);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Deinitialize3(...):return(%d)", nReturn);
	return nReturn;
}

// Send Data3(Send Data, Wait Time)
int	CDevCmn::fnNET_SendData3(LPCTSTR szSendData, int nSendSec)
{
HexDumpPack(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_SendData3(szSendData)", (LPSTR)szSendData, strlen(szSendData));

	int		nReturn = FALSE;
	BYTE	SendData[NETBUFFSIZE];
	int		nSendLength = MakePack((LPSTR)szSendData, SendData, strlen(szSendData));

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
		nReturn = TRUE;
	else
		nReturn = m_pNetWork->SendData3(SendData, nSendLength, nSendSec);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_SendData3(...):return(%d)", nReturn);
	return nReturn;
}

// Recv Data3(Recv Data, Wait Time)
int	CDevCmn::fnNET_RecvData3(CString& strRecvData, int nRecvSec)
{
if (nRecvSec)
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_RecvData3(strRecvData, nRecvSec[%d])", nRecvSec);

	int		nReturn = FALSE;
	BYTE	RecvData[NETBUFFSIZE];
	int		nRecvLength = 0;

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode Support
	{
		nReturn = TRUE;
		strRecvData = "";
	}
	else
	{
		nReturn = m_pNetWork->RecvData3(RecvData, &nRecvLength, nRecvSec);
		strRecvData = MakeUnPack(RecvData, nRecvLength);
	}
	
if (strRecvData.GetLength())
{
	HexDumpPack(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_RecvData3(...):return(strRecvData)", strRecvData.GetBuffer(0), strRecvData.GetLength());
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_RecvData3(...):return(%d)", nReturn);
}

	return nReturn;
}

