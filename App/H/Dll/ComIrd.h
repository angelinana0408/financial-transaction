/////////////////////////////////////////////////////////////////////////////
// ComIrd.h: interface for the CComIrd class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_COMIRD_H__8395DEAA_525A_4D7C_AF5D_5AFF9C09E491__INCLUDED_)
#define AFX_COMIRD_H__8395DEAA_525A_4D7C_AF5D_5AFF9C09E491__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Serial.h"
#include "Profiledef.h"

#define IRDBUFFSIZE				8192

/////////////////////////////////////////////////////////////////////////////
// CCOMIRD thread

class AFX_EXT_CLASS CComIrd : public CSerial
{
	DECLARE_DYNCREATE(CComIrd)
public:
	CComIrd();													// protected constructor used by dynamic creation
	CComIrd(DEVICEINFO	DEVICE, NETWORKINFO NETWORK);
	~CComIrd();

// Attributes
public:
	HANDLE	hComIrd;

	int		IrdDeviceStatus;									// Ird Device Status
	int		IrdDataLength;										// Ird Data Length
	BYTE	IrdData[IRDBUFFSIZE];								// Ird Data

private:

// Operations
public:
	int		Initialize();
	int		Open();
	int		Close();
	int		Read();
	
	CString CheckDeviceAction(int CheckTime);
	int		ClearErrorCode();
	CString	GetErrorCode();
	int		GetDeviceStatus();
	int		GetDataStatus();
	CString GetData();

	int		EntryEnable();
	int		EntryDisable();

public:

	DEVICEINFO	m_DeviceInfo;
	NETWORKINFO	m_NetworkInfo;

	CRITICAL_SECTION m_csLock;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComIrd)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CComIrd)
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

#endif // !defined(AFX_COMIRD_H__8395DEAA_525A_4D7C_AF5D_5AFF9C09E491__INCLUDED_)
