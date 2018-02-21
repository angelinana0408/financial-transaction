/////////////////////////////////////////////////////////////////////////////
// ComDvr.h: interface for the CComDvr class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_COMDVR_H__77667EC1_39B4_11D5_A80C_00E09880ACB0__INCLUDED_)
#define AFX_COMDVR_H__77667EC1_39B4_11D5_A80C_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Serial.h"
#include "Profiledef.h"

#define DVRSENDTBL				8
#define DVRBUFFSIZE				8192

typedef struct tagDvrHeader
{
	BYTE	Data[1];
} DvrHeader;

/////////////////////////////////////////////////////////////////////////////
// CComDvr thread

class AFX_EXT_CLASS CComDvr : public CSerial
{
	DECLARE_DYNCREATE(CComDvr)
public:
	CComDvr();													// protected constructor used by dynamic creation
	CComDvr(DEVICEINFO	DEVICE, NETWORKINFO NETWORK);
	~CComDvr();

// Attributes
public:
	int		AutoProc;

	ULONG	SendDelayTime;
	ULONG	RecvWaitTime;
	int		DvrIdleTime;

	int		DvrRecvLength;										// Dvr Recv Buffer
	BYTE	DvrRecvBuffer[DVRBUFFSIZE];			
	
	int		DATASendLength;										// Dvr Send Buffer
	int		DATASendLengthTbl[DVRSENDTBL];
	int		DATASendInTbl;
	int		DATASendInTblTemp;
	int		DATASendOutTbl;
	int		DATASendOutTblTemp;
	BYTE	DATASendBuffer[DVRBUFFSIZE];
	BYTE	DATASendBufferTbl[DVRSENDTBL][DVRBUFFSIZE];
	int		DATASendDelayTimeTbl[DVRSENDTBL];
	ULONG	DATAGetTickCountTbl[DVRSENDTBL];
	int		DATASendDelayTime;
	ULONG	DATAGetTickCount;

private:
	DvrHeader *pDvrRecvBuffer;

	DvrHeader *pDATASendBuffer;

	int		CheckRecvLength;									// Dvr Recv Length
	int		CheckDataLength;									// Dvr Data Length
	CString	strDvrStatus;

// Operations
public:
	int		Initialize();
	int		Open();
	int		Close();
	int		Write(BYTE* SendBuff, int Length, int nSendDelayTime);
	CString	GetStatus();

protected:
	int		DvrMain();
	int		DvrSendCmd();
	int		DvrSendData(BYTE* SendBuff, int Length);
	int		DvrSendCheck();
	int		DvrRecvCmd();
	int		DvrCheckData();
	int		DvrRecvBufferClear();
	int		DvrSetStatus();

	DEVICEINFO	m_DeviceInfo;
	NETWORKINFO	m_NetworkInfo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComDvr)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CComDvr)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMDVR_H__77667EC1_39B4_11D5_A80C_00E09880ACB0__INCLUDED_)
