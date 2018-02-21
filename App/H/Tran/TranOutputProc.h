/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranOutputProc_H_INCLUDED_)
#define _TranOutputProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Output Variable
/////////////////////////////////////////////////////////////////////////////
	int		SumClearFlag;				// Sum Clear Flag
	int		AddCashFlag;				// Add Cash Flag
    int     iSusTotalNum;               // 可疑交易分批打印批次号
/////////////////////////////////////////////////////////////////////////////
// Operations
public:
////////////////////////////////////////////////////////////////////////////
//	After Recv Host Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_PSPProc(int PrintDevice, int WaitFlag = FALSE);
																// Parameter Slip Print Procedure
	int		fnAPP_PPPProc(int WaitFlag = FALSE);				// Parameter Passbook Print Procedure
	int		fnAPP_PPWProc(int WaitFlag = FALSE);				// Parameter Passbook Write Procedure
	int		fnAPP_PCWProc(int WaitFlag = FALSE);				// Parameter Card Write Procedure
	int		fnAPP_PMDProc();									// Parameter Message Display Procedure

	int		fnAPP_PMEAcceptNgGuideConfirm();					// PME Accept Ng Guide Confirm
	int		fnAPP_PMEAcceptSlipSelect();						// PME Accept Slip Select
	int		fnAPP_PMEDisplayTransferProc();						// PME Display Transfer Procedure
	int		fnAPP_PMEAcceptTranContinue();						// PME Accept Transaction Continue
	int		fnAPP_PMEAcceptInquiryConfirm();					// PME Accept Inquiry Confirm
	int		fnAPP_PMEAcceptDetailConfirm();						// PME Accept Detail Confirm
	int		fnAPP_PMEAcceptIcInquiryConfirm();					// PME Accept Ic Inquiry Confirm
	int		fnAPP_PMEAcceptIcDetailConfirm();					// PME Accept Ic Detail Confirm
	int		fnAPP_PMEAcceptIcLimitConfirm();					// PME Accept Ic Limit Confirm
	int		fnAPP_PMEAcceptChangePassWord();					// PME Accept Change PassWord Confirm
	int		fnAPP_PMEAcceptPassWord();							// PME Accept PassWord
	int     fnAPP_PMEAcceptACMangerContinue();                   // PME Accept A/C Manager Remit Confirm
//	int     fnAPP_PMEAcceptACPayContinue();                     // PME Accept A/C Manager Pay Confirm
	int		fnAPP_PMEAcceptRemit();								// 2008.10.05
	int		fnApp_PMEDisplayBalance();							// added by yusy 2010.05.15
	int		fnAPP_AcceptIcApp();
	int		fnAPP_LotteryWin();									// added by liuxl 20120306
	int		fnAPP_PMEAcceptChargeConfirm();						// PME Accept Charge Confirm

	// 20080905 by yaokq
	int		fnAPP_PMEAcceptOtherConfirm();						//PME Accept Other Inquiry
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_SetHostTimeProc();							// Set Host Time Procedure

	int		fnAPP_ReadyProc();									// Ready Procedure
	int		fnAPP_ReadyPrintResult(int PrintDevice, int WaitFlag = FALSE);
																// Ready Print Result
	int		fnAPP_OpenProc();									// Open Procedure
	int		fnAPP_OpenPrintResult(int PrintDevice, int WaitFlag = FALSE);
																// Open Print Result
	int		fnAPP_SumProc(int InputID);							// Sum Procedure
	int		fnAPP_SumClear();									// Sum Clear
	int		fnAPP_SumPrint(int PrintDevice, int WaitFlag = FALSE);	
																// Sum Print
	int		fnAPP_SumPrintResult(int PrintDevice, int WaitFlag = FALSE);
																// Sum Print Result
	int		fnAPP_CashProc();									// Cash Procedure
	int		fnAPP_CashPrint(int PrintDevice, int WaitFlag = FALSE);	
																// Cash Print
	int		fnAPP_CashPrintResult(int PrintDevice, int WaitFlag = FALSE);
																// Cash Print Result

/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_ErrorTranProc();								// Error Transaction Procedure
	int		fnAPP_ErrorCancelProc();							// Error Cancel Procedure
	int		fnAPP_ErrorHostProc();								// Error Host Procedure

//////////////////////////////////////////////////////////////////////
	int		fnAPP_PrintError(int PrintDevice, int WaitFlag = FALSE);			
																// Print Error
	int		fnAPP_PrintCancel(int PrintDevice, int WaitFlag = FALSE);
																// Print Cancel

	int		fnAPP_SuspiciProc();
/////////////////////////////////////////////////////////////////////////////
//	Edit Function
/////////////////////////////////////////////////////////////////////////////
	CString	fstrAPP_EditAcnt(int HiddenFlag);					// Edit Account
	int		fnAPP_EditPrintLine(int PrintDevice, void* cData, int LineNum, int HiddenFlag = FALSE);
																// Edit Print Line

/////////////////////////////////////////////////////////////////////////////
#endif
