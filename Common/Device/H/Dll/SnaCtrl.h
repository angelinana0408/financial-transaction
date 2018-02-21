#if !defined(AFX_SNACTRL_H__6E66A161_3EFF_11D5_A80C_00E09880ACB0__INCLUDED_)
#define AFX_SNACTRL_H__6E66A161_3EFF_11D5_A80C_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SnaCtrl.h : header file
//

#include "Profiledef.h"
#include "W3270api.h"

#define SNABUFFSIZE				16384

typedef struct tagSNAHeader
{
	BYTE	Data[1];
} SNAHeader;

/////////////////////////////////////////////////////////////////////////////
// CSnaCtrl command target

class AFX_EXT_CLASS CSnaCtrl
{
// Attributes
public:
	int		SNAConnected;
	int		SNAEnabled;

	SENDDATA	SNASendBuffer;					// Sna Buffer
	RCVDATA		SNARecvBuffer;
	
	int		DATASendLength;						// Tran Buffer
	int		DATARecvLength;
	BYTE	DATASendBuffer[SNABUFFSIZE];
	BYTE	DATARecvBuffer[SNABUFFSIZE];

private:
	SNAHeader *pDATASendBuffer;
	SNAHeader *pDATARecvBuffer;

// Operations
public:
	CSnaCtrl(NETWORKINFO NETWORK);
	virtual ~CSnaCtrl();

	int InitSna();
	int OpenSna();
	int CloseSna();
	int SendSna(BYTE *SendBuff, int Length, int SendSec);
	int RecvSna(BYTE *RecvBuff, int *Length, int RecvSec);
	int GetStatusSna();

private:
	NETWORKINFO	m_ConfBuff;

	int			m_FirstFlag;
	HINSTANCE	m_Instance;
	int			m_nLuNo;
	OIAINFO		m_OIAInfo;
	
	CString		GetSnaReturnMsg(int nReturn);
};

#endif // !defined(AFX_SNACTRL_H__6E66A161_3EFF_11D5_A80C_00E09880ACB0__INCLUDED_)
