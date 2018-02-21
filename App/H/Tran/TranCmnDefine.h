/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCmnDefine_H_)
#define _TranCmnDefine_H_

/////////////////////////////////////////////////////////////////////////////
// Version
/////////////////////////////////////////////////////////////////////////////
// V01-01-01 : 2004.03.15 : CHINA CREATE
// V01-02-01 : 2004.05.20 : SHUTTER CHANGE(OMRON => KDE)
// V01-02-02 : 2004.06.14 : fnCSH_GetAmountOfCST() => 
//							fnCSH_GetAmountOfCST(int nWithAvailFlag = FALSE)
// V01-02-03 : 2004.06.18 : IP SETTING UPGRADE
// V01-03-01 : 2004.07.01 : SANDOGN ABC SUPPORT
// V02-02-02 : 2004.07.12 : PARK REVIEW
// V02-06-06 : 2004.07.26 : ERROR SEND
// V02-07-07 : 2004.08.05 : CLERK INFORMATION UPDATE
// V02-08-08 : 2004.08.07 : PASSWORD CLEAR UPDATE
// V02-09-09 : 2004.08.11 : ERROR CODE UPDATE
// V02-12-12 : 2004.08.13 : MWI CST COUNT SET UPDATE
// V02-14-14 : 2004.08.19 : CWC SEND, TOTAL,  NO LOG(PIN, PASSWORD, PASS, PWD)
// V02-15-15 : 2004.08.21 : TOTAL(CST COUNT CLEAR)
// V02-16-16 : 2004.08.26 : JOURNAL DATA UPDATE
// V03-05-05 : 2004.08.29 : TOTAL INFO, OP HOLDING UPDATE
// V03-08-08 : 2004.11.05 : 20/10 & 100/50 YUYAN MERGE, ADD CASH
// V03-08-10 : 2004.11.05 : ACCEPT TMONEY, MAKE CANCEL DATA
// V03-08-11 : 2004.11.05 : MAIN CANCEL PROCEDURE, LOCALMODE CARDREADCHECK
// V04-00-00 : 2004.11.05 : MCD4, REARAP(NCR OP)
// V04-00-01 : 2004.11.05 : RETRACT, SEMI NORMAL, ALL RESET
// V04-00-02 : 2004.11.05 : REJECT COUNT
// V04-00-04 : 2004.11.05 : NO DEVICE, LOST 
// V04-00-06 : 2004.11.05 : CDU NOT AVAIL WITH, DOWN ERROR DEVICE
// V04-00-08 : 2004.11.05 : REINSERT CARD
// V04-00-10 : 2005.02.22 : REJECT/JAM COUNT
// V04-00-12 : 2005.02.24 : CASH VALUE, BANK TEST
// V04-00-14 : 2005.02.24 : LOST CASH
// V04-00-16 : 2005.02.28 : SLIP AUTO-OFF
// V04-00-18 : 2005.03.08 : ERROR REMARK UPDATE
// V04-00-20 : 2005.03.10 : HIDDENFLAG FALSE
// V04-00-22 : 2005.03.16 : CARD READ CHECK
// V04-00-24 : 2005.03.20 : JPR STATUS NORMAL
// V04-00-26 : 2005.03.21 : ADD CASH RESULT
// V04-00-28 : 2005.03.22 : (TO) EDIT TRANSFER ACCOUNT, CARD INFORMATION UPDATE, SERIAL NO CHECK
// V04-01-00 : 2005.03.25 : CARDPAN
// V04-01-01 : 2005.03.30 : DUMMY DEVICE OPEN/CLOSE EVENT CHECK
// V04-01-02 : 2005.03.30 : CHECKDEVICEACTION TIMEOVER
// V04-01-03 : 2005.04.03 : BEFORE CHECKDEVICEACTION TO ERROR CLEAR
// V04-01-04 : 2005.04.06 :	CARD REINSERT
// V04-01-05 : 2005.04.07 :	CWC SEND COUNT LOGIC UPDATE
// V04-01-06 : 2005.04.08 :	CARD OUT RETRY
// V04-01-07 : 2005.04.19 :	VFD,SPL UPDATE
// V04-01-08 : 2005.04.19 :	CASH COUNT RETRY UPDATE
// V04-02-00 : 2005.05.18 :	DB FTP
// V04-02-01 : 2005.05.26 :	DOOR, CDU POSITION, BID, EJR
// V04-03-00 : 2005.06.15 :	DVR, FTP SEND EJR FILE
// V04-03-01 : 2005.06.20 :	CWC MAC MISMATCH OPEN, TRAN AFTER STATUS
// V04-03-02 : 2005.06.29 :	NMS
// V04-03-03 : 2005.07.21 :	CLERK MODE BID SKIP
// V04-03-04 : 2005.07.26 :	HOST CTTL 
// V04-03-05 : 2005.08.22 :	BCTC
// V04-03-06 : 2005.08.31 :	BCTC MODIFY
// V04-03-07 : 2005.09.01 :	BCTC MODIFY
// V04-03-08 : 2005.09.11 :	BCTC MODIFY
// V04-04-01 : 2005.09.11 :	BCTC MODIFY
// V04-04-02 : 2005.10.07 :	SHAREDATA LOCK PROCESS
// V04-04-03 : 2005.10.14 :	MAC CHECK SKIP(HOST NG)
// V04-04-04 : 2005.11.07 :	CDU AUTO-OFF, OS REBOOT
// V04-04-05 : 2005.11.11 :	MB2500N, MBTTW, OP(OPTION), MCU(OPTION)
// V04-04-06 : 2005.12.01 :	MWIOP
// V04-04-07 : 2006.02.09 :	DEVICE STATUS SAVE(OPEN/CLOSE SHUTTER), BCTC(BJ)
// V04-04-08 : 2006.03.21 :	T-DES
// V04-04-09 : 2006.04.19 :	PIN CHECK (REBOOTING)
// V04-04-10 : 2006.05.10 :	DVR STATUS
// V04-04-11 : 2006.05.18 :	HOST
// V04-04-12 : 2006.05.20 :	JPR PRINT
// V05-01-01 : 2006.07.01 :	NMS
// V05-01-02 : 2006.07.03 :	NMS
// V05-01-03 : 2006.08.30 :	ASC2FLOAT
// V05-01-04 : 2006.09.06 :	SPRPRINT
// V05-01-05 : 2006.10.08 :	EATMTTW(G-CDU)

#define	VERSION				"V06-09-01"
#define	VERSIONHOST			"20130917060901"

#define VERSIONBASE			"000"								// 基础版本号
#define VERSIONBEIJING		"001"								// 北京版本号
#define VERSIONHUNAN		"002"								// 湖南版本号
#define VERSIONZHEJIANG		"003"								// 浙江版本号


/////////////////////////////////////////////////////////////////////////////
//	Cash Condition Amount Define
/////////////////////////////////////////////////////////////////////////////
#define CSH_M10050_MAXOUT	4000					// MONEY 100/50 MAX OUT : 2007.06.06
#define CSH_M10050_MINOUT	50						// MONEY 100/50 MIN OUT
#define CSH_M10050_SELECT	2000					// MONEY 100/50 SELECT(MAX) : 2004.11.05 //liyi 2008.05.16
#define CSH_M2010_MAXOUT	400						// MONEY 20/10  MAX OUT : 2004.11.05
#define CSH_M2010_MINOUT	10						// MONEY 20/10  MIN OUT
#define CSH_M2010_SELECT	400						// MONEY 20/10  SELECT(MAX) : 2004.11.05

#define CSH_MONEY_TMAXOUT	9999999999.99			// MONEY TRANSFER MAX OUT
#define CSH_MONEY_TMINOUT	50.00		         	// MONEY TRANSFER MIN OUT

#define _CARDFIT_DAT		"C:\\T1ATM\\APP\\EXERELE\\CARDFIT.DAT"
#define _MONITOR_INI_DAT	"C:\\T1ATM\\APP\\EXERELE\\MONITOR.INI"

/////////////////////////////////////////////////////////////////////////////
//	Line Time Define
/////////////////////////////////////////////////////////////////////////////
#define LINE_OPCL_TIME		6						// LINE OPEN/CLOSE TIME
#define LINE_RETRY_TIME		300						// HOST OPEN RETRY TIME
#define LINE_SEND_TIME		90						// HOST SEND TIME
#define LINE_RECV_TIME		90						// HOST RECV TIME

#define LINE_BID_OPCL_TIME	6						// LINE OPEN/CLOSE TIME(BID)
#define LINE_BID_SEND_TIME	15						// HOST SEND TIME(BID)
#define LINE_BID_RECV_TIME	15						// HOST RECV TIME(BID)
#define LINE_MSR_SEND_TIME	300						// HOST MSR RETRY TIME

/////////////////////////////////////////////////////////////////////////////
//	Operation Time
/////////////////////////////////////////////////////////////////////////////
#define KEYIN_TIME_OUT		30						// KEY IN TIME OUT
#define TAKE_SLIP_TIME_OUT	30						// TAKE SLIP TIME OUT
#define TAKE_CARD_TIME_OUT	30						// TAKE CARD TIME OUT
#define TAKE_CASH_TIME_OUT	30						// TAKE CASH TIME OUT

#define KEYIN_TIME_OUT_MIN		30					// KEY IN TIME OUT MIN
#define TAKE_SLIP_TIME_OUT_MIN	30					// TAKE SLIP TIME OUT MIN
#define TAKE_CARD_TIME_OUT_MIN	30					// TAKE CARD TIME OUT MIN
#define TAKE_CASH_TIME_OUT_MIN	30					// TAKE CASH TIME OUT MIN

/////////////////////////////////////////////////////////////////////////////
//	CWC Retry Count
/////////////////////////////////////////////////////////////////////////////
#define CWC_RETRY_COUNT		5						// CWC RETRY COUNT

/////////////////////////////////////////////////////////////////////////////
//	Key Mode Define
/////////////////////////////////////////////////////////////////////////////
#define SINGLE_DES			0						// Single Des
#define TRIPLE_DES			1						// Triple Des

//add by hyl 20070902
#define TCONSTMASTERKEY		"TCONSTMASTERKEY"		// Triple Const Master Key
#define TMASTERKEY			"TMASTERKEY"			// Triple Master Key
#define TPINKEY				"TPINKEY"				// Triple Pin Key
#define TMACKEY				"TMACKEY"				// Triple Mac Key
#define SCONSTMASTERKEY		"SCONSTMASTERKEY"		// Single Const Master Key
#define SMASTERKEY			"SMASTERKEY"			// Single Master Key
#define SPINKEY				"SPINKEY"				// Single Pin Key
#define SMACKEY				"SMACKEY"				// Single Mac Key

/////////////////////////////////////////////////////////////////////////////
//	AP Profile Define
/////////////////////////////////////////////////////////////////////////////
#define _TRANSACTION_INI					"D:\\INI\\TRANSACTION.INI"	// 2004.11.05
#define _INIKEY_TRANSACTION					_T("TRANSACTION")
#define	_INIKEY_TRANSACTION_TRANPROC		_T("TRANPROC")
#define	_INIKEY_TRANSACTION_TRANCODE		_T("TRANCODE")
#define	_INIKEY_TRANSACTION_TRANDATA		_T("TRANDATA")
#define	_INIKEY_TRANSACTION_TRANERROR		_T("TRANERROR")
#define	_INIKEY_TRANSACTION_TRANCUSTOM		_T("TRANCUSTOM")			// 1: Card TimeOut, 2: Cash TimeOut, 3: Host Retract Card
#define	_INIKEY_TRANSACTION_TRANCARD		_T("TRANCARD")
#define	_INIKEY_TRANSACTION_TRANPB			_T("TRANPB")
#define	_INIKEY_TRANSACTION_TRANKIND		_T("TRANKIND")
#define	_INIKEY_TRANSACTION_TRANSERIALNO	_T("TRANSERIALNO")
#define	_INIKEY_TRANSACTION_TRANDATETIME	_T("TRANDATETIME")
#define	_INIKEY_TRANSACTION_TRANFIELD32		_T("TRANFIELD32")
#define	_INIKEY_TRANSACTION_TRANFIELD33		_T("TRANFIELD33")

#define _INIKEY_TRANSACTION_SPRJPRFLAG		_T("SPRJPRFLAG")   //yaokq add 20081029

#define	_INIKEY_TRANSACTION_HEADADVERSION	_T("HEADADVERSION")
#define	_INIKEY_TRANSACTION_BRANCHADVERSION	_T("BRANCHADVERSION")
#define	_INIKEY_TRANSACTION_POUNDAGEVERSION	_T("POUNDAGEVERSION")
#define	_INIKEY_TRANSACTION_PARMVERSION		_T("PARMVERSION")
#define	_INIKEY_TRANSACTION_FITVERSION		_T("FITVERSION")
#define	_INIKEY_TRANSACTION_PAYVERSION		_T("PAYVERSION")
#define	_INIKEY_TRANSACTION_FITPARM			_T("FITPARM")
#define	_INIKEY_TRANSACTION_ICFITPARM		_T("ICFITPARM")		// added by yusy ic
#define _INIKEY_TRANSACTION_AIDLISTPARM		_T("AIDLISTPARM")
#define	_INIKEY_TRANSACTION_ECHANNELVERSION	_T("ECHANNELVERSION")
//#define	_INIKEY_TRANSACTION_ECHVERSION		_T("ECHVERSION")
#define _INIKEY_TRANSACTION_ECHANNELPARM	_T("ECHANNELPARM")
#define _INIKEY_TRANSACTION_ECHANNELPROT	_T("ECHANNELPROT")
#define _INIKEY_TRANSACTION_SCRIPTLIST		_T("SCRIPTLIST")
#define	_INIKEY_TRANSACTION_PAYPARM			_T("PAYPARM")
#define	_INIKEY_TRANSACTION_ICFITVERSION	_T("ICFITVERSION")	// added by yusy ic
#define	_INIKEY_TRANSACTION_AIDLISTVERSION  _T("AIDLISTVERSION")
#define _INIKEY_TRANSACTION_PBOCPARAVERSION _T("PBOCPARAVERSION")
#define _INIKEY_TRANSACTION_TRANFIELD55		_T("TRANFIELD55")	// added by liuxl 20110508
#define _INIKEY_TRANSACTION_LANGUAGEMODE	_T("LANGUAGEMODE")	// added by liuxl 20120410
#define _INIKEY_TRANSACTION_ATMPTOTALKIND	_T("ATMPTOTALKIND")
#define _INIKEY_TRANSACTION_TOTALKINDSET	_T("TOTALKINDSET")
#define _INIKEY_TIMEDEFINE_CARDRETAINRTYNUM	_T("CARDRETAINNUM")
#define _INIKEY_TRANSACTION_NOCARDTRANSFLAG	_T("NOCARDTRANSFLAG")	// 是否允许无卡交易

#define _INIKEY_TRANSACTION_WITHDRAWALFLAG	_T("WITHDRAWALFLAG")	// 是否允许取款 added by liuxl 20110601
#define _INIKEY_TRANSACTION_WITHDRAWALFLAG1	_T("WITHDRAWALFLAG1")
#define _INIKEY_TRANSACTION_TRANSFERFLAG	_T("TRANSFERFLAG")		// 是否允许转账 added by liuxl 20111008
#define _INIKEY_TRANSACTION_ECHANNELFLAG	_T("ECHANNELFLAG")	// 是否允许电子渠道
#define _INIKEY_TIMEDEFINE_DOWNLOADECHNUM	_T("DOWNLOADECHNUM")	// 自动下载电子渠道参数失败次数
#define _INIKEY_TRANSACTION_SENDDETAILFLAG	_T("SENDDETAILFLAG")	// 是否允许上送明细
#define _INIKEY_TRANSACTION_DOWNLOADECHRSLT	_T("DOWNLOADECHRSLT")	// 自动下载电子渠道参数结果

#define _INIKEY_RETRACTCARD					_T("RETRACTCARD")
#define _INIKEY_SUSPICIOUS					_T("SUSPICIOUS")

#define _INIKEY_BIDDOWNLOADINFO				_T("BIDDOWNLOADINFO")

#define	_INIKEY_TRANSACTION_WITHFLAG		_T("WITHFLAG")
#define	_INIKEY_TRANSACTION_WITHDATA		_T("WITHDATA")
#define	_INIKEY_TRANSACTION_WITHERROR		_T("WITHERROR")
#define	_INIKEY_TRANSACTION_WITHSUM			_T("WITHSUM")
#define	_INIKEY_TRANSACTION_WITHCST1		_T("WITHCST1")
#define	_INIKEY_TRANSACTION_WITHCST2		_T("WITHCST2")
#define	_INIKEY_TRANSACTION_WITHCST3		_T("WITHCST3")
#define	_INIKEY_TRANSACTION_WITHCST4		_T("WITHCST4")
#define	_INIKEY_TRANSACTION_CWCCOUNT		_T("CWCCOUNT")
#define	_INIKEY_TRANSACTION_WITHSERIALNO	_T("WITHSERIALNO")
#define	_INIKEY_TRANSACTION_WITHACCOUNT		_T("WITHACCOUNT")
#define	_INIKEY_TRANSACTION_WITHDATETIME	_T("WITHDATETIME")
#define	_INIKEY_TRANSACTION_WITHAMOUNT		_T("WITHAMOUNT")
#define	_INIKEY_TRANSACTION_REFERENCENO		_T("REFERENCENO")
#define	_INIKEY_TRANSACTION_WITHFIELD32		_T("WITHFIELD32")
#define	_INIKEY_TRANSACTION_WITHFIELD33		_T("WITHFIELD33")
#define	_INIKEY_TRANSACTION_CANCELDATETIME	_T("CANCELDATETIME")
#define	_INIKEY_TRANSACTION_CANCELSERIALNO	_T("CANCELSERIALNO")
#define	_INIKEY_TRANSACTION_RESPONSENO		_T("RESPONSENO")
#define	_INIKEY_TRANSACTION_TRANBATCHNO		_T("TRANBATCHNO")
#define	_INIKEY_TRANSACTION_TOTALBATCHNO	_T("TOTALBATCHNO")
#define	_INIKEY_TRANSACTION_WORKPARAMETER	_T("WORKPARAMETER")
#define	_INIKEY_TRANSACTION_PBOCPARAMETER	_T("PBOCPARAMETER")
#define	_INIKEY_TRANSACTION_CAUTIONPAGE		_T("CAUTIONPAGE")
#define	_INIKEY_TRANSACTION_ADDCASHBATCHNO	_T("ADDCASHBATCHNO")
#define	_INIKEY_TRANSACTION_WITHFEE		    _T("WITHFEE")        //取款手续费
#define	_INIKEY_TRANSACTION_WITHLOCALCARDFLAG _T("WITHLOCALCARDFLAG")        //取款手续费

#define _EJR_DETAILSEND						"D:\\EJR\\DETAILSEND"
#define	_INIKEY_TRANSACTION_DETAILBATCHNO	_T("DETAILBATCHNO")
#define	_INIKEY_TRANSACTION_DETAILBEGINP	_T("DETAILBEGINP")
#define	_INIKEY_TRANSACTION_DETAILSENDP		_T("DETAILSENDP")
#define _INIKEY_TRANSACTION_DETAILWRITEP	_T("DETAILWRITEP")
#define _INIKEY_TRANSACTION_DETAILSENDCNT	_T("DETAILSENDCNT")
#define _INIKEY_TRANSACTION_DETAILSENDCURRCNT	_T("DETAILSENDCURRCNT")
#define _INIKEY_TRANSACTION_DCEJRSENDP		_T("DCEJRSENDP")	// 2006.07.03

#define _INIKEY_TRANSACTION_WITHRETRACTCNT	_T("WITHRETRACTCNT")
#define _INIKEY_TRANSACTION_WITHRETRACTAMT	_T("WITHRETRACTAMT")
#define _INIKEY_TRANSACTION_ACMANGER		_T("ACMANGER")

// Bid Setting : 2005.08.22
//#define _INIKEY_TIMEDEFINE_WITHMINAMOUNT	_T("WITHMINAMOUNT")

// Ap Manual Setting : 2005.08.22
#define _INIKEY_TIMEDEFINE_SENDTIMEOUT		_T("SENDTIMEOUT")
#define _INIKEY_TIMEDEFINE_OPENRETRYTIME	_T("OPENRETRYTIME")
#define _INIKEY_TIMEDEFINE_LINERETRYTIME	_T("LINERETRYTIME")
#define _INIKEY_TIMEDEFINE_CWCRETRYTIME		_T("CWCRETRYTIME")         //冲正自动重发次数
#define _INIKEY_TIMEDEFINE_TRANRETRYTIME	_T("TRANRETRYTIME")
#define _INIKEY_TIMEDEFINE_CARDACCEPTORID	_T("CARDACCEPTORID")
#define _INIKEY_TIMEDEFINE_CARDACCEPTORP	_T("CARDACCEPTORP")
#define _INIKEY_TIMEDEFINE_TRANSFERFEE		_T("TRANSFERFEE")

// 20070626
// NMU SETTING :20060626
#define _INIKEY_TRANSACTION_SOFTUPDATEFILEUSETIME    _T("SOFTUPDATEFILEUSETIME")     //软件升级时间
#define _INIKEY_TRANSACTION_SOFTUPDATEFILELIST       _T("SOFTUPDATEFILELIST")        //软件升级列表
#define _INIKEY_TRANSACTION_SOFTTEMPFILELENGTH       _T("SOFTTEMPFILELENGTH")        //已下完的升级文件长度
#define _INIKEY_TRANSACTION_SOFTTEMPFILENAME         _T("SOFTTEMPFILENAME")          //正在下载的升级文件名
#define _INIKEY_TRANSACTION_SOFTTEMPFILENAMELIST     _T("SOFTTEMPFILENAMELIST")      //正在下载的升级文件列表
#define _INIKEY_TRANSACTION_SOFTTEMPFILENAMELISTNUM  _T("SOFTTEMPFILENAMELISTNUM")   //下载的升级文件数量
#define _INIKEY_TRANSACTION_HOSTDOWNLOADVERSION      _T("HOSTDOWNLOADVERSION")       //升级文件附带前置下传的版本
#define _INIKEY_TRANSACTION_HOSTUPDATEVERSION        _T("HOSTUPDATEVERSION")         //升级完成的前置下传的版本
#define _INIKEY_TRANSACTION_ADTEMPFILELENGTH         _T("ADTEMPFILELENGTH")				//当前文件已下载的长度        
#define _INIKEY_TRANSACTION_ADTEMPFILENAME           _T("ADTEMPFILENAME")				//当前下载的文件名
#define _INIKEY_TRANSACTION_ADTEMPFILENAMELIST       _T("ADTEMPFILENAMELIST")			//当前下载的文件列表
#define _INIKEY_TRANSACTION_ADPICTURENUM             _T("ADPICTURENUM")				//实际的广告数
#define _INIKEY_TRANSACTION_ADTEMPFILENAMELISTNUM    _T("ADTEMPFILENAMELISTNUM")		//下载列表中的当前下载位置
#define _INIKEY_TRANSACTION_ADPICTUREUSETIME         _T("ADPICTUREUSETIME")			//广告的启用时间  
#define _INIKEY_TRANSACTION_ADPICTUREENDTIME         _T("ADPICTUREENDTIME")			//广告的到期时间
#define _INIKEY_TRANSACTION_ADPICTURELIST            _T("ADPICTURELIST")        
#define _INIKEY_TRANSACTION_ADPICTURE                _T("ADPICTURE")          
#define _INIKEY_TRANSACTION_NOTICEPICTURENUM         _T("NOTICEPICTURENUM")
#define _INIKEY_TRANSACTION_NOTICEPICTUREDOWNLOADNUM _T("NOTICEPICTUREDOWNLOADNUM")
#define _INIKEY_TRANSACTION_NOTICEPICTRUEBACKNUM     _T("NOTICEPICTRUEBACKNUM")
#define _INIKEY_TRANSACTION_NOTICETEMPFILENAMELISTNUM    _T("NOTICETEMPFILENAMELISTNUM")
#define _INIKEY_TRANSACTION_NOTICEPICTUREUSETIME      _T("NOTICEPICTUREUSETIME")
#define _INIKEY_TRANSACTION_NOTICEPICTUREENDTIME      _T("NOTICEPICTUREENDTIME")
#define _INIKEY_TRANSACTION_NOTICEPICTURETEMPLIST     _T("NOTICEDOWNTEMPLIST")
#define _INIKEY_TRANSACTION_NOTICETEMPFILENAME        _T("NOTICETEMPFILENAME")
#define _INIKEY_TRANSACTION_NOTICETEMPFILELENGTH      _T("NOTICETEMPFILELENGTH")
#define _INIKEY_TRANSACTION_NOTICETEMPFILENAMELIST    _T("NOTICETEMPFILENAMELIST")			//当前下载的文件列表
// added by yusy 2010.03.22
#define _INIKEY_TRANSACTION_CAUTIONPICTURENUM         _T("CAUTIONPICTURENUM")
#define _INIKEY_TRANSACTION_CAUTIONPICTUREDOWNLOADNUM _T("CAUTIONPICTUREDOWNLOADNUM")
#define _INIKEY_TRANSACTION_CAUTIONPICTRUEBACKNUM     _T("CAUTIONPICTRUEBACKNUM")
#define _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELISTNUM    _T("CAUTIONTEMPFILENAMELISTNUM")
#define _INIKEY_TRANSACTION_CAUTIONPICTUREUSETIME      _T("CAUTIONPICTUREUSETIME")
#define _INIKEY_TRANSACTION_CAUTIONPICTUREENDTIME      _T("CAUTIONPICTUREENDTIME")
#define _INIKEY_TRANSACTION_CAUTIONPICTURETEMPLIST     _T("CAUTIONPICTURETEMPLIST")
#define _INIKEY_TRANSACTION_CAUTIONTEMPFILENAME        _T("CAUTIONTEMPFILENAME")
#define _INIKEY_TRANSACTION_CAUTIONTEMPFILELENGTH      _T("CAUTIONTEMPFILELENGTH")
#define _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELIST    _T("CAUTIONTEMPFILENAMELIST")			//当前下载的文件列表
#define _INIKEY_TRANSACTION_OBTRANINQ    _T("OBTRANINQ")										//跨行转账户名查询

#define _INIKEY_TRANSACTION_TACDENIAL				   _T("TACDENIAL")		// 终端行为代码拒绝
#define _INIKEY_TRANSACTION_TACONLINE				   _T("TACONLINE")		// 终端行为代码联机
#define _INIKEY_TRANSACTION_TACDEFAULT	               _T("TACDEFAULT")		// 终端行为代码缺省
#define _INIKEY_TRANSACTION_CWDCURSHUTTERNUM			_T("CWDCURSHUTTERNUM")	// 当前取款出钞门打开失败次数 added by liuxl 20110601
#define _INIKEY_TRANSACTION_CASHBOXOPTFLAG				_T("CashBoxOptFlag")	// 钞箱状态
#define _INIKEY_TRANSACTION_MAINTAINSTATUSFLAG			_T("MaintainStatusFlag")// 状态报文上送
#define _INIKEY_TRANSACTION_SWITCHCDKFLAG				_T("SwitchCDKFlag")		// CDK校验
#define _INIKEY_TRANSACTION_STATUSFLAG					_T("StatusFlag")	// 上送状态24域
#define _INIKEY_TRANSACTION_TIMEFILTERFLAG				_T("TimeFilterFlag")	// 定时优化标志
#define _INIKEY_TRANSACTION_REPORTSTARTTIME				_T("ReportStartTime")	// 如实状态报告开始时间
#define _INIKEY_TRANSACTION_REPORTENDTTIME				_T("ReportEndTime")		// 如实状态报告结束时间
#define _INIKEY_TIMEDEFINE_POUNDAGESETNUM					_T("POUNDAGESETNUM")    // 资费标准画面数量
#define _INIKEY_TIMEDEFINE_ECHANNELPSETNUM					_T("ECHANNELPSETNUM")    // 电子协议画面数量
#define _INIKEY_TIMEDEFINE_TRANSDAYMAXAMOUNT			_T("TRANSDAYMAXAMOUNT")      // 当日行内转账上限

// 20070907
// setparam by hly
#define _INIKEY_TIMEDEFINE_ADSETNUM			_T("ADSETNUM")            //广告画面数量
#define _INIKEY_TIMEDEFINE_ADTIME			_T("ADTIME")              //广告播放时间
                                                                      //交易超时
#define _INIKEY_TIMEDEFINE_KEYINTIMEOUT		_T("KEYINTIMEOUT")        //无操作等待
                                                                      //超时吞卡时间
                                                                      //超时吞钞时间
                                                                      //定时状态间隔
#define _INIKEY_TIMEDEFINE_DETAILNUM		_T("DETAILNUM")           //查询明细条数
#define _INIKEY_TIMEDEFINE_ERRORNOTICETIME	_T("ERRORNOTICETIME")     //异常通知次数
#define _INIKEY_TIMEDEFINE_AUTORERETRYTIME	_T("AUTORERETRYTIME")     //自动重发次数
                                                                      //自动冲正次数
                                                                      //单笔取款最大
                                                                      //单笔取款最小
#define _INIKEY_TIMEDEFINE_TRANMAXREMITTANCE _T("TRANMAXREMITTANCE")  //单笔汇款最大
#define _INIKEY_TIMEDEFINE_TRANMINREMITTANCE _T("TRANMINREMITTANCE")  //单笔汇款最小
                                                                      //单笔转帐最大
#define _INIKEY_TIMEDEFINE_TRANMINAMOUNT	_T("TRANMINAMOUNT")       //单笔转帐最小
#define _INIKEY_TIMEDEFINE_INPUTREMAINAMOUNT	_T("INPUTREMAINAMOUNT")      //手工输入钞箱

// added by liuxl 20120306
#define _INIKEY_LOTTERY	_T("LOTTERY")									// 抽奖信息
#define _INIKEY_TRANSACTION_LOTTERYSTARTDATE _T("LOTTERYSTARTDATE")		// 抽奖启始日期
#define _INIKEY_TRANSACTION_LOTTERYENDDATE _T("LOTTERYENDDATE")			// 抽奖结束日期
#define _INIKEY_TRANSACTION_LOTTERYSTARTTIME _T("LOTTERYSTARTTIME")		// 抽奖启始时间
#define _INIKEY_TRANSACTION_LOTTERYENDTIME _T("LOTTERYENDTIME")			// 抽奖结束时间
#define _INIKEY_TRANSACTION_LOTTERYWINNUM _T("LOTTERYWINNUM")			// 周期抽奖名额
#define _INIKEY_TRANSACTION_LOTTERYWINREMNUM _T("LOTTERYWINREMNUM")		// 周期剩余名额
#define _INIKEY_TRANSACTION_LOTTERYTRANCODE _T("LOTTERYTRANCODE")		// 抽奖交易类型
#define _INIKEY_TRANSACTION_LOTTERYWINSERIAL _T("LOTTERYWINSERIAL")		// 中奖流水末位
#define _INIKEY_TRANSACTION_LOTTERYSETDATE _T("LOTTERYSETDATE")			// 上次设定日期

/////////////////////////////////////////////////////////////////////////////
//	Atm Dvr Define
/////////////////////////////////////////////////////////////////////////////
#define _INIKEY_TIMEDEFINE_READINTERVALTIMEOUT          _T("READINTERVALTIMEOUT")           //读字符间隔超时时间
#define _INIKEY_TIMEDEFINE_READTOTALTIMEOUTMULTIPLIER   _T("READTOTALTIMEOUTMULTIPLIER")    //读操作时每字符的时间
#define _INIKEY_TIMEDEFINE_READTOTALTIMEOUTCONSTANT     _T("READTOTALTIMEOUTCONSTANT")      //基本的(额外的)读超时时间
#define _INIKEY_TIMEDEFINE_WRITETOTALTIMEOUTMULTIPLIER  _T("WRITETOTALTIMEOUTMULTIPLIER")   //写操作时每字符的时间
#define _INIKEY_TIMEDEFINE_WRITETOTALTIMEOUTCONSTANT    _T("WRITETOTALTIMEOUTCONSTANT")     //基本的(额外的)写超时时间
#define _INIKEY_TRAN_OPENCOMM                           _T("OPENCOMM")
#define _INIKEY_DVRCOMMDATA				                  _T("DVRCOMMDATA")
//#define _INIKEY_TRAN_DVRSTATUS                           _T("DVRSTATUS")
//#define _INIKEY_TRAN_CHANSTATUS                          _T("CHANSTATUS")
#define _INIKEY_TRAN_DVRMODE                             _T("DVRMODE")    // 0:无DVR,1:有DVR
#define _INIKEY_TRAN_DVRSTATUSFLAG						_T("DVRSTATUSFLAG")					// added by liuxl 20110926
#define	TIMER_DVR_THREAD		3						// TIMER THREAD
#define _INIKEY_TIMEDEFINE_DVRIDLETIME	             	_T("DVRIDLETIME")
#define _INIKEY_TRAN_ACMODE                             _T("ACMODE")    // 0:无账户管家,1:有账户管家

#define DVRNOR              0000//(0x30303030)为正常，
#define DVRERR1             0001//(0x30303031)为ATM周边环境报警
#define DVRERR2             0002//(0x30303032)系统不稳定，
#define DVRERR4             0004//(0x30303034)为硬盘满或出问题，
#define DVRERR8             0008//(0x30303038)其它情况，	
#define DVRERR10             0010// (0x30303130) 接近
#define DVRERR20             0020// (0x30303230)出钞
#define DVRERR40             0040// (0x30303430)装钞
#define DVRERR80             0080// (0x30303830) 震动

#define CHANNOR             0000//(0x30303030)四个摄像头正常，
#define CHANNOW             1111//(0x30303031)第四个摄像头正录象，
#define CHANLOST            2222//(0x30303032)为第四个摄像头视频丢失。 

/////////////////////////////////////////////////////////////////////////////
//ATM DVR Define
#define CMDCODE_INCARD				1							// 插卡命令，接收卡号，叠加卡号字符
#define CMDCODE_TAKECARD			2							// 退卡命令，交易结束，卡号叠加结束
#define CMDCODE_OVERLAY				3							// 用户操作查询，取款等，叠加交易类型
#define CMDCODE_SETTIME				4							// 按照ATM机系统时间校时
#define CMDCODE_GETSTATUS           5							// ATM机查询ATM DVR工作状态
#define CMDCODE_RETAINCARD			6							// 吞卡，即回收卡
#define CMDCODE_RETAINCASH			7							// 吞钞，即回收钞票

#define DVRCHAN_IDX0          0000
#define DVRCHAN_IDX12         1100
#define DVRCHAN_IDX123        1110
#define DVRCHAN_IDX124        1101
#define DVRCHAN_IDX1234       1111

#define DVR_TIMEOUT    999 
#define DVR_GETSTATUS_TIMEOUT    000     

#define	TIMER_mMIN1	    	60000		// TIMER MODE 60s
#define	TIMER_mMIN5	    	300000		// TIMER MODE 300s  

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//ATM AGENT RETURN Define 
#define AGENT_SUCCCESS        '1'          
#define AGENT_FAIL            '0'
#define AGENT_NOOPEN          '2'
#define AGENT_INTRAN          '3'
#define AGENT_WAINTTRAN       '4'
#define AGENT_PAUSE           '5'
#define AGENT_ERROR           '6'
#define AGENT_CLERK           '7'
#define AGENT_NOSUPPORT       '8'
#define AGENT_INORDER         '9'
#define AGENT_OTHER           'A'
#define AGENT_FILEINEXIST     'B'

/////////////////////////////////////////////////////////////////////////////
//	Ap Reg Define
/////////////////////////////////////////////////////////////////////////////
#define _AP_EJRTEMP							"EJRTEMP.TXT"
#define	_REGKEY_DEVICE_DAYTOTALCNT			_T("DAYTOTALCNT")
#define	_REGKEY_DEVICE_TOTALDATE			_T("TOTALDATE")

/////////////////////////////////////////////////////////////////////////////
//	SP Reg Define
/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_PINPADSTATUS				_T("SOFTWARE\\ATM\\DEVINFO\\PINPAD")
#define	_REGKEY_PINPADSTATUS_KEYMODE		_T("KEYMODE")

/////////////////////////////////////////////////////////////////////////////
//	Transaction Mode Define
/////////////////////////////////////////////////////////////////////////////
#define TRAN_IDLE 			0						// IDLE
#define TRAN_READY			10						// READY
#define TRAN_OPEN			11						// OPEN				(RQK)			
#define TRAN_CANCEL			12						// CANCEL			(CWC)
#define TRAN_ERROR			13						// ERROR			(OEX)
#define TRAN_AEXSTATUS		14						// AEXSTATUS		(AEX)
#define TRAN_OEXSTATUS		15						// OEXSTATUS		(OEX)
#define TRAN_MSRSTATUS		16						// MSRSTATUS		(MSR)
#define TRAN_BIDCOMMAND		17						// BIDCOMMAND		(BID)
#define TRAN_TRAN			20						// TRANSACTION		(INQ,TFR,CWD,PIN，A/C)
#define TRAN_TOTAL			40						// TOTAL			(TOT)
#define TRAN_SUBTOTAL		41						// SUBTOAL
#define TRAN_ADDCASH		42						// ADDCASH			(ADD)
#define TRAN_SUBCASH		43						// SUBCASH			(ADD)
#define TRAN_CLEARCASH		44						// CLEARCASH		(ADD)
#define TRAN_PRINTCASH		45						// PRINTCASH
#define TRAN_DOWNFIT		46						// DOWNFIT
#define TRAN_SENDDETAIL		47						// SENDDETAIL
#define TRAN_DETAILEND		48						// DETAILEND
#define TRAN_CONFIRMWITH	49						// CONFIRMWITH
#define TRAN_DCSENDEJR		50						// DCSENDEJR : 2006.07.03
#define TRAN_DCEJREND		51						// DCEJREND	 : 2006.07.03
#define TRAN_DOWNPARM		52						// DOWNLOAD PARM   by zjd
#define TRAN_DOWNPAY		53						// DOWNLOAD PAY    by zjd
#define TRAN_INITCDK		54						// INIT CDK        by zjd
#define TRAN_INITMAC		55						// INIT MAC		   by zjd
#define TRAN_INITPIN		56						// INIT PIN		   by zjd
#define TRAN_DOWNICFIT		57						// DOWN IC FIT     by yusy ic
#define TRAN_DOWNAIDLIST	58						// DOWN AID LIST
#define TRAN_DOWNPBOCPARA   59						// DOWN PBOC PARA
#define TRAN_SCRIPT			60
#define TRAN_DOWNECHANNELPARA	61
#define TRAN_DOWNECHANNELPROT	62
#define TRAN_CARDRETAIN		63						// CARD RETAIN SEND
#define TRAN_TOTALKIND		64						// TOTAL KIND
#define TRAN_TOTALPRE		65						// TOTAL PRE

/////////////////////////////////////////////////////////////////////////////
//	PassWord Input Mode
/////////////////////////////////////////////////////////////////////////////
#define	PIN_PASSWORD_MIN			4          //by hyl
#define	PIN_PASSWORD_MAX			6         
#define	PIN_PASSWORD_AUTO_FALSE		FALSE
#define	PIN_PASSWORD_AUTO_TRUE		TRUE
// #define	PIN_PASSWORD_TERM			"ENTER,CANCEL,F7,F8"
#define	PIN_PASSWORD_TERM			"ENTER,CANCEL,F8"  //中断键只能用F8

/////////////////////////////////////////////////////////////////////////////

#endif
