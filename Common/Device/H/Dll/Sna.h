// Sna.h: interface for the CSna class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SNA_H__7A3C6C85_3D61_11D5_A80C_00E09880ACB0__INCLUDED_)
#define AFX_SNA_H__7A3C6C85_3D61_11D5_A80C_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LcpRam.h"
#include "Profiledef.h"

#include "SnaCtrl.h"

class AFX_EXT_CLASS CSna
{
public:
	CSna(NETWORKINFO NETWORK);
	virtual ~CSna();

	int 		LineOpen(int OpenSec, int nAsyncFlag = FALSE);
	int 		LineClose(int CloseSec, int nAsyncFlag = FALSE);
	int 		SendData(BYTE *SendBuff, int Length, int SendSec);
	int 		RecvData(BYTE *RecvBuff, int *Length, int RecvSec);
	int 		GetStatus();

private:
	NETWORKINFO	m_ConfBuff;
	
	CSnaCtrl*	m_pSnaCtrl;

	int			m_InitOpen;
	int			m_LineStatus;
};

#endif // !defined(AFX_SNA_H__7A3C6C85_3D61_11D5_A80C_00E09880ACB0__INCLUDED_)
