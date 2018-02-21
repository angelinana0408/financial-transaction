/////////////////////////////////////////////////////////////////////////////
// ComIrd.cpp: implementation of the CComIrd class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\ComIrd.h"
#include "..\..\H\Dll\Tcsy2008.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CComIrd
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CComIrd, CSerial)

CComIrd::CComIrd()
{
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "CComIrd()");

/////////////////////////////////////////////////////////////////////////////
	m_bAutoDelete = FALSE;
/////////////////////////////////////////////////////////////////////////////

	Initialize();
	
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "CComIrd():return");
}

CComIrd::CComIrd(DEVICEINFO DEVICE, NETWORKINFO NETWORK)
{
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "CComIrd(DEVICEINFO, NETWORKINFO)");

/////////////////////////////////////////////////////////////////////////////
	m_bAutoDelete = FALSE;
/////////////////////////////////////////////////////////////////////////////

	m_DeviceInfo	= DEVICE;
	m_NetworkInfo	= NETWORK;

	InitializeCriticalSection(&m_csLock);

	Initialize();

MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "CComIrd(DEVICEINFO):return");
}

CComIrd::~CComIrd()
{
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "~CComIrd()");

	DeleteCriticalSection(&m_csLock);

MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "~CComIrd():return");
}

BOOL CComIrd::InitInstance()
{
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "InitInstance()");
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "InitInstance():return");
	return FALSE;
}

BEGIN_MESSAGE_MAP(CComIrd, CSerial)
	//{{AFX_MSG_MAP(CComIrd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//	Ird SEND & RECV SPEC
/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////
int CComIrd::Initialize()
{
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "Initialize()");

/////////////////////////////////////////////////////////////////////////////
	hComIrd = INVALID_HANDLE_VALUE;

	IrdDeviceStatus = NORMAL;									// Ird Device Status
	memset(IrdData, 0, sizeof(IrdData));						// Ird Data

/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "Initialize():return");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CComIrd::Open()
{
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "Open()");

	Initialize();

	hComIrd = (HANDLE)TcOpenComm(m_DeviceInfo.IrdComPort, 3, 0, NULL);
	if (hComIrd == NULL)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Ird] Open Port[%d] Fail", m_DeviceInfo.IrdComPort);
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "[Ird] Open Port[%d] Fail", m_DeviceInfo.IrdComPort);
	}

MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "Open():return(%d)", hComIrd);
	return TRUE;
}

int CComIrd::Close()
{
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "Close()");

	int		nReturn = FALSE;

	if (hComIrd != NULL)
		nReturn = TcCloseComm((LONG)hComIrd);

MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "Close():return(%d)", nReturn);
	return nReturn;
}

int CComIrd::Read()
{
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "Read()");

	EnterCriticalSection(&m_csLock);							// 2007.09.07
	
	int			nReturn = FALSE;
	long		nStyle = 0;
	DWORD		dwLen = 0;
	BYTE		bTempBuff[1024];

	memset(bTempBuff, 0, sizeof(bTempBuff));

//////////////////////////////////////////////////////////////////////
	IrdDataLength = 0;
	memset(IrdData, 0, sizeof(IrdData));
//////////////////////////////////////////////////////////////////////

	nStyle = 2;
	dwLen = 560;	

	nReturn = TcVeriMatch((LONG)hComIrd, nStyle, bTempBuff, &dwLen);
	if (nReturn == TCR_SUCC)
	{
		TcSubDev((LONG)hComIrd, 0, 0x02);						// BEEP 200ms
		if ((int)dwLen > 2)
		{
			IrdDataLength = (dwLen - 2);
			memcpy(IrdData, &bTempBuff[2], IrdDataLength);
		}
HexDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "Read", bTempBuff, dwLen);
	}

	LeaveCriticalSection(&m_csLock);
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "Read():return(%d)", nReturn);
	return nReturn;
}

CString CComIrd::CheckDeviceAction(int CheckTime)
{
	ULONG  WaitTime = TimerSet(CheckTime);
	
	while (TRUE)
	{
		if (IrdDeviceStatus == NORMAL)
			return CHK_NORMAL;

		if (IrdDeviceStatus == DOWN)
			return CHK_ERROR;

		if (!CheckTime)
		{
			return CHK_TIMEOVER;
		}

		if (CheckTimer(WaitTime))
		{
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "CheckDeviceAction():ERROR CODE[%s]", "9794902");
			RegSetStr(_REGKEY_DEVERROR, DEVNM_IRD, "9794902");
			IrdDeviceStatus = DOWN;
			return CHK_TIMEOVER;
		}

		Delay_Msg(100);											// Performance Adjust
	}
	
	return CHK_NORMAL;
}

int CComIrd::ClearErrorCode()
{
	int		nReturn = FALSE;

	nReturn = RegSetStr(_REGKEY_DEVERROR, DEVNM_IRD, "");

	return nReturn;
}

CString CComIrd::GetErrorCode()
{
	CString strReturn("");

	strReturn = RegGetStr(_REGKEY_DEVERROR, DEVNM_IRD, "0000000");		
	strReturn += "0000000";										// Error Code Adjust
	strReturn = strReturn.Left(7);

	return strReturn;
}

int CComIrd::GetDeviceStatus()
{
	return IrdDeviceStatus;
}

int CComIrd::GetDataStatus()
{
	return IrdDataLength;
}

CString CComIrd::GetData()
{
	CString strReturn("");
	
	EnterCriticalSection(&m_csLock);							// 2007.09.07

	strReturn = MakeUnPack(IrdData, IrdDataLength, 0);

	IrdDataLength = 0;											// Ird Data
	memset(IrdData, 0, sizeof(IrdData));
	
	LeaveCriticalSection(&m_csLock);
	return strReturn;
}

int CComIrd::EntryEnable()
{
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "EntryEnable()");
	EnterCriticalSection(&m_csLock);							// 2007.09.07
	
	memset(IrdData, 0, sizeof(IrdData));
	
	TcSubDev((LONG)hComIrd, 1, 0xff);							// ON LED

	LeaveCriticalSection(&m_csLock);
	return TRUE;
}

int CComIrd::EntryDisable()
{
MsgDump(TRACE_CODE_MODE, "ComIrd", __FILE__, __LINE__, "EntryDisable()");
	EnterCriticalSection(&m_csLock);							// 2007.09.07
	
	TcSubDev((LONG)hComIrd, 1, 0x00);							// OFF LED

	LeaveCriticalSection(&m_csLock);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
