#if !defined(AFX_DEVCMN_H__C1CA462D_218C_4BA9_88BA_DBC13B14F86A__INCLUDED_)
#define AFX_DEVCMN_H__C1CA462D_218C_4BA9_88BA_DBC13B14F86A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DevCmn.h : header file
//

#include "Profiledef.h"
#include "IcEmv.h"
#include "IcPbc.h"

class CNHMWI;
class CProfile;
class CNetWork;
class CComPort;
class CLanPort;
class CNmsWork;
class CEtcWork;

/////////////////////////////////////////////////////////////////////////////
// CDevCmn window
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CDevCmn : public CWnd
{
/////////////////////////////////////////////////////////////////////////////
// Attributes
/////////////////////////////////////////////////////////////////////////////
public:
	CNHMWI*		m_pMwi;					// Mwi Pointer
	CProfile*	m_pProfile;				// Profile Pointer
	CNetWork*	m_pNetWork;				// NetWork Pointer
	CComPort*	m_pComPort;				// ComPort Pointer
	CLanPort*	m_pLanPort;				// LanPort Pointer
	CNmsWork*	m_pNmsWork;				// NmsWork Pointer
	CEtcWork*	m_pEtcWork;				// EtcWork Pointer
	
	DEVICEINFO	AtmDefine;				// Atm Define(Not Modify)
	NETWORKINFO	NetDefine;				// Network Define(Not Modify)
	TRANSINFO	TranDefine;				// Transaction Define(Not Modify)

	HANDLE	hADComm;					// 广告机串口设备句柄 2008.05.09
	int		IdleADFlag;					// 空闲广告播放标志
	CRITICAL_SECTION  ADMutex;			// 广告机写入串口锁
	int		HostLineMode;				// Host Online/Local
	int		CashCycleMode;				// Cash Recycle/Uncycle
	int		CashCashMode;				// Cash RealCash/TestCash

	int		CashMaxCashOutCnt;			// Cash Max Cash Out Count
	int		CashFixCashCnt;				// Cash Fix Cash Count
	int		CashEmptyCashCnt;			// Cash Empty Cash Count

	int		ApDeviceLangMode;			// Ap Device Language Mode
	int		OpDeviceLangMode;			// Op Device Language Mode

	int		AtmDoing;					// Atm Doing
	int		AtmStatus;					// Atm Status
	int		AtmStatusSave;				// Atm Status Save
	int		AtmStatusReEntryFlag;		// Atm Status ReEntry Flag
	int		HostStatus;					// Host Status
	int		HostStatusSave;				// Host Status Save
	int		TranStatus;					// Transaction Status
	int		TranStatusSave;				// Transaction Status Save
	int		TranProc;					// Transaction Procedure
	int		TranResult;					// Transaction Result

	int		DeviceDefine;				// Device Define Set
	int		DeviceNoDevice;				// Device No Device	Set
	int		DeviceDownDevice;			// Device Down Device Set
	int		DeviceStatus;				// Device Status Set
	int		DeviceAutoOff;				// Device Auto Off Set
	int		DeviceRetract;				// Device Retract Set
	int		DeviceTran;					// Device Transaction Set
	int		DeviceTimeout;				// Device Tiemout Set
	int		DeviceLost;					// Device Lost Set
	int		DeviceEvent;				// Device Event Set

	int		DeviceSetAutoOffBank;		// Device Set Auto Off Bank

	int		DeviceOperationFlag;		// Device Operation Flag
	
	int		SprRecvTime;				// Spr Recv Time
	int		JprRecvTime;				// Jpr Recv Time
	int		McuRecvTime;				// Mcu Recv Time
	int		CshRecvTime;				// Csh Recv Time
	int		PbmRecvTime;				// Pbm Recv Time
	int		RicRecvTime;				// Ric Recv Time
	int		IrdRecvTime;				// Ird Recv Time

    int		OpenKey;					// Open Key
	int		PowerOffDetect;				// Power Off Detect
	int		RecoveryType;				// 0:Auto Recovery,1:Manual Recovery
	int		TranType;					// 0:Customer Not Use,1:Customer Use(Error Send)

//	CString AccountType;				// Custom Select AccountType  1：默认，2：储蓄，3：支票，4：信用
// 	CString	strTFRInaccountType;		// 转帐选择的转出类型，2为卡，1为折        //20070802
	int			CurErrCnt;				// Current Error Count
	ERRSTACK	CurErrBuff;				// Current Error Buffer
	ERRSTACK	FirstErrBuff;			// First   Error Buffer
	ERRSTACK	LastErrBuff;			// Last    Error Buffer
	ERRSTACK	PrintErrBuff;			// Print   Error Buffer

	int		CSTCnt;						// CST Count
	int		FstCSTValue;				// First CST Value
	int		SndCSTValue;				// Second CST Value
	int		TrdCSTValue;				// Third CST Value
	int		FthCSTValue;				// Fourth CST Value
	int		FstCSTSetCash;				// First CST Set Cash
	int		SndCSTSetCash;				// Second CST Set Cash
	int		TrdCSTSetCash;				// Third CST Set Cash
	int		FthCSTSetCash;				// Fourth CST Set Cash
	int		FstCSTCnt;					// First CST Count(ClerkInformation)
	int		SndCSTCnt;					// Second CST Count(ClerkInformation)
	int		TrdCSTCnt;					// Third CST Count(ClerkInformation)
	int		FthCSTCnt;					// Fourth CST Count(ClerkInformation)

	int		RejectCSTCnt;				// Reject CST Count(ClerkInformation)
	int		FstCSTRejectCnt;			// First CST Reject Count
	int		SndCSTRejectCnt;			// Second CST Reject Count
	int		TrdCSTRejectCnt;			// Third CST Reject Count
	int		FthCSTRejectCnt;			// Fourth CST Reject Count

	int		RetractCSTCnt;				// Retract CST Count(ClerkInformation)
	int		Csh100RetractCnt;			// Csh 100 Retract Count
	int		Csh50RetractCnt;			// Csh 50 Retract Count
	int		Csh20RetractCnt;			// Csh 20 Retract Count
	int		Csh10RetractCnt;			// Csh 10 Retract Count
	int		CshUnknownRetractCnt;		// Csh Unknown Retract Count

	int		DepositCSTCnt;				// Deposit CST Count(ClerkInformation)
	int		Csh100DepositCnt;			// Csh 100 Deposit Count
	int		Csh50DepositCnt;			// Csh 50 Deposit Count
	int		Csh20DepositCnt;			// Csh 20 Deposit Count
	int		Csh10DepositCnt;			// Csh 10 Deposit Count
	int		CshUnknownDepositCnt;		// Csh Unknown Deposit Count

	int		CshD100DepositCnt;			// Csh D100 Deposit Count(ClerkInformation)
	int		CshD50DepositCnt;			// Csh D50 Deposit Count(ClerkInformation)
	int		CshC100DepositCnt;			// Csh C100 Deposit Count
	int		CshC50DepositCnt;			// Csh C50 Deposit Count

	int		CshChangeLoc;				// Csh Change Location
	int		CshLoc;						// Csh Location
	int		RejectCSTLoc;				// Reject CST Location(ClerkInformation)
	int		RetractCSTLoc;				// Retract CST Location(ClerkInformation)
	int		DepositCSTLoc;				// Deposit CST Location(ClerkInformation)
	int		CshD100DepositLoc;			// Csh D100 Deposit Location(ClerkInformation)
	int		CshD50DepositLoc;			// Csh D50 Deposit Location(ClerkInformation)
	int		CshC100DepositLoc;			// Csh C100 Deposit Location
	int		CshC50DepositLoc;			// Csh C50 Deposit Location
	int		ClerkCSTLoc;				// Clerk CST Location
	int		FstCSTLoc;					// First CST Location(Cd)
	int		SndCSTLoc;					// Second CST Location(Cd)
	int		TrdCSTLoc;					// Third CST Location(Cd)
	int		FthCSTLoc;					// Fourth CST Location(Cd)
	int		DoorStatus;					// Door	Status
	int		InternalTamperStatus;		// Internal Tamper Status

	int		SlipHeadStatus;				// Slip	Head Status
	int		JnlHeadStatus;				// Journal Head Status
	int		SlipStatus;					// Slip Status
	int		JnlStatus;					// Journal Status

	int		CashMaxRetractCnt;			// Cash Max Retract Count
	int		CashRetractCnt;				// Cash Retract Count
	int		SlipMaxRetractCnt;			// Slip Max Retract Count
	int		CardMaxRetractCnt;			// Card Max Retract Count
	int		PBMaxRetractCnt;			// PassBook Max Retract Count
	int		SlipRetractCnt;				// Slip Retract Count
	int		CardRetractCnt;				// Card Retract Count
	int		PBRetractCnt;				// PassBook Retract Count

	int		RejectCSTStatus;			// Reject CST Status(ClerkInformation)
	int		RetractCSTStatus;			// Retract CST Status(ClerkInformation)
	int		DepositCSTStatus;			// Deposit CST Status(ClerkInformation)
	int		CshD100DepositStatus;		// Csh D100 Deposit Status(ClerkInformation)
	int		CshD50DepositStatus;		// Csh D50 Deposit Status(ClerkInformation)
	int		CshC100DepositStatus;		// Csh C100 Deposit Status
	int		CshC50DepositStatus;		// Csh C50 Deposit Status
	int		ClerkCSTStatus;				// Clerk CST Status
	int		RecycleBoxStatus;			// Recycle Box Status
	int		FstCSTStatus;				// First CST Status
	int		SndCSTStatus;				// Second CST Status
	int		TrdCSTStatus;				// Third CST Status
	int		FthCSTStatus;				// Fourth CST Status

	int		CshInitAmount;				// Init Amount
	int		CshRemainAmount;			// Remain Amount
	
	int		TranAvail;					// Transaction Avail
	int		WithAvail;					// Withdraw Avail
	int		DepAvail;					// Deposit Avail
	int		PbmAvail;					// Pbm Avail

	CString	InServiceLED;				// In ServiceLED
	CString	SupervisorLED;				// Supervisor LED
	CString	ErrorLED;					// Error LED
	CString	CommunicationsLED;			// Communication LED
	int		Indicator;					// Indicator

	int		HostOpenFlag;				// Host Open Flag
	int		HostOpenRetryCnt;			// Host Open Retry Count
	int		HostCancelRetryCnt;			// Host Cancel Retry Count

	int		ATMFirstStart;				// flag of ATM Start 1:First
	bool	bFirstStartRQK;				// 是否机器启动第一次发送RQK  by zjd added by liuxl 20110622

	CString szAgentCreateFileTime;     //  File Create TimeYYYYMMDDHHMMSSsss 
	CString szAgentCreateFilePathName; // File Name of Agent Create 交易
	int     bAgentRestFlag;                // Agent Rest Flag 1:失败 2:成功 
    int     iFileNumber;               // 文件序号
	int     bAgentMode;                // Agent是否启动 0:不开通 1:开通
	
struct	tagTimer						// Timer
{
	int		HostWaitLinkTime;			// Host Wait Link Time
	int		HostOpenRetryTime;			// Host Open Retry Time
	int		HostCancelRetryTime;		// Host Cancel Retry Time

	int		StatusSendDelayTime;		// Status Send Delay Time
	int		AgentStatusSendDelayTime;	// Agent Status Send Delay Time // added by yusy 08.09.09
	int		EtcSendDelayTime;			// Etc Send Delay Time
} Timer;

	int			EjrThreadUse;			// Elec Journal Thread Use
	int			EjrDoing;				// Elec Journal Doing
	EjrTbl		Ejr;					// Elec Journal
	EjrSprTbl	EjrSpr;					// Elec Journal Spr

	CString	CaptureSerialNo;			// Capture Serial No

	int		iMonitorCS_Custom;			// Monitor Custom Number
	int		iMonitorCS_AddCash;			// Monitor Add Cash Number
	int		iFaceCamNo;					// Camera No for Face
	int		iHandCamNo;					// Camera No for Hand
	int		iAddCashCamNo;				// Camera No for AddCash

	int		MonitorCustomNo;			// Monitor Custom Number
	int		MonitorAddCashNo;			// Monitor Add Cash Number
	int		FaceCamNo;					// Camera No for Face
	int		HandCamNo;					// Camera No for Hand
	int		AddCashCamNo;				// Camera No for AddCash

	int		VfdThreadUse;				// Vfd Thread Use
	int		VfdDoing;					// Vfd Doing

	int		FtpThreadUse;				// Ftp Thread Use
	int		FtpDoing;					// Ftp Doing

	int		McuReadMSOnlyFlag;			// Mcu Read Only MS Flag:2010.10.20
	int		McuProtocolCode;			// Mcu Protocol Code:2010.10.20

struct	tagPbc							// Pboc Call Back Send Data : 2010.10.20
{
	DWORD	TranAmount;					// Transaction Amount
	int		POSEntryCode;				// POS Entry Code
} Pbc;

struct	tagScr							// Screen
{
	int		CurrentScreenNo;			// Current Screen No
	int		CurrentLangMode;			// Current Language Mode
} Scr;

struct	tagOp							// For Op Information
{
	// OP Variable
	CString	SendRecvStatus;				// SendRecvStatus(ON,OFF)
	CString	TransationStatus;			// TransationStatus(ON,OFF)

	// Status Information
	CString	Status;						// OP_STATUS
										// 0:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN)
										// 1:OPEN STATUS(ON,OFF)
										// 2:SEND RECV STATUS(ON,OFF)
										// 3:TRANSACTION STATUS(ON,OFF)
										// 4:POWER STATUS(ON,OFF)
										// 5:AS PASSWORD
										// 6:OP PASSWORD
										// 7:SCR BANK NAME
										// 8:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L)
										// 9:OPERATOR SWITCH STATUS(NONE,SUPERVISOR,RUN,SUPERVISORCHANGED,RUNCHANGED)
										// 10:FTP IP(NNNNNNNNNNNN)
										// 11:FTP PORT(NNNNN)
										// 12:ATM SERIAL NUMBER(NNNNNNNN)
										// 13:BRANCH NUMBER(NNNNNNNN)
										// 14:AP VERSION(V00-00-00)
										// 15:TRANSACTION DATE(YYYYMMDD)
										// 16:INTERFACE KIND(N)
										// 17:OP DEVICE(NOT_SET, OP_TOUCH, OP_TSPL, OP_TSPLCHN, OP_HSPL)
										// 18:OP LED(POWERLED,COMMLED,INSERVICELED,SUPERVISORLED,ERRORLED,CARDLED,REPORTLED,NOTELED)
										// 19:SPL DEVICE(NOT_SET, SPL_TOGGLESW, SPL_ONOFFSW)
					
	// Header Information			
	CString	Header;						// OP_HEADER
										// 0:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L)
										// 1:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN)
										// 2:DOOR STATUS(DOOROPEN, DOORCLOSE)
										// 3:LINE STATUS(HOSTONLINE, HOSTOFFLINE)
										// 4:MESSAGE
										// 5:ERROR CODE([P] - [EEEEE(SS)])
										// 6:CST1(CCCC(GGG))
										// 7:CST2(CCCC(GGG))
										// 8:CST3(CCCC(GGG))
										// 9:CST4(CCCC(GGG))
										// 10:CDU(NORMAL, ERROR)
										// 11:MCU(NORMAL, ERROR)
										// 12:SPR(NORMAL, ERROR)
										// 13:JPR(NORMAL, ERROR)
										// 14:CST1(MISSING, NORMAL, LOW, EMPTY, FULL)
										// 15:CST2(MISSING, NORMAL, LOW, EMPTY, FULL)
										// 16:CST3(MISSING, NORMAL, LOW, EMPTY, FULL)
										// 17:CST4(MISSING, NORMAL, LOW, EMPTY, FULL)
										// 18:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL)
										// 19:MCU(C)
										// 20:SPR(MISSING, NORMAL, LOW, EMPTY)
										// 21:JPR(MISSING, NORMAL, LOW, EMPTY)
										// 22:ATM SERIAL NUMBER(NNNNNNNN)
										// 23:BRANCH NUMBER(NNNNNNNN)
										// 24:AP VERSION(V00-00-00)
										// 25:TRANSACTION DATE(YYYYMMDD)

	// ApUpdate Information 
	CString	ApUpdate;					// OP_APUPDATE
										// 0:AP VERSION(V00-00-00)
										// 1:TRANSACTION DATE(YYYYMMDD)

	// TraceCopy Information 
	CString	TraceCopy;					// OP_TRACECOPY
										// 0:AP VERSION(V00-00-00)
										// 1:TRANSACTION DATE(YYYYMMDD)
										// 2:TRACE DATE(YYYYMMDD)

	// Version Information
	CString	Version;					// OP_VERSION
										// 0:AP VERSION(V00-00-00)
										// 1:TRANSACTION DATE(YYYYMMDD)
										// 2:CDU SP VERSION
										// 3:JPR SP VERSION
										// 4:SPR SP VERSION
										// 5:MCU SP VERSION
										// 6:PIN SP VERSION
										// 7:CAM SP VERSION
										// 8:SIU SP VERSION
										// 9:UPS SP VERSION
										// 10:TTU SP VERSION
										// 11:MWI SP VERSION
										// 12:COMMONITOR SP VERSION
										// 13:CDU EP VERSION
										// 14:JPR EP VERSION
										// 15:SPR EP VERSION
										// 16:MCU EP VERSION
										// 17:PIN EP VERSION
										// 18:CAM EP VERSION
										// 19:SIU EP VERSION
										// 20:UPS EP VERSION
										// 21:TTU EP VERSION

	// WorkParam Information
	CString	WorkParam;					// OP_WORKPARAM
										// 0:ATM SERIAL NUMBER(NNNNNNNN)
										// 1:BRANCH NUMBER(NNNNNNNN)
										// 2:RESERVED NUMBER1(NNNNNNNN)
										// 3:RESERVED NUMBER2(NNNNNNNN)
										// 4:HOST IP(NNNnnnNNNnnn)
										// 5:HOST PORT(NNNNN)
										// 6:HOST IP2(NNNnnnNNNnnn)
										// 7:HOST PORT2(NNNNN)
										// 8:ATM IP(NNNnnnNNNnnn)
										// 9:ATM SUBNET MASK(NNNnnnNNNnnn)
										// 10:ATM GATE WAY(NNNnnnNNNnnn)
										// 11:ATM PORT(NNNNN)
										// 12:HOST IP3(NNNnnnNNNnnn) : BID
										// 13:HOST PORT3(NNNNN) : BID
										// 14:HOST IP4(NNNnnnNNNnnn) : FTP
										// 15:HOST PORT4(NNNNN) : FTP

	// AddCash Information
	CString	AddCash;					// OP_ADDCASH
										// 0:CST1(CCCC(GGG))
										// 1:CST2(CCCC(GGG))
										// 2:CST3(CCCC(GGG))
										// 3:CST4(CCCC(GGG))
										// 4:CDU(NORMAL, ERROR)
										// 5:MCU(NORMAL, ERROR)
										// 6:SPR(NORMAL, ERROR)
										// 7:JPR(NORMAL, ERROR)
										// 8:CST1(VVV)
										// 9:CST1(CCCC)
										// 10:CST1(AAAAAA)
										// 11:CST2(VVV)
										// 12:CST2(CCCC)
										// 13:CST2(AAAAAA)
										// 14:CST3(VVV)
										// 15:CST3(CCCC)
										// 16:CST3(AAAAAA)
										// 17:CST4(VVV)
										// 18:CST4(CCCC)
										// 19:CST4(AAAAAA)

	// EjmView Information
	CString	EjmView;					// OP_EJMVIEW
										// 0:TRANSACTION DATE(YYYYMMDD)
										// 1:SERIAL NO(NNNNNN)
										// 2:CARD NUMBER(X19)

	// EjmResult Information
	CString	EjmResult;					// OP_EJMRESULT
										// 0-11:EJR DATA(X40)
										// 12-15:PIC NAME

	// EjmCopy Information 
	CString	EjmCopy;					// OP_EJMCOPY
										// 0:AP VERSION(V00-00-00)
										// 1:TRANSACTION DATE(YYYYMMDD)
										// 2:EJM START DATE(YYYYMMDD)
										// 3:EJM END DATE(YYYYMMDD)
	
	// EjmSend Information 
	CString	EjmSend;					// OP_EJMSEND
										// 0:AP VERSION(V00-00-00)
										// 1:TRANSACTION DATE(YYYYMMDD)
										// 2:EJM START DATE(YYYYMMDD)
										// 3:EJM END DATE(YYYYMMDD)

	// EjmPrint Information 
	CString	EjmPrint;					// OP_EJMPRINT
										// 0:AP VERSION(V00-00-00)
										// 1:TRANSACTION DATE(YYYYMMDD)
										// 2:EJM START DATE(YYYYMMDD)
										// 3:EJM END DATE(YYYYMMDD)
	
	// CashValue Information 
	CString	CashValue;					// OP_CASHVALUE
										// 0:CST1(VVV)
										// 1:CST2(VVV)
										// 2:CST3(VVV)
										// 3:CST4(VVV)

	// DbInitail Information 
	CString	DbInitial;					// OP_DBINITIAL
										// 0:AP VERSION(V00-00-00)
										// 1:TRANSACTION DATE(YYYYMMDD)

	// Total Information
	CString	Total;						// OP_TOTAL
										// 0:TRAN BATCH NO(NNNNNNNNNnNNNN)
										// 1:INPUT BATCH NO(NNNNNNNNNnNNNN)

	// SetParam Information
	CString SetParam;					// OP_SETPARAM
										// 0:RESERVE1(NNNNNNNNNnNNNNNN)
										// 1:RESERVE2(NNNNNNNNNnNNNNNN)
										// 2:RESERVE3(NNNNNNNNNnNNNNNN)
										// 3:RESERVE4(NNNNNNNNNnNNNNNN)
										// 4:RECEIVE TIMEOUT(NNNN)
										// 5:IDLE RETRY TIME(NNNN)
										// 6:WITH MAX AMOUNT(NNNN)
										// 7:TRANS MAX AMOUNT(NNNNNNNNNnNN)
										// 8:HOST IP PAY(NNNnnnNNNnnn)
										// 9:HOST PORT PAY(NNNNN)
	CString ViewAtmcTotal;				// yaokq add 20081031
} Op, OpSave;

/////////////////////////////////////////////////////////////////////////////
// Operations
/////////////////////////////////////////////////////////////////////////////
public:

/////////////////////////////////////////////////////////////////////////////
//	Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
	CDevCmn();
	virtual ~CDevCmn();

/////////////////////////////////////////////////////////////////////////////
//	FUNCTION
/////////////////////////////////////////////////////////////////////////////
//	RETURN RULE1:NORMAL=TRUE, ABNORMAL=FALSE
//			   2:RETURN INT ONLY(TRUE OR FALSE OR VALUE)
//			   3:TRUE!=0, FALSE=0
//			   4:CSTRING APPEND									2003.08.23
/////////////////////////////////////////////////////////////////////////////
	void	SetOwner(CNHMWI* pMwi);					// Set Owner

/////////////////////////////////////////////////////////////////////////////
//	JOURNAL FUNCTION(JPR)
/////////////////////////////////////////////////////////////////////////////
// JPR SET MODE
	int		fnJPR_ClearErrorCode();					// Clear Error Code

// JPR GET MODE
	CString	fstrJPR_GetErrorCode();					// Get Error Code
	CString	fstrJPR_GetSensorInfo();				// Get Sensor Information
	int		fnJPR_GetDeviceStatus();				// Get Device Status

	int		fnJPR_GetPaperStatus();					// Get Paper Status
	int		fnJPR_GetMaterialInfo();				// Get Material Information

// JPR OPERATION MODE
	int		fnJPR_Initialize();						// Initialize
	int		fnJPR_Deinitialize();					// Deinitialize

	int		fnJPR_EmbossPrint();					// Emboss Print
	int		fnJPR_Print(LPCTSTR szPrintData);		// Print(Print Data)
	int		fnJPR_PrintLog(LPCTSTR szPrintData);	// Print Log(Print Data)
	int		fnJPR_PrintFile(LPCTSTR szPrintData);	// Print(Print Data To File)
// 	int  	fnAPL_AgentCreateFile();                // Create File
    int     fnJPR_AgentIdxPrintFile(LPCTSTR szPrintData);// Print(Print Idx To File)	
/////////////////////////////////////////////////////////////////////////////
//	SLIP PRINTER FUNCTION(SPR)
/////////////////////////////////////////////////////////////////////////////
// SPR SET MODE
	int		fnSPR_ClearErrorCode();					// Clear Error Code
	int		fnSPR_ClearRetractCnt();				// Clear Retract Count
	int		fnSPR_SetPrintImageMode(int nPrintImageMode);				
													// Set Print Image Mode

// SPR GET MODE
	CString	fstrSPR_GetErrorCode();					// Get Error Code
	CString	fstrSPR_GetSensorInfo();				// Get Sensor Information
	int		fnSPR_GetDeviceStatus();				// Get Device Status

	int		fnSPR_GetPaperStatus();					// Get Paper Status
	int		fnSPR_GetMaterialInfo();				// Get Material Information

	CString	fstrSPR_GetRetractCnt();				// Get Retract Count

// SPR OPERATION MODE
	int		fnSPR_Initialize();						// Initialize
	int		fnSPR_Deinitialize();					// Deinitialize

	int		fnSPR_Print(LPCTSTR szPrintData);		// Print(Print Data)
	int		fnSPR_PrintImage(LPCTSTR szImageName);	// PrintImage(Image Name)
	int		fnSPR_Eject(int nWaitSec = K_45_WAIT);	// Eject(Wait Time)
	int		fnSPR_WaitTaken();						// Wait Taken
	int		fnSPR_CancelWaitTaken();				// Cancel Wait Taken
	int		fnSPR_Retract();						// Retract
    int		fnSPR_PrintFile(LPCTSTR szPrintData);	// Print(Print Data To File)

/////////////////////////////////////////////////////////////////////////////
//	MAGNETIC CARD UNIT FUNCTION(MCU)
/////////////////////////////////////////////////////////////////////////////
// MCU SET MODE
	int		fnMCU_ClearErrorCode();					// Clear Error Code
	int		fnMCU_ClearRetractCnt();				// Clear Retract Count

// MCU GET MODE
	CString	fstrMCU_GetErrorCode();					// Get Error Code
	CString	fstrMCU_GetSensorInfo();				// Get Sensor Information
	int		fnMCU_GetDeviceStatus();				// Get Device Status

	int		fnMCU_GetMaterialInfo();				// Get Material Information
	int		fnMCU_ICGetChipPowerStatus();			// Get IC Chip Power Status

	CString	fstrMCU_GetRetractCnt();				// Get Retract Count
	CString	fstrMCU_GetCardData();					// Get Card Data
	CString	fstrMCU_GetICData();					// Get IC Data

// MCU OPERATION MODE
	int		fnMCU_Initialize();						// Initialize
	int		fnMCU_Deinitialize();					// Deinitialize

	int		fnMCU_EntryEnable(int nWaitSec = K_NO_WAIT);	
													// Entry Enable(Wait Time)
	int		fnMCU_EntryDisable();					// Entry Disable
	int		fnMCU_Read();							// Read	
	int		fnMCU_Eject(int nWaitSec = K_45_WAIT);	// Eject(Wait Time)	
	int		fnMCU_WaitTaken();						// Wait Taken
	int		fnMCU_CancelWaitTaken();				// Cancel Wait Taken
	int		fnMCU_Retract();						// Retract
	int		fnMCU_Write(LPCTSTR szTrackData);		// Write(Track Data)

	int		fnMCU_ICChipInitialize();				// Initialize IC Chip:Move Point
	int		fnMCU_ICEntryEnable(int nWaitSec = K_NO_WAIT);	
													// IC Entry Enable(Wait Time)
	int		fnMCU_ICEntryDisable();					// IC Entry Disable
	int		fnMCU_ICSendData(int nProtocolId, LPCTSTR szICSendData, int nWaitSec = K_NO_WAIT);	
													// IC Send Data(Protocol ID, Send Data, Wait Time)
	int		fnMCU_ICChipPower(int nActType);		// IC Chip Power

/////////////////////////////////////////////////////////////////////////////
//	EMV IC CARD UNIT FUNCTION(MCU)
/////////////////////////////////////////////////////////////////////////////
// MCU SET MODE
// MCU GET MODE
// MCU OPERATION MODE
// EMV Application
	int		fnEMV_SelApplicationSelection(BYTE btSupportPSE, EMVst_AIDLIST* stpAidList, int* npCandiDateCnt);
													// Emv Sel Application Selection
	int		fnEMV_SelFinalAppSelection(WORD wAidLen, BYTE* btpAidVal, BYTE* btpSW);
													// Emv Sel Final Application Selection
	int		fnEMV_SelMakeDisplayAppList(BYTE btSupportUserConfirm, WORD wCandiDateCnt, EMVst_APPLIST* stpAppList);
													// Emv Sel Make Display Application List
	int		fnEMV_SelRemoveCandidateList(WORD wCandidateCnt, WORD wAidLen, BYTE* btpAidVal);
													// Emv Sel Remove Candidate List

// EMV Transaction
	int		fnEMV_TransInitApplication();			// Emv Trans Initialize Application
	int		fnEMV_TransReadAppData();				// Emv Trans Read Application Data
	int		fnEMV_TransOfflineDataAuth(EMVst_CA_PUBKEY* stpCaPubKey);
													// Emv Trans Offline Data Authentification
	int		fnEMV_TransProcessRestrict();			// Emv Trans Process Restrict
	int		fnEMV_TransCardholderVerify(WORD wOnlinePinLen, BYTE* btpEncipheredOnlinePin);
													// Emv Trans Cardholder Verify
	int		fnEMV_TransTerminalRiskMgmt(BYTE btSupportForceOnline);
													// Emv Trans Terminal Risk Mangement
	int		fnEMV_TransTerminalActionAnalysis();	// Emv Trans Terminal Action Analysis
	int		fnEMV_TransCardActionAnalysis(BYTE* btpOnlineFlag);
													// Emv Trans Card Action Analysis
	int		fnEMV_TransOnlineProcess(int nOnlineMode, BYTE btAcquirerCid);
													// Emv Trans Online Process
	int		fnEMV_TransCompletion();				// Emv Trans Completion

// EMV Tlv Function
	int		fnEMV_TlvGetTidTag(int nTid, BYTE* btpTag);
													// Emv Tlv Get Tid Tag
	int		fnEMV_TlvGetTid(BYTE* btpTag, int nLen);// Emv Tlv Get Tid
	int		fnEMV_TlvGetTag(BYTE* btpBuf, BYTE* btpTag);
													// Emv Tlv Get Tag
	int		fnEMV_TlvGetVal(int nTid, int* npLen, BYTE* btpVal);
													// Emv Tlv Get Value
	int		fnEMV_TlvGetTidLen(int nTid);			// Emv Tlv Get Tid Length
	int		fnEMV_TlvGetLen(BYTE* btpBuf, int* npValLen);
													// Emv Tlv Get Length
	int		fnEMV_TlvStoreVal(int nTid, int nLen, BYTE* btpVal);
													// Emv Tlv Store Value
	int		fnEMV_TlvStoreValFromRecord(BYTE* btpRecord, int nType, int nInLen, int nSource);
													// Emv Tlv Store Value From Record

// EMV Utility
	int		fnEMV_UtilBINtoN(int nVal, BYTE* btpData, int nLen);
													// Emv Utility BIN to N
	int		fnEMV_UtiltoYYYYMMDD(BYTE* btpData, BYTE* btpOut, int nFormat);
													// Emv Utility to YYYYMMDD
	int		fnEMV_UtilGetCNRealLen(BYTE* btpData, int nLen);
													// Emv Utility Get CN Real Length
	int		fnEMV_UtilRightJustify(BYTE* btpData, int nLen, BYTE* btpDest);
													// Emv Utility Right Justify
	int		fnEMV_UtilDecompress(BYTE* btpSource, BYTE* btpDestination, int nCount);
													// Emv Utility Decompress
	int		fnEMV_UtilCompress(BYTE* btpSource, BYTE* btpDestination, int nCount);
													// Emv Utility Compress

// EMV Other Function
	int		fnEMV_UtilGetAPDUSW(BYTE* btpSW12);		// Emv Utility Get APDU SW
	int		fnEMV_UtilGetVersion(BYTE* btpKernelVer);
													// Emv Utility Get Version
	int		fnEMV_SelDescretionaryDataList(EMVst_APPTAGLIST* stpAppTagList);
													// Emv Sel Descretionary Data List
	int		fnEMV_SelSetOnlyOneAID(BYTE btUcMatchUpYN);
													// Emv Sel Set Only One Aid
	int		fnEMV_UtilSetKernelLogPath(BYTE* btpPathName, UINT nPathLen);
													// Emv Utility Set Kernel Log Path

/////////////////////////////////////////////////////////////////////////////
//	PBOC IC CARD UNIT FUNCTION(MCU)
/////////////////////////////////////////////////////////////////////////////
// MCU SET MODE
// MCU GET MODE
// MCU OPERATION MODE
// PBOC Initialize
	int		fnPBC_Initialize();						// Pboc Initialize

// PB0C Application
	int		fnPBC_InitAddApplication(PBCst_APPLIST* stpApp);		
													// Pboc Initialize Add Application
	int		fnPBC_InitGetApplication(int nIndex, PBCst_APPLIST* stpApp);
													// Pboc Initialize Get Applications
	int		fnPBC_InitDelApplication(BYTE* btpAID, int nAidLen);		
													// Pboc Initialize Del Application
	int		fnPBC_SelApplicationSelection(BYTE btSlotIn, DWORD dwTransNoIn, BYTE btTryCntIn, PBCst_APPLIST* stpListOut, int* npAppNumOut, int nReadLogFlag);
													// Pboc Sel Application Selection
	int		fnPBC_SelFinalAppSelection(int nSelIndexIn, int nReadLogFlag);
													// Pboc Sel Final Application Selection

// Pboc Transaction	
	int		fnPBC_TransReadAppData(int nGetTag);	// Pboc Trans Read Application Data
	int		fnPBC_TransOfflineDataAuth();			// Pboc Trans Offline Data Authentification
	int		fnPBC_TransProcess(BYTE* btpIfGoOnline);// Pboc Trans Process
	int		fnPBC_TransCompletion(BYTE btResult, BYTE* btpRspCode, BYTE* btpAuthCode, int nAuthCodeLen, BYTE* btpAuthData, int nAuthDataLen, BYTE* btpScriptData, int nScriptLen);
													// Pboc Trans Completion

// Pboc Electronic Cash Transaction
	int		fnPBC_EcGetBalance(BYTE* btpBcdBalance);// Pboc EC Get Balance 
	int		fnPBC_EcGetIccData(WORD wTag, BYTE* btpDataOut, int* npOutLen);
													// Pboc EC Get Icc Data
	int		fnPBC_EcReadLogRecord(int nRecordNo);	// Pboc EC Read Log Record
	int		fnPBC_EcGetLogItem(WORD wTag, BYTE* btpTagData, int* npTagLen);
													// Pboc EC Get Log Item

// Pboc Tlv Function
	int		fnPBC_TlvGetParam(PBCst_TERM* stpParam);// Pboc Tlv Get Param
	int		fnPBC_TlvSetParam(PBCst_TERM* stpParam);// Pboc Tlv Set Param
	int		fnPBC_TlvGetValue(WORD wTag, BYTE* btpDataOut, int* npDataLen);
													// Pboc Tlv Get Value
	int		fnPBC_TlvStoreValue(WORD wTag, BYTE* btpDataIn, int nDataLen);
													// Pboc Tlv Store Value
	int		fnPBC_TlvGetScriptResult(BYTE* btpResult, int* npRetLen);
													// Pboc Tlv Get Script Result

// Pboc Other Function
	int		fnPBC_UtilGetVersion();					// Pboc Utility Get Version
	int		fnPBC_UtilClearTransLog();				// Pboc Utility Clear Trans Log
	int		fnPBC_InitAddCaPubKey(PBCst_CA_PUBKEY* stpCaPubKey);// Pboc Init Add Ca Pub Key
	int		fnPBC_InitGetCaPubKey(int nIndex, PBCst_CA_PUBKEY* stpCaPubKey);
													// Pboc Init Get Ca Pub Key
	int		fnPBC_InitDelCaPubKey(BYTE btKeyID, BYTE* btpRID);
													// Pboc Init Del Ca Pub Key
	int		fnPBC_InitCheckCaPubKey(BYTE* btpKeyID, BYTE* btpRID);
													// Pboc Init Check Ca Pub Key
	
/////////////////////////////////////////////////////////////////////////////
//	BRM FUNCTION(BRM)
/////////////////////////////////////////////////////////////////////////////
// BRM SET MODE
	int		fnBRM_ClearErrorCode();					// Clear Error Code
	int		fnBRM_ClearDispenseInfo();				// Clear Dispense Information

	int		fnBRM_SetValueOfCash(LPCTSTR szCashValueData);		
													// Set Value Of Cash(Cash Value Data)
	int		fnBRM_SetMinMaxCST(LPCTSTR szMinCntData, LPCTSTR szMaxCntData);
													// Set MinMax Of CST(Min Count Data, Max Count Data)
	int		fnBRM_SetNumberOfCash(LPCTSTR szCashCntData);
													// Set Number Of Cash(Cash Count Data)
	int		fnBRM_SetAvailDeposit(int nDepType);	// Set Avail Deposit(Deposit Type)
	
// BRM GET MODE
	CString	fstrBRM_GetErrorCode();					// Get Error Code
	CString	fstrBRM_GetSensorInfo();				// Get Sensor Information
	int		fnBRM_GetDeviceStatus();				// Get Device Status
	int		fnBRM_CimGetDeviceStatus();				// Cim Get Device Status
	int		fnBRM_CduGetDeviceStatus();				// Cdu Get Device Status
	int		fnBRM_GetShutterStatus();				// Get Shutter Status
	int		fnBRM_CimGetShutterStatus();			// Cim Get Shutter Status
	int		fnBRM_CduGetShutterStatus();			// Cdu Get Shutter Status
	int		fnBRM_GetRetractAction();				// Get Retract Action
	int		fnBRM_CimGetRetractAction();			// Cim Get Retract Action
	int		fnBRM_CduGetRetractAction();			// Cdu Get Retract Action

	int		fnBRM_GetPosition();					// Get Position
	int		fnBRM_GetChangeUnitLock();				// Get Change Unit Lock
	int		fnBRM_GetClerkCSTStatus();				// Get Clerk CST Status
	int		fnBRM_GetRejectCSTStatus();				// Get Reject CST Status
	int		fnBRM_GetRetractCSTStatus();			// Get Retract CST Status
	int		fnBRM_GetDepositCSTStatus(int nCashType);			
													// Get Deposit CST Status(Cash Type)
	int		fnBRM_GetRecycleBoxStatus();			// Get Recycle Box Status
	int		fnBRM_GetAutoloadCondition();			// Get Autoload Condition
	int		fnBRM_GetMaterialInfo();				// Get Material Information
	int		fnBRM_GetAvailDeposit();				// Get Avail Deposit
	int		fnBRM_GetAvailWithdraw();				// Get Avail Withdraw

	int		fnBRM_GetAcceptCountStatus();			// Get Accept Count Status
	CString	fstrBRM_GetCountResult();				// Get Count Result
	CString	fstrBRM_GetRetractResult();				// Get Retract Result
	CString	fstrBRM_GetRefillResult();				// Get Refill Result
	CString	fstrBRM_GetTakeupResult();				// Get Takeup Result

	int		fnBRM_GetNumberOfCST();					// Get Number Of CST
	int		fnBRM_GetCSTType(int nCSTNo);			// Get CST Type(CST No)
	int		fnBRM_GetCSTStatus(int nCSTNo);			// Get CST Status(CST No)
	int		fnBRM_GetValueOfCash(int nCSTNo);		// Get Value Of Cash(CST No)
	int		fnBRM_GetNumberOfCash(int nCSTNo);		// Get Number Of Cash(CST No)
	int		fnBRM_GetNumberOfSetCash(int nCSTNo);	// Get Number Of Set Cash(CST No)
	CString	fstrBRM_GetDispenseOfCST(int nCashAmt);	// Get Dispense Of CST(Cash Amount)
	int		fnBRM_GetNumberOfReject(int nCSTNo);	// Get Number Of Reject(CST No)
	CString	fstrBRM_GetNumberOfRetract();			// Get Number Of Retract()
	CString	fstrBRM_GetNumberOfDeposit(int nCashType);
													// Get Number Of Deposit(Cash Type)

// BRM OPERATION MODE
	int		fnBRM_Initialize();						// Initialize
	int		fnBRM_Deinitialize();					// Deinitialize

	int		fnBRM_OpenShutter();					// Open Shutter
	int		fnBRM_CloseShutter();					// Close Shutter

	int		fnBRM_Ready();							// Ready

	int		fnBRM_Accept(int nWaitSec = K_NO_WAIT);	// Accept(Wait Time)
	int		fnBRM_CancelAcceptCash();				// Cancel Accept : Add OKI-BRM 2005.07.18
	int		fnBRM_ReadyCount();						// Ready Count
	int		fnBRM_Count();							// Count
	int		fnBRM_Stack();							// Stack
	int		fnBRM_Reject(int nWaitSec = K_45_WAIT);	// Reject(Wait Time):Reject All
	int		fnBRM_WaitLift();						// Wait Lift
	int		fnBRM_WaitTaken();						// Wait Taken
	int		fnBRM_CancelWaitTaken();				// Cancel Wait Taken
	int		fnBRM_Retract();						// Retract
	int		fnBRM_Present2(int nWaitSec = K_45_WAIT);			
													// Present2(Wait Time):Reject Part

	int		fnBRM_ReadyDispense();					// Ready Dispense
	int		fnBRM_Dispense(int nCashAmt);			// Dispense(Cash Amount)
	int		fnBRM_Present(int nWaitSec = K_45_WAIT);// Present(Wait Time)

	int		fnBRM_RefillMoney(int nCashCnt);		// Refill Money(Cash Count)
	int		fnBRM_TakeupMoney(int nCashCnt);		// Takeup Money(Cash Count)

/////////////////////////////////////////////////////////////////////////////
//	CDU FUNCTION(CDU)
/////////////////////////////////////////////////////////////////////////////
// CDU SET MODE
	int		fnCDU_ClearErrorCode();					// Clear Error Code
	int		fnCDU_ClearDispenseInfo();				// Clear Dispense Information

	int		fnCDU_SetValueOfCash(LPCTSTR szCashValueData);		
													// Set Value Of Cash(Cash Value Data)
	int		fnCDU_SetMinMaxCST(LPCTSTR szMinCntData, LPCTSTR szMaxCntData);
													// Set MinMax Of CST(Min Count Data, Max Count Data)
	int		fnCDU_SetNumberOfCash(LPCTSTR szCashCntData);
													// Set Number Of Cash(Cash Count Data)
// CDU GET MODE
	CString	fstrCDU_GetErrorCode();					// Get Error Code
	CString	fstrCDU_GetSensorInfo();				// Get Sensor Information
	int		fnCDU_GetDeviceStatus();				// Get Device Status
	int		fnCDU_GetShutterStatus();				// Get Shutter Status
	int		fnCDU_GetRetractAction();				// Get Retract Action

	int		fnCDU_GetPosition();					// Get Position
	int		fnCDU_GetRejectCSTStatus();				// Get Reject CST Status
	int		fnCDU_GetRecycleBoxStatus();			// Get Recycle Box Status
	int		fnCDU_GetMaterialInfo();				// Get Material Information
	int		fnCDU_GetAvailWithdraw();				// Get Avail Withdraw

	int		fnCDU_GetNumberOfCST();					// Get Number Of CST
	int		fnCDU_GetCSTStatus(int nCSTNo);			// Get CST Status(CST No)
	int		fnCDU_GetValueOfCash(int nCSTNo);		// Get Value Of Cash(CST No)
	int		fnCDU_GetNumberOfCash(int nCSTNo);		// Get Number Of Cash(CST No)
	int		fnCDU_GetNumberOfSetCash(int nCSTNo);	// Get Number Of Set Cash(CST No)
	CString	fstrCDU_GetDispenseOfCST(int nCashAmt);	// Get Dispense Of CST(Cash Amount)
	int		fnCDU_GetNumberOfDispense(int nCSTNo);	// Get Number Of Dispense(CST No)
	int		fnCDU_GetNumberOfReject(int nCSTNo);	// Get Number Of Reject(CST No)

// CDU OPERATION MODE
	int		fnCDU_Initialize();						// Initialize
	int		fnCDU_Deinitialize();					// Deinitialize

	int		fnCDU_OpenShutter();					// Open Shutter
	int		fnCDU_CloseShutter();					// Close Shutter

	int		fnCDU_WaitTaken();						// Wait Taken
	int		fnCDU_CancelWaitTaken();				// Cancel Wait Taken
	int		fnCDU_Retract();						// Retract

	int		fnCDU_Dispense(int nCashAmt);			// Dispense(Cash Amount)
	int		fnCDU_Present(int nWaitSec = K_45_WAIT);// Present(Wait Time)

/////////////////////////////////////////////////////////////////////////////
//	PASSBOOK FUNCTION(PBM)
/////////////////////////////////////////////////////////////////////////////
// PBM SET MODE
	int		fnPBM_ClearErrorCode();					// Clear Error Code
	int		fnPBM_ClearRetractCnt();				// Clear Retract Count

// PBM GET MODE
	CString	fstrPBM_GetErrorCode();					// Get Error Code
	CString	fstrPBM_GetSensorInfo();				// Get Sensor Information
	int		fnPBM_GetDeviceStatus();				// Get Device Status

	int		fnPBM_GetMaterialInfo();				// Get Material Information
	
	CString	fstrPBM_GetRetractCnt();				// Get Retract Count
	CString	fstrPBM_GetMsData();					// Get PB Ms Data
	CString	fstrPBM_GetBarData();					// Get PB Bar Data
	CString	fstrPBM_GetLineData();					// Get PB Line Data

// PBM OPERATION MODE
	int		fnPBM_Initialize();						// Initialize
	int		fnPBM_Deinitialize();					// Deinitialize

	int		fnPBM_EntryEnable(int nWaitSec = K_NO_WAIT);	
													// EntryEnable(Wait Time)
	int		fnPBM_EntryDisable();					// Entry Disable
	int		fnPBM_Read();							// PB Ms Read
	int		fnPBM_BarLineRead();					// PB Bar Line Read	
	int		fnPBM_Print(LPCTSTR szPrintData);		// Print(Print Data)
	int		fnPBM_Eject(int nWaitSec = K_45_WAIT);	// Eject(Wait Time)
	int		fnPBM_WaitTaken();						// Wait Taken
	int		fnPBM_CancelWaitTaken();				// Cancel Wait Taken
	int		fnPBM_Retract();						// Retract

/////////////////////////////////////////////////////////////////////////////
//	CAMERA FUNCTION(CMR)
/////////////////////////////////////////////////////////////////////////////
// CAMERA SET MODE
	int		fnCMR_ClearErrorCode();					// Clear Error Code

// CAMERA GET MODE
	CString	fstrCMR_GetErrorCode();					// Get Error Code
	int		fnCMR_GetDeviceStatus();				// Get Device Status

// CAMERA OPERATION MODE
	int		fnCMR_Initialize();						// Initialize
	int		fnCMR_Deinitialize();					// Deinitialize

	int		fnCMR_CaptureFace(LPCTSTR szFileName);	// Capture Face(File Name)
	int		fnCMR_CaptureHand(LPCTSTR szFileName);	// Capture Hand(File Name)

/////////////////////////////////////////////////////////////////////////////
//	AVI FUNCTION(MONITOR2)
/////////////////////////////////////////////////////////////////////////////
// AVI SET MODE

// AVI GET MODE
	int		fnAVI_GetDeviceCount();					// Get Device Count

// AVI OPERATION MODE
	int		fnAVI_Register();						// Register
	int		fnAVI_CustomerServiceBegin();			// Customer Service Begin
	int		fnAVI_CustomerServiceEnd(DWORD dwCustomer);
													// Customer Service End
	int		fnAVI_AddInfo(DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult);
													// Add Info
	int		fnAVI_StartCap(DWORD dwSlotNum, DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult);
													// Start Cap
	int		fnAVI_StartCapWithCapSeconds(DWORD dwSlotNum, DWORD dwSeconds, DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult);
													// Start Cap With Cap Seconds
	int		fnAVI_StopCap(DWORD dwSlotNum);			// Stop Cap
	int		fnAVI_StopCapAfterSeconds(DWORD dwSlotNum, DWORD dwSeconds);
													// Stop Cap After Seconds
	int		fnAVI_SetVideoSize(DWORD dwSlotNum, DWORD dwWidth, DWORD dwHeight);
													// Set Video Size
	int		fnAVI_StartPreRecording(DWORD dwSlotNum, DWORD dwSeconds);
													// Start Pre Recording
	int		fnAVI_RestartAllowed();					// Restart Allowed

/////////////////////////////////////////////////////////////////////////////
//	DOOR FUNCTION(DOR)
/////////////////////////////////////////////////////////////////////////////
// DOOR SET MODE
	int		fnDOR_ClearErrorCode();					// Clear Error Code

// DOOR GET MODE
	CString	fstrDOR_GetErrorCode();					// Get Error Code
	int		fnDOR_GetDeviceStatus();				// Get Device Status

	int		fnDOR_GetDoorStatus();					// Get Door Status

// DOOR OPERATION MODE
	int		fnDOR_Initialize();						// Initialize
	int		fnDOR_Deinitialize();					// Deinitialize

/////////////////////////////////////////////////////////////////////////////
//	LIGHT FUNCTION(LGT)
/////////////////////////////////////////////////////////////////////////////
// LIGHT SET MODE
	int		fnLGT_ClearErrorCode();					// Clear Error Code

	int		fnLGT_SetFlicker(int nIndex, int nValue);					
													// Set Flicker(Index, Value)
	int		fnLGT_SetIndicator(int nValue);			// Set Indicator(Value)

// LIGHT GET MODE
	CString	fstrLGT_GetErrorCode();					// Get Error Code
	int		fnLGT_GetDeviceStatus();				// Get Device Status

// LIGHT OPERATION MODE
	int		fnLGT_Initialize();						// Initialize
	int		fnLGT_Deinitialize();					// Deinitialize

/////////////////////////////////////////////////////////////////////////////
//	SPL FUNCTION(SPL)
/////////////////////////////////////////////////////////////////////////////
// SPL SET MODE
	int		fnSPL_ClearErrorCode();					// Clear Error Code

	int		fnSPL_SetSegment(LPCTSTR szValue);		// Set Segment(Value)
	int		fnSPL_SetLed(LPCTSTR szValue);			// Set Led(Value)
	int		fnSPL_SetRpl(LPCTSTR szValue);			// Set Rpl(Value)
	int		fnSPL_SetInform(LPCTSTR szValue);		// Set Inform(Value)
	int		fnSPL_SetInServiceLED(LPCTSTR szValue);	// Set InService LED(Value)
	int		fnSPL_SetSuperviosrLED(LPCTSTR szValue);// Set Superviosr LED(Value)
	int		fnSPL_SetErrorLED(LPCTSTR szValue);		// Set Error LED(Value)
	int		fnSPL_SetCommunicationsLED(LPCTSTR szValue);		
													// Set Communications LED(Value)
	
// SPL GET MODE
	CString	fstrSPL_GetErrorCode();					// Get Error Code
	int		fnSPL_GetDeviceStatus();				// Get Device Status

	int		fnSPL_GetTestKey();						// Get Test Key
	int		fnSPL_GetLoadSwitch();					// Get Load Switch
	int		fnSPL_GetCallKey();						// Get Call Key

// SPL OPERATION MODE
	int		fnSPL_Initialize();						// Initialize
	int		fnSPL_Deinitialize();					// Deinitialize

/////////////////////////////////////////////////////////////////////////////
//	UPS FUNCTION(UPS)
/////////////////////////////////////////////////////////////////////////////
// UPS SET MODE
	int		fnUPS_ClearErrorCode();					// Clear Error Code

// UPS GET MODE
	CString	fstrUPS_GetErrorCode();					// Get Error Code
	int		fnUPS_GetDeviceStatus();				// Get Device Status

	int		fnUPS_GetPowerStatus();					// Get Power Status

// UPS OPERATION MODE
 	int		fnUPS_Initialize();						// Initialize
	int		fnUPS_Deinitialize();					// Deinitialize

	int		fnUPS_PowerOff(int nWaitSec);			// Power Off(Wait Time)
	int		fnUPS_PowerRestart(int nWaitSec, int nRestartSec);					
													// Power Restart(Wait Time, Restart Time)

/////////////////////////////////////////////////////////////////////////////
//	PIN FUNCTION(PIN)
/////////////////////////////////////////////////////////////////////////////
// PIN SET MODE
	int		fnPIN_ClearErrorCode();					// Clear Error Code

	int		fnPIN_SetMacKeyName(LPCTSTR szKeyName);	// Set MacKey Name
	int		fnPIN_SetPinKeyName(LPCTSTR szKeyName);	// Set PinKey Name

// PIN GET MODE
	CString	fstrPIN_GetErrorCode();					// Get Error Code
	int		fnPIN_GetDeviceStatus();				// Get Device Status

	CString	fstrPIN_GetPinKeyData();				// Get Pin Key Data
	CString	fstrPIN_GetMacingData();				// Get Macing Data

	CString	fnPIN_GetEncryptedData();				// Get Encrypted Data
	CString	fnPIN_GetDecryptedData();				// Get Decrypted Data

// PIN OPERATION MODE
	int		fnPIN_Initialize();						// Initialize
	int		fnPIN_Deinitialize();					// Deinitialize

	int		fnPIN_EntryEnable(int nEnableMode, int nMinKeyCount, int nMaxKeyCount, int bAutoEnd, LPCTSTR szActiveKeys, LPCTSTR szTerminatorKeys, LPCTSTR szCardData, int nTimeout = K_NO_WAIT);
													// Entry Enable(EnableMode, MinKeyCount, MaxKeyCount, AutoEnd, ActiveKeys, TerminatorKeys, CardData, Timeout)
	int		fnPIN_EntryDisable();					// Entry Disable

	int		fnPIN_LoadIV(LPCTSTR szKeyName);		// Load IV(Key Name)
	int		fnPIN_LoadKey(LPCTSTR szKeyName, LPCTSTR szKeyValue);		
													// Load Key(Key Name, Key Value)
	int		fnPIN_MacingData(LPCTSTR szMacData);	// Macing Data(Mac Data)

	int		fnPIN_InitializeEPP();					// Initialize EPP
	int		fnPIN_ImportKey(LPCTSTR szKeyName, LPCTSTR szKeyEncKeyName, LPCTSTR szValue, LPCTSTR szUse = "CRYPT,FUNCTION,MACING,KEYENCKEY");
													// Import Key(szKeyName, szKeyEncKeyName, szValue, szUse)
	int		fnPIN_EncryptECB(LPCTSTR szAlgorithm, LPCTSTR szKeyName, LPCTSTR szKeyEncKeyName, short sPadchar, LPCTSTR szCryptData);
													// Encrypt ECB(szAlgorithm, szKeyName, szKeyEncKeyName, sPadchar, szCryptData)
	int		fnPIN_DecryptECB(LPCTSTR szAlgorithm, LPCTSTR szKeyName, LPCTSTR szKeyEncKeyName, short sPadchar, LPCTSTR szCryptData);
													// Decrypt ECB(szAlgorithm, szKeyName, szKeyEncKeyName, sPadchar, szCryptData)

/////////////////////////////////////////////////////////////////////////////
//	SENSOR FUNCTION(SNS)
/////////////////////////////////////////////////////////////////////////////
// SENSOR SET MODE
	int		fnSNS_ClearErrorCode();					// Clear Error Code
	int		fnSNS_ClearOperatorSwitch();			// Clear Operator Switch

// SENSOR GET MODE
	CString	fstrSNS_GetErrorCode();					// Get Error Code
	int		fnSNS_GetDeviceStatus();				// Get Device Status

	int		fnSNS_GetOperatorSwitch();				// Get Operator Switch
	int		fnSNS_GetInternalTamper();				// Get Internal Tamper

// SENSOR OPERATION MODE
	int		fnSNS_Initialize();						// Initialize
	int		fnSNS_Deinitialize();					// Deinitialize

/////////////////////////////////////////////////////////////////////////////
//	VFD FUNCTION(VFD)
/////////////////////////////////////////////////////////////////////////////
// VFD SET MODE
	int		fnVFD_ClearErrorCode();					// Clear Error Code	
	int		fnVFD_SetBrightnessLevel(int nLevel);	// Set Brightness Level
	int		fnVFD_SetDisplayAtPowerOn(int nSMemIndex, int nEMemIndex, int nScrolled, int nInterval);
													// Set Display At Power On

// VFD GET MODE	
	CString fstrVFD_GetErrorCode();					// Get Error Code
	int		fnVFD_GetDeviceStatus();				// Get Device Status

// VFD OPERATION MODE
	int		fnVFD_Initialize(int nPortNum = 9, int nBaudRate = 38400);
													// Initialize
	int		fnVFD_Deinitialize();					// Deinitialize

	int		fnVFD_InitializeDisplay();				// Initialize Display
	int		fnVFD_ClearDisplay();					// Clear Display
	int		fnVFD_ScrollDisplay(int nShiftNum, int nRepeatNum, int nInterval);
													// Scroll Display
	int		fnVFD_BlinkDisplay(int nPattern, int nNormalTime, int nReverseTime, int nRepeatNumber);
													// Blink Display
	int		fnVFD_CurtainDisplay(int nDirection, int nSpeed, int nPattern);
													// Curtain Display

	int		fnVFD_RealTimeTextDisplay(LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName);	
													// Real Time Text Display
	int		fnVFD_RealTimeBitImageDisplay(LPCTSTR szFileName, int nOffsetX, int nOffsetY);
													// Real Time Bit Image Display
	int		fnVFD_RealTimeTextBitImageDisplay(LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName, LPCTSTR szFileName, int nImgOffsetX, int nImgOffsetY);
													// Real Time Text Bit Image Display

	int		fnVFD_FROMTextSave(int nMemIndex, LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName);
													// FROM Text Save
	int		fnVFD_FROMBitImageSave(int nMemIndex, LPCTSTR szFileName, int nOffsetX, int nOffsetY);
													// FROM Bit Image Save
	int		fnVFD_FROMTextBitImageSave(int nMemIndex, LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBolStyle, int nItalic, LPCTSTR szFontName, LPCTSTR szFileName, int nImgOffsetX, int nImgOffsetY);
													// FROM Text Bit Image Save
	int		fnVFD_FROMDisplay(int nMemIndex);		// FROM Display

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

	int		fnCMN_ProcBackupKalTrace(LPCTSTR szFileName, int nFileSize = 10000000);
													// Kal Trace Backup(File Name, File Size)
	
	int		fnCMN_SaveLineTrace(void* lpData, int len, int SaveType = 0);
	                                                // SaveType:0:String,1:Byte

/////////////////////////////////////////////////////////////////////////////
//	DVR FUNCTION(DVR)
/////////////////////////////////////////////////////////////////////////////
// DVR SET MODE
// DVR GET MODE
	CString fstrDVR_GetStatus();					// Get Status

// DVR OPERATION MODE
	int		fnDVR_Initialize();						// Initialize
	int		fnDVR_Deinitialize();					// Deinitialize
	int 	fnDVR_SendData(LPCTSTR szSendData, int nSendDelayTime);		
													// Send Data(Send Data, Send Delay Time)

/////////////////////////////////////////////////////////////////////////////
//	ETC FUNCTION(SECURITY)
/////////////////////////////////////////////////////////////////////////////
// ETC SET MODE
// ETC GET MODE
	CString fstrETC_GetStatus();					// Get Status

// ETC OPERATION MODE
	int		fnETC_Initialize();						// Initialize
	int		fnETC_Deinitialize();					// Deinitialize
	int 	fnETC_SendData(LPCTSTR szSendData, int nSendDelayTime);		
													// Send Data(Send Data, Send Delay Time)

/////////////////////////////////////////////////////////////////////////////
//	ETC FUNCTION(DVR)
/////////////////////////////////////////////////////////////////////////////
// ETC SET MODE
	int		fnETC_DVRClearErrorCode();				// DVR Clear Error Code

// ETC GET MODE
	CString	fstrETC_DVRGetErrorCode();				// DVR Get Error Code
	int		fnETC_DVRGetDeviceStatus();				// DVR Get Device Status

	int		fnETC_DVRGetUseStatus();				// DVR Get Use Status

// ETC OPERATION MODE
	int		fnETC_DVRInitialize();					// DVR Initialize
	int		fnETC_DVRInitializeIP(LPCTSTR szIpAddress, int nPortNumber);
													// DVR InitializeIP
	int		fnETC_DVRDeinitialize();				// DVR Deinitialize
	int 	fnETC_DVRSendData(LPCTSTR szSendData);	// DVR Send Data(Send Data)

/////////////////////////////////////////////////////////////////////////////
//	RIC FUNCTION
/////////////////////////////////////////////////////////////////////////////
// RIC SET MODE
	int		fnRIC_ClearErrorCode();					// RIC Clear Error Code

// RIC GET MODE
	CString	fstrRIC_GetErrorCode();					// RIC Get Error Code
	int		fnRIC_GetDeviceStatus();				// RIC Get Device Status
	int		fnRIC_GetDataStatus();					// RIC Get Data Status
	CString fstrRIC_GetIcCardSerialNo();			// RIC Get Ic Card Serial No

// RIC OPERATION MODE
	int		fnRIC_Initialize();						// RIC Initialize
	int		fnRIC_Deinitialize();					// RIC Deinitialize
	int		fnRIC_RfReset();						// RIC Rf Reset
	int		fnRIC_DetectCard();						// RIC Detect Card
	int 	fnRIC_CpuReset();						// RIC Cpu Reset
	CString	fstrRIC_SelectFile();					// RIC Select File
	int		fnRIC_VerifyPIN(LPCTSTR szPinData);		// RIC Verify PIN
	CString	fstrRIC_ReadBalance();					// RIC Read Balance
	CString	fstrRIC_InitializeForLoad(LPCTSTR szAmount, LPCTSTR szTerminalID, int nKeyIndex);
													// RIC Initialize For Load
	CString	fstrRIC_LoadEP(LPCTSTR szDate, LPCTSTR szTime, LPCTSTR szMac2);
													// RIC Load EP
	CString	fstrRIC_GetRejectCode();				// RIC Get Reject Code
	int		fnRIC_EntryEnable();					// RIC Entry Enable
	int		fnRIC_EntryDisable();					// RIC Entry Disable
	CString fstrRIC_CheckDeviceAction(int nCheckTime);
													// RIC Check Device Action

/////////////////////////////////////////////////////////////////////////////
//	IRD FUNCTION
/////////////////////////////////////////////////////////////////////////////
// IRD SET MODE
	int		fnIRD_ClearErrorCode();					// IRD Clear Error Code

// IRD GET MODE
	CString	fstrIRD_GetErrorCode();					// IRD Get Error Code
	int		fnIRD_GetDeviceStatus();				// IRD Get Device Status
	int		fnIRD_GetDataStatus();					// IRD Get Data Status
	CString	fstrIRD_GetData();						// IRD Get Data

// IRD OPERATION MODE
	int		fnIRD_Initialize();						// IRD Initialize
	int		fnIRD_Deinitialize();					// IRD Deinitialize
	int		fnIRD_Read();							// IRD Read
	int		fnIRD_EntryEnable();					// IRD Entry Enable
	int		fnIRD_EntryDisable();					// IRD Entry Disable
	CString fstrIRD_CheckDeviceAction(int nCheckTime);
													// IRD Check Device Action

/////////////////////////////////////////////////////////////////////////////
//	NETWORK FUNCTION(NET)
/////////////////////////////////////////////////////////////////////////////
// NET SET MODE
	int		fnNET_ClearErrorCode();					// Clear Error Code

// NET GET MODE
	CString	fstrNET_GetErrorCode();					// Get Error Code
	int		fnNET_GetDeviceStatus();				// Get Device Status
	int		fnNET_GetDeviceStatus2();				// Get Device Status2
	int		fnNET_GetDeviceStatus3();				// Get Device Status3

// NET OPERATION MODE
	int		fnNET_Initialize(int nOpenSec, int nAsyncFlag = FALSE);			
													// Initialize
	int		fnNET_InitializeIP(LPCTSTR szIpAddress, int nPortNumber, int nOpenSec, int nAsyncFlag = FALSE);
													// InitializeIP
	int		fnNET_Deinitialize(int nCloseSec, int nAsyncFlag = FALSE);		
													// Deinitialize
	int 	fnNET_SendData(LPCTSTR szSendData, int nSendSec);
													// Send Data(Send Data, Wait Time)
	int		fnNET_RecvData(CString& strRecvData, int nRecvSec);			
													// Recv Data(Recv Data, Wait Time)

	int		fnNET_Initialize2(int nOpenSec, int nAsyncFlag = FALSE);		
													// Initialize2
	int		fnNET_Deinitialize2(int nCloseSec, int nAsyncFlag = FALSE);		
													// Deinitialize2
	int 	fnNET_SendData2(LPCTSTR szSendData, int nSendSec);
													// Send Data2(Send Data, Wait Time)

	int		fnNET_Initialize3(int nOpenSec, int nAsyncFlag = FALSE);
													// Initialize3
	int		fnNET_Deinitialize3(int nCloseSec, int nAsyncFlag = FALSE);
													// Deinitialize3
	int 	fnNET_SendData3(LPCTSTR szSendData, int nSendSec);
													// Send Data3(Send Data, Wait Time)
	int		fnNET_RecvData3(CString& strRecvData, int nRecvSec);			
													// Recv Data3(Recv Data, Wait Time)

/////////////////////////////////////////////////////////////////////////////
//	NMS FUNCTION(NMS)
/////////////////////////////////////////////////////////////////////////////
// NMS SET MODE
	int		fnNMS_ClearErrorCode();					// Clear Error Code

// NMS GET MODE
	CString	fstrNMS_GetErrorCode();					// Get Error Code
	int		fnNMS_GetDeviceStatus();				// Get Device Status

	int		fnNMS_GetUseStatus();					// Get Use Status

// NMS OPERATION MODE
	int		fnNMS_Initialize();						// Initialize
	int		fnNMS_InitializeIP(LPCTSTR szIpAddress, int nPortNumber);
													// InitializeIP
	int		fnNMS_Deinitialize();					// Deinitialize
	int 	fnNMS_SendData(LPCTSTR szSendData);		// Send Data(Send Data)

/////////////////////////////////////////////////////////////////////////////
//	SCR FUNCTION(SCR)
/////////////////////////////////////////////////////////////////////////////
// SCR SET MODE
	int		fnSCR_ClearErrorCode();					// Clear Error Code
	int		fnSCR_ClearKeyData();					// Clear Key Data
	int		fnSCR_SetScreenData(LPCTSTR szSetScreenDataName, LPCTSTR szSetScreenDataValue);
													// Set Screen Data(Screen Data Name, Screen Data Value)
	int		fnSCR_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue);
													// Set Display Data(Display Data Name, Display Data Value)
	int		fnSCR_SetCurrentLangMode(int nLangMode = CHN_MODE);				
													// Set Current Language Mode

// SCR GET MODE
	CString	fstrSCR_GetErrorCode();					// Get Error Code
	int		fnSCR_GetDeviceStatus();				// Get Device Status

	CString	fstrSCR_GetKeyData();					// Get Key Data
	CString	fstrSCR_GetKeyString(int nInitFlag = FALSE);	
													// Get Key String(Init Flag)
	CString	fstrSCR_GetCheckScreen(int nScreenNo, int nLangMode = CHN_MODE);	
													// Get Check Screen(Screen No, Language Mode)
	int		fnSCR_GetCurrentScreenNo();				// Get Current Screen No
	int		fnSCR_GetCurrentLangMode();				// Get Current Language Mode
	
	CString	fstrSCR_GetKeyNumeric(int nRetLength = 0, int nInitFlag = FALSE);	
													// Get Key Numeric(Ret Length:0=Real Length)

// SCR OPERATION MODE
	int		fnSCR_Initialize();						// Initialize
	int		fnSCR_Deinitialize();					// Deinitialize

	int 	fnSCR_DisplayScreen(int nScreenNo, int nDisplaySec = K_NO_WAIT, int nPinInputMode = PIN_DISABLE_MODE, LPCTSTR szCardData = "", int nPinPassWordMin = 4, int nPinPassWordMax = 6, int nPinPassWordAuto = FALSE, LPCTSTR szPinPassWordTerm = "ENTER,CANCEL");
													// Display Screen(Screen No, Input Time, Pin Input Mode, Card Data)
	int 	fnSCR_DisplayUpdate(int nScreenNo, int nDisplaySec = K_NO_WAIT, int nPinInputMode = PIN_DISABLE_MODE, LPCTSTR szCardData = "", int nPinPassWordMin = 4, int nPinPassWordMax = 6, int nPinPassWordAuto = FALSE, LPCTSTR szPinPassWordTerm = "ENTER,CANCEL");
													// Display Update(Screen No, Input Time, Pin Input Mode, Card Data)
	int 	fnSCR_DisplayMessage();					// Display Message
	int		fnSCR_DisplayImage(int nImageNo, int nOnOffFlag);
													// Display Image(Image No, OnOff Flag)
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
	int		fnSCR_DisplayNumeric(int nStringNo, int nNumeric);
													// Display Numeric(String No, Numeric)
	int		fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, int nInLength, char cMarkChar = ' ', int nIsMinus = FALSE);
													// Display Amount(String No, Amount, Length, Mark, IsMinus)
	int		fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, char cMarkChar = ' ', int nIsMinus = FALSE);
													// Display Amount(String No, Amount, Mark, IsMinus)
	int		fnSCR_DisplayAmount(int nStringNo, int nAmount,	char cMarkChar = ' ', int nIsMinus = FALSE);
													// Display Amount(String No, Amount, Mark, IsMinus)

/////////////////////////////////////////////////////////////////////////////
//	BASIC FUNCTION(APL)
/////////////////////////////////////////////////////////////////////////////
// APL SET MODE
// APL GET MODE
// APL OPERATION MODE
	int		fnAPL_InitializeDS();					// Initialize DS(Variable)

	int		fnAPL_SetCashInfo(int nCashMaxOutAmt, int nUserMaxOutAmt);	
													// Set Cash Information
	int		fnAPL_GetCashMaxInfo(int nTranProc);	// Get Cash Max Information

	int		fnAPL_CheckRebootDevice();				// Check Reboot Device
	int		fnAPL_LoadDevice();						// Load Device
	int		fnAPL_UnLoadDevice();					// Unload Device

	int		fnAPL_GetDefineDevice(int nDevId);		// Get Define Device
	int		fnAPL_GetYesDevice(int nDevId);			// Get Yes Device
	int		fnAPL_GetNoDevice(int nDevId);			// Get No Device
	int		fnAPL_GetDownDevice(int nDevId);		// Get Down Device
	int		fnAPL_GetAvailDevice(int nDevId);		// Get Avail Device
	int		fnAPL_GetAvailErrorDevice(int nDevId);	// Get Avail Error Device
	int		fnAPL_GetErrorDevice(int nDevId);		// Get Error Device
	int		fnAPL_GetDownErrorDevice(int nDevId);	// Get Down Error Device
	int		fnAPL_GetUseDevice(int nDevId);			// Get Use Device
	int		fnAPL_GetAutoOffBankDevice(int nDevId);	// Get Auto Off Bank Device

	int		fnAPL_SetAutoOffBankDevice(int nDevId = DEV_NONE);	
													// Set Auto Off Bank Device

	int 	fnAPL_GetAvailTrans();					// Get Avail Transaction
	int 	fnAPL_GetAvailWithdraw();				// Get Avail Withdraw
	int 	fnAPL_GetAvailDeposit();				// Get Avail Deposit
	int 	fnAPL_GetAvailPassbook();				// Get Avail Passbook
	int 	fnAPL_GetClerkTransOnOff();				// Get Clerk Transaction OnOff

	int		fnAPL_SetSensorInfo();					// Set Sensor Information
	int		fnAPL_SetDate(LPCTSTR szDate);			// Set Date
	int		fnAPL_ClearSerialNo();					// Clear Serial No
	int 	fnAPL_AddSerialNo(int nSerialLength = 8);					
													// Add Serial No
	int 	fnAPL_SetSerialNo(LPCTSTR szSerialNo, int nSerialLength = 8);	
													// Set Serial No					
	int 	fnAPL_SetProcCount(char chProcCount);	// Set Process Count
	int 	fnAPL_SetOnTransaction(int nTransDoing);// Set On Transaction
	int		fnAPL_SetTranStart();					// Set Tranaction Start : Reduce Transaction Start Time(fnAPL_SetOnTransaction(ON)+fnAPL_SetProcCount(1)) : 2003.11.21
	int 	fnAPL_ClearDeviceExecOffInfo(int nInitFlag = FALSE);			
													// Clear Device Execute Off Information
	int		fnAPL_AddDeviceExecOffInfo(LPCTSTR szHsErrorCode);
													// Add Device Execute Off Information
	int		fnAPL_AddDeviceRetractCnt(int nDevId);	// Add Device Retract Count
	int		fnAPL_ClearStatistics();				// Clear Statistics
	int		fnAPL_AddTransactionStatistics(int nTranProc, LPCTSTR szAmount, int nAmountLength);	
													// Add Transaction Statistics
	int		fnAPL_AddErrorStatistics(LPCTSTR szErrorCode);	
													// Add Error Statistics

	int		fnAPL_SetClerkPowerOff();				// Set Clerk Power Off
	int		fnAPL_CheckPowerStatus(int nRealFlag = FALSE);				
													// Check Power Status
	int 	fnAPL_CheckPowerOffError();				// Check Power Off Error

	int		fnAPL_ClearError();						// Clear Error
	int 	fnAPL_CheckError();						// Check Error
	int		fnAPL_CheckErrorCodeValid(LPCTSTR szErrorCode);		
													// Check ErrorCode Valid
	int 	fnAPL_StackError(LPCTSTR szErrorCode, LPCTSTR szErrorString, int nDevId = DEV_NONE);
													// Stack Error

	int 	fnAPL_CheckHostLine();					// Check Host Line
	int		fnAPL_CheckHostOpen();					// Check Host Open
	int		fnAPL_CloseLine();						// Close Line
	int		fnAPL_CloseHost();						// Close Host

	int		fnAPL_InitializeDevice(int nDevId, int nInitFlag = FALSE);
													// Initialize Device
	int		fnAPL_InitializeDeviceMcu(int nDevId, int nInitFlag = FALSE);
													// Initialize Device Mcu
	int		fnAPL_CheckDevice();					// Check Device
	int		fnAPL_CheckRetractDevice(int nDevId, int nInitFlag = FALSE);				
													// Check Retract Device
	int		fnAPL_CheckDeviceAction(int nDevId);	// Check Device Action
	int		fnAPL_CheckDeviceActionAutoOff(int nDevId);	
													// Check Device Action AutoOff
	int		fnAPL_CheckMaterial(int nDevId = DEV_MAIN);		
													// Check Material
	int		fnAPL_ClearRetractCnt(int nDevId = DEV_MAIN);		
													// Clear Retract Count
	int		fnAPL_RetractMaterial(int nDevId);		// Retract Material
	int		fnAPL_AutoOffDevice(int nDevId = DEV_MAIN);		
													// Auto Off Device
	int		fnAPL_AutoRecoverCheck(int nDevId = DEV_MAIN);		
													// Auto Recover Check
	int		fnAPL_AutoRecoverDevice(int nDevId = DEV_MAIN);	
													// Auto Recover Device
	int		fnAPL_AutoRecoverDeviceMcu(int nDevId = DEV_MAIN);	
													// Auto Recover Mcu
	
	int 	fnAPL_DisplayNormalSegment();			// Display Normal Segment
	int 	fnAPL_DisplayErrorSegment(char chProcCount, LPCTSTR szErrorCode);
													// Display Error Segment
	int 	fnAPL_DisplayProcCountSegment();		// Display ProcCount Segment
	int 	fnAPL_DisplayInformationLed(int nInitFlag = FALSE);			
													// Display Information Led
	int		fnAPL_DisplayInformationIndicator(int nInitFlag = FALSE);			
													// Display Information Indicator
	int 	fnAPL_SendInformationRpl(int nInitFlag = FALSE);			
													// Send Information Rpl
	int 	fnAPL_SendInformationInform(int nInitFlag = FALSE);			
													// Send Information Inform
	
	int		fnAPL_DisplayPrintError(int nInitFlag = FALSE);
													// Display & Print Error
	int 	fnAPL_PrintError(int nDevId);			// Print Error

	int 	fnAPL_CheckPrintDevicePosition();		// Check Print Device Position
	int 	fnAPL_CheckPrintCshPosition();			// Check Print Csh Position
	int		fnAPL_CheckPrintRearDoorStatus();		// Check Print RearDoor Status
	int 	fnAPL_PrintPosition(LPCTSTR szPrintData);
													// Print Position
	
	int		fnAPL_PrintCashMoveInfo(int nMoveFlag, int nRealMove, int nHundredRJT, int nFiftyRJT, int nTwentyRJT, int nTenRJT, int nUnknownRJT);
													// Print Cash Move Information
	int		fnAPL_PrintJnlInfo(LPCTSTR szJnlInfoData, int nWaitFlag = FALSE);	
													// Print Journal Information
    int     fnAPL_AgentLineCount();   
	int     fnAPL_AgentIdxPrintJnlInfo(LPCTSTR szJnlInfoData, int nWaitFlag = FALSE);	
	int		fnAPL_PrintJnlInfoDirect(LPCTSTR szJnlInfoData, int nWaitFlag = FALSE);	
													// Print Journal Information Direct

	int 	fnAPL_AutoRefillCsh(int nCshRefillMode);// Auto Refill Csh
	int 	fnAPL_AutoResetCsh();					// Auto Reset Csh
	int 	fnAPL_AutoReadyCsh();					// Auto Ready Csh

	int		fnAPL_ResetDevice(int nDevId, int nInitFlag = FALSE);				
													// Reset Device
	int		fnAPL_UnUsed();							// Un Used
	int 	fnAPL_DeviceEnDisable(int nDevId, int nAction, int nPinInputMode = PIN_DISABLE_MODE, LPCTSTR szCardData = "", int nPinPassWordMin = 4, int nPinPassWordMax = 6, int nPinPassWordAuto = FALSE, LPCTSTR szPinPassWordTerm = "ENTER,CANCEL", int nWaitFlag = FALSE);
													// Device EnDisable
	int 	fnAPL_DeviceSetFlicker(int nDevId, int nAction);
													// Device Set Flicker
	int 	fnAPL_GetDeviceEvent(int nDevId, int nEventKind = EVENT_IN);		
													// Get Device Event

	int		fnAPL_EjrSpoolPrintStart(int nDevId, EjrTbl* pEjrData = NULL);
													// Elec Journal Spool Print Start
	int		fnAPL_EjrSpoolData(int nDevId, int nLine, int nSize, LPCTSTR szSpoolData);
													// Elec Journal Spool Data
	int		fnAPL_EjrSpoolEnd(int nDevId, int nWaitFlag = FALSE);			
													// Elec Journal Spool End & Save & Print
	int		fnAPL_EjrSpoolEnd2(int nDevId, char cPrintChar, int nWaitFlag = FALSE);			
													// Elec Journal Spool End & Save & Print 2		// djp 20090213
	int		fnAPL_EjrDirectPrint(int nDevId, int nWaitFlag = FALSE);
													// Elec Journal Direct Print
	int		fnAPL_EjrAddSerialNo();					// Elec Journal Add Serial No
	int		fnAPL_EjrSave();						// Elec Journal Save
	int		fnAPL_EjrSaveHistory();					// Elec Journal Save History
	int		fnAPL_EjrSaveHistoryTemp();				// Elec Journal Save History Temp
	int		fnAPL_EjrCheckFileSize();				// Elec Journal Check File Size

	int		fnAPL_CaptureFileClear(int nAddSerialFlag = FALSE, int nSerialLength = 6);
													// Capture File Clear
	int		fnAPL_CaptureFaceSave(int nIndex, int nFirstFlag = FALSE, int nAddSerialFlag = FALSE, int nSerialLength = 6);
													// Capture Face Save
	int		fnAPL_CaptureHandSave(int nIndex, int nFirstFlag = FALSE, int nAddSerialFlag = FALSE, int nSerialLength = 6);
													// Capture Hand Save

	int		fnAPL_ClerkInitModeSet();				// Clerk Initialize Mode Set
	int		fnAPL_ClerkInformation(int nInitFlag = FALSE, int nOnlyHeaderUpdateFlag = FALSE);
													// Clerk Information
	CString	fstrAPL_ClerkInformationMake(int nIndex);	
													// Clerk Information Make
	int		fnAPL_ClerkExecCheck();					// Clerk Exec Check

	int		fnAPL_IcSendApdu(BYTE btPrtCode, BYTE* btpApdu, UINT nApduLen, BYTE* btpResp, int* npRespLen);
													// Ic Send Apdu
	int		fnAPL_CashRetract(int nRetractFlag);	// Tran Retract : 2011.01.04

/////////////////////////////////////////////////////////////////////////////
//	MAIN FUNCTION(APM)
/////////////////////////////////////////////////////////////////////////////
// APM SET MODE
// APM GET MODE
// APM OPERATION MODE
	int		fnAPM_AtmModeCtrl();					// Atm Mode Ctrl
	int		fnAPM_AtmHostCtrl();					// Atm Host Control
	int		fnAPM_ClerkProc();						// Clerk Procedure

/////////////////////////////////////////////////////////////////////////////
//	THREAD
/////////////////////////////////////////////////////////////////////////////
// APT SET MODE
// APT GET MODE
// APT OPERATION MODE
	int		fnAPT_VfdControlThread();				// Vfd Control Thread

/////////////////////////////////////////////////////////////////////////////
//	CASH FUNCTION ROUTING(CASH)
/////////////////////////////////////////////////////////////////////////////
// CASH SET MODE
	int		fnCSH_ClearErrorCode();					// Clear Error Code
	int		fnCSH_ClearDispenseInfo();				// Clear Dispense Information

	int		fnCSH_SetValueOfCash(LPCTSTR szCashValueData);		
													// Set Value Of Cash(Cash Value Data)
	int		fnCSH_SetMinMaxCST(LPCTSTR szMinCntData, LPCTSTR szMaxCntData);
													// Set MinMax Of CST(Min Count Data, Max Count Data)
	int		fnCSH_SetNumberOfCash(LPCTSTR szCashCntData, LPCTSTR szTotalCashCntData);		
													// Set Number Of Cash(Cash Count Data, Total Cash Count Data)
	int		fnCSH_SetNumberOfDispenseCash(LPCTSTR szDispCntData);
													// Set Number Of Dispense Cash(Dispense Count Data)
	int		fnCSH_SetAvailDeposit(int nDepType);	// Set Avail Deposit(Deposit Type)

// CASH GET MODE
	CString	fstrCSH_GetErrorCode();					// Get Error Code
	CString	fstrCSH_GetSensorInfo();				// Get Sensor Information
	int		fnCSH_GetDeviceStatus();				// Get Device Status
	int		fnCSH_BrmCimGetDeviceStatus();			// Brm Cim Get Device Status
	int		fnCSH_BrmCduGetDeviceStatus();			// Brm Cdu Get Device Status
	int		fnCSH_GetShutterStatus();				// Get Shutter Status
	int		fnCSH_BrmCimGetShutterStatus();			// Brm Cim Get Shutter Status
	int		fnCSH_BrmCduGetShutterStatus();			// Brm Cdu Get Shutter Status
	int		fnCSH_GetRetractAction();				// Get Retract Action
	int		fnCSH_BrmCimGetRetractAction();			// Brm Cim Get Retract Action
	int		fnCSH_BrmCduGetRetractAction();			// Brm Cdu Get Retract Action

	int		fnCSH_GetPosition();					// Get Position
	int		fnCSH_GetChangeUnitLock();				// Get Change Unit Lock
	int		fnCSH_GetClerkCSTStatus();				// Get Clerk CST Status
	int		fnCSH_GetRejectCSTStatus();				// Get Reject CST Status
	int		fnCSH_GetRetractCSTStatus();			// Get Retract CST Status
	int		fnCSH_GetDepositCSTStatus(int nCashType);			
													// Get Deposit CST Status(Cash Type)
	int		fnCSH_GetRecycleBoxStatus();			// Get Recycle Box Status
	int		fnCSH_GetAutoloadCondition();			// Get Autoload Condition
	int		fnCSH_GetMaterialInfo();				// Get Material Information
	int		fnCSH_GetAvailDeposit();				// Get Avail Deposit
	int		fnCSH_GetAvailWithdraw();				// Get Avail Withdraw

	int		fnCSH_GetAcceptCountStatus();			// Get Accept Count Status
	CString	fstrCSH_GetCountResult();				// Get Count Result
	CString	fstrCSH_GetRetractResult();				// Get Retract Result
	CString	fstrCSH_GetRefillResult();				// Get Refill Result
	CString	fstrCSH_GetTakeupResult();				// Get Takeup Result

	int		fnCSH_GetNumberOfCST();					// Get Number Of CST
	int		fnCSH_GetCSTType(int nCSTNo);			// Get CST Type(CST No)
	int		fnCSH_GetAmountOfCST();					// Get Amount Of CST
	int		fnCSH_GetCSTStatus(int nCSTNo);			// Get CST Status(CST No)
	int		fnCSH_GetValueOfCash(int nCSTNo);		// Get Value Of Cash(CST No)
	int		fnCSH_GetNumberOfCash(int nCSTNo);		// Get Number Of Cash(CST No)
	int		fnCSH_GetAmountOfCash(int nCSTNo);		// Get Amount Of Cash(CST No)
	int		fnCSH_GetNumberOfSetCash(int nCSTNo);	// Get Number Of Set Cash(CST No)
	CString	fstrCSH_GetDispenseOfCST(int nCashAmt);	// Get Dispense Of CST(Cash Amount)
	int		fnCSH_GetNumberOfDispense(int nCSTNo);	// Get Number Of Dispense(CST No)
	int		fnCSH_GetNumberOfReject(int nCSTNo);	// Get Number Of Reject(CST No)
	CString	fstrCSH_GetNumberOfRetract();			// Get Number Of Retract()
	CString	fstrCSH_GetNumberOfDeposit(int nCashType);
													// Get Number Of Deposit(Cash Type)
	int		fnCSH_GetNumberOfTotalSetCash(int nCSTNo);	
													// Get Number Of Total Set Cash(CST No)
	int		fnCSH_GetNumberOfDispenseCash(int nCSTNo);	
													// Get Number Of Dispense Cash(CST No)
		
// CASH OPERATION MODE
	int		fnCSH_Initialize();						// Initialize
	int		fnCSH_Deinitialize();					// Deinitialize

	int		fnCSH_OpenShutter();					// Open Shutter
	int		fnCSH_CloseShutter();					// Close Shutter

	int		fnCSH_Ready();							// Ready

	int		fnCSH_Accept(int nWaitSec = K_NO_WAIT);	// Accept(Wait Time)
	int		fnCSH_CancelAcceptCash();				// Cancel Accept : Add OKI-BRM 2005.07.18
	int		fnCSH_ReadyCount();						// Ready Count
	int		fnCSH_Count();							// Count
	int		fnCSH_Stack();							// Stack
	int		fnCSH_Reject(int nWaitSec = K_45_WAIT);	// Reject(Wait Time):Reject All
	int		fnCSH_WaitLift();						// Wait Lift
	int		fnCSH_WaitTaken();						// Wait Taken
	int		fnCSH_CancelWaitTaken();				// Cancel Wait Taken
	int		fnCSH_Retract();						// Retract
	int		fnCSH_Present2(int nWaitSec = K_45_WAIT);			
													// Present2(Wait Time):Reject Part

	int		fnCSH_ReadyDispense();					// Ready Dispense
	int		fnCSH_Dispense(int nCashAmt);			// Dispense(Cash Amount)
	int		fnCSH_Present(int nWaitSec = K_45_WAIT);// Present(Wait Time)

	int		fnCSH_RefillMoney(int nCashCnt);		// Refill Money(Cash Count)
	int		fnCSH_TakeupMoney(int nCashCnt);		// Takeup Money(Cash Count)
	
	void    fnAPL_GetNetMAC(char *pRetVal);         // Agent Get NetMac
	int     fnAPL_AgentRestWaitResult();            // Agent Wait Rest Result  0:失败 1:成功  
    int     fnAPL_ShareResetDeviceResult(int bAgentRestReturnFlag);   // Agent share data to device   
	
	// 2008.05.09
	BOOL	fnAPP_OpenADComm(const char* pPort, int nBaudRate=57600, int nParity=NOPARITY, int nByteSize=8, int nStopBits=ONESTOPBIT);
	BOOL	fnAPP_CloseADComm();
	int		fnAPP_WriteADComm(void* pData, int nLength);
/////////////////////////////////////////////////////////////////////////////
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDevCmn)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CDevCmn)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//	THREAD
/////////////////////////////////////////////////////////////////////////////
	UINT	fnAPT_EjrSendThread(LPVOID pParam);		// Elec Journal Send Thread
	UINT	fnAPT_FtpSendThread(LPVOID pParam);		// Ftp Send Thread

/////////////////////////////////////////////////////////////////////////////
//	CALLBACK
/////////////////////////////////////////////////////////////////////////////
	void	fnAPC_SetOwner(CDevCmn* pDevice);		// Set Owner
	int		CALLBACK fnAPC_IcSendApdu(BYTE btPrtCode, BYTE* btpApdu, UINT nApduLen, BYTE* btpResp, int* npRespLen);							
													// Ic Send Apdu

/////////////////////////////////////////////////////////////////////////////
//	PBOC CALLBACK
/////////////////////////////////////////////////////////////////////////////
	int		fnAPC_PbcAccTypeSelect(BYTE* btpAccountType);
													// Pboc Account Type Select
	int		fnAPC_PbcInputAmt(DWORD* dwpAuthAmt, DWORD* dwpCashBackAmt);
													// Pboc Input Amount : 2010.10.20
	int		fnAPC_PbcGetDateTime(BYTE* btpDateTime);// Pboc Get Date Time
	int		fnAPC_PbcGetHolderPwd(int nTryFlag, int nRemainCnt, BYTE* nPin);
													// Pboc Get Holder Password
	int		fnAPC_PbcGetUnknownTLV(WORD wTag, BYTE* btpData, int nDataLen);
													// Pboc Get Unknown TLV
	void	fnAPC_PbcIoCtrl(BYTE btIoName, BYTE* btpIoValue);
													// Pboc Io Ctrl
	void	fnAPC_PbcAdviceProc();					// Pboc Advice Proc
	int		fnAPC_PbcVerifyCert();					// Pboc Verify Cert
	int		fnAPC_PbcReferProc();					// Pboc Refer Proc
	void	fnAPC_PbcDispVerifyPinResult(BYTE btResult);
													// Pboc Display Verify Pin Result
	void	fnAPC_PbcReadIfdSN(BYTE* btpIfdSn);		// Pboc Read Ifd SN
	int		fnAPC_PbcIcSendRecvApdu(BYTE btSlot, PBCst_APDU_SEND* stpApduSend, PBCst_APDU_RESP* stpApduResp);
													// Pboc Ic Send Recv Apdu
	int		fnAPC_PbcRegistFunc();					// Pboc Regist Function

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVCMN_H__C1CA462D_218C_4BA9_88BA_DBC13B14F86A__INCLUDED_)
