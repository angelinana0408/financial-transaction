/////////////////////////////////////////////////////////////////////////////
// ComRic.cpp: implementation of the CComRic class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\ComRic.h"
#include "..\..\H\Dll\dcrf32.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CComRic
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CComRic, CSerial)

CComRic::CComRic()
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "CComRic()");

/////////////////////////////////////////////////////////////////////////////
	m_bAutoDelete = FALSE;
/////////////////////////////////////////////////////////////////////////////

	Initialize();
	
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "CComRic():return");
}

CComRic::CComRic(DEVICEINFO DEVICE, NETWORKINFO NETWORK)
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "CComRic(DEVICEINFO, NETWORKINFO)");

/////////////////////////////////////////////////////////////////////////////
	m_bAutoDelete = FALSE;
/////////////////////////////////////////////////////////////////////////////

	m_DeviceInfo	= DEVICE;
	m_NetworkInfo	= NETWORK;

	InitializeCriticalSection(&m_csLock);

	Initialize();

MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "CComRic(DEVICEINFO):return");
}

CComRic::~CComRic()
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "~CComRic()");

	DeleteCriticalSection(&m_csLock);

MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "~CComRic():return");
}

BOOL CComRic::InitInstance()
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "InitInstance()");
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "InitInstance():return");
	return FALSE;
}

BEGIN_MESSAGE_MAP(CComRic, CSerial)
	//{{AFX_MSG_MAP(CComRic)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//	Ric SEND & RECV SPEC
/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////
int CComRic::Initialize()
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "Initialize()");

/////////////////////////////////////////////////////////////////////////////
	hComRic = INVALID_HANDLE_VALUE;

	RicDeviceStatus = NORMAL;									// Ric Device Status
	memset(RejectCode, 0, sizeof(RejectCode));
	IcCardSerialNo = 0;

/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "Initialize():return");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CComRic::Open()
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "Open()");

	Initialize();

	hComRic = dc_init(m_DeviceInfo.RicComPort, 115200);			//100 : USB

	if ((LONG)hComRic <= 0)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Ric] Open Port[%d] Fail", m_DeviceInfo.RicComPort);
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "[Ric] Open Port[%d] Fail", m_DeviceInfo.RicComPort);
	}
	else
		dc_beep(hComRic, 10);

MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "Open():return");
	return TRUE;
}

int CComRic::Close()
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "Close()");

	int		nReturn = FALSE;

	if (hComRic != INVALID_HANDLE_VALUE)
		nReturn = dc_exit(hComRic);

MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "Close():return(%d)", nReturn);
	return nReturn;
}

int CComRic::RfReset()
{
MsgDump(TRACE_DUMP_MODE, "ComRic", __FILE__, __LINE__, "RfReset()");

	EnterCriticalSection(&m_csLock);							// 2007.09.07

	static int	nSaveReturn = FALSE;
	int			nReturn = FALSE;

	nReturn = dc_reset(hComRic, 1);

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "RfReset():return(%d)", nReturn);
}
	LeaveCriticalSection(&m_csLock);
	return nReturn;
}

int CComRic::DetectCard()
{
MsgDump(TRACE_DUMP_MODE, "ComRic", __FILE__, __LINE__, "DetectCard()");

	EnterCriticalSection(&m_csLock);							// 2007.09.07

	static int	nSaveReturn = FALSE;
	int			nReturn = FALSE;

	IcCardSerialNo = 0;
	
	nReturn = dc_card(hComRic, 1, &IcCardSerialNo);

if (nReturn != nSaveReturn)
{
	dc_beep(hComRic, 10);
	nSaveReturn = nReturn;
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "DetectCard():return(%d)(%lu)", nReturn, IcCardSerialNo);
}
	LeaveCriticalSection(&m_csLock);
	return nReturn;
}

int CComRic::CpuReset()
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "CpuReset()");

	EnterCriticalSection(&m_csLock);							// 2007.09.07

	int		nReturn = FALSE;
	UCHAR	bRecvLength[1024];
	UCHAR	bRecvBuff[1024];

	memset(bRecvLength, 0, sizeof(bRecvLength));
	memset(bRecvBuff, 0, sizeof(bRecvBuff));

	nReturn = dc_pro_resethex(hComRic, bRecvLength, (char*)bRecvBuff);

	LeaveCriticalSection(&m_csLock);
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "CpuReset():return(%d)", nReturn);
	return nReturn;
}

CString CComRic::SelectFile()
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "SelectFile()");

	EnterCriticalSection(&m_csLock);							// 2007.09.07

	int		nReturn = FALSE;
	int		nSendLength = 0;
	int		nRecvLength = 0;
	UCHAR	bRecvLength[1024];
	UCHAR	bRecvBuff[1024];
	UCHAR	bSendBuff[1024];
	UCHAR	bTempBuff[1024];
	CString strReturn("");

	memset(bRecvLength, 0, sizeof(bRecvLength));
	memset(bRecvBuff, 0, sizeof(bRecvBuff));
	memset(bSendBuff, 0, sizeof(bSendBuff));
	memset(bTempBuff, 0, sizeof(bTempBuff));

//////////////////////////////////////////////////////////////////////
	memset(RejectCode, 0, sizeof(RejectCode));
//////////////////////////////////////////////////////////////////////
	
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0xA4;
	bSendBuff[nSendLength++] = 0x04;
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0x0E;
	bSendBuff[nSendLength++] = 0x31;
	bSendBuff[nSendLength++] = 0x50;
	bSendBuff[nSendLength++] = 0x41;
	bSendBuff[nSendLength++] = 0x59;
	bSendBuff[nSendLength++] = 0x2E;
	bSendBuff[nSendLength++] = 0x53;
	bSendBuff[nSendLength++] = 0x59;
	bSendBuff[nSendLength++] = 0x53;
	bSendBuff[nSendLength++] = 0x2E;
	bSendBuff[nSendLength++] = 0x44;
	bSendBuff[nSendLength++] = 0x44;
	bSendBuff[nSendLength++] = 0x46;
	bSendBuff[nSendLength++] = 0x30;
	bSendBuff[nSendLength++] = 0x31;
	bSendBuff[nSendLength++] = 0x00;

HexDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "[Select File CMD1]", (char*)bSendBuff, nSendLength);

	nReturn = dc_pro_commandlink(hComRic, nSendLength, bSendBuff, bRecvLength, bRecvBuff, 7, 40);
	if (nReturn == 0)
	{
		nRecvLength = MAKEWORD(bRecvLength[0], bRecvLength[1]);
		MakeUnPack(bRecvBuff, bTempBuff, nRecvLength);
		strReturn.Format("%s", bTempBuff);
		if (strReturn.Right(4) == "9000")
			;
		else
		{
			LeaveCriticalSection(&m_csLock);
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "SelectFile():CMD1 return(%d)(%s)", nReturn, strReturn);
			return strReturn;
		}
	}
	else
	{
		LeaveCriticalSection(&m_csLock);
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "SelectFile():CMD1 return(%d)(%s)", nReturn, strReturn);
		return strReturn;
	}

	strReturn.Empty();
	nSendLength = 0;
	nRecvLength = 0;
	memset(bRecvLength, 0, sizeof(bRecvLength));
	memset(bRecvBuff, 0, sizeof(bRecvBuff));
	memset(bSendBuff, 0, sizeof(bSendBuff));
	memset(bTempBuff, 0, sizeof(bTempBuff));
	
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0xA4;
	bSendBuff[nSendLength++] = 0x04;
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0x09;
	bSendBuff[nSendLength++] = 0xA0;
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0x03;
	bSendBuff[nSendLength++] = 0x86;
	bSendBuff[nSendLength++] = 0x98;
	bSendBuff[nSendLength++] = 0x07;
	bSendBuff[nSendLength++] = 0x01;
	bSendBuff[nSendLength++] = 0x00;

HexDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "[Select File CMD2]", (char*)bSendBuff, nSendLength);

	nReturn = dc_pro_commandlink(hComRic, nSendLength, bSendBuff, bRecvLength, bRecvBuff, 7, 40);
	if (nReturn == 0)
	{
		nRecvLength = MAKEWORD(bRecvLength[0], bRecvLength[1]);
		MakeUnPack(bRecvBuff, bTempBuff, nRecvLength);
		strReturn.Format("%s", bTempBuff);
		memcpy(RejectCode, strReturn.Right(4), 4);
	}

	LeaveCriticalSection(&m_csLock);
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "SelectFile():return(%d)(%s)", nReturn, strReturn);
	return strReturn;
}

int CComRic::VerifyPIN(LPCTSTR szPinData)
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "VerifyPIN()");

	EnterCriticalSection(&m_csLock);							// 2007.09.07

	int		nResult = FALSE;
	int		nReturn = FALSE;
	int		nSendLength = 0;
	UCHAR	bRecvLength[1024];
	UCHAR	bRecvBuff[1024];
	UCHAR	bSendBuff[1024];
	CString strPinData("");

	memset(bRecvLength, 0, sizeof(bRecvLength));
	memset(bRecvBuff, 0, sizeof(bRecvBuff));
	memset(bSendBuff, 0, sizeof(bSendBuff));

//////////////////////////////////////////////////////////////////////
	memset(RejectCode, 0, sizeof(RejectCode));
//////////////////////////////////////////////////////////////////////
	
	strPinData.Format("%6.6s", szPinData);
	strPinData.Remove(' ');
	
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0x20;
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0x00;
	if (strPinData.GetLength() <= 4)
	{
		bSendBuff[nSendLength++] = 0x02;
		MakePack(strPinData.GetBuffer(0), &bSendBuff[nSendLength], strPinData.GetLength());
		nSendLength += 2;
	}
	else
	{
		bSendBuff[nSendLength++] = 0x03;
		MakePack(strPinData.GetBuffer(0), &bSendBuff[nSendLength], strPinData.GetLength());
		nSendLength += 3;
	}

HexDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "[VerifyPIN]", (char*)bSendBuff, nSendLength);
	nResult = dc_pro_commandlink(hComRic, nSendLength, bSendBuff, bRecvLength, bRecvBuff, 7, 40);
	if (nResult == 0)
	{
		MakeUnPack(bRecvBuff, RejectCode, 2);
		if (memcmp(RejectCode, "9000", 4) == 0)
			nReturn = TRUE;
	}

	LeaveCriticalSection(&m_csLock);
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "VerifyPIN():nResult[%d]return(%d)", nResult, nReturn);
	return nReturn;
}

CString CComRic::ReadBalance()
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "ReadBalance()");

	EnterCriticalSection(&m_csLock);							// 2007.09.07

	int		nReturn = FALSE;
	int		nSendLength = 0;
	int		nRecvLength = 0;
	UCHAR	bRecvLength[1024];
	UCHAR	bRecvBuff[1024];
	UCHAR	bSendBuff[1024];
	UCHAR	bTempBuff[1024];
	char	szTempBalance[1024];
	CString strReturn("");
	CString strBalance("");

	memset(bRecvLength, 0, sizeof(bRecvLength));
	memset(bRecvBuff, 0, sizeof(bRecvBuff));
	memset(bSendBuff, 0, sizeof(bSendBuff));
	memset(bTempBuff, 0, sizeof(bTempBuff));
	memset(szTempBalance, 0, sizeof(szTempBalance));

//////////////////////////////////////////////////////////////////////
	memset(RejectCode, 0, sizeof(RejectCode));
//////////////////////////////////////////////////////////////////////
	
	bSendBuff[nSendLength++] = 0x80;
	bSendBuff[nSendLength++] = 0x5C;
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0x02;
	bSendBuff[nSendLength++] = 0x04;

HexDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "[ReadBalance]", (char*)bSendBuff, nSendLength);
	nReturn = dc_pro_commandlink(hComRic, nSendLength, bSendBuff, bRecvLength, bRecvBuff, 7, 40);
	if (nReturn == 0)
	{
HexDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "[ReadBalance]", bRecvLength, 2);
		nRecvLength = MAKEWORD(bRecvLength[0], bRecvLength[1]);
HexDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "[ReadBalance]", bRecvBuff, nRecvLength);
		MakeUnPack(bRecvBuff, bTempBuff, nRecvLength);
		memcpy(RejectCode, &bTempBuff[8], 4);
		memcpy(szTempBalance, bTempBuff, 8);
		if (memcmp(RejectCode, "9000", 4) == 0)
		{
			dc_beep(hComRic, 10);
			strReturn.Format("%d", strtol(szTempBalance, NULL, 16));
//			strReturn.Format("%.2f", Asc2Float(strBalance.GetBuffer(0), strBalance.GetLength()) / 100);
		}
	}

	LeaveCriticalSection(&m_csLock);
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "ReadBalance():return(%d)(%s)", nReturn, strReturn);
	return strReturn;
}

CString CComRic::InitializeForLoad(LPCTSTR szAmount, LPCTSTR szTerminalID, int nKeyIndex)
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "InitializeForLoad()");

	EnterCriticalSection(&m_csLock);							// 2007.09.07

	int		nReturn = FALSE;
	int		nSendLength = 0;
	int		nRecvLength = 0;
	UCHAR	bRecvLength[1024];
	UCHAR	bRecvBuff[1024];
	UCHAR	bSendBuff[1024];
	UCHAR	bTempBuff[1024];
	CString strTemp("");
	CString strReturn("");

	memset(bRecvLength, 0, sizeof(bRecvLength));
	memset(bRecvBuff, 0, sizeof(bRecvBuff));
	memset(bSendBuff, 0, sizeof(bSendBuff));
	memset(bTempBuff, 0, sizeof(bTempBuff));

//////////////////////////////////////////////////////////////////////
	memset(RejectCode, 0, sizeof(RejectCode));
//////////////////////////////////////////////////////////////////////

	bSendBuff[nSendLength++] = 0x80;
	bSendBuff[nSendLength++] = 0x50;
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0x02;
	bSendBuff[nSendLength++] = 0x0B;
	bSendBuff[nSendLength++] = nKeyIndex;
	strTemp.Format("%08.8s", szAmount);
	MakePack(strTemp.GetBuffer(0), &bSendBuff[nSendLength], strTemp.GetLength());
	nSendLength += 4;
	strTemp.Empty();
	strTemp.Format("%012.12s", szTerminalID);
	MakePack(strTemp.GetBuffer(0), &bSendBuff[nSendLength], strTemp.GetLength());
	nSendLength += 6;
	bSendBuff[nSendLength++] = 0x10;

HexDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "[InitializeForLoad] bSendBuff", (char*)bSendBuff, nSendLength);
	nReturn = dc_pro_commandlink(hComRic, nSendLength, bSendBuff, bRecvLength, bRecvBuff, 7, 40);
	if (nReturn == 0)
	{
		nRecvLength = MAKEWORD(bRecvLength[0], bRecvLength[1]);
HexDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "[InitializeForLoad] bRecvBuff", (char*)bRecvBuff, nRecvLength);
		MakeUnPack(bRecvBuff, bTempBuff, nRecvLength);
		memcpy(RejectCode, &bTempBuff[32], 4);
		if (memcmp(RejectCode, "9000", 4) == 0)
		{
			dc_beep(hComRic, 10);
			strReturn.Format("%36.36s", bTempBuff);
		}
	}

	LeaveCriticalSection(&m_csLock);
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "InitializeForLoad():return(%d)(%s)", nReturn, strReturn);
	return strReturn;
}

CString CComRic::LoadEP(LPCTSTR szDate, LPCTSTR szTime, LPCTSTR szMac2)
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "LoadEP()");

	EnterCriticalSection(&m_csLock);							// 2007.09.07

	int		nReturn = FALSE;
	int		nSendLength = 0;
	int		nRecvLength = 0;
	UCHAR	bRecvLength[1024];
	UCHAR	bRecvBuff[1024];
	UCHAR	bSendBuff[1024];
	UCHAR	bTempBuff[1024];
	CString strTemp("");
	CString strReturn("");

	memset(bRecvLength, 0, sizeof(bRecvLength));
	memset(bRecvBuff, 0, sizeof(bRecvBuff));
	memset(bSendBuff, 0, sizeof(bSendBuff));
	memset(bTempBuff, 0, sizeof(bTempBuff));

//////////////////////////////////////////////////////////////////////
	memset(RejectCode, 0, sizeof(RejectCode));
//////////////////////////////////////////////////////////////////////

	bSendBuff[nSendLength++] = 0x80;
	bSendBuff[nSendLength++] = 0x52;
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0x00;
	bSendBuff[nSendLength++] = 0x0B;
	strTemp.Format("%08.8s", szDate);
	MakePack(strTemp.GetBuffer(0), &bSendBuff[nSendLength], strTemp.GetLength());
	nSendLength += 4;
	strTemp.Empty();
	strTemp.Format("%06.6s", szTime);
	MakePack(strTemp.GetBuffer(0), &bSendBuff[nSendLength], strTemp.GetLength());
	nSendLength += 3;
	strTemp.Format("%08.8s", szMac2);
	MakePack(strTemp.GetBuffer(0), &bSendBuff[nSendLength], strTemp.GetLength());
	nSendLength += 4;
	bSendBuff[nSendLength++] = 0x04;

HexDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "[LoadEP]", (char*)bSendBuff, nSendLength);
	nReturn = dc_pro_commandlink(hComRic, nSendLength, bSendBuff, bRecvLength, bRecvBuff, 7, 40);
	if (nReturn == 0)
	{
		nRecvLength = MAKEWORD(bRecvLength[0], bRecvLength[1]);
		MakeUnPack(bRecvBuff, bTempBuff, nRecvLength);
		strReturn.Format("%s", bTempBuff);
	}

	LeaveCriticalSection(&m_csLock);
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "LoadEP():return(%d)(%s)", nReturn, strReturn);
	return strReturn;
}

CString CComRic::GetRejectCode()
{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "GetRejectCode()");
	
	CString strReturn("");

	strReturn.Format("%4.4s", RejectCode);
	strReturn.Remove(' ');

MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "GetRejectCode():return(%s)", strReturn);
	return strReturn;
}

CString CComRic::CheckDeviceAction(int CheckTime)
{
	ULONG  WaitTime = TimerSet(CheckTime);
	
	while (TRUE)
	{
		if (RicDeviceStatus == NORMAL)
			return CHK_NORMAL;

		if (RicDeviceStatus == DOWN)
			return CHK_ERROR;

		if (!CheckTime)
		{
			return CHK_TIMEOVER;
		}

		if (CheckTimer(WaitTime))
		{
MsgDump(TRACE_CODE_MODE, "ComRic", __FILE__, __LINE__, "CheckDeviceAction():ERROR CODE[%s]", "9794901");
			RegSetStr(_REGKEY_DEVERROR, DEVNM_RIC, "9794901");
			RicDeviceStatus = DOWN;
			return CHK_TIMEOVER;
		}

		Delay_Msg(100);											// Performance Adjust
	}
	
	return CHK_NORMAL;
}

int CComRic::ClearErrorCode()
{
	int		nReturn = FALSE;

	nReturn = RegSetStr(_REGKEY_DEVERROR, DEVNM_RIC, "");

	return nReturn;
}

CString CComRic::GetErrorCode()
{
	CString strReturn("");

	strReturn = RegGetStr(_REGKEY_DEVERROR, DEVNM_RIC, "0000000");		
	strReturn += "0000000";										// Error Code Adjust
	strReturn = strReturn.Left(7);

	return strReturn;
}

int CComRic::GetDeviceStatus()
{
	return RicDeviceStatus;
}

int CComRic::GetDataStatus()
{
	EnterCriticalSection(&m_csLock);							// 2007.09.07

	int nResult = FALSE;
	
	nResult = RfReset();
	if (nResult != 0)
	{
		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}

	nResult = DetectCard();
	if (nResult != 0)
	{
		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}

	LeaveCriticalSection(&m_csLock);
	return TRUE;
}

CString CComRic::GetIcCardSerialNo()
{
	CString strReturn("");

	strReturn.Format("%lu", IcCardSerialNo);
	
	return MakeUnPack(strReturn.GetBuffer(0), strReturn.GetLength());
}

int CComRic::EntryEnable()
{
	return TRUE;
}

int CComRic::EntryDisable()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
