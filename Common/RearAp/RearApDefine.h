/////////////////////////////////////////////////////////////////////////////
#if !defined(_RearApDefind_H_)
#define _RearApDefind_H_

/////////////////////////////////////////////////////////////////////////////
//	Reg Define
/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_DEVERROR				_T("SOFTWARE\\ATM\\ERRORCODE")
#define	_REGKEY_ADMINPIN				_T("SOFTWARE\\ATM\\APP\\ADMINPIN")
#define	_REGKEY_POSTADMINPIN			_T("SOFTWARE\\ATM\\APP\\POSTADMINPIN")
#define	_REGKEY_POSTTECHADMINPIN		_T("SOFTWARE\\ATM\\APP\\POSTTECHADMINPIN")
#define	_REGKEY_ADDCASHMODE				_T("SOFTWARE\\ATM\\APP\\ADDCASHMODE")
#define _MONITOR_INI_DAT				"C:\\T1ATM\\APP\\EXERELE\\MONITOR.INI"

/////////////////////////////////////////////////////////////////////////////
//	Language Mode
/////////////////////////////////////////////////////////////////////////////
#define CHN_MODE			0						// CHINA
#define ENG_MODE			1						// ENGLISH
#define KOR_MODE			2						// KOREA
#define JPN_MODE			3						// JAPAN

/////////////////////////////////////////////////////////////////////////////
//	Scr Dir Define
/////////////////////////////////////////////////////////////////////////////
#define _SCR_DIR			"../SWF/"

/////////////////////////////////////////////////////////////////////////////
//	Pin Input Mode
/////////////////////////////////////////////////////////////////////////////
#define	PIN_DISABLE_MODE	1
#define	PIN_MENU_MODE		2
#define	PIN_NUM_AMT_MODE	3
#define	PIN_NUMERIC_MODE	4
#define	PIN_NUM_ALL_MODE	5
#define	PIN_PASSWORD_MODE	6
#define	PIN_PASSWORD_MODE2	7
#define	PIN_PASSWORD_MODE3	8
#define	PIN_PASSWORD_MODE4	9
#define	PIN_PASSWORD_MODE5	10
#define	PIN_PASSWORD_MODE6	11
#define	PIN_PASSWORD_MODE7	12
#define	PIN_AMOUNT_MODE		13
#define	PIN_INPUT_MODE		14
#define	PIN_ALL_MODE		15

/////////////////////////////////////////////////////////////////////////////
//	Device Check Time
/////////////////////////////////////////////////////////////////////////////
#define OPM_DEVRSP_TIME		3605					// OP MODE DEVICE RESPONSE TIME : 2005.10.14
#define MAX_DEVRSP_TIME		365						// MAX DEVICE RESPONSE TIME
#define MID_DEVRSP_TIME		125						// MID DEVICE RESPONSE TIME(TAKE TIME + 5 SEC : 2003.10.29)
#define MIN_DEVRSP_TIME		5						// MIN DEVICE RESPONSE TIME

/////////////////////////////////////////////////////////////////////////////
//	Op Result
/////////////////////////////////////////////////////////////////////////////
#define OP_DO				"DO"					// DO
#define OP_NORMAL			"NORMAL"				// NORMAL
#define OP_ERROR			"ERROR"					// ERROR

/////////////////////////////////////////////////////////////////////////////
//	AP Dir & Bat Define
/////////////////////////////////////////////////////////////////////////////
#define _AP_TRACE_DIR		"D:\\TRACE"
#define _AP_UPDATE_DIR		"C:\\UPDATE"
#define _AP_TEMPZIP_DIR		"D:\\TEMPZIP"

/////////////////////////////////////////////////////////////////////////////
//	Device Profile Define
/////////////////////////////////////////////////////////////////////////////
#define _INI_DIR			"D:\\INI"

/////////////////////////////////////////////////////////////////////////////
//	Ejr Dir Define
/////////////////////////////////////////////////////////////////////////////
#define _EJR_DIR			"D:\\EJR"
#define _EJR_TXT			"D:\\EJR\\TXT"
#define _EJR_IMG			"D:\\EJR\\IMG"
#define _EJR_SEND			"D:\\EJR\\SEND"

#define _EJR_JNL			"D:\\EJR\\JOURNAL"
#define _EJR_JNL_BACKUP		"D:\\EJR\\JOURNALBACKUP"
#define _EJR_JNL_HISTORY	"HISTORY.TXT"
#define _EJR_JNL_TEMP		"TEMPJNL.TXT"
#define _EJR_DETAILSEND		"D:\\EJR\\DETAILSEND"

/////////////////////////////////////////////////////////////////////////////
//	Device Reg Define
/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_PATCHVERSION			_T("SOFTWARE\\NEXTWARE\\PATCHVERSION")
#define	_REGKEY_DEVICE					_T("SOFTWARE\\ATM\\APP\\DEVICE")
#define	_REGKEY_DEVICE_FTPUSERNAME		_T("FTPUSERNAME")
#define	_REGKEY_DEVICE_FTPPASSWORD		_T("FTPPASSWORD")
#define	_REGKEY_DEVICE_FTPDIRNAME		_T("FTPDIRNAME")
#define	_REGKEY_DEVICE_JNLPOSITION		_T("JNLPOSITION")
#define _REGKEY_DEVICE_ATMPTOTALS		_T("ATMPTOTALS")		// added by yusy 2010.03.22

/////////////////////////////////////////////////////////////////////////////
//	Statistics Reg Define
/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_STAT					_T("SOFTWARE\\ATM\\APP\\STAT")
#define	_REGKEY_STAT_TRANDATE			_T("TRANDATE")
#define	_REGKEY_STAT_TRANWITHDRAW		_T("TRANWITHDRAW")
#define	_REGKEY_STAT_TRANINQUIRY		_T("TRANINQUIRY")
#define	_REGKEY_STAT_TRANTRANSFER		_T("TRANTRANSFER")
#define	_REGKEY_STAT_TRANDEPOSIT		_T("TRANDEPOSIT")
#define	_REGKEY_STAT_TRANPASSBOOK		_T("TRANPASSBOOK")
#define	_REGKEY_STAT_TRANIC				_T("TRANIC")
#define	_REGKEY_STAT_ERRORDATE			_T("ERRORDATE")
#define	_REGKEY_STAT_ERRORDB			_T("ERRORDB")

#define	ERROR_DB_MAX_COUNT				128



typedef struct tagStatErrorDb						// Total Area(40 BYTE) * 128
{
	char	ProcCount;								// ProcCount			// 0
	char	Fill1;									// Fill1				// 1
	char	ErrorCode[7];							// ErrorCode			// 2
	char	Fill2;									// Fill2				// 9
	char	ErrorCount[6];							// ErrorCount			// 10
	char	Fill3;									// Fill3				// 16
	char	ErrorBeginDate[8];						// ErrorBeginDate		// 17
	char	Fill4;									// Fill4				// 25
	char	ErrorEndDate[8];						// ErrorEndDate			// 26
	char	Fill5;									// Fill5				// 34
	char	Dummy[5];								// Dummy				// 35[40]
} StatErrorDbTbl;

/////////////////////////////////////////////////////////////////////////////
//	Device Profile Define
/////////////////////////////////////////////////////////////////////////////
#define _OPERRORCODE_INI				"D:\\INI\\OPERRORCODE.INI"
#define _INIKEY_OPERRORCODE				_T("OPERRORCODE")
#define _OPHOSTNGCODE_INI				"D:\\INI\\OPHOSTNGCODE.INI"
#define _INIKEY_OPHOSTNGCODE			_T("OPHOSTNGCODE")


/////////////////////////////////////////////////////////////////////////////
//	Screen Button Information
/////////////////////////////////////////////////////////////////////////////
#define S_RETURN			"RETURN"				// RETURN
#define S_EXIT				"EXIT"					// EXIT
#define S_CANCEL			"CANCEL"				// CANCEL
#define S_TIMEOVER			"TIMEOVER"				// TIME OVER
#define S_INPUTOVER			"INPUTOVER"				// INPUT OVER

#define S_CONTINUE			"CONTINUE"				// CONTINUE
#define S_CONFIRM			"CONFIRM"				// CONFIRM
#define S_CHANGE			"CHANGE"				// CHANGE
#define S_MODIFY			"MODIFY"				// MODIFY
#define S_CLEAR				"CLEAR"					// CLEAR
#define S_OTHER				"OTHER"					// OTHER
#define	S_YES				"YES"					// YES
#define	S_NO				"NO"					// NO

/////////////////////////////////////////////////////////////////////////////
//	Ttu Button Information
/////////////////////////////////////////////////////////////////////////////
#define T_CANCEL			"<UserCancelled>"		// CANCEL
#define T_TIMEOVER			"<Timeout>"				// TIME OVER

/////////////////////////////////////////////////////////////////////////////
//	Guide Screen Header
/////////////////////////////////////////////////////////////////////////////
typedef struct tagGuideScrHeader
{
	char	GuideScrName[81];
	int		GuideScrNum;
	char	GuideScrData[16][81];
	char	GuideScrDisplay[1024];
	char	GuideScrAccept[2048];					// liyi add 2008.09.23
} GuideScrHeader;

/////////////////////////////////////////////////////////////////////////////
//	ElecJournal Form(TXT)
/////////////////////////////////////////////////////////////////////////////
#define	SPR_MAX_ROW			25		
#define	SPR_MAX_COL			40		
#define	JPR_MAX_ROW			25
#define	JPR_MAX_COL			40						// JPR SIZE > SPR SIZE(ElecJournal)

typedef struct tagEjrTbl							// Total Area(2048 BYTE)
{
													// Key Area(128 BYTE)
	char	SerialNo[8];							// SerialNo				// 0
	char	Fill1;									// Filler1				// 8
	char	Date[8];								// Date					// 9
	char	Fill2;									// Filler2				// 17
	char	Time[6];								// Time					// 18
	char	Fill3;									// Filler5				// 24
	char	AccountNum[24];							// AccountNum			// 25
	char	Fill4;									// Filler3				// 49
	char	TranSerialNo[8];						// TranSerialNo			// 50
	char	Fill5;									// Filler4				// 58
	char	TranAmount[12];							// TranAmount			// 59
	char	Fill6;									// Filler6				// 71
	char	KeyNum[24];								// KeyNum				// 72
	char	Fill7;									// Filler7				// 96
	char	TranMaterial[10];						// TranMaterial			// 97
	char	Fill8;									// Filler8				// 107
	char	Dummy[18];								// Dummy				// 108
	char	Cr;										// Cr					// 126
	char	Lf;										// Lf					// 127[128]

													// Data Area(1920 BYTE)
	char	Data[JPR_MAX_ROW][JPR_MAX_COL + 3];		// Ejr Area('|'+Cr+Lf)		
	char	DataDummy[2048 - 128 - (JPR_MAX_ROW * (JPR_MAX_COL + 3)) - 2];				
													// DataDummy
	char	DataDummyCr;							// Cr
	char	DataDummyLf;							// Lf
} EjrTbl;

/////////////////////////////////////////////////////////////////////////////
#endif
