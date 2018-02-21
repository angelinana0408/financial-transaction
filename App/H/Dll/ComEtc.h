/////////////////////////////////////////////////////////////////////////////
// ComEtc.h: interface for the CComEtc class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_COMETC_H__BB3E00D5_BC1A_4A1C_9A3B_5C1E847BCC3B__INCLUDED_)
#define AFX_COMETC_H__BB3E00D5_BC1A_4A1C_9A3B_5C1E847BCC3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Serial.h"
#include "Profiledef.h"

#define ETCSENDTBL				8
#define ETCBUFFSIZE				8192

typedef struct tagEtcHeader
{
	BYTE	Data[1];
} EtcHeader;

/////////////////////////////////////////////////////////////////////////////
// CComEtc thread

class AFX_EXT_CLASS CComEtc : public CSerial  
{
	DECLARE_DYNCREATE(CComEtc)
public:
	CComEtc();													// protected constructor used by dynamic creation
	CComEtc(DEVICEINFO	DEVICE, NETWORKINFO NETWORK);
	virtual ~CComEtc();

// Attributes
public:
	int		CheckTime;
	int		AlarmAllOnFlag;
	int		Alarm1OffFlag;
	int		Alarm2OffFlag;
	int		Alarm3OffFlag;
	int		Alarm4OffFlag;
	int		AlarmOffDelayTimeTbl[ETCSENDTBL];

	int		AutoProc;

	ULONG	SendDelayTime;
	ULONG	RecvWaitTime;
	int		EtcIdleTime;
	char	EtcHostAddress[3];

	int		EtcRecvLength;										// Etc Recv Buffer
	BYTE	EtcRecvBuffer[ETCBUFFSIZE];			
	
	int		DATASendLength;										// Etc Send Buffer
	int		DATASendLengthTbl[ETCSENDTBL];
	int		DATASendInTbl;
	int		DATASendInTblTemp;
	int		DATASendOutTbl;
	int		DATASendOutTblTemp;
	BYTE	DATASendBuffer[ETCBUFFSIZE];
	BYTE	DATASendBufferTbl[ETCSENDTBL][ETCBUFFSIZE];
	int		DATASendDelayTimeTbl[ETCSENDTBL];
	ULONG	DATAGetTickCountTbl[ETCSENDTBL];
	int		DATASendDelayTime;
	ULONG	DATAGetTickCount;

private:
	EtcHeader *pEtcRecvBuffer;

	EtcHeader *pDATASendBuffer;

	int		CheckRecvLength;									// Etc Recv Length
	int		CheckDataLength;									// Etc Data Length
	char	szEtcStatus[8];

// Operations
public:
	int		Initialize();
	int		Open();
	int		Close();
	int		Write(BYTE* SendBuff, int Length, int nSendDelayTime);
	CString	GetStatus();

protected:
	int		EtcMain();
	int		EtcSendCmd();
	int		EtcSendData(BYTE* SendBuff, int Length);
	int		EtcSendCheck();
	int		EtcRecvCmd();
	int		EtcCheckData();
	int		EtcRecvBufferClear();
	int		EtcSetStatus();

	DEVICEINFO	m_DeviceInfo;
	NETWORKINFO	m_NetworkInfo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ComEtc)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(ComEtc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_COMETC_H__BB3E00D5_BC1A_4A1C_9A3B_5C1E847BCC3B__INCLUDED_)
