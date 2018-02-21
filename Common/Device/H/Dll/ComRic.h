/////////////////////////////////////////////////////////////////////////////
// ComRic.h: interface for the CComRic class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_COMRIC_H__8395DEAA_525A_4D7C_AF5D_5AFF9C09E491__INCLUDED_)
#define AFX_COMRIC_H__8395DEAA_525A_4D7C_AF5D_5AFF9C09E491__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Serial.h"
#include "Profiledef.h"

#define RICBUFFSIZE				8192

/////////////////////////////////////////////////////////////////////////////
// CCOMRIC thread

class AFX_EXT_CLASS CComRic : public CSerial
{
	DECLARE_DYNCREATE(CComRic)
public:
	CComRic();													// protected constructor used by dynamic creation
	CComRic(DEVICEINFO	DEVICE, NETWORKINFO NETWORK);
	~CComRic();

// Attributes
public:
	HANDLE	hComRic;

	int		RicDeviceStatus;									// Ric Device Status
	BYTE	RejectCode[4];
	ULONG	IcCardSerialNo;										// Ic Card Serial No

private:

// Operations
public:
	int		Initialize();
	int		Open();
	int		Close();
	int		RfReset();
	int		DetectCard();
	int		CpuReset();
	CString	SelectFile();
	int		VerifyPIN(LPCTSTR szPinData);
	CString	ReadBalance();
	CString	InitializeForLoad(LPCTSTR szAmount, LPCTSTR szTerminalID, int nKeyIndex);
	CString	LoadEP(LPCTSTR szDate, LPCTSTR szTime, LPCTSTR szMac2);
	CString	GetRejectCode();
	CString CheckDeviceAction(int CheckTime);
	
	int		ClearErrorCode();
	CString	GetErrorCode();
	int		GetDeviceStatus();
	int		GetDataStatus();
	CString	GetIcCardSerialNo();

	int		EntryEnable();
	int		EntryDisable();

public:

	DEVICEINFO	m_DeviceInfo;
	NETWORKINFO	m_NetworkInfo;

	CRITICAL_SECTION m_csLock;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComRic)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CComRic)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//	THREAD
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMRIC_H__8395DEAA_525A_4D7C_AF5D_5AFF9C09E491__INCLUDED_)
