#if !defined(AFX_REARAPCTL_H__1108737B_4328_47EB_B7FD_BF5D0113F66B__INCLUDED_)
#define AFX_REARAPCTL_H__1108737B_4328_47EB_B7FD_BF5D0113F66B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RearApCtl.h : Declaration of the CRearApCtrl ActiveX Control class.
#include <afxctl.h>

#include "ConstDef.h"
#include "RearApDefine.h"
#include "NHMwiOp.h"



/////////////////////////////////////////////////////////////////////////////
//	Atm Dvr Define
/////////////////////////////////////////////////////////////////////////////
#define _INIKEY_TIMEDEFINE_READINTERVALTIMEOUT          _T("READINTERVALTIMEOUT")           //读字符间隔超时时间
#define _INIKEY_TIMEDEFINE_READTOTALTIMEOUTMULTIPLIER   _T("READTOTALTIMEOUTMULTIPLIER")    //读操作时每字符的时间
#define _INIKEY_TIMEDEFINE_READTOTALTIMEOUTCONSTANT     _T("READTOTALTIMEOUTCONSTANT")      //基本的(额外的)读超时时间
#define _INIKEY_TIMEDEFINE_WRITETOTALTIMEOUTMULTIPLIER  _T("WRITETOTALTIMEOUTMULTIPLIER")   //写操作时每字符的时间
#define _INIKEY_TIMEDEFINE_WRITETOTALTIMEOUTCONSTANT    _T("WRITETOTALTIMEOUTCONSTANT")     //基本的(额外的)写超时时间
#define _INIKEY_TRAN_OPENCOMM                            _T("OPENCOMM") 
#define _INIKEY_DVRCOMMDATA				      	         _T("DVRCOMMDATA")
#define _INIKEY_TRAN_DVRMODE				             _T("DVRMODE")
#define _INIKEY_TRAN_DVRSTATUSFLAG						_T("DVRSTATUSFLAG")					// added by liuxl 20110926
#define _TRANSACTION_INI					           	"D:\\INI\\TRANSACTION.INI"
#define _INIKEY_TRANSACTION								_T("TRANSACTION")

#define _TIMEDEFINE_INI					               	"D:\\INI\\TIMEDEFINE.INI"
#define _INIKEY_TRAN_DVRSTATUS                           _T("DVRSTATUS")
#define _INIKEY_TRAN_CHANSTATUS                          _T("CHANSTATUS")

#define CMDCODE_SETTIME             1
#define CMDCODE_STARTDVR            2
#define CMDCODE_ENDDVR              3
#define CMDCODE_GETSTATUS           4

#define DVRCHAN_IDX0          0000
#define DVRCHAN_IDX9          1001
#define DVRCHAN_IDX14         1110
#define DVRCHAN_IDX15         1111

#define DVR_TIMEOUT    999 
#define DVR_GETSTATUS_TIMEOUT    000  


/////////////////////////////////////////////////////////////////////////////
// CRearApCtrl : See RearApCtl.cpp for implementation.
class CMwiScr;													// For Screen Logic based Mwi
class CNHMwiOp;													// For TTU Logic based Mwi Op

class CRearApCtrl : public COleControl
{
	DECLARE_DYNCREATE(CRearApCtrl)

// Constructor
public:
	CRearApCtrl();

/////////////////////////////////////////////////////////////////////////////
// Attributes
/////////////////////////////////////////////////////////////////////////////
	CNHMwiOp	MwiOp;											// MwiOp Object
	CNHMwiOp*	m_pMwiOp;										// MwiOp Pointer

	int		MwPatchVersion;										// MiddleWare Patch Version
	int		ApDeviceLangMode;									// Ap Device Language Mode
	int		OpDeviceLangMode;									// Op Device Language Mode

struct	tagScr													// Screen
{
	int		CurrentScreenNo;									// Current Screen No
	int		CurrentLangMode;									// Current Language Mode
} Scr;

struct	tagTtu													// Ttu
{
	CString			APCenterMovie;								// AP Center Movie
	CString			APMaxTime;									// AP Max Time
	CStringArray	APValueArray;								// AP Value Array
	CStringArray	APStateArray;								// AP State Array
	CStringArray	APValueArrayTemp;							// AP Value Array Temp
	CStringArray	APStateArrayTemp;							// AP State Array Temp

	CString			APUpdate;									// AP Update
	CString			APProgress;									// AP Progress

	int				Index;										// Index
	GuideScrHeader	Data;										// Data
	GuideScrHeader	Temp;										// Temp
	CStringArray	ScreenDisplayArray;							// Screen Display Array
	CStringArray	ScreenAcceptArray;							// Screen Accept Array
	
	int				GetAuto;									// Get Autometor
	int				GetPosition;								// Get Position
	int				GetPositionIndex[10];						// Get Position Index
	int				GetPositionSave;							// Get Position Save
	int				GetIndex;									// Get Index

	CString			GetAPValueSave;								// Get AP Value Save
	CString			GetData;									// Get Data
	CStringArray	GetAcceptArray;								// Get Accept Array
	CStringArray	GetDisplayArray;							// Get Display Array

	int		CurrentScreenNo;									// Current Screen No
	int		CurrentLangMode;									// Current Language Mode
} Ttu;

struct	tagClerk												// Clerk
{
	int		Doing;												// Doing
	int		PowerOffFlag;										// Power Off Flag
	int		ReadyFlag;											// Ready Flag
	int		DoorOpen11Flag;										// Door Open 11 Flag

	int		Index;												// Index
	int		TempIndex;											// Temp Index
	int		SaveIndex;											// Save Index
	CString	AtmOpStatus;										// Atm Op Status
	CString	AtmOpMessage;										// Atm Op Message

	int		UpdateScreen;										// Update Screen
	int		UpdateButton;										// Update Button
	int		UpdateMessage;										// Update Message

	CString	AtmStatus;											// Atm Status
	CString	OpenStatus;											// Open Status
	CString	SendRecvStatus;										// Send Recv Status
	CString	TransactionStaus;									// Transaction Status
	CString	PowerStaus;											// Power Status
	CString	AsPassWord;											// As PassWord
	CString	OpPassWord;											// Op PassWord
	CString	ScrBankName;										// Screen Bank Name
	CString	MachineKind;										// Machine Kind
	CString	OpSwitchStatus;										// Op Switch Status
	CString	FtpIp;												// Ftp Ip
	CString	FtpPort;											// Ftp Port
	CString	AtmSerialNumber;									// AtmSerial Number
	CString	BranchNumber;										// Branch Number
	CString	ApVersion;											// Ap Version
	CString	TransactionDate;									// Transaction Date
	CString	InterfaceKind;										// Interface Kind
	CString	OpDevice;											// Op Device : 2005.11.11
	CString	OpLed;												// Op Led : 2005.12.01
	CString	SplDevice;											// Spl Device : 2006.06.02
} Clerk;

struct	tagEjm													// Ejm
{
	CString	Date;												// Date
	CString	TranSerialNo;										// TranSerialNo
	CString	AccountNum;											// AccountNum
	
	int		Count;												// Count
	int		FindOkFlag;											// Find Ok Flag
	int		Position;											// Position
	EjrTbl	EjrData;											// Ejr Data
} Ejm;

//DVR
typedef struct
{
	BYTE    	StartCode[4];     //
	BYTE		CmdCode[3];       //
	BYTE		Time[16];
	BYTE		ChanIdx[4];	      //叠加的画面
	BYTE	    TimeOut[3];       //叠加的时间
	BYTE		Line1Data[44];	
	BYTE		Line2Data[44];
	BYTE		Checksum[2];      //按位异或
}DVR_CMD_DATASEND;

typedef	struct
{
	BYTE	  StartCode[4];
	BYTE	  DvrStatus[4];
	BYTE	  ChanStatus[4];	
	BYTE	  CheckSum[2];
	BYTE	  Reserved[2];  
} DVR_CMD_DATARECEIVE;

DVR_CMD_DATASEND     DvrCmdDataSend;
DVR_CMD_DATARECEIVE  DvrCmdDataReceive;
/////////////////////////////////////////////////////////////////////////////
// Operations
/////////////////////////////////////////////////////////////////////////////
	int		ClerkModeCtrl();									// Clerk Mode Control

	// MODE
	int		ClerkSelectModeProc();								// Clerk Select Mode Procedure
	
	// OP
	int		ClerkOpInputPinProc();								// Clerk Op Input Pin Procedure
	int		ClerkOpModeProc();									// Clerk Op Mode Procedure
	int		ClerkOpInputChangePinProc();						// Clerk Op Input Change Pin Procedure
	int		ClerkAddCashProc();									// Clerk Add Cash Procedure
	int		ClerkCashValueProc();								// Clerk Cash Value Procedure
	int		ClerkTotalProc();									// Clerk Total Procedure
	int		ClerkSpecialModeProc();								// Clerk Special Mode Procedure
	int		ClerkSetParamProc();								// Clerk Set Param Procedure
	int		ClerkSetMasterKeyProc();							// Clerk Set Master Key Procedure
	int		ClerkInputAuthCodeProc();							// Clerk Input Auth Code Procedure
	int		ClerkCodeInquiryProc();								// Clerk Code Inquiry Procedure
	int		ClerkInputMasterKeyABProc();						// Clerk Input Master Key AB Procedure
	
	// AS
	int		ClerkAsInputPinProc();								// Clerk As Input Pin Procedure
	int		ClerkAsModeProc();									// Clerk As Mode Procedure
	int		ClerkAsInputChangePinProc();						// Clerk As Input Change Pin Procedure
	int		ClerkApUpdateProc();								// Clerk Ap Update Procedure
	int		ClerkTraceCopyProc();								// Clerk Trace Copy Procedure
	int		ClerkVersionProc();									// Clerk Version Procedure
	int		ClerkWorkParamProc();								// Clerk Work Parameter Procedure
	int		ClerkDBInitialProc();								// Clerk DB Initialization Procedure
	int		ClerkOtherOperationProc();							// Clerk Other Operation Procedure
	int		ClerkReadIcMasterKeyProc();							// Clerk Read Ic Master Key Procedure
	
	// LOGIN
	int		ClerkAdminLoginProc();								// Clerk Administrator Login Procedure
	int		ClerkSuperAdminModeProc();							// Clerk Super Administrator Mode Procedure
	int		ClerkAddAdminProc();								// Clerk Add Administrator Procedure
	int		ClerkDelAdminProc();								// Clerk Delete Administrator Procedure
	int		ClerkChangeAdminPinProc();							// Clerk Change Administrator Pin Procedure
	
	// EJM
	int		ClerkEjmProc();										// Clerk Ejm Procedure
	int		ClerkEjmViewProc();									// Clerk Ejm View Procedure
	int		ClerkEjmResultProc();								// Clerk Ejm Result Procedure
	int		ClerkEjmCopyProc();									// Clerk Ejm Copy Procedure
	int		ClerkEjmSendProc();									// Clerk Ejm Send Procedure
	int		ClerkEjmPrintProc();								// Clerk Ejm Print Procedure
	int		ClerkEjmJprDirectPrintProc();						// Clerk Ejm Jpr Direct Print Procedure

	int		ClerkPostCountMenuProc();
	int		ClerkTotalInqureProc();
	int		ClerkCounterOtherProc();
	int		ClerkPostEJCopyProc();
	int		ClerkOpAdmInputPinProc();
	int		ClerkModifyAdmPinProc();
	int		ClerkPostTechMenuProc();
	int		ClerkKeyManageProc();
	int		ClerkOtherFunctionProc();
	int		ClerkDownloadParamProc();
	int		ClerkChangeMaterial();								// added by yusy 08.09.09
	int		ClerkInitEPP();										// added by yusy 2010.03.22
	int		ClerkTechModifyAdmPinProc();
	int		ClerkTechAddCashModeProc();
	int		ClerkTotalKindProc();								// Clerk Total Kind Procedure
	int		ClerkTechAdmInputPinProc();
	int		ClerkPostViewAtmcTotalProc();						// yaokq add 20081029
/////////////////////////////////////////////////////////////////////////////
	CString	ClerkCheckDriveIsInstall(unsigned nDriveType);		// Clerk Check Driver Is Install
	int		ClerkCheckDiskIsInsert(CString strDriveNum);		// Clerk Check Disk Is Insert
	DWORD	ClerkCheckDiskFreeSpace(CString strDriveNum);		// Clerk Check Disk Free Space
	int		ClerkCopyTmpFile(CString strPath, CString strFileType, CString strInputDate);
	int		ClerkMergeTmpFile(CString strPath, CString strFileType, CString strInputDate, int nEjmType);
	int		ClerkCopyEJFile(CString strPath, CString strFileType, CString strInputDate);
	int		ClerkFtpRun(CString strIP, CString strPort, CString strUserName, CString strPassword, CString strServerDir, CString strLocalFileName, CString strFtpFileName, int nRunType = 0);

/////////////////////////////////////////////////////////////////////////////
	CString	ClerkInformationMake(int nIndex);					// Clerk Information Make
	int		ClerkEjmRecordFind(int nFindKind);					// Clerk Ejm Record Find
	int		ClerkStatusUpdate(int InitFlag = FALSE);			// Clerk Status Update
	int		ClerkSetButton(CString strAtmStatus);				// Clerk Set Button
	int		ClerkWaitResult(int nWaitSec = OPM_DEVRSP_TIME);	// Clerk Wait Result

/////////////////////////////////////////////////////////////////////////////
//	SCR FUNCTION(SCR)
/////////////////////////////////////////////////////////////////////////////
// SCR SET MODE
	int		fnSCR_ClearErrorCode();								// Clear Error Code
	int		fnSCR_ClearKeyData();								// Clear Key Data
	int		fnSCR_SetScreenData(LPCTSTR szSetScreenDataName, LPCTSTR szSetScreenDataValue);
																// Set Screen Data(Screen Data Name, Screen Data Value)
	int		fnSCR_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue);
																// Set Display Data(Display Data Name, Display Data Value)
	int		fnSCR_SetCurrentLangMode(int nLangMode = CHN_MODE);	// Set Current Language Mode

// SCR GET MODE
	CString	fstrSCR_GetErrorCode();								// Get Error Code
	int		fnSCR_GetDeviceStatus();							// Get Device Status

	CString	fstrSCR_GetKeyData();								// Get Key Data
	CString	fstrSCR_GetKeyString(int nInitFlag = FALSE);		// Get Key String(Init Flag)
	CString	fstrSCR_GetCheckScreen(int nScreenNo, int nLangMode = CHN_MODE);	
																// Get Check Screen(Screen No, Language Mode)
	int		fnSCR_GetCurrentScreenNo();							// Get Current Screen No
	int		fnSCR_GetCurrentLangMode();							// Get Current Language Mode

	CString	fstrSCR_GetKeyNumeric(int nRetLength = 0, int nInitFlag = FALSE);	
																// Get Key Numeric(Ret Length:0=Real Length)
	
// SCR OPERATION MODE
	int		fnSCR_Initialize();									// Initialize
	int		fnSCR_Deinitialize();								// Deinitialize

	int 	fnSCR_DisplayScreen(int nScreenNo, int nDisplaySec = K_NO_WAIT, int nPinInputMode = PIN_DISABLE_MODE, LPCTSTR szCardData = "");
																// Display Screen(Screen No, Input Time, Pin Input Mode, Card Data)
	int 	fnSCR_DisplayUpdate(int nScreenNo, int nDisplaySec = K_NO_WAIT, int nPinInputMode = PIN_DISABLE_MODE, LPCTSTR szCardData = "");
																// Display Update(Screen No, Input Time, Pin Input Mode, Card Data)
	int 	fnSCR_DisplayMessage();								// Display Message
	int		fnSCR_DisplayImage(int nImageNo, int nOnOffFlag);	// Display Image(Image No, OnOff Flag)
	int		fnSCR_DisplayString(int nStringNo, LPCTSTR szString, int nInLength);
																// Display String(String No, String, Length)
	int		fnSCR_DisplayStringArray(int nStringNo, LPCTSTR szString);
																// Display String Array(String No, String)
	int		fnSCR_DisplayStringArray(int nStringNo, LPCTSTR szString, int nInLength);
																// Display String Array(String No, String, Length)
	int		fnSCR_DisplayString(int nStringNo, LPCTSTR szString);
																// Display String(String No, String)
	int		fnSCR_DisplayNumeric(int nStringNo, LPCTSTR szNumeric, int nInLength);
																// Display Numeric(String No, Numeric, Length)
	int		fnSCR_DisplayNumeric(int nStringNo, LPCTSTR szNumeric);
																// Display Numeric(String No, Numeric)
	int		fnSCR_DisplayNumeric(int nStringNo, int nNumeric);	// Display Numeric(String No, Numeric)
	int		fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, int nInLength, char cMarkChar = ' ', int nIsMinus = FALSE);
																// Display Amount(String No, Amount, Length, Mark, IsMinus)
	int		fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, char cMarkChar = ' ', int nIsMinus = FALSE);
																// Display Amount(String No, Amount, Mark, IsMinus)
	int		fnSCR_DisplayAmount(int nStringNo, int nAmount,	char cMarkChar = ' ', int nIsMinus = FALSE);
																// Display Amount(String No, Amount, Mark, IsMinus)

	int		fnSCR_ScanDeviceAction(int nDevId, int nScanTime, int nEventKind);
																// Scan Device Action(Device Id, Check Time, Event Kind)
	
/////////////////////////////////////////////////////////////////////////////
//	SFL FUNCTION(SFL)
/////////////////////////////////////////////////////////////////////////////
// SFL SET MODE
	int		fnSFL_ClearErrorCode();								// Clear Error Code
	int		fnSFL_ClearKeyData();								// Clear Key Data
	int		fnSFL_SetScreenData(LPCTSTR szSetScreenDataName, LPCTSTR szSetScreenDataValue);
																// Set Screen Data(Screen Data Name, Screen Data Value)
	int		fnSFL_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue);
																// Set Display Data(Display Data Name, Display Data Value)
	int		fnSFL_SetCurrentLangMode(int nLangMode = CHN_MODE);	// Set Current Language Mode

// SFL GET MODE
	CString	fstrSFL_GetErrorCode();								// Get Error Code
	int		fnSFL_GetDeviceStatus();							// Get Device Status

	CString	fstrSFL_GetKeyData();								// Get Key Data
	CString	fstrSFL_GetKeyString(int nInitFlag = FALSE);		// Get Key String(Init Flag)
	CString	fstrSFL_GetCheckScreen(int nScreenNo, int nLangMode = CHN_MODE);	
																// Get Check Screen(Screen No, Language Mode)
	int		fnSFL_GetCurrentScreenNo();							// Get Current Screen No
	int		fnSFL_GetCurrentLangMode();							// Get Current Language Mode

// SFL OPERATION MODE
	int		fnSFL_Initialize();									// Initialize
	int		fnSFL_Deinitialize();								// Deinitialize

/////////////////////////////////////////////////////////////////////////////
//	STT FUNCTION(STT)
/////////////////////////////////////////////////////////////////////////////
// STT SET MODE
	int		fnSTT_ClearErrorCode();								// Clear Error Code
	int		fnSTT_ClearKeyData();								// Clear Key Data
	int		fnSTT_SetScreenData(LPCTSTR szSetScreenDataName, LPCTSTR szSetScreenDataValue);
																// Set Screen Data(Screen Data Name, Screen Data Value)
	int		fnSTT_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue);
																// Set Display Data(Display Data Name, Display Data Value)
	int		fnSTT_SetCurrentLangMode(int nLangMode = CHN_MODE);	// Set Current Language Mode

// STT GET MODE
	CString	fstrSTT_GetErrorCode();								// Get Error Code
	int		fnSTT_GetDeviceStatus();							// Get Device Status

	CString	fstrSTT_GetKeyData();								// Get Key Data
	CString	fstrSTT_GetKeyString(int nInitFlag = FALSE);		// Get Key String(Init Flag)
	CString	fstrSTT_GetCheckScreen(int nScreenNo, int nLangMode = CHN_MODE);	
																// Get Check Screen(Screen No, Language Mode)
	int		fnSTT_GetCurrentScreenNo();							// Get Current Screen No
	int		fnSTT_GetCurrentLangMode();							// Get Current Language Mode

// STT OPERATION MODE
	int		fnSTT_Initialize();									// Initialize
	int		fnSTT_Deinitialize();								// Deinitialize

	int		fnSTT_APCenterMovie();								// AP Center Movie
	int		fnSTT_APUpdate();									// AP Update
	int		fnSTT_APProgress();									// AP Progress

	int		fnSTT_APScreenDataMakeGetAcceptArray();				// AP Screen Data Make Get Accept Array
	int		fnSTT_APScreenDataMakeGuideScrData();				// AP Screen Data Make Guide Screen Data
	int		fnSTT_APNormalDisplay();							// AP Normal Display
	int		fnSTT_APInvertedDisplay();							// AP Inverted Display

	int		fnSTT_APGetReadAuto();								// AP Get Read Auto

	int		fnSTT_APGetClear();									// AP Get Clear
	int		fnSTT_APGetSendCmd();								// AP Get Send Command
	int		fnSTT_APGetDataSet();								// AP Get Data Set
	int		fnSTT_APGetDataDisplay();							// AP Get Data Display
	int		fnSTT_APGetNextCmdSet();							// AP Get Next Command Set
	CString	fstrSTT_APGetReadDataSet();							// AP Get Read Data Set

/////////////////////////////////////////////////////////////////////////////
//	SOP FUNCTION(SOP)
/////////////////////////////////////////////////////////////////////////////
// SOP SET MODE
	int		fnSOP_ClearKeyData();								// Clear Key Data
	int		fnSOP_SetScreenData(LPCTSTR szSetScreenDataName, LPCTSTR szSetScreenDataValue);
																// Set Screen Data(Screen Data Name, Screen Data Value)
	int		fnSOP_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue);
																// Set Display Data(Display Data Name, Display Data Value)

// SOP GET MODE
	CString	fstrSOP_GetKeyData();								// Get Key Data

// SOP OPERATION MODE

/////////////////////////////////////////////////////////////////////////////
//	TTU FUNCTION(TTU)
/////////////////////////////////////////////////////////////////////////////
// TTU SET MODE
	int		fnTTU_ClearErrorCode();								// Clear Error Code

// SCR GET MODE
	CString	fstrTTU_GetErrorCode();								// Get Error Code
	int		fnTTU_GetDeviceStatus();							// Get Device Status
	CString	fstrTTU_GetReadData();								// Get Read Data

// TTU OPERATION MODE
	int		fnTTU_Initialize();									// Initialize
	int		fnTTU_Deinitialize();								// Deinitialize
	int		fnTTU_ClearScreenSync();							// Clear Screen Sync
	int		fnTTU_SetLEDSync(int LEDNumber, LPCTSTR State);		// Set LED Sync
	int		fnTTU_ReadAt(int x, int y, int NumberOpChars, LPCTSTR EchoMode, LPCTSTR EchoFormat, LPCTSTR InputType, int CursorOn, int Flush, int AutoEnd, int Timeout);
																// Read At
	int		fnTTU_WriteAtSync(int x, int y, LPCTSTR Format, LPCTSTR Text);
																// Write At Sync
	int		fnTTU_ClearAreaSync(int x, int y, int Width, int Hight);
																// Clear Area Sync
	int		fnTTU_CancelRead();									// Cancel Read

/////////////////////////////////////////////////////////////////////////////
//	COMMON FUNCTION(CMN)
/////////////////////////////////////////////////////////////////////////////
// CMN SET MODE
	int		fnCMN_ClearEventDevice(int nDevId);		// Clear Event Device(Device Id)
	int		fnCMN_ClearStatusDevice(int nDevId);	// Clear Status Device(Device Id)

// CMN GET MODE
	int		fnCMN_GetErrorDevice(int nDevId);		// Get Error Device(Device Id)
	int		fnCMN_GetTimeoutDevice(int nDevId);		// Get Timeout Device(Device Id)
	int		fnCMN_GetEventDevice(int nDevId);		// Get Event Device(Device Id)
	int		fnCMN_GetStatusDevice(int nDevId);		// Get Status Device(Device Id)

// CMN OPERATION MODE
	CString	fstrCMN_CheckDeviceAction(int nDevId, int nCheckTime);	
													// Check Device Action(Device Id, Check Time)
	int		fnCMN_ScanDeviceAction(int nDevId, int nScanTime, int nEventKind);	
													// Scan Device Action(Device Id, Check Time, Event Kind)
//DVR
	BOOL fnAPP_OpenComm(const char* pPort, int nBaudRate=57600, int nParity=NOPARITY, int nByteSize=8, int nStopBits=ONESTOPBIT);
	BOOL fnAPP_CloseComm();
	int  fnAPP_ReadComm(void* pData, int nLength);
    int  fnAPP_WriteComm(void* pData, int nLength);
	int  fnAPP_DvrDataComm(int DvrCmdCode,int DvrChanIdx,int DvrTimeOut,char UseStat[8]);
//////////////////////////////////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRearApCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnSetClientSite();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CRearApCtrl();

	DECLARE_OLECREATE_EX(CRearApCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CRearApCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CRearApCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CRearApCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CRearApCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//-------------------------------------------------------------------------
	afx_msg void OnDisplayData(LPCTSTR szDataName, LPCTSTR szDataValue);	// AXINAX

	DECLARE_EVENTSINK_MAP()										// AXINAX
	//-------------------------------------------------------------------------

// Dispatch maps
	//{{AFX_DISPATCH(CRearApCtrl)
	afx_msg long RearStart();
	afx_msg long SetScreenData(LPCTSTR SetScreenDataName, LPCTSTR SetScreenDataValue);
	afx_msg long TraceDump(LPCTSTR szTraceData);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CRearApCtrl)
	void FireOnDisplayData(LPCTSTR szDataName, LPCTSTR szDataValue)
		{FireEvent(eventidOnDisplayData,EVENT_PARAM(VTS_BSTR  VTS_BSTR), szDataName, szDataValue);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CRearApCtrl)
	dispidRearStart = 1L,
	dispidSetScreenData = 2L,
	dispidTraceDump = 3L,
	eventidOnDisplayData = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REARAPCTL_H__1108737B_4328_47EB_B7FD_BF5D0113F66B__INCLUDED)
