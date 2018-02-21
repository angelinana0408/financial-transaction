/////////////////////////////////////////////////////////////////////////////
// ComUps.cpp: implementation of the CComUps class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\ComUps.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CComUps
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CComUps, CSerial)

CComUps::CComUps()
{
MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "CComUps()");

/////////////////////////////////////////////////////////////////////////////
	m_bAutoDelete = FALSE;
/////////////////////////////////////////////////////////////////////////////

	Initialize();
	
MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "CComUps():return");
}

CComUps::CComUps(DEVICEINFO DEVICE, NETWORKINFO NETWORK)
{
MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "CComUps(DEVICEINFO, NETWORKINFO)");

/////////////////////////////////////////////////////////////////////////////
	m_bAutoDelete = FALSE;
/////////////////////////////////////////////////////////////////////////////

	m_DeviceInfo	= DEVICE;
	m_NetworkInfo	= NETWORK;
	Initialize();

MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "CComUps(DEVICEINFO):return");
}

CComUps::~CComUps()
{
MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "~CComUps()");
MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "~CComUps():return");
}

BOOL CComUps::InitInstance()
{
MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "InitInstance()");

	if (m_DeviceInfo.PowerHandler == EUC_TYPE)
	{
		while ((WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT))
		{
			if (!WaitCommEvent(m_hComm, &m_dwEventMask, &m_osWait))
			{
				m_dwLastError = ::GetLastError();
				if (m_dwLastError == ERROR_INVALID_HANDLE)	break;
			}
			
			Delay_Msg(100);										// For Thread
			UpsMain();											// Ups Main
		}
	}

MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "InitInstance():return");
	return FALSE;
}

BEGIN_MESSAGE_MAP(CComUps, CSerial)
	//{{AFX_MSG_MAP(CComUps)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//	UPS SEND & RECV SPEC
/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////
int CComUps::Initialize()
{
MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "Initialize()");

/////////////////////////////////////////////////////////////////////////////
	AutoProc = 0;

	SendDelayTime = 0;
	RecvWaitTime = 0;
	UpsIdleTime = 0;
	UpsFailCount = 0;
	UpsBatLowCount = 0;
	UpsAcStopCount = 0;

	UpsRecvLength = 0;											// Ups Recv Buffer
	memset(UpsRecvBuffer, 0, sizeof(UpsRecvBuffer));

	DATASendLength = 0;											// Ups Send Buffer
	memset(DATASendLengthTbl, 0, sizeof(DATASendLengthTbl));
	DATASendInTbl = 0;
	DATASendInTblTemp = 0;
	DATASendOutTbl = 0;
	DATASendOutTblTemp = 0;										// 2007.06.06
	memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
	memset(DATASendBufferTbl, 0, sizeof(DATASendBufferTbl));

	pUpsRecvBuffer = (UpsHeader *)UpsRecvBuffer;

	pDATASendBuffer = (UpsHeader *)DATASendBuffer;

	CheckRecvLength = 0;										// Ups Recv Length
	CheckDataLength = 0;										// Ups Data Length
/////////////////////////////////////////////////////////////////////////////

	UpsIdleTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_UPSIDLETIME, 10);
	if (UpsIdleTime < 5)
		UpsIdleTime = 5;
	else
	if (UpsIdleTime > 60)
		UpsIdleTime = 60;
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_UPSIDLETIME, UpsIdleTime);
MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "Initialize() _INIKEY_TIMEDEFINE_UPSIDLETIME[%d]", UpsIdleTime);

MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "Initialize():return");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//		Hardware:
//		BAUD RATE . . . . . . . . . . : 	2400 bps 
//		DATA LENGTH . . . . . . . . . :		8 bits 
//		STOP BIT	  . . . . . . . . : 	1 bits 
//		PARITY 	. . . . . . . . . . . : 	NONE 
/////////////////////////////////////////////////////////////////////////////
//		(1)Status Inquiry : 
//		Computer : Q<cr>
//		UPS: UPS status data stream ,such as MMM.MNNN.NPPP.PQQQRR.RS.SSTT.TU<cr> UPS status data stream :
//					(a) Start byte : (
//		
//					(b) I/P voltage : MMM.M 
//							M is an integer number ranging from 0 to 9.
//							The unit is Volt.  
//		
//					(c) I/P fault voltage : NNN.N 
//							N is an integer number ranging from 0 to 9.
//							The unit is Volt.
//		
//					(d) O/P voltage : PPP.P
//							P is an integer number ranging from 0 to 9.
//							The unit is Volt.
//		
//					(e) O/P load : QQQ   For Off-line UPS:
//							QQQ is a percent of maximum VA, not an absolute value.
//		
//					(f) I/P frequency : RR.R 
//							R is an integer number ranging from 0 to 9. The unit is HZ.
//		
//					(g) Battery voltage: SS.S or S.SS
//							S is an integer number ranging from 0 to 9.
//		
//					(h) Temperature : TT.T 
//							T is an integer number ranging from 0 to 9. The unit is degree of centigrade.
//					(i) UPS Status : <U>		
//							<U> is one byte of binary information such as <b7b6b5b4b3b2b1b0>.
//
//						//////////////////////////////////////////////////////////
//						// Bit 	// 	 Remarks								    //
//						//	7 	//	 1 : Utility Fail ( Immediate )				//
//						//	6 	//	 1 : Battery Low							//
//						//	5 	//	 1 : Bypass/Boost Active					//
//						//	4 	//	 1 : UPS Failed								//
//						//	3 	//	 1 : UPS Type is Standby (0 is On-line)		//
//						//	2 	//	 1 : Test in Progress						//
//						//	1 	//	 1 : Shutdown Active						//
//						//	0 	//	 Reserved (always 0)						//
//						//////////////////////////////////////////////////////////	
//					(j) Stop Byte: <cr>
//					
//					Example : 
//					Computer : Q<cr> 
//					UPS: (208.4140.0208.403459.92.0535.0U<cr> Let U is 00110000 
//					Means : 	I/P voltage is 208.4V.
//								I/P fault voltage is 140.0V.
//								O/P voltage is 208.4V.
//								O/P load is 34 %.
//								I/P frequency is 59.9 HZ.
//								Battery voltage is 2.05V.
//								Temperature is 35.0 degrees of centigrade.
//								UPS type is on-line , UPS failed, Bypass active , and Shutdown not active .
//
//
//
//		(2) Status Inquiry : 
//		Computer : Q1<cr> 
//		UPS: UPS status data stream ,such as (MMM.M NNN.N PPP.P QQQ RR.R S.SS TT.Tb7b6b5b4b3b2b1b0<cr>
//			 The binary code is modified to 8 bytes ASCII code for avoiding the binary code confused with ASCII 
//			 control code. Between each data stream add one space for the data separation.
//			 
//		Example :
//		Computer : Q1<cr>
//		UPS: (208.4 140.0 208.4 034 59.9 2.05 35.0 00110000<cr>
//			 
//		Means : I/P voltage is 208.4V.
//				I/P fault voltage is 140.0V. 
//				O/P voltage is 208.4V. 
//				O/P load is 34 % . 
//				I/P frequency is 59.9 HZ. 
//				Battery voltage is 2.05V. 
//				Temperature is 35.0 degrees of centigrade. 
//				UPS type is on-line , UPS failed, Bypass active , and Shutdown not active . 
//		
//
//		Shut Down Command :
//		Computer : S<n><cr>
//		UPS: Shut UPS output off in <n> minutes. 
//			 (a) The UPS output will be off in <n> minutes, evenif the utility is present.
//			 (b) But if the battery under occur before <n> minutes, the output is turned off immediately.
//			 (c) After UPS shut down, the controller of UPS monitors the utility. If the utility is there, the UPS will waiting for 10 seconds and connect the utility to output. 
//			 (d) <n> is a number ranging from .2,.3,...,01,02,.., to 10.
//			 For example : S.3<cr> --- shut output off in (.3) minutes
/////////////////////////////////////////////////////////////////////////////
int CComUps::Open()
{
MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "Open()");

	Initialize();

	if (!OpenPort(6))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[UPS] Open Port Fail");
		MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "[UPS] Open Port Fail");
		return FALSE;
	}

    SetupStat(CBR_2400, 8, NOPARITY, ONESTOPBIT);
    SetupFlow(FALSE, FALSE, FALSE);
    StartPort();

MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "Open():return");
	return TRUE;
}

int CComUps::Close()
{
MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "Close()");

	int		nReturn = FALSE;

	nReturn = TerminatePort();

MsgDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "Close():return(%d)", nReturn);
	return nReturn;
}

int CComUps::GetPowerStatus()
{
	int	UpsStatus = POWER_NORMAL;
	
	if (UpsFailCount >= 5)
		UpsStatus |= POWER_ABNORMAL;
	if (UpsBatLowCount >= 5)
		UpsStatus |= INT_BATTERY_LOW;
	if (UpsAcStopCount >= 5)
		UpsStatus |= EXT_AC_OFF;
	
	return UpsStatus;
}

int CComUps::PowerOff(int nWaitSec)
{
	int		nTempTime = 0;

	DATASendInTblTemp = DATASendInTbl;
	DATASendInTblTemp++;
	if (DATASendInTblTemp >= UPSSENDTBL)						// In Tbl Temp Adjust
		DATASendInTblTemp = 0;

	if (DATASendInTblTemp != DATASendOutTbl)					// Data Tbl Ok
	{
		DATASendLengthTbl[DATASendInTbl] = 4;
		DATASendBufferTbl[DATASendInTbl][0] = 'S';

		if (nWaitSec >= 60)
			nTempTime = nWaitSec / 60;
		if (nTempTime > 10)
			nTempTime = 10;

		if (nTempTime)
			memcpy(&DATASendBufferTbl[DATASendInTbl][1], GetSprintf("%02d", nTempTime), 2);
		else
		{
			nTempTime = nWaitSec / 6;
			if (nTempTime < 1)
				nTempTime = 1;
			
			DATASendBufferTbl[DATASendInTbl][1] = '.';
			memcpy(&DATASendBufferTbl[DATASendInTbl][2], Int2Asc(nTempTime), 1);
		}

		DATASendBufferTbl[DATASendInTbl][3] = A_CR;

		DATASendInTbl = DATASendInTblTemp;						// In Tbl Adjust
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CComUps::UpsMain()
{
	switch	(AutoProc)
	{
		case 0:													// Send Command
			UpsRecvBufferClear();
			if (UpsSendCmd())
			{
				RecvWaitTime = TimerSet(10);
				AutoProc++;										// Next Procedure
			}
			break;

		case 1:													// Recv Command
			if (CheckTimer(RecvWaitTime))
				AutoProc = 0;									// Send Command
			else
			if (UpsRecvCmd())
				AutoProc++;										// Next Procedure
			break;

		case 2:													// Send Delay
			SendDelayTime = TimerSet(UpsIdleTime);				// 2005.12.02
			AutoProc++;											// Next Procedure
			break;
			
		case 3:													// Send Time Delay
			if (UpsSendCheck())
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

int CComUps::UpsSendCmd()
{
	if (UpsSendCheck())											// Data Ok
	{
		memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
		memcpy(DATASendBuffer, DATASendBufferTbl[DATASendOutTbl], DATASendLengthTbl[DATASendOutTbl]);
		DATASendLength = DATASendLengthTbl[DATASendOutTbl];

		DATASendOutTblTemp = DATASendOutTbl;					// 2007.06.06
		DATASendOutTblTemp++;
		if (DATASendOutTblTemp >= UPSSENDTBL)					// Out Tbl Adj.
			DATASendOutTblTemp = 0;
		DATASendOutTbl = DATASendOutTblTemp;

		if (!DATASendLength)
			return FALSE;
																// 2006.02.09
HexDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "[UPS SENDDATA]", (char*)DATASendBuffer, DATASendLength);
	}
	else														// Polling Make
	{
//		ATM: Q1<cr>
//		UPS: (208.4 140.0 208.4 034 59.9 2.05 35.0 00110000<cr>
		memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
		DATASendBuffer[0] = 'Q';
		DATASendBuffer[1] = '1';
		DATASendBuffer[2] = A_CR;
		DATASendLength = 3;
	}
	
	UpsSendData(DATASendBuffer, DATASendLength);
	return TRUE;
}

int CComUps::UpsSendData(BYTE* SendBuff, int Length)
{
HexDump(TRACE_DUMP_MODE, "ComUps", __FILE__, __LINE__, "[UPS SENDDATA]", (char*)SendBuff, Length);

	while (WritePort(SendBuff, Length) == FALSE)
		Delay_Msg();
	while (GetOutqueSize())
		Delay_Msg();
	
MsgDump(TRACE_DUMP_MODE, "ComUps", __FILE__, __LINE__, "UpsSendData(...):return");
	return TRUE;
}

int CComUps::UpsSendCheck()
{
	if (DATASendInTbl != DATASendOutTbl)						// Data Ok
		return TRUE;
	else
		return FALSE;
}

int CComUps::UpsRecvCmd()
{
	BYTE	TempBuffer[UPSBUFFSIZE];
	int		nByte;
	int		CheckDataFlag = FALSE;
	int		RecvOkFlag = FALSE;									// 2005.12.23

	do															// Merge
	{
		nByte = ReadPort(TempBuffer, UPSBUFFSIZE);
		if (nByte <= 0)
			break;

HexDump(TRACE_DUMP_MODE, "ComUps", __FILE__, __LINE__, "[UPS RECVDATA]", (char*)TempBuffer, nByte);

		if ((UpsRecvLength + nByte) < UPSBUFFSIZE)
		{
			memcpy(&UpsRecvBuffer[UpsRecvLength], TempBuffer, nByte);
			UpsRecvLength += nByte;
		}
	} while (TRUE);
	
	while (TRUE)
	{
		CheckDataFlag = UpsCheckData();
		
		if (CheckDataFlag)		
		{
			RecvOkFlag = TRUE;									// 2005.12.23
			if (UpsRecvLength > CheckRecvLength)				// Split
			{
				UpsRecvLength -= CheckRecvLength;
				memcpy(UpsRecvBuffer, &UpsRecvBuffer[CheckRecvLength], UpsRecvLength);
				continue;										// 2 Data
			}
			else												// Initialize
			{
				UpsRecvLength = 0;
				memset(UpsRecvBuffer, 0, sizeof(UpsRecvBuffer));
			}
		}
		
		break;
	} 

	return RecvOkFlag;
}

int CComUps::UpsCheckData()
{
	int				UPS_BASE_LEN = 0;
	CString			strTemp;
	CStringArray	strTempArray;

	CheckRecvLength = UpsRecvLength;							// Ups Recv Length
	CheckDataLength = 0;										// Ups Data Length

	for (int i = 0; i < UpsRecvLength; i++)
	{
		if (UpsRecvBuffer[i] == A_CR)
		{
			CheckDataLength = i + 1;
			break;
		}
	}
	
	if (CheckDataLength < 1)									// Not Found
		return FALSE;

	if (CheckDataLength > UPSBUFFSIZE)							// Data Length Adjust
		CheckDataLength = UPSBUFFSIZE;
	CheckRecvLength = CheckDataLength + UPS_BASE_LEN;			// Data Length(Header Length Include)
	if (CheckRecvLength > UPSBUFFSIZE)							// Max Length Adjust
		CheckRecvLength = UPSBUFFSIZE;

	if (UpsRecvLength < CheckRecvLength)						// Max Length Check
		return FALSE;											// Recv Waiting

	// Data Recv Ok
	if (CheckRecvLength <= 3)
		return FALSE;
	else
	if (CheckRecvLength == 4)									// 2006.02.09
	{
HexDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "[UPS RECVDATA]", (char*)UpsRecvBuffer, CheckRecvLength);
	}
	else
	{
		Strcpy(strTemp, UpsRecvBuffer, CheckRecvLength);
		SplitString(strTemp, SPACE, strTempArray);
		
		if (strTempArray.GetSize() == 8)						// Fix Check
			;
		else
		{
			UpsRecvBufferClear();								// 2006.08.22
			return FALSE;
		}

		if (strTempArray[7].GetBuffer(0)[3] == '1')				// Ups Fail
			UpsFailCount++; 
		else
			UpsFailCount = 0;

		if (strTempArray[7].GetBuffer(0)[1] == '1')				// Battery Low
			UpsBatLowCount++; 
		else
			UpsBatLowCount = 0;

		if (strTempArray[7].GetBuffer(0)[0] == '1')				// AC Stop
			UpsAcStopCount++; 
		else
			UpsAcStopCount = 0;

		if ((UpsFailCount >= 1)		||							// 2006.02.09
			(UpsBatLowCount >= 1)	||
			(UpsAcStopCount >= 1))
		{
HexDump(TRACE_CODE_MODE, "ComUps", __FILE__, __LINE__, "[UPS RECVDATA]", (char*)UpsRecvBuffer, CheckRecvLength);
		}
	}
	
	return TRUE;												// Clear
}

int CComUps::UpsRecvBufferClear()
{
	memset(UpsRecvBuffer, 0, sizeof(UpsRecvBuffer));
	UpsRecvLength = 0;

	ClearPort();
	return TRUE;
}

