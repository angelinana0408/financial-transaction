/////////////////////////////////////////////////////////////////////////////
// ComDvr.cpp: implementation of the CComDvr class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Dll\DevDefine.h"

#include "..\..\H\Dll\ComDvr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComDvr
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CComDvr, CSerial)

CComDvr::CComDvr()
{
MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "CComDvr()");

/////////////////////////////////////////////////////////////////////////////
	m_bAutoDelete = FALSE;
/////////////////////////////////////////////////////////////////////////////

	Initialize();
	
MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "CComDvr():return");
}

CComDvr::CComDvr(DEVICEINFO DEVICE, NETWORKINFO NETWORK)
{
MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "CComDvr(DEVICEINFO, NETWORKINFO)");

/////////////////////////////////////////////////////////////////////////////
	m_bAutoDelete = FALSE;
/////////////////////////////////////////////////////////////////////////////

	m_DeviceInfo	= DEVICE;
	m_NetworkInfo	= NETWORK;
	Initialize();

MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "CComDvr(DEVICEINFO):return");
}

CComDvr::~CComDvr()
{
MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "~CComDvr()");
MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "~CComDvr():return");
}

BOOL CComDvr::InitInstance()
{
MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "InitInstance()");

	if (m_DeviceInfo.DvrDevice)
	{
		while ((WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT))
		{
			if (!WaitCommEvent(m_hComm, &m_dwEventMask, &m_osWait))
			{
				m_dwLastError = ::GetLastError();
				if (m_dwLastError == ERROR_INVALID_HANDLE)	break;
			}
			
			Delay_Msg(100);										// For Thread
			DvrMain();											// Dvr Main
		}
	}

MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "InitInstance():return");
	return FALSE;
}

BEGIN_MESSAGE_MAP(CComDvr, CSerial)
	//{{AFX_MSG_MAP(CComDvr)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//	DVR SEND & RECV SPEC
/////////////////////////////////////////////////////////////////////////////
//
//	SEND : 
//	POLLING			0x93 Length[2] Date[8] Time[6] ST BCC[2]
//	CARD INSERT		0x93 Length[2] Date[8] Time[6] CI CARDNO[19] BCC[2]
//	TRANSACITON		0x93 Length[2] Date[8] Time[6] TT CWD BCC[2]
//	MONEY PRESENT	0x93 Length[2] Date[8] Time[6] CE BCC[2]	=> 15 SEC WAIT
//	CARD PRESENT	0x93 Length[2] Date[8] Time[6] C0 BCC[2]	=> 15 SEC WAIT
//
//	RECV :
//	RESPONSE		0x83 Length[2] DVR DVRSTATUS[1] CAMERASTATUS[4] BCC[2]
//
/////////////////////////////////////////////////////////////////////////////
int CComDvr::Initialize()
{
MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "Initialize()");

/////////////////////////////////////////////////////////////////////////////
	AutoProc = 0;

	SendDelayTime = 0;
	RecvWaitTime = 0;
	DvrIdleTime = 0;

	DvrRecvLength = 0;											// Dvr Recv Buffer
	memset(DvrRecvBuffer, 0, sizeof(DvrRecvBuffer));

	DATASendLength = 0;											// Dvr Send Buffer
	memset(DATASendLengthTbl, 0, sizeof(DATASendLengthTbl));
	DATASendInTbl = 0;
	DATASendInTblTemp = 0;
	DATASendOutTbl = 0;
	DATASendOutTblTemp = 0;										// 2007.06.06
	memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
	memset(DATASendBufferTbl, 0, sizeof(DATASendBufferTbl));
	memset(DATASendDelayTimeTbl, 0, sizeof(DATASendDelayTimeTbl));
	memset(DATAGetTickCountTbl, 0, sizeof(DATAGetTickCountTbl));
	DATASendDelayTime = 0;
	DATAGetTickCount = 0;

	pDvrRecvBuffer = (DvrHeader *)DvrRecvBuffer;

	pDATASendBuffer = (DvrHeader *)DATASendBuffer;

	CheckRecvLength = 0;										// Dvr Recv Length
	CheckDataLength = 0;										// Dvr Data Length

	strDvrStatus = MakeUnPack(ZERO5, 5);						// 2006.05.15
/////////////////////////////////////////////////////////////////////////////
	
	DvrIdleTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DVRIDLETIME, 10);
	if (DvrIdleTime < 5)
		DvrIdleTime = 5;
	else
	if (DvrIdleTime > 60)
		DvrIdleTime = 60;
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DVRIDLETIME, DvrIdleTime);
MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "Initialize() _INIKEY_TIMEDEFINE_DVRIDLETIME[%d]", DvrIdleTime);

MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "Initialize():return");
	return TRUE;
}

int CComDvr::Open()
{
MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "Open()");

	Initialize();

	if (!OpenPort(m_DeviceInfo.DvrDevice))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[DVR] Open Port[%d] Fail", m_DeviceInfo.DvrDevice);
		MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "[DVR] Open Port[%d] Fail", m_DeviceInfo.DvrDevice);
		return FALSE;
	}

    SetupStat(CBR_9600, 8, NOPARITY, ONESTOPBIT);
    SetupFlow(FALSE, FALSE, FALSE);
    StartPort();

MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "Open():return");
	return TRUE;
}

int CComDvr::Close()
{
MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "Close()");

	int		nReturn = FALSE;

	nReturn = TerminatePort();

MsgDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "Close():return(%d)", nReturn);
	return nReturn;
}

int CComDvr::Write(BYTE* SendBuff, int Length, int nSendDelayTime)
{
	DATASendInTblTemp = DATASendInTbl;
	DATASendInTblTemp++;
	if (DATASendInTblTemp >= DVRSENDTBL)						// In Tbl Temp Adjust
		DATASendInTblTemp = 0;

	if (DATASendInTblTemp != DATASendOutTbl)					// Data Tbl Ok
	{
		DATASendLengthTbl[DATASendInTbl] = Length;
		memcpy(DATASendBufferTbl[DATASendInTbl], SendBuff, Length);
		DATASendDelayTimeTbl[DATASendInTbl] =  nSendDelayTime;
		DATAGetTickCountTbl[DATASendInTbl] =  TimerSet(nSendDelayTime);

		DATASendInTbl = DATASendInTblTemp;						// In Tbl Adjust
	}

	return TRUE;
}

CString CComDvr::GetStatus()
{
	return strDvrStatus;										// 2006.05.15
}

/////////////////////////////////////////////////////////////////////////////
int CComDvr::DvrMain()
{
	switch	(AutoProc)
	{
		case 0:													// Send Command
			DvrRecvBufferClear();
			if (DvrSendCmd())
			{
				RecvWaitTime = TimerSet(10);
				AutoProc++;										// Next Procedure
			}
			break;

		case 1:													// Recv Command
			if (CheckTimer(RecvWaitTime))
			{
				AutoProc = 0;									// Send Command
				DvrSetStatus();									// Set Status : 2006.05.15
			}
			else
			if (DvrRecvCmd())
				AutoProc++;										// Next Procedure
			break;

		case 2:													// Send Delay
			SendDelayTime = TimerSet(DvrIdleTime);				// 2005.12.02
			AutoProc++;											// Next Procedure
			break;
			
		case 3:													// Send Time Delay
			if (DvrSendCheck())
				AutoProc = 0;									// Send Command
			else
			if (CheckTimer(SendDelayTime))
				AutoProc = 0;									// Send Command
			break;
			
		default:
			AutoProc = 0;										// Send Command
			break;
	}

	return TRUE;
}

int CComDvr::DvrSendCmd()
{
	if (DvrSendCheck())											// Data Ok
	{
		memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
		memcpy(DATASendBuffer, DATASendBufferTbl[DATASendOutTbl], DATASendLengthTbl[DATASendOutTbl]);
		DATASendLength = DATASendLengthTbl[DATASendOutTbl];
		DATASendDelayTime = DATASendDelayTimeTbl[DATASendOutTbl];
		DATAGetTickCount = DATAGetTickCountTbl[DATASendOutTbl];

		DATASendOutTblTemp = DATASendOutTbl;					// 2007.06.06
		DATASendOutTblTemp++;
		if (DATASendOutTblTemp >= DVRSENDTBL)					// Out Tbl Adj.
			DATASendOutTblTemp = 0;
		DATASendOutTbl = DATASendOutTblTemp;

		if (!DATASendLength)
			return FALSE;

		while (!CheckTimer(DATAGetTickCount))
		{
			Delay_Msg(100);
			if (DvrSendCheck())									// Data Ok
				break;
		}
	}
	else														// Polling Make
	{
//	POLLING			0x93 Length[2] Date[8] Time[6] ST BCC[2]
		memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
		DATASendBuffer[0] = 0x93;
		DATASendBuffer[1] = '1';
		DATASendBuffer[2] = '8';
		memcpy(&DATASendBuffer[3], GetDate().GetBuffer(0), 8);
		memcpy(&DATASendBuffer[11], GetTime().GetBuffer(0), 6);
		memcpy(&DATASendBuffer[17], "ST", 2);
		BYTE Bcc = MakeBcc(&DATASendBuffer[1], 18);
		MakeUnPack(&Bcc, &DATASendBuffer[19], 1);
		DATASendLength = 21;
	}
	
	DvrSendData(DATASendBuffer, DATASendLength);
	return TRUE;
}

int CComDvr::DvrSendData(BYTE* SendBuff, int Length)
{
HexDump(TRACE_DUMP_MODE, "ComDvr", __FILE__, __LINE__, "[DVR SENDDATA]", (char*)SendBuff, Length);

	while (WritePort(SendBuff, Length) == FALSE)
		Delay_Msg();
	while (GetOutqueSize())
		Delay_Msg();
	
MsgDump(TRACE_DUMP_MODE, "ComDvr", __FILE__, __LINE__, "DvrSendData(...):return");
	return TRUE;
}

int CComDvr::DvrSendCheck()
{
	if (DATASendInTbl != DATASendOutTbl)						// Data Ok
		return TRUE;
	else
		return FALSE;
}

int CComDvr::DvrRecvCmd()
{
	BYTE	TempBuffer[DVRBUFFSIZE];
	int		nByte;
	int		CheckDataFlag = FALSE;
	int		RecvOkFlag = FALSE;									// 2005.12.23

	do															// Merge
	{
		nByte = ReadPort(TempBuffer, DVRBUFFSIZE);
		if (nByte <= 0)
			break;

HexDump(TRACE_DUMP_MODE, "ComDvr", __FILE__, __LINE__, "[DVR RECVDATA]", (char*)TempBuffer, nByte);

		if ((DvrRecvLength + nByte) < DVRBUFFSIZE)
		{
			memcpy(&DvrRecvBuffer[DvrRecvLength], TempBuffer, nByte);
			DvrRecvLength += nByte;
		}
	} while (TRUE);
	
	while (TRUE)
	{
		CheckDataFlag = DvrCheckData();
		
		if (CheckDataFlag)		
		{
			RecvOkFlag = TRUE;									// 2005.12.23
			if (DvrRecvLength > CheckRecvLength)				// Split
			{
				DvrRecvLength -= CheckRecvLength;
				memcpy(DvrRecvBuffer, &DvrRecvBuffer[CheckRecvLength], DvrRecvLength);
				continue;										// 2 Data
			}
			else												// Initialize
			{
				DvrRecvLength = 0;
				memset(DvrRecvBuffer, 0, sizeof(DvrRecvBuffer));
			}
		}
		
		break;
	} 

	return RecvOkFlag;
}

int CComDvr::DvrCheckData()
{
	int		DVR_BASE_LEN = 3;
	static	BYTE	TempBuffer[DVRBUFFSIZE];

	CheckRecvLength = DvrRecvLength;							// Dvr Recv Length
	CheckDataLength = 0;										// Dvr Data Length

	if (DvrRecvLength < DVR_BASE_LEN)							// Header Length Check
		return FALSE;											// Recv Waiting

	if (pDvrRecvBuffer->Data[0] != 0x83)						// 2006.08.22
	{
		DvrRecvBufferClear();
		return FALSE;
	}

	CheckDataLength = Asc2Int(&pDvrRecvBuffer->Data[1], 2);
	if (CheckDataLength > DVRBUFFSIZE)							// Data Length Adjust
		CheckDataLength = DVRBUFFSIZE;
	CheckRecvLength = CheckDataLength + DVR_BASE_LEN;			// Data Length(Header Length Include)
	if (CheckRecvLength > DVRBUFFSIZE)							// Max Length Adjust
		CheckRecvLength = DVRBUFFSIZE;

	if (DvrRecvLength < CheckRecvLength)						// Max Length Check
		return FALSE;											// Recv Waiting

	if (memcmp(&pDvrRecvBuffer->Data[3], "DVR", 3) != 0)		// 2006.08.22
	{
		DvrRecvBufferClear();
		return FALSE;
	}

	if (memcmp(TempBuffer, DvrRecvBuffer, CheckRecvLength) != 0)// 2006.02.09
	{
		memcpy(TempBuffer, DvrRecvBuffer, CheckRecvLength);
HexDump(TRACE_CODE_MODE, "ComDvr", __FILE__, __LINE__, "[DVR RECVDATA]", (char*)DvrRecvBuffer, CheckRecvLength);

	}
	
	// Data Recv Ok
	if (CheckRecvLength >= 11)									// 2006.05.15
		strDvrStatus =  MakeUnPack(&pDvrRecvBuffer->Data[6], 5);

	return TRUE;												// Clear
}

int CComDvr::DvrRecvBufferClear()
{
	memset(DvrRecvBuffer, 0, sizeof(DvrRecvBuffer));
	DvrRecvLength = 0;

	ClearPort();
	return TRUE;
}

int CComDvr::DvrSetStatus()
{
	CString	strTemp("");

	if (strDvrStatus.GetLength() >= 10)							// 2006.05.15
	{
		strTemp = "33";											// Dvr Error
		strTemp += strDvrStatus.Mid(2, 9);
		strDvrStatus = strTemp;
	}
	else
		strDvrStatus = "33" + MakeUnPack(ZERO4, 4);

	return TRUE;
}

