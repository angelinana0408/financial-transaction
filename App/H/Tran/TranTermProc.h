/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranTermProc_H_INCLUDED_)
#define _TranTermProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Terminate Variable
/////////////////////////////////////////////////////////////////////////////
	int		CancelProcNum;				// Cancel Procedure Number
	char	CancelProcCount;			// Cancel Procedure Count 
	int		CancelDepRejectFlag;		// Cancel Deposit Reject Flag
	int		CancelDepRejectProcFlag;	// Cancel Deposit Reject Procedure Flag
	int		CancelPrintFlag;			// Cancel Print Flag
	int		CancelDepPrintFlag;			// Cancel Deposit Print Flag

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Terminate Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_CancelDepRejectProc(int CancelCode, char *pCancelMsg = NULL, int CancelScrNum = 0);
																// Cancel Deposit Reject Procedure(Normal Process)
	int		fnAPP_CancelProc(int CancelCode, char *pCancelMsg = NULL, int CancelScrNum = 0);
																// Cancel Procedure
	int		fnAPP_CancelProc2(int CancelCode, char *pCancelMsg = NULL, int CancelScrNum = 0);
																// Cancel Procedure2
	int		fnAPP_CancelProc3(int CancelCode, char *pCancelMsg = NULL, int CancelScrNum = 0);
																// Cancel Procedure3
	int		fnAPP_CancelProc4(int CancelCode, char *pCancelMsg = NULL, int CancelScrNum = 0);
																// Cancel Procedure4
	int		fnAPP_CancelProc5(int CancelCode, char *pCancelMsg = NULL, int CancelScrNum = 0);
																// Cancel Procedure5
	int		fnAPP_TerminateProc(int TerminateCode);				// Terminate Procedure

/////////////////////////////////////////////////////////////////////////////
#endif
