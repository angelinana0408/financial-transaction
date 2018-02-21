/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranMainProc_H_INCLUDED_)
#define _TranMainProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Main Variable
/////////////////////////////////////////////////////////////////////////////
	int		MainTranDoingFlag;			// Main Transaction Doing Flag
	int		MainTranDoingCount;			// Main Transaction Doing Count
	int		MainTranReturnFlag;			// Main Transaction Return Flag

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Main Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_MainOpenProc();								// Main Open Procedure
	int		fnAPP_MainTranProc(int TranValue);					// Main Transaction Procedure
	int		fnAPP_TranVirtualMainProc();						// Virtual Main Procedure
	int		fnAPP_TranMainProc();								// Main Procedure
	int		fnAPP_TranReadyProc();								// Ready Procedure
	int		fnAPP_TranOpenProc();								// Open Procedure
	int		fnAPP_TranCancelProc();								// Cancel Procedure
	int		fnAPP_TranErrorProc();								// Error Procedure
	int		fnAPP_TranAexStatusProc();							// Aex Status Procedure
	int		fnAPP_TranOexStatusProc();							// Oex Status Procedure
	int		fnAPP_TranMsrStatusProc();							// Msr Status Procedure
	int		fnAPP_TranBidResponseProc();						// Bid Response Procedure
	int		fnAPP_TranTotalProc();								// Total Procedure
	int		fnAPP_TranSubTotalProc();							// Sub Total Procedure
	int		fnAPP_TranAddCashProc();							// Add Cash Procedure
	int		fnAPP_TranSubCashProc();							// Sub Cash Procedure
	int		fnAPP_TranClearCashProc();							// Clear Cash Procedure
	int		fnAPP_TranPrintCashProc();							// Print Cash Procedure
	int		fnAPP_TranDownFitProc();							// Down Fit Procedure
	int		fnAPP_TranDownICFitProc();							// Down IC Fit Procedure added by yusy ic
	int		fnAPP_TranDownAIDListProc();						// Down AID List
	int		fnAPP_TranDownPBOCParaProc();						// Down PBOC Para
	int		fnAPP_TranDownEchannelParaProc();					// Down ECHANNEL Parameter
	int		fnAPP_TranDownEchannelProtProc();					// Down ECHANNEL Protocol
	int		fnAPP_EditECProtDataProc(int ProtOrder);
	int		fnAPP_TranScriptProc();
	int		fnAPP_TranDownParmProc();							// Down Parm Procedure
	int		fnAPP_TranDownPayProc();							// Down Pay Procedure
	int		fnAPP_TranSendDetailProc();							// Send Detail Procedure
	int		fnAPP_TranDetailEndProc();							// Detail End Procedure
	int		fnAPP_TranAtmBidAdDownloadProc();					// Atm Bid Send Detail Procedure
    int		fnAPP_TranAtmBidSendDetailProc();					// Atm Bid Send Detail Procedure
	int		fnAPP_TranAtmBidSoftUpdateProc();					// Atm Bid Download update procedure
	int		fnAPP_TranInitCDKProc();							// OP INIT CDK Procedure
	int		fnAPP_TranInitMACProc();							// OP INIT MAC Procedure
	int		fnAPP_TranInitPINProc();							// OP INIT PIN Procedure
	int		fnAPP_TranCardRetainProc();							// Card Retain Procedure
	int		fnAPP_TranTotalKindProc();							// Total Kind procedure
	int		fnAPP_TranTotalPreProc();
	int     fnAPP_TranAtmBidDownloadFileProc(CString szTempFileNamePath);      //ATM BID SoftUpdate DownLoad Procedure
	int     fnAPP_TranAtmBidAdverDownLoadFileProc(CString szTempFileNamePath,int i);    //ATM BID AD DownLoad Procedure
	int     fnAPP_TranAtmBidNoticeDownLoadFileProc(CString szTempFileNamePath,int i);   //ATM BID NOTICE DownLoad Procedure
	int     fnAPP_TranAtmBidCautionDownLoadFileProc(CString szTempFileNamePath,int i);   //ATM BID CAUTION DownLoad Procedure
/////////////////////////////////////////////////////////////////////////////
//	Nms Main Function : 2006.07.03
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranAtmBidDCSendEjrProc();					// Atm DC Bid Send Ejr Procedure
	int		fnAPP_TranDCSendEjrProc();							// DC Send Ejr Procedure
	int		fnAPP_TranDCEjrEndProc();							// DC Ejr End Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranInitProc();								// Initialize Procedure
	int		fnAPP_TranMenuProc();								// Menu Procedure
	int		fnAPP_TranGetTranProc();							// Get Transaciton Procedure
	int		fnAPP_TranGetMenuDoingProc();						// Get Menu Doing Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Withdraw Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranWithInitProc();							// Withdraw Initialize Procedure
	int		fnAPP_TranWithInputProc();							// Withdraw Input Procedure
	int		fnAPP_TranWithOutputProc();							// Withdraw Output Procedure
	int		fnAPP_TranWithCheckDeviceActionProc();				// Withdraw Check Device Action Procedure
	int		fnAPP_TranWithCheckMenuContinueProc();				// Withdraw Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Inquiry Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranInqInitProc();							// Inquiry Initialize Procedure
	int		fnAPP_TranInqInputProc();							// Inquiry Input Procedure
	int		fnAPP_TranInqOutputProc();							// Inquiry Output Procedure
	int		fnAPP_TranInqCheckDeviceActionProc();				// Inquiry Check Device Action Procedure
	int		fnAPP_TranInqCheckMenuContinueProc();				// Inquiry Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Transfer Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranTransInitProc();							// Transfer Initialize Procedure
	int		fnAPP_TranTransInputProc();							// Transfer Input Procedure
	int		fnAPP_TranTransOutputProc();						// Transfer Output Procedure
	int		fnAPP_TranTransCheckDeviceActionProc();				// Transfer Check Device Action Procedure
	int		fnAPP_TranTransCheckMenuContinueProc();				// Transfer Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Deposit Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranDepInitProc();							// Deposit Initialize Procedure
	int		fnAPP_TranDepInputProc();							// Deposit Input Procedure
	int		fnAPP_TranDepOutputProc();							// Deposit Output Procedure
	int		fnAPP_TranDepCheckDeviceActionProc();				// Deposit Check Device Action Procedure
	int		fnAPP_TranDepCheckMenuContinueProc();				// Deposit Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Passbook Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranPbInitProc();								// Passbook Initialize Procedure
	int		fnAPP_TranPbInputProc();							// Passbook Input Procedure
	int		fnAPP_TranPbOutputProc();							// Passbook Output Procedure
	int		fnAPP_TranPbCheckDeviceActionProc();				// Passbook Check Device Action Procedure
	int		fnAPP_TranPbCheckMenuContinueProc();				// Passbook Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Ic Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranIcInitProc();								// Ic Initialize Procedure
	int		fnAPP_TranIcInputProc();							// Ic Input Procedure
	int		fnAPP_TranIcOutputProc();							// Ic Output Procedure
	int		fnAPP_TranIcCheckDeviceActionProc();				// Ic Check Device Action Procedure
	int		fnAPP_TranIcCheckMenuContinueProc();				// Ic Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
#endif
