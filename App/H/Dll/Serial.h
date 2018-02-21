#if !defined(AFX_SERIAL_H__0F46176B_39A0_11D5_A80C_00E09880ACB0__INCLUDED_)
#define AFX_SERIAL_H__0F46176B_39A0_11D5_A80C_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Serial.h : header file
//

typedef struct tagSERIALDATA
{
	int		port;
	int		buadrate;
	int		parity;
	int		databit;
	int		stopbit;
} SERIALDATA,*LPSERIALDATA;

/////////////////////////////////////////////////////////////////////////////
// CSerial thread

class AFX_EXT_CLASS CSerial : public CWinThread
{
	DECLARE_DYNCREATE(CSerial)
protected:
	CSerial();           // protected constructor used by dynamic creation

// Attributes
public:
    HANDLE      m_hComm;
    DWORD       m_dwLastError;
    DWORD       m_dwCommError;
    DWORD       m_dwEventMask;

    OVERLAPPED  m_osWrite;
    OVERLAPPED  m_osRead;
    OVERLAPPED  m_osWait;

	HANDLE		m_hEventKill;

// Operations
public:
	int			OpenPort(LPSERIALDATA lpData);

    int         OpenPort(int port);
	void		ClosePort();
	int			SetupStat(int baudrate,int byte,int parity,int stop);

	int			SetupFlow(int bDTRDSR,int bRTSCTS,int bXONXOFF);

	int			GetInqueSize();
	int			GetOutqueSize();
	int			ReadPort( LPVOID lpBuf,int nbyte);
	int			WritePort(LPVOID lpBuf,int nbyte);
	void		ClearPort();

	int			StartPort();
	int			KillPort();

	int			TerminatePort();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerial)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSerial();

	// Generated message map functions
	//{{AFX_MSG(CSerial)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIAL_H__0F46176B_39A0_11D5_A80C_00E09880ACB0__INCLUDED_)
