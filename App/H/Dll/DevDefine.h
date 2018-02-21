/////////////////////////////////////////////////////////////////////////////
#if !defined(_DEVDEFINE_H_INCLUDED_)
#define _DEVDEFINE_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Version
/////////////////////////////////////////////////////////////////////////////
// Common C0001 : 2007.11.01 :	1. CDU-M REJECT COUNT UPDATE(SP)
//								2. MCU, CDU 82160 ERROR LOGIC UPDATE(DEVICE)
//								3. DEVICE_NO_RECOVERY_NONE(DEVICE)
//								4. ATM ID PRINT(REARAP) 
// Common C0002 : 2007.11.06 :	1. CDU-M CLOSE SHUTTER(DEVICE)
// Common C0003 : 2007.12.11 :	1. NCD MAC(DEVICE)
//								2. PIN BLOCK SYNC(MWI)
// Common C0004 : 2008.01.11 :	1. PIN BLOCK RETRY COUNT(DEVICE)
//								2. MCU SANKYO ANTISKIMMING(DEVICE)
// Common C0005 : 2008.01.31 :	1. TTW2E, MCD4E(DEVICE)
// Common C0006 : 2008.02.04 :	1. MX8000T(DEVICE)
// Common C0006 : 2008.03.28 :	1. MCU INITIAL(MWI)
//								2. PRINT IMAGE(MWI)
//								3. SPR/JPR PAPER OPTION(DEVICE)
//								4. CDU EP/SP LOG OPTION(DEVICE)
// Common C0007 : 2008.09.22 :	1. ATM+CD MERGE
// Common C0008 : 2008.10.28 :	1. MX8000L(DEVICE)
// Common C0009 : 2009.01.19 :	1. CDU/CIM OPEN RETRY(MWI)
// Common C0010 : 2009.08.25 :	1. MX5600T
//				  2009.10.21 :	2. MX5600L
// Common C0011 : 2010.08.24 :	1. CRM RETRACT ROUTER(CDU,BRM)
// Common C0012 : 2010.10.20 :	1. FORCE RETRACT(CDU)
//								2. MWI ERROR CODE UPDATE(400FF)
// Common C0013 : 2010.10.20 :	1. IC EMV APPEND
// Common C0014 : 2010.10.20 :	1. IC PBOC APPEND
//
//

#define	COMMON_VERSION		"C0014"

/////////////////////////////////////////////////////////////////////////////
// Operation Mode
/////////////////////////////////////////////////////////////////////////////
#define HOST_ONLINE_MODE	1						// HOST ONLINE MODE
#define HOST_LOCAL_MODE		0						// HOST LOCAL MODE

#define CSH_RECYCLE_MODE	1						// RECYCLE MODE
#define CSH_UNCYCLE_MODE	0						// UNCYCLE MODE(NOT USED)

#define CSH_REALCASH_MODE	254						// REAL CASH
#define CSH_TESTCASH_MODE	255						// TEST CASH

/////////////////////////////////////////////////////////////////////////////
//	Cash Condition Count Define(Mecha)
/////////////////////////////////////////////////////////////////////////////
#define CSH_MAXCASHOUTCNT	20						// MAX CASH OUT COUNT : 2004.11.05
#define CSH_FIXCASHCNT		10						// FIX CASH COUNT : 2004.11.05
#define CSH_EMPTYCASHCNT	(CSH_MAXCASHOUTCNT + CSH_FIXCASHCNT)
													// EMPTY CASH COUNT : 2004.11.05
#define CSH_MAXCASHOUTCNT2	50						// MAX CASH OUT COUNT2 : 2010.03.12(40 -> 50)
#define CSH_EMPTYCASHCNT2	(CSH_MAXCASHOUTCNT2 + CSH_FIXCASHCNT)
													// EMPTY CASH COUNT2 : 2010.03.12

#define CSH_MAXCASHOUTCNT4BRM 100					// MAX CASH OUT COUNT4BRM : 2010.03.12
#define CSH_EMPTYCASHCNT4BRM  (CSH_MAXCASHOUTCNT4BRM + CSH_FIXCASHCNT)
													// EMPTY CASH COUNT4BRM : 2010.03.12

#define CSH_NEARCASHCNT		100						// NEAR CASH COUNT
#define CSH_FULLREJECTCNT	200						// FULL REJECT COUNT
#define CSH_FULLREJECTCNT2	460						// FULL REJECT COUNT(G-CDU) : 2005.12.08
#define CSH_MAXREJECTCNT2	120						// MAX  REJECT COUNT(G-CDU) : 2005.12.08

#define CSH_MAXCASHINCNT	200						// MAX CASH IN COUNT

/////////////////////////////////////////////////////////////////////////////
//	Device Define
/////////////////////////////////////////////////////////////////////////////
#define DEVICE_NO_RECOVERY		(DEV_CSH)			// 2005.11.07
#define DEVICE_NO_RECOVERY_NONE	(DEV_NONE)			// 2007.11.01
#define DEVICE_YES_CHECK		(DEV_MCU)			// 2005.11.07

/////////////////////////////////////////////////////////////////////////////
//	Timer
/////////////////////////////////////////////////////////////////////////////
#define	TIMER_THREAD			1					// TIMER THREAD
#define	TIMER_VFD_THREAD		2					// TIMER VFD THREAD
#define	TIMER_AGENT_THREAD		4					// TIMER VFD THREAD
#define	TIMER_mSEC1000			1000				// TIMER MODE 1000ms
#define	AGENT_DEVICE_SUCCESS	1					// AGENT REST 
#define	AGENT_DEVICE_FAIL 		2					// AGENT REST

/////////////////////////////////////////////////////////////////////////////
//	Power Off Time Define
/////////////////////////////////////////////////////////////////////////////
#define POWER_OFF_TIME		45						// POWER OFF TIME

/////////////////////////////////////////////////////////////////////////////
//	Ejr Line Time Define
/////////////////////////////////////////////////////////////////////////////
#define LINE_EJR_OPCL_TIME	6						// LINE OPEN/CLOSE TIME(EJR)
#define LINE_EJR_SEND_TIME	30						// HOST SEND TIME(EJR)

/////////////////////////////////////////////////////////////////////////////
//	Play Movie Msg Define
/////////////////////////////////////////////////////////////////////////////
#define	WUM_SOUND_OFF	(WM_USER) + 9101
#define	WUM_SOUND_ON	(WM_USER) + 9102
#define	WUM_INDICATOR	(WM_USER) + 9103
#define	WUM_PLAY_OFF	(WM_USER) + 9104
#define	WUM_PLAY_ON		(WM_USER) + 9105

/////////////////////////////////////////////////////////////////////////////
//	SP Reg Define
/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_WIN2000_COMPUTERNAME	_T("SYSTEM\\CURRENTCONTROLSET\\SERVICES\\TCPIP\\PARAMETERS")
#define	_REGKEY_GCDUUSB_DRIVER			_T("SYSTEM\\CURRENTCONTROLSET\\SERVICES\\NHUSBC1\\PARAMETERS")
#define	_REGKEY_NEXTWARE				_T("SOFTWARE\\NEXTWARE")

#define	_REGKEY_ATM						_T("SOFTWARE\\ATM")
#define	_REGKEY_MWI						_T("SOFTWARE\\ATM\\MWI")
#define	_REGKEY_DEVERROR				_T("SOFTWARE\\ATM\\ERRORCODE")
#define	_REGKEY_SPVERSION				_T("SOFTWARE\\ATM\\SP_VERSION")
#define	_REGKEY_EPVERSION				_T("SOFTWARE\\ATM\\EP_VERSION")
#define	_REGKEY_SPCONFIGTXTLOG			_T("SOFTWARE\\ATM\\SPCONFIG\\TXTLOG")

#define	_REGKEY_SPRSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\SPR")
#define	_REGKEY_SPRCAPSSTATUS			_T("SOFTWARE\\ATM\\DEVINFO\\SPR\\CAPS")
#define	_REGKEY_SPRDEVSTATUS			_T("SOFTWARE\\ATM\\DEVINFO\\SPR\\DEVSTATUS")
#define	_REGKEY_SPRDEFAULTFONT			_T("SOFTWARE\\ATM\\DEVINFO\\SPR\\DEFAULTFONT")
#define	_REGKEY_JPRSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\JPR")
#define	_REGKEY_JPRDEVSTATUS			_T("SOFTWARE\\ATM\\DEVINFO\\JPR\\DEVSTATUS")
#define	_REGKEY_MCUSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\IDC")
#define	_REGKEY_CDUSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\CDM")
#define	_REGKEY_BRMSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\BRM")
#define	_REGKEY_PBMSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\PBM")
#define	_REGKEY_PINSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\PINPAD")
#define	_REGKEY_SIUSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\SIU")
#define	_REGKEY_SIUCAPSSTATUS			_T("SOFTWARE\\ATM\\DEVINFO\\SIU\\CAPS")
#define	_REGKEY_SIUCONFSTATUS			_T("SOFTWARE\\ATM\\DEVINFO\\SIU\\CONF")
#define	_REGKEY_UPSSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\UPS")
#define	_REGKEY_CAMSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\CAM")
#define	_REGKEY_TTUSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\TTU")
#define	_REGKEY_TTUKEYMAPPING			_T("SOFTWARE\\ATM\\DEVINFO\\TTU\\KEYBOARDMAPPING")
#define	_REGKEY_VDMSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\NHVDM")
#define	_REGKEY_KEYMGRSTATUS			_T("SOFTWARE\\ATM\\DEVINFO\\KEYMGR")

#define	_REGKEY_DCPEMULATOR				_T("SOFTWARE\\ATM\\DEVINFO\\DCPEMULATOR")
#define	_REGKEY_WATCHDOG				_T("SOFTWARE\\ATM\\DEVINFO\\WATCHDOG")

#define	_REGKEY_SOFTXFSMANAGER			_T("SOFTWARE\\XFS\\XFS_MANAGER")
#define	_REGKEY_SOFTXFSSPR				_T("SOFTWARE\\XFS\\SERVICE_PROVIDERS\\NW_KSPR")
#define	_REGKEY_SOFTXFSJPR				_T("SOFTWARE\\XFS\\SERVICE_PROVIDERS\\NW_DOTJPR")
#define	_REGKEY_SOFTXFSCDU				_T("SOFTWARE\\XFS\\SERVICE_PROVIDERS\\CASHDISPENSER")
#define	_REGKEY_OKIBRMSHUTTER			_T("SOFTWARE\\OKI\\DRIVER\\OKIBRMSHT\\COMMUNICATION_DEVICE\\CAB\\DEVICE_INFO")

#define	_REGKEY_OKISPOKICASHRECYCLER	_T("SOFTWARE\\XFS\\SERVICE_PROVIDERS\\OKICASHRECYCLER")

#define	_REGKEY_OKIPSCIMSPCONFIG		_T("SOFTWARE\\XFS\\PHYSICAL_SERVICES\\CASHACCEPTOR\\DEVICE_INFORMATION\\SPCONFIG")
#define	_REGKEY_OKIPSCDMSPCONFIG		_T("SOFTWARE\\XFS\\PHYSICAL_SERVICES\\CASHDISPENSER\\DEVICE_INFORMATION\\SPCONFIG")
#define	_REGKEY_OKIPSCIMLIMIT			_T("SOFTWARE\\XFS\\PHYSICAL_SERVICES\\CASHACCEPTOR\\CUSTOM_INFORMATION\\LIMIT")
#define	_REGKEY_OKIPSCDMCSTTYPE			_T("SOFTWARE\\XFS\\PHYSICAL_SERVICES\\CASHDISPENSER\\COMMON\\CASSETTETYPE")
#define	_REGKEY_OKIPSCIMNOTETYPE		_T("SOFTWARE\\XFS\\PHYSICAL_SERVICES\\CASHACCEPTOR\\CUSTOM_INFORMATION\\NOTETYPE")
#define	_REGKEY_OKIPSCIMCSTCONFIG		_T("SOFTWARE\\XFS\\PHYSICAL_SERVICES\\CASHACCEPTOR\\DEVICE_INFORMATION\\CASSETTECONFIG")
#define	_REGKEY_OKIPSCDMPCSTINFO		_T("SOFTWARE\\XFS\\PHYSICAL_SERVICES\\CASHDISPENSER\\DEVICE_INFORMATION\\PHYSICALCASSETTEINFO")

#define	_REGKEY_OKIKALCASHACCEPTOR		_T("SOFTWARE\\KAL\\KALYPSO\\CASHACCEPTOR")
#define	_REGKEY_OKIKALCASHDISPENSER		_T("SOFTWARE\\KAL\\KALYPSO\\CASHDISPENSER")

#define	_REGKEY_ADDCASHMODE				_T("SOFTWARE\\ATM\\APP\\ADDCASHMODE")

/////////////////////////////////////////////////////////////////////////////
//	SP Profile Define
/////////////////////////////////////////////////////////////////////////////
#define _SP_CDMINFO_INI		"C:\\T1ATM\\INI\\CDMINFO.INI"
#define _SP_CSTINFO_INI		"C:\\T1ATM\\INI\\CSTINFO.INI"
#define _SP_DISPINFO_INI	"C:\\T1ATM\\INI\\DISPINFO.INI"

/////////////////////////////////////////////////////////////////////////////
//	Scr Dir Define
/////////////////////////////////////////////////////////////////////////////
#define _SCR_DIR			"../SWF/"

/////////////////////////////////////////////////////////////////////////////
//	Ap Dir & Bat Define
/////////////////////////////////////////////////////////////////////////////
#define _AP_TRACE_DIR		"D:\\TRACE"
#define _AP_UPDATE_DIR		"C:\\UPDATE"
#define _AP_TEMP_DIR		"D:\\TEMP"
#define _AP_TEMPFTP_DIR		"D:\\TEMPFTP"
#define _AP_VDM				"C:\\T1ATM\\NHVDM\\NHVDM.EXE"
#define _EJR_DETAILSEND	   	"D:\\EJR\\DETAILSEND"

/////////////////////////////////////////////////////////////////////////////
//	Option Reg Define
/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_OPTION					_T("SOFTWARE\\ATM\\APP\\OPTION")
#define	_REGKEY_OPTION_BRMMAXCASHINCNT	_T("BRMMAXCASHINCNT")
#define	_REGKEY_OPTION_BRMCSTTYPE		_T("BRMCSTTYPE")
#define	_REGKEY_OPTION_BRMNOTETYPE		_T("BRMNOTETYPE")
#define	_REGKEY_OPTION_BRMHIDELOGO		_T("BRMHIDELOGO")
#define	_REGKEY_OPTION_BRMPLAYMOVIE		_T("BRMPLAYMOVIE")
#define _REGKEY_OPTION_BRMFLAG			_T("BRMFLAG")
#define	_REGKEY_OPTION_BHCSPRNORESET	_T("BHCSPRNORESET")
#define	_REGKEY_OPTION_BHCCDUMIXTYPE	_T("BHCCDUMIXTYPE")		// 2011.01.05
#define	_REGKEY_OPTION_BHCNWLOGSAVEDAY	_T("BHCNWLOGSAVEDAY")	// 2011.08.01
#define	_REGKEY_OPTION_BHCNWLOGSPACE	_T("BHCNWLOGSPACE")		// 2011.08.01
#define	_REGKEY_OPTION_BHCTAMPERFLAG	_T("BHCTAMPERFLAG")		// 2012.08.24

/////////////////////////////////////////////////////////////////////////////
//	Agent Dir Define
/////////////////////////////////////////////////////////////////////////////
#define _AGENT_PATH     	"C:\\ATMS"
#define _AGENT_JNL_TEMPPATH "D:\\ATMS\\TEMP"
#define _AGENT_ATMS     	"C:\\ATMS\\ATMS.INI"
#define _AGENT_COMMON     	"COMMON"
#define _AGENT_ATM_AP     	"ATM_AP"
#define _AGENT_FTPDOWNINFO  "FTPDOWNINFO"
#define _AGENT_SYSINFO      "SYSINFO"

/////////////////////////////////////////////////////////////////////////////
//	Device Reg Define
/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_DEVICE					_T("SOFTWARE\\ATM\\APP\\DEVICE")
#define	_REGKEY_DEVICE_OSREBOOT			_T("OSREBOOT")
#define	_REGKEY_DEVICE_APINSTALL		_T("APINSTALL")
#define	_REGKEY_DEVICE_CAMERACHECK		_T("CAMERACHECK")
#define	_REGKEY_DEVICE_PINCHECK			_T("PINCHECK")
#define	_REGKEY_DEVICE_REBOOT			_T("REBOOT")
#define	_REGKEY_DEVICE_LOADINGSTEP		_T("LOADINGSTEP")
#define	_REGKEY_DEVICE_EJRWRITEP		_T("EJRWRITEP")
#define	_REGKEY_DEVICE_EJRSENDP			_T("EJRSENDP")
#define	_REGKEY_DEVICE_TOTALSETCASHCST1	_T("TOTALSETCASHCST1")	// 2004.08.23
#define	_REGKEY_DEVICE_TOTALSETCASHCST2	_T("TOTALSETCASHCST2")
#define	_REGKEY_DEVICE_TOTALSETCASHCST3	_T("TOTALSETCASHCST3")
#define	_REGKEY_DEVICE_TOTALSETCASHCST4	_T("TOTALSETCASHCST4")
#define	_REGKEY_DEVICE_DISPCASHCST1		_T("DISPCASHCST1")
#define	_REGKEY_DEVICE_DISPCASHCST2		_T("DISPCASHCST2")
#define	_REGKEY_DEVICE_DISPCASHCST3		_T("DISPCASHCST3")
#define	_REGKEY_DEVICE_DISPCASHCST4		_T("DISPCASHCST4")
#define	_REGKEY_DEVICE_FTPUSERNAME		_T("FTPUSERNAME")
#define	_REGKEY_DEVICE_FTPPASSWORD		_T("FTPPASSWORD")
#define	_REGKEY_DEVICE_FTPKIND			_T("FTPKIND")
#define	_REGKEY_DEVICE_FTPKIND1			_T("FTPKIND1")
#define	_REGKEY_DEVICE_FTPKIND2			_T("FTPKIND2")
#define	_REGKEY_DEVICE_FTPKIND3			_T("FTPKIND3")
#define	_REGKEY_DEVICE_FTPKIND4			_T("FTPKIND4")
#define	_REGKEY_DEVICE_FTPDIRNAME		_T("FTPDIRNAME")
#define	_REGKEY_DEVICE_FTPDIRNAME1		_T("FTPDIRNAME1")
#define	_REGKEY_DEVICE_FTPDIRNAME2		_T("FTPDIRNAME2")
#define	_REGKEY_DEVICE_FTPDIRNAME3		_T("FTPDIRNAME3")
#define	_REGKEY_DEVICE_FTPDIRNAME4		_T("FTPDIRNAME4")
#define	_REGKEY_DEVICE_FTPFILENAME		_T("FTPFILENAME")
#define	_REGKEY_DEVICE_FTPFILENAME1		_T("FTPFILENAME1")
#define	_REGKEY_DEVICE_FTPFILENAME2		_T("FTPFILENAME2")
#define	_REGKEY_DEVICE_FTPFILENAME3		_T("FTPFILENAME3")
#define	_REGKEY_DEVICE_FTPFILENAME4		_T("FTPFILENAME4")
#define	_REGKEY_DEVICE_TRANBATCHNO		_T("TRANBATCHNO")
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
#define _INI				"D:\\INI\\"
#define _DEVICE_INI			"D:\\INI\\DEVICE.INI"
#define _NETWORK_INI 		"D:\\INI\\NETWORK.INI"
#define _TRANS_INI  		"D:\\INI\\TRANS.INI"
#define _ERRSTACK_INI		"D:\\INI\\ERRSTACK.INI"
#define _ERRHOST_INI 		"D:\\INI\\ERRHOST.INI"
#define _ATMTOTAL_INI		"D:\\INI\\ATMTOTAL.INI"

/////////////////////////////////////////////////////////////////////////////
//	Vfd Define
/////////////////////////////////////////////////////////////////////////////
#define _VFDINFO_INI		"D:\\INI\\VFDINFO.INI"
#define _INIKEY_VFDINFO		_T("VFDINFO")
#define _INIKEY_VFDINFO_CMD	_T("CMD")

/////////////////////////////////////////////////////////////////////////////
//	Ejr Dir Define
/////////////////////////////////////////////////////////////////////////////
#define _EJR_DIR			"D:\\EJR"
#define _EJR_TXT			"D:\\EJR\\TXT"
#define _EJR_IMG			"D:\\EJR\\IMG"
#define _EJR_SEND			"D:\\EJR\\SEND"
#define _EJR_FTP			"D:\\EJR\\FTP"
#define _EJR_DCNMS			"D:\\EJR\\DCNMS"
#define _EJR_NDCNMS			"D:\\EJR\\NDCNMS"
#define _EJR_HXNMS			"D:\\EJR\\HXNMS"
#define _EJR_TOTALDETAIL	"D:\\EJR\\TOTALDETAIL"

#define _EJR_JNL			"D:\\EJR\\JOURNAL"
#define _EJR_JNL_BACKUP		"D:\\EJR\\JOURNALBACKUP"
#define _EJR_JNL_HISTORY	"HISTORY.TXT"
#define _EJR_JNL_TEMP		"TEMPJNL.TXT"

#define _EJR_JPRDATA		"D:\\EJR\\JPRDATA"
#define _EJR_CUPJNEJR		"D:\\EJR\\CUPJNEJR"

/////////////////////////////////////////////////////////////////////////////
//	User Define
/////////////////////////////////////////////////////////////////////////////
#define _TIMEDEFINE_INI						"D:\\INI\\TIMEDEFINE.INI"
#define _INIKEY_TIMEDEFINE					_T("TIMEDEFINE")
#define _INIKEY_TIMEDEFINE_RESERVE1			_T("RESERVE1")
#define _INIKEY_TIMEDEFINE_RESERVE2			_T("RESERVE2")
#define _INIKEY_TIMEDEFINE_RESERVE3			_T("RESERVE3")
#define _INIKEY_TIMEDEFINE_RESERVE4			_T("RESERVE4")
#define _INIKEY_TIMEDEFINE_RECVTIMEOUT		_T("RECVTIMEOUT")           //交易超时
#define _INIKEY_TIMEDEFINE_IDLERETRYTIME	_T("IDLERETRYTIME")         //定时状态查询间隔
#define _INIKEY_TIMEDEFINE_AGENTRETRYTIME	_T("AGENTRETRYTIME")        // added by yusy 08.09.09
#define _INIKEY_TIMEDEFINE_WITHMAXAMOUNT	_T("WITHMAXAMOUNT")         //单笔取款最大
#define _INIKEY_TIMEDEFINE_WITHMINAMOUNT	_T("WITHMINAMOUNT")
#define _INIKEY_TIMEDEFINE_TRANMAXAMOUNT	_T("TRANMAXAMOUNT")         //单笔转账最大
#define _INIKEY_TIMEDEFINE_WITHDAYMAXAMOUNT	_T("WITHDAYMAXAMOUNT")      
#define _INIKEY_TIMEDEFINE_TAKECARDTIMEOUT	_T("TAKECARDTIMEOUT")       //超时吞卡时间
#define _INIKEY_TIMEDEFINE_TAKECASHTIMEOUT	_T("TAKECASHTIMEOUT")       //超时吞钞时间
#define _INIKEY_TIMEDEFINE_HOSTIPPAY		_T("HOSTIPPAY")
#define _INIKEY_TIMEDEFINE_HOSTPORTPAY		_T("HOSTPORTPAY")
#define _INIKEY_TIMEDEFINE_HOSTIP2			_T("HOSTIP2")
#define _INIKEY_TIMEDEFINE_HOSTPORT2		_T("HOSTPORT2")
#define _INIKEY_TIMEDEFINE_HOSTIP3			_T("HOSTIP3")
#define _INIKEY_TIMEDEFINE_HOSTPORT3		_T("HOSTPORT3")
#define _INIKEY_TIMEDEFINE_DVRIDLETIME		_T("DVRIDLETIME")
#define _INIKEY_TIMEDEFINE_ETCCOMIDLETIME	_T("ETCCOMIDLETIME")
#define _INIKEY_TIMEDEFINE_ETCCOMADDRESS	_T("ETCCOMADDRESS")
#define _INIKEY_TIMEDEFINE_UPSIP			_T("UPSIP")
#define _INIKEY_TIMEDEFINE_UPSPORT			_T("UPSPORT")
#define _INIKEY_TIMEDEFINE_UPSIDLETIME		_T("UPSIDLETIME")
#define _INIKEY_TIMEDEFINE_NMSIP			_T("NMSIP")
#define _INIKEY_TIMEDEFINE_NMSPORT			_T("NMSPORT")
#define _INIKEY_TIMEDEFINE_NMSKIND			_T("NMSKIND")
#define _INIKEY_TIMEDEFINE_ETCIP			_T("ETCIP")
#define _INIKEY_TIMEDEFINE_ETCPORT			_T("ETCPORT")
#define _INIKEY_TIMEDEFINE_CSHMAXRETRACTCNT _T("CSHMAXRETRACTCNT")
#define _INIKEY_TIMEDEFINE_CSHRETRACTCNT	_T("CSHRETRACTCNT")

#define _INIKEY_TIMEDEFINE_DEPCASHTIMEOUT	_T("DEPCASHTIMEOUT")
#define _INIKEY_TIMEDEFINE_DEPMAXAMOUNT		_T("DEPMAXAMOUNT")
#define _INIKEY_TIMEDEFINE_CASHINRETRY		_T("CASHINRETRY")
#define _INIKEY_TIMEDEFINE_CWDMAXSHUTTERNUM	_T("CWDMAXSHUTTERNUM")	// added by liuxl 20110601

// 20080903 by yaokq
#define _INIKEY_TIMEDEFINE_ZZTOHMAXNUM		_T("ZZTOHMAXNUM")
#define _INIKEY_TIMEDEFINE_ZZTOHMINNUM		_T("ZZTOHMINNUM")
#define _INIKEY_TIMEDEFINE_DHTOHMAXNUM		_T("DHTOHMAXNUM")
#define _INIKEY_TIMEDEFINE_DHTOHMINNUM		_T("DHTOHMINNUM")
#define _INIKEY_TIMEDEFINE_TZCKTOHMAXNUM	_T("TZCKTOHMAXNUM")
#define _INIKEY_TIMEDEFINE_TZCKTOHMINNUM	_T("TZCKTOHMINNUM")
#define _INIKEY_TIMEDEFINE_HTODHMAXNUM		_T("HTODHMAXNUM")
#define _INIKEY_TIMEDEFINE_HTODHMINNUM		_T("HTODHMINNUM")
#define _INIKEY_TIMEDEFINE_HTOTZCQMAXNUM	_T("HTOTZCQMAXNUM")
#define _INIKEY_TIMEDEFINE_HTOTZCQMINNUM	_T("HTOTZCQMINNUM")
#define _INIKEY_TIMEDEFINE_HTOZZMAXNUM		_T("HTOZZMAXNUM")
#define _INIKEY_TIMEDEFINE_HTOZZMINNUM		_T("HTOZZMINNUM")
#define _INIKEY_TIMEDEFINE_SAVEKINDPARA		_T("SAVEKINDPARA")

// added by yusy ic
#define _INIKEY_TIMEDEFINE_ICLOADMINNUM		_T("ICLOADMINNUM")
#define _INIKEY_TIMEDEFINE_ICLOADMAXNUM		_T("ICLOADMAXNUM")
/////////////////////////////////////////////////////////////////////////////
//	ADmachine Define 2008.05.09
/////////////////////////////////////////////////////////////////////////////
#define _ADMACHINE_INI						"D:\\INI\\ADMACHINE.INI"
#define _INIKEY_ADMACHINE					_T("ADCOMMDATA")
#define _INIKEY_ADMACHINE_ADMODE			_T("ADMODE")
#define _INIKEY_ADMACHINE_OPENADCOMM		_T("OPENADCOMM")
#define _INIKEY_ADMACHINE_SHORTADTIME		_T("SHORTTIME")
#define _INIKEY_ADMACHINE_LONGADTIME		_T("LONGADTIME")

/////////////////////////////////////////////////////////////////////////////
//	AP Profile Define   // yaokq add 20081031
/////////////////////////////////////////////////////////////////////////////
#define _TRANSACTION_INI					"D:\\INI\\TRANSACTION.INI"	// 2004.11.05
#define _ECHANNELPROTOCOL_INI				"C:\\T1ATM\\APP\\EXERELE\\ECHANNELPROTOCOL.INI"
#define _INIKEY_TRANSACTION					_T("TRANSACTION")
#define _INIKEY_TRANSACTION_WITHRETRACTCNT	_T("WITHRETRACTCNT")
#define _INIKEY_TRANSACTION_WITHRETRACTAMT	_T("WITHRETRACTAMT")


/////////////////////////////////////////////////////////////////////////////
//	Atm Mode Define
/////////////////////////////////////////////////////////////////////////////
#define ATM_INIT			0						// INIT		MODE
#define ATM_ERROR			1						// ERROR	MODE
#define ATM_CLERK			2						// CLERK	MODE
#define ATM_READY			3						// READY	MODE
#define ATM_CUSTOM			4						// CUSTOM	MODE 
#define ATM_TRAN			5						// TRAN		MODE
#define ATM_DOWN			6						// DOWN		MODE

/////////////////////////////////////////////////////////////////////////////
//	Host Mode Define
/////////////////////////////////////////////////////////////////////////////
#define HOST_OFFLINE 		0						// OFFLINE
#define HOST_WAITLINK		1						// WAITLINK
#define HOST_LINK			2						// LINK
#define HOST_WAITREADY		3						// WAITREADY
#define HOST_READY			4						// READY
#define HOST_WAITONLINE		5						// WAITONLINE
#define HOST_ONLINE			6						// ONLINE

/////////////////////////////////////////////////////////////////////////////
//	Transaction Mode Define
/////////////////////////////////////////////////////////////////////////////
#define TRAN_IDLE 			0						// IDLE

/////////////////////////////////////////////////////////////////////////////
//	Transacion Proc Define
/////////////////////////////////////////////////////////////////////////////
#define TRAN_NULL			0						// NULL
#define TRAN_WITH			1						// WITHDRAW
#define TRAN_INQ			2						// INQUIRY
#define TRAN_TRANS			3						// TRANSFER
#define TRAN_DEP			4						// DEPOSIT
#define TRAN_PB				5						// PASSBOOK
#define TRAN_IC				6						// IC

/////////////////////////////////////////////////////////////////////////////
//	Buffer Size
/////////////////////////////////////////////////////////////////////////////
#define	TEMPBUFFSIZE		256
#define	BASEBUFFSIZE		32768
#define	NETBUFFSIZE			32768
#define	COMBUFFSIZE			8192

/////////////////////////////////////////////////////////////////////////////
//	Device Check Time
/////////////////////////////////////////////////////////////////////////////
#define MAX_DEVRSP_TIME		365						// MAX DEVICE RESPONSE TIME
#define MID_DEVRSP_TIME		125						// MID DEVICE RESPONSE TIME(TAKE TIME + 5 SEC : 2003.10.29)
#define MIN_DEVRSP_TIME		5						// MIN DEVICE RESPONSE TIME

//20080327 BY HYL
#define AGENT_DEVRSP_TIME	25						// AGENT DEVICE RESPONSE TIME 

/////////////////////////////////////////////////////////////////////////////
//	Transaction Avail Id
/////////////////////////////////////////////////////////////////////////////
#define TRAN_WITH_NOT		0x00
#define TRAN_WITH_100		0x01
#define TRAN_WITH_50		0x02
#define TRAN_WITH_BOTH		(TRAN_WITH_100 | TRAN_WITH_50)

#define TRAN_DEP_NOT 		0x00
#define TRAN_DEP_100		0x04
#define TRAN_DEP_50			0x08
#define TRAN_DEP_20			0x10
#define TRAN_DEP_10			0x20
#define TRAN_DEP_CASH		(TRAN_DEP_100 | TRAN_DEP_50 | TRAN_DEP_20 | TRAN_DEP_10)

#define TRAN_PBM_NOT 		0x00
#define TRAN_PBM_OK			0x80

#define TRAN_ALL_NOT		0x00
#define TRAN_ALL_OK			(TRAN_WITH_BOTH | TRAN_DEP_CASH | TRAN_PBM_OK)

/////////////////////////////////////////////////////////////////////////////
//	Spr & Jpr Header Information
/////////////////////////////////////////////////////////////////////////////
#define HEADER_NORMAL		0x00					// SPR & JPR HEADER NORMAL
#define	SLIP_HEAD_UP		0x01					// SPR HEAD UP
#define	JNL_HEAD_UP			0x10					// JPR HEAD UP

/////////////////////////////////////////////////////////////////////////////
//	On/Off
/////////////////////////////////////////////////////////////////////////////
#define ON					1
#define OFF					0

/////////////////////////////////////////////////////////////////////////////
//	En/Disable(Mcu, Pbm)
/////////////////////////////////////////////////////////////////////////////
#define	ENABLE				1
#define	DISABLE				2

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

#define	PIN_MENU_PAD			"CANCEL,F1,F2,F3,F4,F5,F6,F7,F8"
#define	PIN_NUM_AMT_PAD			"1,2,3,4,5,6,7,8,9,0,00,CANCEL,CLEAR,ENTER"
#define	PIN_NUMERIC_PAD			"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_NUM_ALL_PAD			"1,2,3,4,5,6,7,8,9,0,.,00,CANCEL,CLEAR,ENTER"
#define	PIN_PASSWORD_PAD		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_PASSWORD_PAD2		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F4,F6,F7,F8"
#define	PIN_PASSWORD_PAD3		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F4,F6,F8"
#define	PIN_PASSWORD_PAD4		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F6,F7,F8"
#define	PIN_PASSWORD_PAD5		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F6,F8"
#define	PIN_PASSWORD_PAD6		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F7,F8"
#define	PIN_PASSWORD_PAD7		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F8"
#define	PIN_AMOUNT_PAD			"1,2,3,4,5,6,7,8,9,0,00,CANCEL,CLEAR,ENTER,F1,F2,F3,F4,F5,F6,F7,F8"
#define	PIN_INPUT_PAD			"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F1,F2,F3,F4,F5,F6,F7,F8"
#define	PIN_ALL_PAD				"1,2,3,4,5,6,7,8,9,0,.,00,CANCEL,CLEAR,ENTER,F1,F2,F3,F4,F5,F6,F7,F8"

#define	PIN_MENU_TOUCH			"CANCEL"
#define	PIN_NUM_AMT_TOUCH		"1,2,3,4,5,6,7,8,9,0,00,CANCEL,CLEAR,ENTER"
#define	PIN_NUMERIC_TOUCH		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_NUM_ALL_TOUCH		"1,2,3,4,5,6,7,8,9,0,.,00,CANCEL,CLEAR,ENTER"
#define	PIN_PASSWORD_TOUCH		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_AMOUNT_TOUCH		"1,2,3,4,5,6,7,8,9,0,00,CANCEL,CLEAR,ENTER"
#define	PIN_INPUT_TOUCH			"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_ALL_TOUCH			"1,2,3,4,5,6,7,8,9,0,.,00,CANCEL,CLEAR,ENTER"

/////////////////////////////////////////////////////////////////////////////
//	Track Infomation
/////////////////////////////////////////////////////////////////////////////
#define ISO1_TRACK			0x01
#define ISO2_TRACK			0x02
#define ISO3_TRACK			0x04
#define JIS_TRACK			0x08
#define ALL_TRACK			0x0f

/////////////////////////////////////////////////////////////////////////////
//	Cash Move Information
/////////////////////////////////////////////////////////////////////////////
#define REFILL_MOVE			0x01
#define TAKEUP_MOVE			0x02

#define CSH_AUTOLOAD		0x10
#define CSH_MANUALLOAD		0x20
#define CSH_RESET			0x40
#define CSH_LOST			0x80

/////////////////////////////////////////////////////////////////////////////
//	Language Mode
/////////////////////////////////////////////////////////////////////////////
#define CHN_MODE			0						// CHINA
#define ENG_MODE			1						// ENGLISH
#define KOR_MODE			2						// KOREA
#define JPN_MODE			3						// JAPAN

/////////////////////////////////////////////////////////////////////////////
//	TranCtrl Return Value
/////////////////////////////////////////////////////////////////////////////
#define T_OK				0						// OK
#define T_EXIT				1						// EXIT
#define T_CANCEL			2						// CANCEL
#define T_TIMEOVER			3						// TIME OVER
#define T_INPUTOVER			4						// INPUT OVER
#define T_INPUTERROR		5						// INPUT ERROR
#define T_ERROR				6						// DEVICE  ERROR(DEVICE ERROR MSG)
#define T_SENDERROR			7						// SEND    ERROR(BALANCE CONFIRM MSG)
#define T_RECVERROR			8						// RECV    ERROR(BALANCE CONFIRM MSG)
#define T_PROGRAM			98						// PROGRAM ERROR(REQUEST BANK)
#define T_MSG				99						// CANCEL(MSG)

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
#define	S_PAY1				"PAY1"					// NO
#define	S_PAY2				"PAY2"					// NO
#define	S_ACREMIT			"TRANPAY"				// 转账汇款
#define	S_ACPAY		    	"PAY"			    	// 管家婆缴费
#define S_ENTER             "ENTER"
#define	S_INCREASE			"INCREASE"				// INCREASE
#define	S_AGAIN				"AGAIN"					// AGAIN

#define S_REMIT_ADD			"ADDRESSPAY"
#define S_REMIT_PWD			"PINPAY"
#define S_REMIT_IN			"REMITPAY"

//20080905 by yaokq
#define S_Previous			"PREVIOUS"				//上一页
#define S_NEXT				"NEXT"					//下一页
/////////////////////////////////////////////////////////////////////////////
//	Op Result
/////////////////////////////////////////////////////////////////////////////
#define OP_DO				"DO"					// DO
#define OP_NORMAL			"NORMAL"				// NORMAL
#define OP_ERROR			"ERROR"					// ERROR

/////////////////////////////////////////////////////////////////////////////
//	Card Information
/////////////////////////////////////////////////////////////////////////////
typedef struct	tagISO2Form
{
	BYTE MembershipNo[16];							// 000
	BYTE Seperator;									// 016
	BYTE EndDay[4];									// 017
	BYTE ServiceNo[3];								// 021
	BYTE ScrambleNo[4];								// 024
	BYTE Dummy[228];								// 028(256)
} MCISO2;

typedef struct	tagISO3Form
{
	BYTE FormatCode[2]; 							// 000
	BYTE IdNo[2]; 									// 002
	BYTE BankNo[3];									// 004
	BYTE Separator1;								// 007
	BYTE AccountNo[16];								// 008
	BYTE AccountCheck;								// 024
	BYTE Separator2;								// 025
	BYTE Nation[3]; 								// 026
	BYTE CashCode[3];								// 029
	BYTE Unknown1[15];								// 032
	BYTE ValidCode;									// 047
	BYTE Password[6];								// 048
	BYTE Trade;										// 054
	BYTE Tran1[2];									// 055
	BYTE Tran2[4];									// 057
	BYTE OtherData[27];								// 061
	BYTE Remark[16];								// 088
	BYTE Dummy[152];								// 104(256)
} MCISO3;

/////////////////////////////////////////////////////////////////////////////
//	Pb Information
/////////////////////////////////////////////////////////////////////////////
typedef struct	tagPBMSForm
{
	BYTE AccountNo[17];								// 000
	BYTE Filler[239];								// 017(256)
} PBMS;

/////////////////////////////////////////////////////////////////////////////
//	Magnetic Card Area
/////////////////////////////////////////////////////////////////////////////
typedef struct	tagMCVW 
{
	BYTE TrInfo;
	WORD JISsize;
	BYTE JISBuff[256];         
	WORD ISO1size;               
	BYTE ISO1Buff[256];     
	WORD ISO2size;              
	BYTE ISO2Buff[256];    
	WORD ISO3size;             
	BYTE ISO3Buff[256];   
} MCAP;

/////////////////////////////////////////////////////////////////////////////
//	Magnetic Pb Area
/////////////////////////////////////////////////////////////////////////////
typedef struct tagMS 
{
	WORD Len;
	BYTE sBuf[256];
} MS;

/////////////////////////////////////////////////////////////////////////////
// DepositMoney Area
/////////////////////////////////////////////////////////////////////////////
typedef	struct tagDEPOSIT_MONEY 
{
	int	 mStatus;									// Status
	int	 mTotalAmount;								// Total Amount
	WORD mHundred;									// Hundred
	WORD mFifty;									// Fifty
	WORD mTwenty;									// Twenty
	WORD mTen;										// Ten
	WORD mUnknown;									// Unknown
} DepositMoney;

/////////////////////////////////////////////////////////////////////////////
// RejectMoney Area
/////////////////////////////////////////////////////////////////////////////
typedef	struct tagREJECT_MONEY 
{
	int	 mTotalAmount;								// Total Amount
	WORD mHundred;									// Hundred
	WORD mFifty;									// Fifty
	WORD mTwenty;									// Twenty
	WORD mTen;										// Ten
	WORD mUnknown;									// Unknown
} RejectMoney;

/////////////////////////////////////////////////////////////////////////////
// RetInformation Area
/////////////////////////////////////////////////////////////////////////////
typedef struct tagRET_INFORMATION 
{
	int	 mStatus;									// Status
	WORD mActNum;									// Act Num
	WORD mRJTNum;									// RJT Num
	WORD mHundredRJTNum;							// Hundred RJT Num
	WORD mFiftyRJTNum;								// Fifty RJT Num
	WORD mTwentyRJTNum;								// Twenty RJT Num
	WORD mTenRJTNum;								// Ten RJT Num
	WORD mUnknownRJTNum;							// Unknown RJT Num
} RetInformation;

/////////////////////////////////////////////////////////////////////////////
//	Menu Header
/////////////////////////////////////////////////////////////////////////////
typedef struct tagMENUHeader
{
	int		MenuPinInputMode;
	char	MenuScrName[81];
	int		MenuScrNum;
	char	MenuKind[16][81];
	int		MenuInfo[16][7];
} MENUHeader;

/////////////////////////////////////////////////////////////////////////////
//	ElecJournal Form(TXT)
/////////////////////////////////////////////////////////////////////////////
#define	SPR_MAX_ROW			50
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

typedef struct tagEjrSprTbl							// Total Area(2048 BYTE)
{
	char	Data[SPR_MAX_ROW][SPR_MAX_COL];
} EjrSprTbl;

/////////////////////////////////////////////////////////////////////////////
#endif
