/////////////////////////////////////////////////////////////////////////////
// ComEtc.cpp: implementation of the CComEtc class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Dll\DevDefine.h"

#include "..\..\H\Dll\ComEtc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CComEtc
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CComEtc, CSerial)

CComEtc::CComEtc()
{
MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "CComEtc()");

/////////////////////////////////////////////////////////////////////////////
	m_bAutoDelete = FALSE;
/////////////////////////////////////////////////////////////////////////////

	Initialize();
	
MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "CComEtc():return");
}

CComEtc::CComEtc(DEVICEINFO DEVICE, NETWORKINFO NETWORK)
{
MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "CComEtc(DEVICEINFO, NETWORKINFO)");

/////////////////////////////////////////////////////////////////////////////
	m_bAutoDelete = FALSE;
/////////////////////////////////////////////////////////////////////////////

	m_DeviceInfo	= DEVICE;
	m_NetworkInfo	= NETWORK;
	Initialize();

MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "CComEtc(DEVICEINFO):return");
}

CComEtc::~CComEtc()
{
MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "~CComEtc()");
MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "~CComEtc():return");
}

BOOL CComEtc::InitInstance()
{
MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "InitInstance()");

	if (m_DeviceInfo.EtcComKind)
	{
		while ((WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT))
		{
			if (!WaitCommEvent(m_hComm, &m_dwEventMask, &m_osWait))
			{
				m_dwLastError = ::GetLastError();
				if (m_dwLastError == ERROR_INVALID_HANDLE)	break;
			}
			
			Delay_Msg(100);										// For Thread
			EtcMain();											// Etc Main
		}
	}

MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "InitInstance():return");
	return FALSE;
}

BEGIN_MESSAGE_MAP(CComEtc, CSerial)
	//{{AFX_MSG_MAP(CComEtc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//	CComEtc SEND & RECV SPEC
/////////////////////////////////////////////////////////////////////////////
//
//	SEND : 
//	POLLING			0xE0 0x00-0F 0x3F 0x00 0x00 BCC
//
//	RECV :
//	RESPONSE		0xF0 0x00-0F 0xD0 0xA0£­A7 0xA8£­A15 BCC
//
/////////////////////////////////////////////////////////////////////////////
int CComEtc::Initialize()
{
MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "Initialize()");

/////////////////////////////////////////////////////////////////////////////
	CheckTime = 0;
	AlarmAllOnFlag = FALSE;
	Alarm1OffFlag = FALSE;
	Alarm2OffFlag = FALSE;
	Alarm3OffFlag = FALSE;
	Alarm4OffFlag = FALSE;
	memset(AlarmOffDelayTimeTbl, 0, sizeof(AlarmOffDelayTimeTbl));
	AlarmOffDelayTimeTbl[0] = 60 * 30;							// Com RecvOver Time : 2006.09.18
	
	AutoProc = 0;

	SendDelayTime = 0;
	RecvWaitTime = 0;
	EtcIdleTime = 0;
	memset(EtcHostAddress, 0, sizeof(EtcHostAddress));

	EtcRecvLength = 0;											// Etc Recv Buffer
	memset(EtcRecvBuffer, 0, sizeof(EtcRecvBuffer));

	DATASendLength = 0;											// Etc Send Buffer
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

	pEtcRecvBuffer = (EtcHeader*)EtcRecvBuffer;

	pDATASendBuffer = (EtcHeader*)DATASendBuffer;

	CheckRecvLength = 0;										// Etc Recv Length
	CheckDataLength = 0;										// Etc Data Length
	memset(szEtcStatus, '0', sizeof(szEtcStatus));

/////////////////////////////////////////////////////////////////////////////
	
	EtcIdleTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ETCCOMIDLETIME, 10);
	if (EtcIdleTime < 2)
		EtcIdleTime = 2;
	else
	if (EtcIdleTime > 60)
		EtcIdleTime = 60;
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ETCCOMIDLETIME, EtcIdleTime);
MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "Initialize() _INIKEY_TIMEDEFINE_ETCCOMIDLETIME[%d]", EtcIdleTime);

	CString	strTemp("");
	strTemp = IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ETCCOMADDRESS, "00");
	if (strTemp.GetLength() >= 2)
		memcpy(EtcHostAddress, strTemp.Left(2), 2);
	else
		memcpy(EtcHostAddress, GetSprintf("%02.2s", strTemp), 2);
	IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ETCCOMADDRESS, GetSprintf("%2.2s", EtcHostAddress));
MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "Initialize() _INIKEY_TIMEDEFINE_ETCCOMADDRESS[%s]", GetSprintf("%2.2s", EtcHostAddress));

MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "Initialize():return");
	return TRUE;
}

int CComEtc::Open()
{
MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "Open()");

	Initialize();

	if (!OpenPort(m_DeviceInfo.EtcComKind))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ETC] Open Port[%d] Fail", m_DeviceInfo.EtcComKind);
		MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "[ETC] Open Port[%d] Fail", m_DeviceInfo.EtcComKind);
		return FALSE;
	}

    SetupStat(CBR_9600, 8, NOPARITY, ONESTOPBIT);
    SetupFlow(FALSE, FALSE, FALSE);
    StartPort();

MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "Open():return");
	return TRUE;
}

int CComEtc::Close()
{
MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "Close()");

	int		nReturn = FALSE;

	nReturn = TerminatePort();

MsgDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "Close():return(%d)", nReturn);
	return nReturn;
}

int CComEtc::Write(BYTE* SendBuff, int Length, int nSendDelayTime)
{
	DATASendInTblTemp = DATASendInTbl;
	DATASendInTblTemp++;
	if (DATASendInTblTemp >= ETCSENDTBL)						// In Tbl Temp Adjust
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

CString CComEtc::GetStatus()
{
	CString strReturn("");

	strReturn.Format("%8.8s", szEtcStatus);

	return strReturn;											// 2006.05.15
}

/////////////////////////////////////////////////////////////////////////////
int CComEtc::EtcMain()
{
	CheckTime++;
	if (CheckTime >= 10)
	{
		CheckTime = 0;
		for (int j = 0; j < ETCSENDTBL; j++)
		{
			if (AlarmOffDelayTimeTbl[j] > 0)
			{
				AlarmOffDelayTimeTbl[j] -= 1;
			}
		}
	}

	switch (AutoProc)
	{
		case 0:													// Send Command
			EtcRecvBufferClear();
			if (EtcSendCmd())
			{
				RecvWaitTime = TimerSet(5);
				AutoProc++;										// Next Procedure
			}
			break;

		case 1:													// Recv Command
			if (CheckTimer(RecvWaitTime))
			{
				AutoProc = 0;									// Send Command
				EtcSetStatus();									// Set Status : 2006.05.15
			}
			else
			if (EtcRecvCmd())
				AutoProc++;										// Next Procedure
			break;

		case 2:													// Send Delay
			SendDelayTime = TimerSet(EtcIdleTime);				// 2005.12.02
			AutoProc++;											// Next Procedure
			break;
			
		case 3:													// Send Time Delay
			if (EtcSendCheck())
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

int CComEtc::EtcSendCmd()
{
	if (EtcSendCheck())											// Data Ok
	{
		memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
		memcpy(DATASendBuffer, DATASendBufferTbl[DATASendOutTbl], DATASendLengthTbl[DATASendOutTbl]);
		DATASendLength = DATASendLengthTbl[DATASendOutTbl];
		DATASendDelayTime = DATASendDelayTimeTbl[DATASendOutTbl];
		DATAGetTickCount = DATAGetTickCountTbl[DATASendOutTbl];

		DATASendOutTblTemp = DATASendOutTbl;					// 2007.06.06
		DATASendOutTblTemp++;
		if (DATASendOutTblTemp >= ETCSENDTBL)					// Out Tbl Adj.
			DATASendOutTblTemp = 0;
		DATASendOutTbl = DATASendOutTblTemp;

		if (!DATASendLength)
			return FALSE;

		while (!CheckTimer(DATAGetTickCount))
		{
			Delay_Msg(100);
			if (EtcSendCheck())									// Data Ok
				break;
		}
	}
	else														// Polling Make
	{
//	POLLING			0xE0 0x00-0F 0x3F 0x00 0x00 BCC
		memset(DATASendBuffer, 0, sizeof(DATASendBuffer));		// 2006.08.22
		memset(&DATASendBuffer[0], 0xe0, 1);
		MakePack(EtcHostAddress, &DATASendBuffer[1], 2);
		if (AlarmAllOnFlag)
		{
			AlarmAllOnFlag = FALSE;
			memset(&DATASendBuffer[2], 0x3d, 1);
			memset(&DATASendBuffer[3], 0x0f, 1);
			memset(&DATASendBuffer[4], 0x00, 1);
		}
		else
		if ((Alarm1OffFlag)	&& (AlarmOffDelayTimeTbl[1] == 0))
		{
			Alarm1OffFlag = FALSE;
			AlarmOffDelayTimeTbl[1] = -1;
			memset(&DATASendBuffer[2], 0x3d, 1);
			memset(&DATASendBuffer[3], 0x0e, 1);
			memset(&DATASendBuffer[4], 0x00, 1);
		}
		else
		if ((Alarm2OffFlag)	&& (AlarmOffDelayTimeTbl[2] == 0))
		{
			Alarm2OffFlag = FALSE;
			AlarmOffDelayTimeTbl[2] = -1;
			memset(&DATASendBuffer[2], 0x3d, 1);
			memset(&DATASendBuffer[3], 0x0c, 1);
			memset(&DATASendBuffer[4], 0x00, 1);
		}
		else
		if ((Alarm3OffFlag)	&& (AlarmOffDelayTimeTbl[3] == 0))
		{
			Alarm3OffFlag = FALSE;
			AlarmOffDelayTimeTbl[3] = -1;
			memset(&DATASendBuffer[2], 0x3d, 1);
			memset(&DATASendBuffer[3], 0x08, 1);
			memset(&DATASendBuffer[4], 0x00, 1);
		}
		else
		if ((Alarm4OffFlag)	&& (AlarmOffDelayTimeTbl[4] == 0))
		{
			Alarm4OffFlag = FALSE;
			AlarmOffDelayTimeTbl[4] = -1;
			memset(&DATASendBuffer[2], 0x3d, 1);
			memset(&DATASendBuffer[3], 0x00, 1);
			memset(&DATASendBuffer[4], 0x00, 1);
		}
		else
		{
			memset(&DATASendBuffer[2], 0x3f, 1);
			memset(&DATASendBuffer[3], 0x00, 2);
		}
		
		DATASendBuffer[5] = 0x00;
		for (int i = 0; i < 5; i++)
			DATASendBuffer[5] += DATASendBuffer[i];

		DATASendLength = 6;
	}
	
	EtcSendData(DATASendBuffer, DATASendLength);
	return TRUE;
}

int CComEtc::EtcSendData(BYTE* SendBuff, int Length)
{
HexDump(TRACE_DUMP_MODE, "ComEtc", __FILE__, __LINE__, "[ETC SENDDATA]", (char*)SendBuff, Length);

	static	BYTE	TempBuffer[ETCBUFFSIZE];

	if (memcmp(TempBuffer, SendBuff, Length) != 0)				// 2006.09.11
	{
		memcpy(TempBuffer, SendBuff, Length);
HexDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "[ETC SENDDATA]", (char*)SendBuff, Length);
	}

	while (WritePort(SendBuff, Length) == FALSE)
		Delay_Msg();
	while (GetOutqueSize())
		Delay_Msg();
	
MsgDump(TRACE_DUMP_MODE, "ComEtc", __FILE__, __LINE__, "EtcSendData(...):return");
	return TRUE;
}

int CComEtc::EtcSendCheck()
{
	if (DATASendInTbl != DATASendOutTbl)						// Data Ok
		return TRUE;
	else
		return FALSE;
}

int CComEtc::EtcRecvCmd()
{
	BYTE	TempBuffer[ETCBUFFSIZE];
	int		nByte;
	int		CheckDataFlag = FALSE;
	int		RecvOkFlag = FALSE;									// 2005.12.23

	do															// Merge
	{
		nByte = ReadPort(TempBuffer, ETCBUFFSIZE);
		if (nByte <= 0)
			break;

HexDump(TRACE_DUMP_MODE, "ComEtc", __FILE__, __LINE__, "[ETC RECVDATA]", (char*)TempBuffer, nByte);

		if ((EtcRecvLength + nByte) < ETCBUFFSIZE)
		{
			memcpy(&EtcRecvBuffer[EtcRecvLength], TempBuffer, nByte);
			EtcRecvLength += nByte;
		}
	} while (TRUE);
	
	while (TRUE)
	{
		CheckDataFlag = EtcCheckData();
		
		if (CheckDataFlag)		
		{
			RecvOkFlag = TRUE;									// 2005.12.23
			if (EtcRecvLength > CheckRecvLength)				// Split
			{
				EtcRecvLength -= CheckRecvLength;
				memcpy(EtcRecvBuffer, &EtcRecvBuffer[CheckRecvLength], EtcRecvLength);
				continue;										// 2 Data
			}
			else												// Initialize
			{
				EtcRecvLength = 0;
				memset(EtcRecvBuffer, 0, sizeof(EtcRecvBuffer));
			}
		}
		
		break;
	} 

	return RecvOkFlag;
}

int CComEtc::EtcCheckData()
{
	int		ETC_BASE_LEN = 0;
	static	BYTE	TempBuffer[ETCBUFFSIZE];
	BYTE	TempByte = 0x00;

//	RESPONSE		0xF0 0x00-0F 0xD0 0xA0£­A7 0xA8£­A15 BCC

	CheckRecvLength = EtcRecvLength;							// Etc Recv Length
	CheckDataLength = 0;										// Etc Data Length

	if (EtcRecvLength < ETC_BASE_LEN)							// Header Length Check
		return FALSE;											// Recv Waiting

	CheckDataLength = CheckRecvLength;
	if (CheckDataLength > ETCBUFFSIZE)							// Data Length Adjust
		CheckDataLength = ETCBUFFSIZE;
	CheckRecvLength = CheckDataLength + ETC_BASE_LEN;			// Data Length(Header Length Include)
	if (CheckRecvLength > ETCBUFFSIZE)							// Max Length Adjust
		CheckRecvLength = ETCBUFFSIZE;

	if (EtcRecvLength < CheckRecvLength)						// Max Length Check
		return FALSE;											// Recv Waiting

	if (memcmp(TempBuffer, EtcRecvBuffer, CheckRecvLength) != 0)// 2006.02.09
	{
		memcpy(TempBuffer, EtcRecvBuffer, CheckRecvLength);
HexDump(TRACE_CODE_MODE, "ComEtc", __FILE__, __LINE__, "[ETC RECVDATA]", (char*)EtcRecvBuffer, CheckRecvLength);
	}
	
	// Data Recv Ok
	if (CheckRecvLength < 6)
		return FALSE;

	if (pEtcRecvBuffer->Data[0] != 0xf0)					// 2006.08.22
	{
		EtcRecvBufferClear();
		return FALSE;
	}

	if ((pEtcRecvBuffer->Data[2] == 0xd0)	||					// 2006.08.22
		(pEtcRecvBuffer->Data[2] == 0xd2))						// 2006.08.22
		;
	else
	{
		EtcRecvBufferClear();
		return FALSE;
	}

	TempByte = 0x00;											// Make Bcc
	for (int i = 0; i < 5; i++)
		TempByte += pEtcRecvBuffer->Data[i];
	if (TempByte != pEtcRecvBuffer->Data[5])					// Bcc Check : 2006.08.22
	{
		EtcRecvBufferClear();
		return FALSE;
	}
	
	if (pEtcRecvBuffer->Data[2] == 0xd0)						// 2006.08.22
	{
		AlarmOffDelayTimeTbl[0] = 60 * 30;						// Com RecvOver Time : 2006.09.18
		memset(szEtcStatus, '0', sizeof(szEtcStatus));
		TempByte = pEtcRecvBuffer->Data[3];
		if (TempByte & 0x01)
			szEtcStatus[0] = '1';

		if (TempByte & 0x02)
			szEtcStatus[1] = '1';

		if (TempByte & 0x04)
			szEtcStatus[2] = '1';

		if (TempByte & 0x08)
			szEtcStatus[3] = '1';

		if (TempByte & 0x10)
			szEtcStatus[4] = '1';

		if (TempByte & 0x20)
			szEtcStatus[5] = '1';
		
		if (TempByte & 0x40)
			szEtcStatus[6] = '1';

		if (TempByte & 0x80)
			szEtcStatus[7] = '1';

		if (TempByte)
		{
			AlarmAllOnFlag = TRUE;
			Alarm1OffFlag = TRUE;
			Alarm2OffFlag = TRUE;
			Alarm3OffFlag = TRUE;
			Alarm4OffFlag = TRUE;
			AlarmOffDelayTimeTbl[1] = 10;
			AlarmOffDelayTimeTbl[2] = 60 * 5;
			AlarmOffDelayTimeTbl[3] = 60 * 20;
			AlarmOffDelayTimeTbl[4] = 60 * 60;
		}
	}

	return TRUE;												// Clear
}

int CComEtc::EtcRecvBufferClear()
{
	memset(EtcRecvBuffer, 0, sizeof(EtcRecvBuffer));
	EtcRecvLength = 0;

	ClearPort();
	return TRUE;
}

int CComEtc::EtcSetStatus()
{
	if (AlarmOffDelayTimeTbl[0] == 0)							// RecvOver Time : 2006.09.18
		memset(szEtcStatus, '2', sizeof(szEtcStatus));
	return TRUE;
}

