/////////////////////////////////////////////////////////////////////////////
// Profile.h: interface for the CProfile class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_PROFILE_H__2203AD10_FC4B_11D4_A80E_00E09880ACB0__INCLUDED_)
#define AFX_PROFILE_H__2203AD10_FC4B_11D4_A80E_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProfileDef.h"

class AFX_EXT_CLASS CProfile  
{
public:
	CProfile();
	virtual ~CProfile();

private:
	static	CRITICAL_SECTION m_csLock;

public:
	DEVICEINFO			DEVICE;
	NETWORKINFO			NETWORK;
	TRANSINFO			TRANS;
	WITHTOTALINFO		WITHTOTAL;
	INQTOTALINFO		INQTOTAL;
	TRANSTOTALINFO		TRANSTOTAL;
	DEPTOTALINFO		DEPTOTAL;

public:
/////////////////////////////////////////////////////////////////////////////
// Internal Function
/////////////////////////////////////////////////////////////////////////////
	int		GetPInt(char* SectName, char* KeyName, int nDef, char* FileName);
	void	GetPStr(char* SectName, char* KeyName, char sDef, void *cDest, int nSize, char* FileName);
	void	GetPByten(char* SectName, char* KeyName, BYTE *Dest, int nSize, char* FileName);
	void	PutPInt(char* SectName, char* KeyName, int Src, char* FileName, int nFlushFlag = TRUE);
	void	PutPStr(char* SectName, char* KeyName, void *cSrc, int nSize, char* FileName, int nFlushFlag = TRUE);
	void	PutPByten(char* SectName, char* KeyName, BYTE *Src, int nSize, char* FileName, int nFlushFlag = TRUE);

/////////////////////////////////////////////////////////////////////////////
// Device/Network/Trans
/////////////////////////////////////////////////////////////////////////////
	void	InitProfileCheck();
	void	InitProfile(int type);

	void	InitDeviceProfile();
	void	InitNetworkProfile();
	void	InitTransProfile();

	void	GetDeviceProfile();
	void	GetNetworkProfile();
	void	GetTransProfile();

	void	PutDeviceProfile();
	void	PutNetworkProfile();
	void	PutTransProfile();

/////////////////////////////////////////////////////////////////////////////
// ErrStack
/////////////////////////////////////////////////////////////////////////////
public :
	void	ErrStack(char pProcCounter, char* pErrorCode, char* pErrorString);	
																// Error Stack
	void	ClearAllErrStack();									// Clear All Error Stack
	int		GetCrtErrStack(ERRSTACK* pEditBuff, bool pCommand);	// Current Error Stack
	int		GetFirstErrStack(ERRSTACK* pEditBuff);				// First Error Stack
	int		GetLastErrStack(ERRSTACK* pEditBuff);				// Last Error Stack
	int		GetAllErrStack(ERRSTACK* pEditBuff);				// All Error Stack

/////////////////////////////////////////////////////////////////////////////
// ErrHost
/////////////////////////////////////////////////////////////////////////////
public:
	void	ErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec);			// Set Error Info Sub
	void	ClearAllErrSub(char* MainSec, char* SubSec);						// Clear Error Sub
	int		GetCrtErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec);		// Current Error Info Sub
	int		GetAllErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec);		// All Error Info Sub

	void	ErrTran(DERRINFO* DErrInfo);										// Set Error Info Tran
	void	ClearAllErrTran();													// Clear Error Tran
	int		GetCrtErrTran(DERRINFO* DErrInfo);									// Current Error Info Tran
	int		GetAllErrTran(DERRINFO* DErrInfo);									// All Error Info Tran
	
	void	ErrCancel(DERRINFO* DErrInfo);										// Set Error Info Cancel
	void	ClearAllErrCancel();												// Clear Error Cancel
	int		GetCrtErrCancel(DERRINFO* DErrInfo);								// Current Error Info Cancel
	int		GetAllErrCancel(DERRINFO* DErrInfo);								// All Error Info Cancel

	void	ErrHost(DERRINFO* DErrInfo);										// Set Error Info Host
	void	ClearAllErrHost();													// Clear Error Host
	int		GetCrtErrHost(DERRINFO* DErrInfo);									// Current Error Info Host
	int		GetAllErrHost(DERRINFO* DErrInfo);									// All Error Info Host

/////////////////////////////////////////////////////////////////////////////
// Total
/////////////////////////////////////////////////////////////////////////////
public:
	void	InitWithTotal();									// Init Withdraw Total
	void	InitInqTotal();										// Init Inquiry Total
	void	InitTransTotal();									// Init Transfer Total
	void	InitDepTotal();										// Init Deposit Total
	void	GetWithTotal();										// Withdraw Info
	void	GetInqTotal();										// Inquiry Info
	void	GetTransTotal();									// Transfer Info
	void	GetDepTotal();										// Deposit Info
	void	PutWithTotal();										// Withdraw Total
	void	PutInqTotal();										// Inquiry Total
	void	PutTransTotal();									// Transfer Total
	void	PutDepTotal();										// Deposit Total
};

#endif // !defined(AFX_PROFILE_H__2203AD10_FC4B_11D4_A80E_00E09880ACB0__INCLUDED_)
