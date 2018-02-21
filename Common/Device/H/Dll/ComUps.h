/////////////////////////////////////////////////////////////////////////////
// ComUps.h: interface for the CComUps class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_COMUPS_H__8395DEAA_525A_4D7C_AF5D_5AFF9C09E491__INCLUDED_)
#define AFX_COMUPS_H__8395DEAA_525A_4D7C_AF5D_5AFF9C09E491__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Serial.h"
#include "Profiledef.h"

#define UPSSENDTBL				8
#define UPSBUFFSIZE				8192

typedef struct tagUpsHeader
{
	BYTE	Data[1];
} UpsHeader;

/////////////////////////////////////////////////////////////////////////////
// CComUps thread

class AFX_EXT_CLASS CComUps : public CSerial
{
	DECLARE_DYNCREATE(CComUps)
public:
	CComUps();													// protected constructor used by dynamic creation
	CComUps(DEVICEINFO	DEVICE, NETWORKINFO NETWORK);
	~CComUps();

// Attributes
public:
	int		AutoProc;

	ULONG	SendDelayTime;
	ULONG	RecvWaitTime;
	int		UpsIdleTime;
	int		UpsFailCount;
	int		UpsBatLowCount;
	int		UpsAcStopCount;

	int		UpsRecvLength;										// Ups Recv Buffer
	BYTE	UpsRecvBuffer[UPSBUFFSIZE];			
	
	int		DATASendLength;										// Ups Send Buffer
	int		DATASendLengthTbl[UPSSENDTBL];
	int		DATASendInTbl;
	int		DATASendInTblTemp;
	int		DATASendOutTbl;
	int		DATASendOutTblTemp;
	BYTE	DATASendBuffer[UPSBUFFSIZE];
	BYTE	DATASendBufferTbl[UPSSENDTBL][UPSBUFFSIZE];

private:
	UpsHeader *pUpsRecvBuffer;

	UpsHeader *pDATASendBuffer;

	int		CheckRecvLength;									// Ups Recv Length
	int		CheckDataLength;									// Ups Data Length

// Operations
public:
	int		Initialize();
	int		Open();
	int		Close();
	int		GetPowerStatus();
	int		PowerOff(int nWaitSec);

protected:
	int		UpsMain();
	int		UpsSendCmd();
	int		UpsSendData(BYTE* SendBuff, int Length);
	int		UpsSendCheck();
	int		UpsRecvCmd();
	int		UpsCheckData();
	int		UpsRecvBufferClear();

	DEVICEINFO	m_DeviceInfo;
	NETWORKINFO	m_NetworkInfo;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComUps)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CComUps)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMUPS_H__8395DEAA_525A_4D7C_AF5D_5AFF9C09E491__INCLUDED_)
