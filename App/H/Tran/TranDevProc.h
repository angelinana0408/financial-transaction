/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranDevProc_H_INCLUDED_)
#define _TranDevProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Media Variable
/////////////////////////////////////////////////////////////////////////////
	int		DevEnDisable;				// Device EnDisable
	int		CardTran;					// Card Transaction
	int		FallBack;					// Fallback Tag			added by liuxl 20110505
	int		SeriesTran;					// 连续交易				added by yusy 20110526
	CString	HZTFlag;					// HZTFlag				added by yusy 20110526
	int		CardBank;					// Card Bank
	int		CardService;				// Card Service
	int		PbTran;						// Pb Transaction
	int		i101Error;					// 101 Page Reload times    by zjd

	MCAP	CardData;					// Card Data 
	MCISO2*	pCardData2;					// 2TR Pointer
	MCISO3*	pCardData3;					// 3TR Pointer
	char	CardAccountNo[256];			// Card Account No
	char	CardAccNoForMonitor[256];	// by zjd
	char	CardPan[256];				// Card Pan
	MS		PbMsData;					// Pb Ms Data
	PBMS*	pPbMsData;					// Pb Pointer

	int		PbDataCnt;					// Pb Data Count
	char	PbData[10240];				// Pb Data

	int		EjrIndexFlag;				// Elec journal Index Flag
	EjrTbl	EjrData;					// Elec journal Data

	char	ShuData[SPR_MAX_ROW][256];	// Shu Data
	int		SlipPrintFlag;				// Slip Print Flag
	int		JnlPrintFlag;				// Journal Print Flag
	
	int		MaterialOutFlag;			// Material Out Flag
	int		MaterialOutCheckFlag;		// Material Out Check Flag
	int		MaterialRetractFlag;		// Material Retract Flag
	int		MoneyOutFlag;				// Money Out Flag
	int		MoneyOutCheckFlag;			// Money Out Check Flag
	int		MoneyRetractFlag;			// Money Retract Flag

	int		ICScriptWriteFlag;			// 是否包含写脚本的域

/////////////////////////////////////////////////////////////////////////////
//	Cash Variable
/////////////////////////////////////////////////////////////////////////////
	int		CashFstCSTDispCnt;			// Cash First CST Dispence Count
	int		CashSndCSTDispCnt;			// Cash Second CST Dispence Count
	int		CashTrdCSTDispCnt;			// Cash Third CST Dispence Count
	int		CashFthCSTDispCnt;			// Cash Fourth CST Dispence Count

	int		CashRejCSTBeforeCnt;		// Cash Reject CST Before Count
	int		CashFstCSTBeforeCnt;		// Cash First CST Before Count
	int		CashSndCSTBeforeCnt;		// Cash Second CST Before Count
	int		CashTrdCSTBeforeCnt;		// Cash Third CST Before Count
	int		CashFthCSTBeforeCnt;		// Cash Fourth CST Before Count

	int		CashRejCSTAfterCnt;			// Cash Reject CST After Count
	int		CashFstCSTAfterCnt;			// Cash First CST After Count
	int		CashSndCSTAfterCnt;			// Cash Second CST After Count
	int		CashTrdCSTAfterCnt;			// Cash Third CST After Count
	int		CashFthCSTAfterCnt;			// Cash Fourth CST After Count
		
	int		CashRejCSTRejCnt;			// Cash Reject CST Reject Count
	int		CashFstCSTDispRejCnt;		// Cash First CST Dispense Reject Count
	int		CashSndCSTDispRejCnt;		// Cash Second CST Dispense Reject Count
	int		CashTrdCSTDispRejCnt;		// Cash Third CST Dispense Reject Count
	int		CashFthCSTDispRejCnt;		// Cash Fourth CST Dispense Reject Count

	int		CashDispPrintFlag;			// Cash Dispense Print Flag

	int		CashOutOpenFlag;			// Cash Out Open Flag
	int		CashCountFlag;				// Cash Count Flag
	int		CashCountEjrFlag;			// Cash Count Ejr Flag

	int		CashInOpenFlag;				// Cash In Open Flag
	int		CashInCountFlag;			// Cash In Count Flag
	int		CashRejectFlag;				// Cash Reject Flag
	int		CashRejectAFlag;			// Cash Reject A Flag
	int		CashDispErrorFlag;			// Cash Dispense Error Flag
	int		CashCountErrorFlag;			// added by liuxl 20110805
	int		TransCommTimeOut;			// Transaction Communication Time Out
	int		SprPrintATCFlag;
	int		LotteryWin;					// added by liuxl 20120306 
/////////////////////////////////////////////////////////////////////////////
//	Pbc Variable
/////////////////////////////////////////////////////////////////////////////
	PBCst_APPLIST		PbcAppList[MAX_APP_NUM];	// Pboc App List: 2010.10.20
	PBCst_TERM			PbcTerm;					// Pboc Term

	int					PbcReadLogFlag;				// Pboc Read Log Flag
	int					PbcAppTotal;				// Pboc App Total
	int					PbcAppIndex;				// Pboc App Index

	int					PbcCardData2Length;			// Pboc Card Data 2TR Length
	BYTE				PbcCardData2[256];			// Pboc Card Data 2TR

	int					PbcTermFlag;				// Pboc Terminate Flag
	PBCst_HOST_DATA		PbcHostData;				// Pboc Host Data
	BYTE				icdata9F36[256];

	PBCst_APPLIST		PbcAppDisp[MAX_APP_NUM];
	int					PbcAppDispTotal;


/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Media Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPD_CheckDeviceAction(int ChkID, int ErrChkFlag = TRUE);			
																// Check Device Action

	int		fnAPD_CardEnDisable(int Action, int InitFlag = FALSE, int WaitFlag = FALSE);
																// Card EnDisable
	int		fnAPD_PbEnDisable(int Action, int InitFlag = FALSE, int WaitFlag = FALSE);			
																// Pb EnDisable
	int		fnAPD_PinEnDisable(int Action, int InitFlag = FALSE, int nPinInputMode = PIN_DISABLE_MODE, LPCTSTR szCardData = NULL, int nPinPassWordMin = PIN_PASSWORD_MIN, int nPinPassWordMax = PIN_PASSWORD_MAX, int nPinPassWordAuto = PIN_PASSWORD_AUTO_FALSE, LPCTSTR szPinPassWordTerm = PIN_PASSWORD_TERM, int WaitFlag = FALSE);
																// Pin EnDisable

	int		fnAPD_CardReadCheck(LPCTSTR strInCardData = NULL);	// Card Read Check
	int		fnAPD_CardIcCheck(LPCTSTR strInCardData);			// Card Ic Check
	int		fnAPD_CardFallBack();								// Card Fall Back : 2010.10.20
	int		fnAPD_PbReadCheck();								// Pb Read Check

	int		fnAPD_CardWrite(MCAP* WriteCardData, int WaitFlag = FALSE);
																// Card Write

	int		fnAPD_PbPrint(int WaitFlag = FALSE);				// Pb Print
	int		fnAPD_EjrIndexSet();								// Elec journal Index Set
	int		fnAPD_ShuPrint(int PrintDevice, int WaitFlag = FALSE);	
																// Shu Print
	int		fnAPD_JnlPrint(int WaitFlag = FALSE);				// Journal Print(Internal Process)

	int		fnAPD_MaterialOut(int Device, int CheckTime = K_45_WAIT, int WaitFlag = FALSE);	
																// Material Out
	int		fnAPD_MaterialOutCheckRetract(int Device, int WaitFlag = TRUE);
																// Material Out Check Retract
	int		fnAPD_MaterialRetract(int Device, int WaitFlag = TRUE);			
																// Material Retract

	int		fnAPD_MoneyCountInitialize();						// Money Count Initialize
	int		fnAPD_MoneyCount(int WaitFlag = FALSE);				// Money Count
	int		fnAPD_MoneyCount10050(int WaitFlag = FALSE);		// Money Count 100 50
	int		fnAPD_MoneyCountCheck();							// Money Count Check
	int		fnAPD_MoneyOutCheck(int ScrNum = 0, int CheckTime = K_45_WAIT);	
																// Money Out Check : 2004.08.23
	int		fnAPD_MoneyRetract();								// Money Retract
	int		fnAPD_MoneyCountPrint(int WaitFlag = FALSE);		// Money Count Print
	int		fnAPD_MoneyRetractPrint(int WaitFlag = FALSE);		// Money Retract Print

	int		fnAPD_PbcInitialData();								// Pboc Initial Data
	int		fnAPD_PbcSelApp(int nSelAppMode, int btTryCntIn);	// Pboc Sel App
	int		fnAPD_PbcFinalApp(int nFinalMode);					// Pboc Final App
	int		fnAPD_PbcReadTrack();								// Pboc Read Track
	int		fnAPD_PbcReadPan();									// Pboc Read Pan
	int		fnAPD_PbcReadApp();									// Pboc Read App
	int		fnAPD_PbcStoreTlv();								// Pboc Store Tlv
	int		fnAPD_PbcReadyTrans();								// Pboc Ready Trans
	int		fnAPD_PbcSetResponse();								// Pboc Set Response
	int		fnAPD_PbcTerminate();								// Pboc Terminate
	CString	fstrAPD_PbcMakeCardData();							// Pboc Make Card Data
	CString	fstrAPD_PbcECInq();									// Pboc EC Inq
	CString	fstrAPD_PbcECLimit();									// Pboc EC Limit
	CString	fstrAPD_PbcECDetailInq();								// Pboc EC Detail Inq
	int		fnAPD_PbcMakeAppDisplay(BOOL bFlag);

	int		fnAPD_PbcReadIcData(char *icdatalist, int *nlen);	// 组合55域的值
	int		fnAPD_PbcReadIcDataSmp(char *icdatalist, int *nlen);	// 取款冲正用
	int		fnAPD_PbcChangeScript();
	int		fnAPD_CwcInfoSet(int TranCodeFlag);					// 冲正数据记录 added by liuxl 20110714
	int		fnAPD_CardRetainData(int cardRetainFlag);			// 吞卡原因 0:无, 1:超时, 2:故障, 3:主机返回码
	
/////////////////////////////////////////////////////////////////////////////
#endif
