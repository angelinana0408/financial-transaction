/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranLibProc_H_INCLUDED_)
#define _TranLibProc_H_INCLUDED_

#define ConstKey	"00000000"
typedef struct tagDetailTbl
{	
	char	AccountNo[19];										// Account No
	char	TranHandleCode[6];									// TranHandleCode
	char	TranAmount[12];										// Tran Amount
	char	SerialNo[8];										// Serial No
	char	TranTime[6];										// TranTime
	char	TranDate[8];										// TranDate

	char	DetailBatchNo[14];									// Detail Batch No
} DetailTbl;


/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Library Variable
/////////////////////////////////////////////////////////////////////////////
	char	PinKey[16];											// Pin Key
	char	MacKey[16];											// Mac Key
	char	FitFileContent[1024][128];							// Fit File Content
	char	FITContent_POST[1024][128];							// 邮储统一版本的FIT结构   by zjd
	char	ICFITContent_POST[1024][128];						// 邮储IC卡FIT结构 added by yusy ic
	char	ECHANNELContent_POST[1024][64];
	char	ECHANNELInq_POST[1024][8];
	BYTE	ECHANNELSEL[1024];
//	BYTE	PROTVERSIONDIFF[1024];

	DetailTbl	DetailData;
	CString		strDetailData;
	int			DetailBeginPSetFlag;
	
/////////////////////////////////////////////////////////////////////////////
//	Nms Variable : 2006.07.03
/////////////////////////////////////////////////////////////////////////////
	int			RecvOkFlag;										// Recv Ok Flag
	int			NmsSendFlag;									// Nms Send Flag
	int			DvrStatusReturn;								// Dvr Status Return
	int			StatusEtcLength;								// Status Etc Length
	char		StatusEtcBuffer[256];							// Status Etc Buffer
	char		DCNmsDateTime[14];								// DC Nms Date Tim
	char		DCBidEjrDate[8];								// DC Bid Ejr Date

	char		SaveCDK[16];									// CDK Key
	char		SaveMACKEY[8];									// MAC Key
	char		SavePINKEY[16];									// PIN Key

	int			DCEjrWriteFlag;									// DC Ejr Write Flag
	int			DCEjrSendCnt;									// DC Ejr Send Cnt
	CString		DCEjrSendData;									// DC Ejr Send Data

	int			CashRetractFlag;								// Cash Retract Flag : 2011.01.04

/////////////////////////////////////////////////////////////////////////////
// FitFileContent's Format
/////////////////////////////////////////////////////////////////////////////
//  CardType,CardNoTrackNo,CardNoOffset,CardNoLength,CardFlagTrackNo,CardFlag,CardFlagOffset,LocalOrUnLocalFlagOffset,LocalOrUnLocalBeginFlag,LocalOrUnLocalEndFlag,LocalFlag,AvailDateOffset,CurrencyCode
//
//  For Example: 0100,2,1,16,2,6103,1,1,610300,610399,0,19,C
//
//  0:  CardType, X1X2X3X4; 
//				  X1(0: Visa; 1: Master; 2: JCB; 3: Dinner; 4: AE)
//				  X2(0: Other Card, 1: Mother Bank Card, 2: Mother Credit Card), May OneSelf Define
//				  X3(0: Magcard; 1: IC; 2: Magcard + IC)
//				  X4(0: National; 1: International)
//	1:  CardNoTrackNo, Get CardNo's Track Number
//	2:  CardNoOffset, Get CardNo's Offset In CardData
//	3:  CardNoLength, CardNo's Length
//	4:  CardFlagTrackNo, Judge CardFlag's Track Number
//	5:  CardFlag, CardFlag Data
//	6:  CardFlagOffset, 
//	7:  LocalOrUnLocalFlagOffset,
//	8:  LocalOrUnLocalBeginFlag,
//	9:  LocalOrUnLocalEndFlag,
//	10: LocalFlag,
//	11: AvailDateOffset, Card's Available Date Offset, Length = 4
//	12: CurrencyCode


/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Library Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPL_GetErrorCodeKind(LPCTSTR szHsErrorCode);		// Get Error Code Kind
	CString	fnAPL_GetBankErrorCode(LPCTSTR szHsErrorCode);		// Get Bank Error Code
	int		fnAPL_FormatMacData(LPCTSTR MacData, int& Length, LPSTR DeData);
																// Fotmat Mac Data
	int		fnAPL_SetTotalDate();								// Set Total Date
	int		fnAPL_AddDayTotalCount();							// Add Day Total Count
	int		fnAPL_WriteTransactonEjr(int nEjrKind);				// Write Transaction Ejr
	int		fnAPL_FtpSendEjrProc();								// Ftp Send Ejr Proc
	int		fnAPL_FtpSetFileNameProc();							// Ftp Set File Name Proc
	int		fnAPL_DvrSendDataProc(LPCTSTR strSendKind);			// Dvr Send Data Proc
	int		fnAPL_FtpSendEjmProc(int nEjmType);					// Ftp Send Ejm Proc
	int		fnAPL_MergeTmpFile(CString strPath, CString strFileType, CString strInputDate, int nEjmType);
																// Merge Temp File
	int		fnAPL_SaveDetailData();								// Save Detail Data
	int		fnAPL_GetDetailData();								// Get Detail Data
	int		fnAPL_GetDetailCount();								// Get Detail Count
	int		fnAPL_BidSetWorkParamProc();						// Bid Set Work Param Procedure
//    void    fnAPL_GetNetMAC(char *pRetVal);
	
/////////////////////////////////////////////////////////////////////////////
//	Nms Function : 2006.07.03
/////////////////////////////////////////////////////////////////////////////
	int		fnAPL_SendHost();									// Send Host
	int		fnAPL_SendMake();									// Send Make
	int		fnAPL_GetUseStatus();								// Get Use Status
	int		fnAPL_EndProcess();									// End Process
	int		fnAPL_WriteDCTransactionEjr();						// Write DC Transaction Ejr
	int		fnAPL_GetDCTransactionEjr();						// Get DC Transaction Ejr
	int		fnAPL_GetDCEjrCount();								// Get DC Ejr Count
	int		fnAPL_GetDvrStatus();								// Get Dvr Status
	
	int		fnAPL_DesEncryptDecrypt(UCHAR*  szKey, UCHAR* szData, int Mode);
																// Des Encrypt or Decrypt(nMode 0:Decrypt, 1:Encrypt)
	int		fnAPL_EncryptPIN(LPSTR szPin, LPSTR szEncryptedPin);// Encrypt PIN
	int		fnAPL_GetFitFileContentSave(LPCTSTR FitFileName);	// Get Fit File Content Save
	int		fnAPL_GetFitFileContent();							// Get Fit Content From IniFile   by zjd
	int		fnAPL_GetICFitFileContent();						// Get IC Fit Content From IniFile added by yusy ic
	int		fnAPL_GetAIDListFileContent(PBCst_APPLIST *PbcAppList, int *len);	// Get AID List Content
	int		fnAPL_GetEchannelFileContent();
	int     fnAPL_GetCardAvailData(char* pTrack2, char* pTrack3, char* pAccountNo, int* pCardType);
	int     fnAPL_GetPOSTCardAvailData(char* pTrack2, char* pTrack3, char* pAccountNo, int* pCardType);		//by zjd
	int		fnAPL_GetICCardAvailData(char* pTrack2, char* pAccountNo, int* pCardType);
	int		fnAPL_AddBatchNo(int nBatchLength = 4);				// Add Batch No

	int		fnAPL_GetMACData(CString strMacData, UCHAR* szMacKey, UCHAR* szResult);

/////////////////////////////////////////////////////////////////////////////

	BOOL	fnAPL_checkLocalCard(CString pAccountNo);
	BOOL	fnAPL_checkOBTRANINQ(CString pAccountNo);

#endif
