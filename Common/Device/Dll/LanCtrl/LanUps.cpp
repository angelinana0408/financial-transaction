/////////////////////////////////////////////////////////////////////////////
// LanUps.cpp: implementation of the CLanUps class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\LanUps.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
CLanUps::CLanUps(DEVICEINFO DEVICE, NETWORKINFO NETWORK)
{
	m_DeviceInfo	= DEVICE;
	m_NetworkInfo	= NETWORK;

	ThreadUse		= FALSE;
	Doing			= FALSE;
	AutoProc		= 0;

	OpenWaitTime	= 0;
	SendDelayTime	= 0;
	RecvWaitTime	= 0;
	UpsIdleTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_UPSIDLETIME, 10);
	if (UpsIdleTime < 5)
		UpsIdleTime = 5;
	else
	if (UpsIdleTime > 60)
		UpsIdleTime = 60;
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_UPSIDLETIME, UpsIdleTime);
MsgDump(TRACE_CODE_MODE, "LanUps", __FILE__, __LINE__, "CLanUps() _INIKEY_TIMEDEFINE_UPSIDLETIME[%d]", UpsIdleTime);
	LineOpenRetryCount = 0;
	UpsFailCount = 0;
	UpsBatLowCount = 0;
	UpsAcStopCount = 0;
	
	UPSRecvLength	= 0;
	memset(UPSRecvBuffer, 0, sizeof(UPSRecvBuffer));

	DATASendLength	= 0;
	memset(DATASendLengthTbl, 0, sizeof(DATASendLengthTbl));
	DATASendInTbl	= 0;
	DATASendInTblTemp = 0;
	DATASendOutTbl	= 0;
	DATASendOutTblTemp = 0;
	memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
	memset(DATASendBufferTbl, 0, sizeof(DATASendBufferTbl));

/////////////////////////////////////////////////////////////////////////////
	memset(m_IpAddress, 0, sizeof(m_IpAddress));
	m_PortNumber	= 0;

	sprintf(m_IpAddress, "%15.15s", IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_UPSIP, "192.168.001.250"));
	m_PortNumber = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_UPSPORT, 50000);

	m_pSocketUps	= NULL;

	m_InitOpen		= FALSE;
	m_LineStatus	= LINE_CLOSE;

	if ((strlen(m_IpAddress) == 0)				||				// 2005.12.28
		(memcmp(m_IpAddress, "0.", 2) == 0)		||
		(memcmp(m_IpAddress, "00.", 3) == 0)	||
		(memcmp(m_IpAddress, "000.", 4) == 0)	||
		(m_PortNumber == 0))
	{
		m_pSocketUps = new CSocketUps(m_IpAddress, m_PortNumber);
		Doing = FALSE;
	}
	else
	{
		// Ups Ip Adjust										// 2005.12.29
		BYTE	AtmIPAddress[4], AtmSubnetMask[4], UpsIPAddress[4];
		
		for (int i = 0; i < 4; i++)
		{
			AtmIPAddress[i]		= Asc2Int(&m_NetworkInfo.AtmIPAddress[i * 4], 3);
			AtmSubnetMask[i]	= Asc2Int(&m_NetworkInfo.AtmSubnetMask[i * 4], 3);
			UpsIPAddress[i]		= Asc2Int(&m_IpAddress[i * 4], 3);

			AtmIPAddress[i]		= AtmIPAddress[i] & AtmSubnetMask[i];
			UpsIPAddress[i]		= UpsIPAddress[i] & (~(AtmSubnetMask[i]));
			UpsIPAddress[i]		= AtmIPAddress[i] | UpsIPAddress[i];
		}

		memset(m_IpAddress, 0, sizeof(m_IpAddress));
		sprintf(m_IpAddress, "%3d.%3d.%3d.%3d", UpsIPAddress[0],
												UpsIPAddress[1],
												UpsIPAddress[2],
												UpsIPAddress[3]);

		m_pSocketUps = new CSocketUps(m_IpAddress, m_PortNumber);
		Doing = TRUE;
		AfxBeginThread(UpsMainThread, this, THREAD_PRIORITY_NORMAL);
	}
}

CLanUps::~CLanUps()
{
	if (m_pSocketUps)	{	delete m_pSocketUps;	m_pSocketUps	= NULL;	}
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
UINT UpsMainThread(LPVOID pParam)
{
MsgDump(TRACE_CODE_MODE, "LanUps", __FILE__, __LINE__, "UpsMainThread()");

	CLanUps*	m_pUps = (CLanUps*)pParam;

/////////////////////////////////////////////////////////////////////////////
	m_pUps->ThreadUse = TRUE;
/////////////////////////////////////////////////////////////////////////////
	m_pUps->AutoProc = 0;										// Initial Open Value
	m_pUps->LineOpenRetryCount = 0;
	m_pUps->UpsFailCount = 0;
	m_pUps->UpsBatLowCount = 0;
	m_pUps->UpsAcStopCount = 0;

	while (m_pUps->Doing)
	{
		Delay_Msg(100);											// Performance Adjust
		switch (m_pUps->AutoProc)
		{
			case 0:												// Open Command
				m_pUps->LineOpen(30, TRUE);							
				m_pUps->OpenWaitTime = TimerSet(30);
				m_pUps->AutoProc++;								// Next Procedure
				m_pUps->LineOpenRetryCount++;
				break;

			case 1:												// Open Wait
				if (CheckTimer(m_pUps->OpenWaitTime))
					m_pUps->AutoProc = 0;						// Open Command
				else
				if (m_pUps->GetStatus() == LINE_OPEN)
				{
					m_pUps->AutoProc++;							// Next Procedure
					m_pUps->LineOpenRetryCount = 0;
				}
				break;

			case 2:												// Send Command
				m_pUps->UpsRecvBufferClear();
				if (m_pUps->UpsSendCmd())
				{
					m_pUps->RecvWaitTime = TimerSet(10);
					m_pUps->AutoProc++;							// Next Procedure
				}
				break;

			case 3:												// Recv Command
				if (CheckTimer(m_pUps->RecvWaitTime))
					m_pUps->AutoProc = 0;						// Open Command
				else
				if (m_pUps->UpsRecvCmd())
					m_pUps->AutoProc++;							// Next Procedure
				break;

			case 4:												// Send Delay
				m_pUps->SendDelayTime = TimerSet(m_pUps->UpsIdleTime);
				m_pUps->AutoProc++;								// Next Procedure
				break;
				
			case 5:												// Send Time Delay
				if (m_pUps->UpsSendCheck())
					m_pUps->AutoProc = 2;						// Send Command
				else
				if (CheckTimer(m_pUps->SendDelayTime))
					m_pUps->AutoProc = 2;						// Send Command
				break;
				
			default:
				m_pUps->AutoProc = 0;							// Open Command
				break;
		}
	}

/////////////////////////////////////////////////////////////////////////////
	m_pUps->ThreadUse = FALSE;
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "LanUps", __FILE__, __LINE__, "UpsMainThread():return");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CLanUps::Open()
{
	if ((strlen(m_IpAddress) == 0)				||				// 2005.12.28
		(memcmp(m_IpAddress, "0.", 2) == 0)		||
		(memcmp(m_IpAddress, "00.", 3) == 0)	||
		(memcmp(m_IpAddress, "000.", 4) == 0)	||
		(m_PortNumber == 0))
		Doing = FALSE;
	else
		Doing = TRUE;

	return Doing;
}

int CLanUps::Close()
{
	Doing = FALSE;
	while (ThreadUse)											// End Check : 2005.12.23
		Delay_Msg(1000);

	return Doing;
}

int CLanUps::GetPowerStatus()
{
	int	UpsStatus = POWER_NORMAL;

	if (LineOpenRetryCount >= 5)
		UpsStatus |= POWER_ABNORMAL;
	if (UpsFailCount >= 5)
		UpsStatus |= POWER_ABNORMAL;
	if (UpsBatLowCount >= 5)
		UpsStatus |= INT_BATTERY_LOW;
	if (UpsAcStopCount >= 5)
		UpsStatus |= EXT_AC_OFF;
	
	return UpsStatus;
}

int CLanUps::PowerOff(int nWaitSec)
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

int CLanUps::UpsSendCmd()
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
HexDump(TRACE_CODE_MODE, "LanUps", __FILE__, __LINE__, "[TCPIP SENDDATA]", (char*)DATASendBuffer, DATASendLength);
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
	
	SendData(DATASendBuffer, DATASendLength, 10);
	return TRUE;
}

int CLanUps::UpsSendCheck()
{
	if (DATASendInTbl != DATASendOutTbl)						// Data Ok
		return TRUE;
	else
		return FALSE;
}

int CLanUps::UpsRecvCmd()
{
	int				nRecvResult = FALSE;
	CString			strTemp;
	CStringArray	strTempArray;

	nRecvResult = RecvData(UPSRecvBuffer, &UPSRecvLength, 0);
	if (nRecvResult == FALSE)
		return FALSE;

	if (UPSRecvLength <= 3)
		return FALSE;
	else
	if (UPSRecvLength == 4)										// 2006.02.09
	{
HexDump(TRACE_CODE_MODE, "LanUps", __FILE__, __LINE__, "[TCPIP RECVDATA]", (char*)UPSRecvBuffer, UPSRecvLength);
	}
	else
	{
		Strcpy(strTemp, UPSRecvBuffer, UPSRecvLength);
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
HexDump(TRACE_CODE_MODE, "LanUps", __FILE__, __LINE__, "[TCPIP RECVDATA]", (char*)UPSRecvBuffer, UPSRecvLength);
		}
	}

	LineOpenRetryCount = 0;
	return TRUE;
}

int CLanUps::UpsRecvBufferClear()
{
	while (RecvData(UPSRecvBuffer, &UPSRecvLength, 0));

	UPSRecvLength = 0;
	memset(UPSRecvBuffer, 0, sizeof(UPSRecvBuffer));
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CLanUps::LineOpen(int OpenSec, int nAsyncFlag)
{
	DWORD	WaitTime;

	LineClose(OpenSec, nAsyncFlag);								// 2004.07.12
	m_InitOpen = TRUE;	

/////////////////////////////////////////////////////////////////////////////
	if (nAsyncFlag)												// 2004.07.12
	{
		return m_pSocketUps->OpenSocket();
	}
/////////////////////////////////////////////////////////////////////////////
																
	if (m_pSocketUps->OpenSocket())								// LINE OPEN
	{
		WaitTime = GetTickCount() + 1000 * OpenSec;
		do
		{
			Delay_Msg();
			GetStatus();										// LINE STATUS CONTROL
			if (m_pSocketUps->TCPConnected)
				break;
		} while (GetTickCount() < WaitTime);
	}

	if (m_pSocketUps->TCPConnected)								// OPEN OK
		return TRUE;
	else 
	{															// OPEN ERROR
MsgDump(TRACE_CODE_MODE, "LanUps", __FILE__, __LINE__, "[TCP/IP] Open Fail");
		m_pSocketUps->TCPFirstConnected = FALSE;
		m_pSocketUps->TCPConnected = FALSE;
		return FALSE;
	}
}

int CLanUps::LineClose(int CloseSec, int nAsyncFlag)
{
/////////////////////////////////////////////////////////////////////////////
	if (!m_pSocketUps->TCPFirstConnected)						// 2007.06.06
	{
		if (m_InitOpen)
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Close Connect Fail");
		}
		m_pSocketUps->TCPFirstConnected = FALSE;
		m_pSocketUps->TCPConnected = FALSE;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	if (m_InitOpen)												// LINE CLOSE 
	{
		m_InitOpen = FALSE;
		return (m_pSocketUps->CloseSocket());
	}
	return TRUE;
}

int CLanUps::SendData(BYTE *SendBuff, int Length, int SendSec)
{
	int		Ret;
	DWORD	WaitTime;

/////////////////////////////////////////////////////////////////////////////
	Delay_Msg(500);												// Performance Adjust 2004.07.26
/////////////////////////////////////////////////////////////////////////////
	m_pSocketUps->DATARecvLength = 0;							// RECV SIZE CLEAR

/////////////////////////////////////////////////////////////////////////////
	if (!m_pSocketUps->TCPFirstConnected)						// 2004.07.12
	{
MsgDump(TRACE_CODE_MODE, "LanUps", __FILE__, __LINE__, "[TCP/IP] Send Connect Fail");
		m_pSocketUps->TCPFirstConnected = FALSE;
		m_pSocketUps->TCPConnected = FALSE;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	if (m_pSocketUps->SendSocket(SendBuff, Length))				// SEND DATA
	{
		WaitTime = GetTickCount() + 1000 * SendSec;
		do
		{
			Delay_Msg();
			GetStatus();										// LINE STATUS CONTROL
			if (!m_pSocketUps->DATASendLength)
				break;
			if (!m_pSocketUps->TCPConnected)					// 2004.06.22
				break;
		} while (GetTickCount() < WaitTime);
	}

	Ret = m_pSocketUps->DATASendLength;
	m_pSocketUps->DATASendLength = 0;							// SEND LENGTH CLEAR
	if (!Ret)													// SEND OK
		return TRUE;
	else 
	{															// SEND ERROR
MsgDump(TRACE_CODE_MODE, "LanUps", __FILE__, __LINE__, "[TCP/IP] Send Fail");
		m_pSocketUps->TCPFirstConnected = FALSE;
		m_pSocketUps->TCPConnected = FALSE;
		return FALSE;
	}
}

int CLanUps::RecvData(BYTE *RecvBuff, int *Length, int RecvSec)
{
	int		Ret;
	DWORD	WaitTime;

/////////////////////////////////////////////////////////////////////////////
	Delay_Msg(500);												// Performance Adjust 2004.07.26
/////////////////////////////////////////////////////////////////////////////
	if (!m_pSocketUps->TCPFirstConnected)						// 2004.07.12
	{
		if (RecvSec)											// RECV WAIT TIME EXIST
		{
MsgDump(TRACE_CODE_MODE, "LanUps", __FILE__, __LINE__, "[TCP/IP] Recv Connect Fail");
			m_pSocketUps->TCPFirstConnected = FALSE;
			m_pSocketUps->TCPConnected = FALSE;
		}
		*Length = 0;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	if (RecvSec)
	{
		WaitTime = GetTickCount() + 1000 * RecvSec;
		do
		{
			Delay_Msg();
			GetStatus();										// LINE STATUS CONTROL
			if (m_pSocketUps->DATARecvInTbl != m_pSocketUps->DATARecvOutTbl)
				break;
			if (!m_pSocketUps->TCPConnected)					// 2004.06.22
				break;
		} while (GetTickCount() < WaitTime);
	}
	else
	{
		GetStatus();											// LINE STATUS CONTROL
	}

	Ret = m_pSocketUps->DATARecvSocket(RecvBuff, Length);		// RECV DATA
	if (Ret)													// RECV OK
	{
		m_pSocketUps->DATARecvLength = 0;						// RECV LENGTH CLEAR
		return TRUE;
	}
	else 
	{															// RECV ERROR
		if (RecvSec)											// RECV WAIT TIME EXIST
		{
MsgDump(TRACE_CODE_MODE, "LanUps", __FILE__, __LINE__, "[TCP/IP] Recv Fail");
			m_pSocketUps->TCPFirstConnected = FALSE;
			m_pSocketUps->TCPConnected = FALSE;
		}
		*Length = 0;
		return FALSE;
	}
}

int CLanUps::GetStatus()
{
	if (m_pSocketUps->TCPConnected)								// Connected Type
		m_LineStatus = LINE_OPEN;
	else m_LineStatus = LINE_CLOSE;

	return m_LineStatus;
}
