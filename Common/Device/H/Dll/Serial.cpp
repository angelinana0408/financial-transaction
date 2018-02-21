// Serial.cpp : implementation file
//

#include "stdafx.h"
#include "Serial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSerial

IMPLEMENT_DYNCREATE(CSerial, CWinThread)

CSerial::CSerial()
{
    m_dwLastError = 0;
    m_dwCommError = 0;
    m_dwEventMask = 0;

    memset(&m_osWrite,0,sizeof(OVERLAPPED));
    memset(&m_osRead ,0,sizeof(OVERLAPPED));
    memset(&m_osWait ,0,sizeof(OVERLAPPED));

    m_hComm = INVALID_HANDLE_VALUE;

	m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CSerial::~CSerial()
{
	CloseHandle(m_hEventKill);
}

BOOL CSerial::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	while (WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT);
	return FALSE;
}

int CSerial::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSerial, CWinThread)
	//{{AFX_MSG_MAP(CSerial)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerial message handlers
int CSerial::StartPort()
{
	return CreateThread();
}

int CSerial::KillPort()
{
    if(m_hThread)
    {
        if(SetEvent(m_hEventKill))
        {
            WaitForSingleObject(m_hThread, INFINITE);
        }
    }

	return TRUE;
}
int CSerial::TerminatePort()
{
	DWORD dwCode;

	ClosePort();

	if(m_hThread==NULL)	return FALSE;

	if(GetExitCodeThread(m_hThread,&dwCode))
	{
		TerminateThread(m_hThread,dwCode);	
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CSerial message handlers
int	CSerial::OpenPort(LPSERIALDATA lpData)
{
	if(!OpenPort(	lpData->port))	return FALSE;

	if(!SetupStat(	lpData->buadrate,lpData->databit,
					lpData->parity,lpData->stopbit))	return FALSE;


	return TRUE;
}

int CSerial::OpenPort(int port)
{
	COMMTIMEOUTS	CommTimeOuts ;
	char			szPort[32];

	ClosePort();

	wsprintf(szPort,"COM%d",port);
	m_hComm = ::CreateFile(szPort,	GENERIC_READ|GENERIC_WRITE,
									0,NULL,OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL | 
									FILE_FLAG_WRITE_THROUGH |
									FILE_FLAG_OVERLAPPED,NULL);
	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		m_dwLastError = ::GetLastError();
		return FALSE;
	}
	
	m_osRead.hEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_osRead.hEvent==NULL)
	{
		m_dwLastError = ::GetLastError();
		return FALSE;
	}
	m_osWrite.hEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);

	if(m_osWrite.hEvent==NULL)
	{
		m_dwLastError = ::GetLastError();
		return FALSE;
	}

	::SetCommMask(	m_hComm,EV_RXCHAR); 		
	::SetupComm(	m_hComm,1024,1024);	
	::PurgeComm(	m_hComm,PURGE_TXABORT | PURGE_RXABORT |
                            PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

	CommTimeOuts.ReadIntervalTimeout		= 0xFFFFFFFF;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant	= 1000;
    CommTimeOuts.WriteTotalTimeoutMultiplier= 0;
    CommTimeOuts.WriteTotalTimeoutConstant	= 1000;

    ::SetCommTimeouts( m_hComm,&CommTimeOuts ) ;

	return TRUE;
}

void CSerial::ClosePort()
{
	if(m_hComm!=INVALID_HANDLE_VALUE)
	{
		::PurgeComm(	m_hComm,PURGE_TXABORT | PURGE_RXABORT |
								PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

		if(m_osRead.hEvent)		CloseHandle(m_osRead.hEvent);
		if(m_osWrite.hEvent)	CloseHandle(m_osWrite.hEvent);
		if(m_osWait.hEvent)		CloseHandle(m_osWait.hEvent);

		CloseHandle(m_hComm);
	}

    m_dwLastError = 0;
    m_dwCommError = 0;
    m_dwEventMask = 0;

    memset(&m_osWrite,0,sizeof(OVERLAPPED));
    memset(&m_osRead ,0,sizeof(OVERLAPPED));
    memset(&m_osWait ,0,sizeof(OVERLAPPED));

    m_hComm = INVALID_HANDLE_VALUE;
}

int CSerial::GetInqueSize()
{
	if(m_hComm==INVALID_HANDLE_VALUE)	return 0;

	COMSTAT    ComStat ;
	::ClearCommError(m_hComm, &m_dwCommError,&ComStat ) ;
	return ComStat.cbInQue;
}

int CSerial::GetOutqueSize()
{
	if(m_hComm==INVALID_HANDLE_VALUE)	return 0;

	COMSTAT    ComStat ;
	::ClearCommError(m_hComm, &m_dwCommError,&ComStat ) ;
	return ComStat.cbOutQue;
}

int CSerial::ReadPort(LPVOID lpBuf,int nbyte)
{
	COMSTAT    ComStat ;
	DWORD      dwLength;
	
	if(m_hComm==INVALID_HANDLE_VALUE)	return 0;

	::ClearCommError(m_hComm, &m_dwCommError,&ComStat ) ;
	dwLength = min((DWORD) nbyte, ComStat.cbInQue ) ;

	if(dwLength==0)	return 0;

	if(!ReadFile(m_hComm,lpBuf,dwLength,&dwLength,&m_osRead))
	{
		m_dwCommError = ::GetLastError();

		if(m_dwCommError == ERROR_IO_PENDING)
		{
			while(!GetOverlappedResult(m_hComm,&m_osRead,&dwLength,TRUE))
			{
				m_dwCommError = ::GetLastError();
				if(m_dwCommError != ERROR_IO_INCOMPLETE)
				{
					::ClearCommError(m_hComm, &m_dwCommError,&ComStat ) ;
					break;
				}
			}
		}
		else
		{
			dwLength=0;
			::ClearCommError(m_hComm, &m_dwCommError,&ComStat) ;
		}
	}

	return(dwLength) ;
}

int CSerial::WritePort(LPVOID lpBuf,int nbyte)
{
	DWORD		dwLength;
	COMSTAT     ComStat;

	if(m_hComm==INVALID_HANDLE_VALUE)	return 0;

	if(!WriteFile(m_hComm,lpBuf,nbyte,&dwLength,&m_osWrite))
	{
		m_dwCommError = ::GetLastError();

		if(m_dwCommError == ERROR_IO_PENDING)
		{
			while(!GetOverlappedResult(m_hComm,&m_osWrite,&dwLength,TRUE))
			{
				m_dwCommError = ::GetLastError();
				if(m_dwCommError != ERROR_IO_INCOMPLETE)
				{
					::ClearCommError(m_hComm, &m_dwCommError,&ComStat ) ;
					break;
				}
			}
		}
		else
		{
			dwLength = 0;
			::ClearCommError(m_hComm,&m_dwCommError,&ComStat ) ;

			return(dwLength);
		}
	}
	return(dwLength);
}

void CSerial::ClearPort()
{
	COMSTAT     ComStat;
	char		buf[1024];

	if(m_hComm==INVALID_HANDLE_VALUE)	return;

	::ClearCommError(m_hComm, &m_dwCommError,&ComStat ) ;
	FlushFileBuffers(m_hComm);

	while(ReadPort(buf,1024));
}

int CSerial::SetupStat(int baudrate,int byte,int parity,int stop)
{
	DCB		dcb ;

	dcb.DCBlength = sizeof( DCB ) ;

	GetCommState( m_hComm, &dcb ) ;

	dcb.BaudRate	= baudrate;
	dcb.ByteSize	= byte;
	dcb.Parity		= parity;
	dcb.StopBits	= stop;

	return ::SetCommState(m_hComm, &dcb);
}

int CSerial::SetupFlow(int bDTRDSR,int bRTSCTS,int bXONXOFF)
{
	DCB		dcb ;

	dcb.DCBlength = sizeof( DCB ) ;

	GetCommState( m_hComm, &dcb ) ;

	dcb.fOutxDsrFlow = bDTRDSR;
	if (bDTRDSR)	dcb.fDtrControl = DTR_CONTROL_HANDSHAKE ;
	else			dcb.fDtrControl = DTR_CONTROL_ENABLE ;

	dcb.fOutxCtsFlow = bRTSCTS;
	if (bRTSCTS)	dcb.fRtsControl = RTS_CONTROL_HANDSHAKE ;
	else			dcb.fRtsControl = RTS_CONTROL_ENABLE ;

	// setup software flow control
	dcb.fInX		= bXONXOFF;
	dcb.fOutX		= bXONXOFF;
	dcb.XonChar		= 0x11;
	dcb.XoffChar	= 0x13;
	dcb.XonLim		= 100 ;
	dcb.XoffLim		= 100 ;

	// other various settings

	dcb.fBinary = TRUE ;
	dcb.fParity = TRUE ;

	return ::SetCommState(m_hComm, &dcb);
}
