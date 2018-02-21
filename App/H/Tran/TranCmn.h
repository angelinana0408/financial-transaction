#if !defined(AFX_TRANCMN_H__94D1FADB_96A6_4E42_A156_4886674FB303__INCLUDED_)
#define AFX_TRANCMN_H__94D1FADB_96A6_4E42_A156_4886674FB303__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TranCmn.h : header file
//
/////////////////////////////////////////////////////////////////////////////
#include "..\Common\Class.h"
#include "TranCmnEtc.h"
#include "TranCmnHost.h"
#include "TranCmnMenu.h"
#include "TranCmnUser.h"
#include "AccApi.h"

#define NMSRECVTBL				8
#define NMSBUFFSIZE				32768

/////////////////////////////////////////////////////////////////////////////
// CTranCmn window

class AFX_EXT_CLASS CTranCmn : public CWnd
{
#include "..\Builder\TranCtrlProc.h"
#include "..\Builder\TranCtrlProc_STA.h"
#include "..\Builder\TranFuncProc.h"
#include "..\Builder\TranFuncProc_STA.h"
#include "..\Builder\TranRetCode.h"
#include "..\Builder\TranTranProc.h"

#include "..\Common\ClassPointer.h"
#include "TranDevProc.h"
#include "TranAtmpProc.h"
#include "TranAgentProc.h"
#include "TranHostProc.h"
#include "TranInputProc.h"
#include "TranLibProc.h"
#include "TranMainProc.h"
#include "TranMenuProc.h"
#include "TranNmsProc.h"
#include "TranOutputProc.h"
#include "TranTermProc.h"
#include "Comm.h"
	
// #include "AccApi.h"
/////////////////////////////////////////////////////////////////////////////
// Construction
public:
	CTranCmn();

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Builder Information Variable
/////////////////////////////////////////////////////////////////////////////
	int		m_nScrNum;					// Screen Number		
	int		m_nRetCode;					// Return Code
	int		setAtmStatus;

	int		iDEBUGFLAG;					// 测试模式标志  by zjd
	char	szADDCashCounterNumber[7];	// 添钞输入的柜员号 by zjd
	char	totalKindFlag;				// 轧账方式
	int     ACInqFlag;                  // 账户管家是否发查询包验证过密码 0:未发送，1：已发送
    int     PayAssuCn;                    // 5507返回的预定缴费记录数
	int     UnPayAssuCn;                  // 5507返回的非预定缴费记录数
	int     RemitCn;                      // 5507返回的汇款类型记录数
	int     UnAssuPayOpenNum;             // 开通非约定缴费的数目，如果为0,并且约定缴费数目为0（5507）则缴费不开通

	// 20080905 by yaokq
	int			ZzhTotalNum;			//子账户总数目
	int			ZzhCurNum;				//已查子账户数目

	CString	strVersion;											// 060411 合并版本
	CString	strRebootTimeStart;									// 060411 重启时间调整
	CString	strRebootTimeEnd;									// 060411 重启时间调整
	CString strRebootTime;										// 060502 随机重启时间

	int		transInqCnt;					// 转账查询户名次数
	int		LangMode;											// 流水语言模式 added by liuxl 20120410
	int		iFstPIN;											// 首次验密标志

	CString		tacDenial;				//终端行为代码拒绝
	CString		tacOnline;				//终端行为代码联机
	CString		tacDefault;				//终端行为代码缺省
/////////////////////////////////////////////////////////////////////////////
//	Initialize Information Variable
/////////////////////////////////////////////////////////////////////////////
struct	tagInitInfo
{
	int		WithMaxAmt;					// Withdraw Max Amount
	int		WithMinAmt;					// Withdraw Min Amount
	double	TransferMaxAmt;				// Transfer Max Amount
	double	WithDayMaxAmt;				// Withdraw Max Amount A Day

	int		UserWithAvail;				// User Withdraw Avail
	int		UserInquiryAvail;			// User Inquiry Avail
	int		UserDetailAvail;			// User Detail Avail
	int		UserTransAvail;				// User Trans Avail
	int		UserChangePinAvail;			// User Change Pin Avail
	int		UserDepAvail;				// User Deposit Avail
	int		UserPbmAvail;				// User Pbm Avail

	int		CwcRetryCnt;				// Cwc Retry Cnt
	int		KeyInTimeOut;				// Key In Time Out  无操作等待时间
	int		TakeSlipTimeOut;			// Take Slip Time Out
	int		TakeCardTimeOut;			// Take Card Time Out
	int		TakeCashTimeOut;			// Take Cash Time Out

	int		LineSendTime;				// Line Send Time
	int		LineRecvTime;				// Line Recv Time
	int		OpenRetryTime;				// Open Retry Time
	int		LineRetryTime;				// Line Retry Time
	int		TransRetryTime;				// Trans Retry Max Time
	int		CwcRetryTime;				// Cwc Retry Time
	int		MsrSendTime;				// Msr Send Time
	int		AgentMsrSendTime;			// Agent Msr Send Time // added by yusy 08.09.09

	int		BidAtmColseFlag;			// Bid Atm Close Flag
	int		TotalDisplayNo;				// Total Display No

	int		KeyMode;					// Key Mode

	char	NmsIp[16];					// Nms Ip
	int		NmsPort;					// Nms Port
	char	ACCounterNumber[8];		// 账户管家，银行柜员号
    
	// add by hyl
	double	TransferMinAmt;				// Transfer Min Amount
	int     ADSetNum;                   // AD PICTURE NUMBER
	int     ADTime;                     // AD TIME 
	int     DetailNum;                  // Inquire Detail Number
	int     ErrorNoticeTime;            // 异常通知次数
    int     AutoRetryTime;              // 自动重发次数(存款)
    double  TranMaxRemittance;          // 单笔汇款最大
	double  TranMinRemittance;          // 单笔汇款最小

	int     TranDvrMode;

	char    DvrStatus[5];              // DVR 状态
 	char    DvrChanStatus[5];          // DVR 通道状态
	// 20080903 by yaokq
	double		ZztohMaxNum;			//整整转活期最大限额
	double		ZztohMinNum;			//整整转活期最小限额
	double		DhtohMaxNum;			//定活转活期最大限额
	double		DhtohMinNum;			//定活转活期最小限额
	double		TzcktohMaxNum;			//通知存款转活期最大限额
	double		TzcktohMinNum;			//通知存款转活期最小限额
	double		HtodhMaxNum;			//活转定活最大限额
	double		HtodhMinNum;			//活转定活最小限额
	double		HtotzcqMaxNum;			//活转通知存款最大限额
	double		HtotzcqMinNum;			//活转通知存款最小限额
	double		HtozzMaxNum;			//活转整整最大限额
	double		HtozzMinNum;			//活转整整最小限额
	double		SaveKindPara;			//储蓄种类参数
    
	// added by yusy ic
	double		nIcLoadMinNum;			//圈存最小金额
	double		nIcLoadMaxNum;			//圈存最大金额

} InitInfo;

struct	tagCwcInfo						// added by liuxl 20110714
{
	char		TranKind[6];			// 原交易处理码
	char		TranDate[8];			// 原交易日期
	char		TranTime[6];			// 原交易时间
	char		TranSerialNo[8];		// 原交易流水号
	char		TranAmount[12];			// 原交易金额
	char		ICSerialNo[3];			// IC卡序列号
	char		CancelSerialNo[8];		// 原冲正流水号
	char		TranAccount[128];		// 原交易账号
	int			TranAccountLen;			// 账号长度
	char		ICCwcData[1024];		// IC卡冲正数据
	int			ICCwcDataLen;			// 数据长度
	char		TranExpInfo[256];		// 扩展消息原因码
	int			TranExpInfoLen;			// 原因码长度
	int			CwcFlag;				// 冲正标志位
	int			CwcRetryCnt;			// 冲正次数

} CwcInfo;

struct	tagCardRetainData
{
	char		TranDateTime[14];		// 原交易日期时间
	char		CardRetainKind;			// 吞卡方式 默认为1
	char		CardRetainInfo[2];		// 吞卡原因 00:超时, 01:故障, XX:主机返回码
	char		TranAccount[128];		// 原交易账号
	char		TranType;				// 输入方式
	int			TranAccountLen;			// 账号长度
	int			CardRetFlag;			// 吞卡标志位
	int			CardRetCnt;				// 吞卡次数
} CardRetainData;
	
typedef struct	tagPayStruct
{
	char		szPayName[11];
	int			iPayNum;
	char		szPayContral[4];

}PayStruct;

typedef struct tagPayMenu
{
	char		szPayName[11];
	int			iPayNum;
}PayMenu;

	PayStruct	stPayCZ[50];					// added by liuxl 20111013
	PayStruct	stPayJF[50];					// added by liuxl 20111013
	int			iPayMenuNumCZ;
	int			iPayMenuNumJF;
	PayMenu		stPayMenuCZ[50];				// added by liuxl 20111013
	PayMenu		stPayMenuJF[50];				// added by liuxl 20111013
	CString     PayType;
	int         PayNum;

	CString			RemitType;					// 2008.10.05
	//监控   BY HYL  移到TranAtmpProc.h中   by zjd
//	long    SendFileLength;            //文件长度
//	long    DownFilelength;
//	int     SendBufferLength;       //上传数据包长度
//	long    SetBegin;                  //偏移量
//	char    DetailTblTemp[400];      //数据长度
 	char    szDownLoadFileName[12];
//	char    FileEnd;
    
	char     InputRemainAmount[12];
	
	CString AccountType;				// Custom Select AccountType  1：默认，2：储蓄，3：支票，4：信用
	CString	strTFRInaccountType;		// 转帐选择的转出类型，2为卡，1为折

	CString strTFRSubType;				// liyi add 绿卡通 转入帐户类型，1：定活，2：整整，3：通知
	CString strTFRInData;				// liyi add 绿卡通 存期
	CString strTFRSubNo;				// liyi add 绿卡通 其它储种子帐号
// A/C Manager
	CString ACPayType;                  //管家婆缴费类型

    CString strRemitAddNo;				//收款人序号 2008.10.05
	CString strRemitAddName;			//收款人姓名
	CString strRemitAddAdd;				//收款人地址

    CString strRemitInNo;

	CString strPayInqRefNo;				// liyi add 缴费/充值查询 20110926


/////////////////////////////////////////////////////////////////////////////
//	Ap Dir & Bat Define
/////////////////////////////////////////////////////////////////////////////
#define _AP_SOFTUPDATE_DIR	         "C:\\SOFTUPDATE"
#define _AP_SOFTBACKUPDATE_DIR	     "C:\\SOFTUPDATEBACK"
#define _AP_ADUPDATE_DIR	         "C:\\T1atm\\App\\Swf\\image\\AD"
#define _AP_ADUPDATEDOWNLOAD_DIR     "C:\\T1atm\\App\\Swf\\image\\ADDOWNLOAD"
#define _AP_ADUPDATEBACK_DIR	     "C:\\T1atm\\App\\Swf\\image\\ADBACK"
#define _AP_NOTICEUPDATE_DIR	     "C:\\T1atm\\App\\Swf\\image\\NOTICE"
#define _AP_NOTICEUPDATEDOWNLOAD_DIR "C:\\T1atm\\App\\Swf\\image\\NOTICEDOWNLOAD"
#define _AP_NOTICEUPDATEBACK_DIR	 "C:\\T1atm\\App\\Swf\\image\\NOTICEBACK"
// added by yusy 2010.03.22
#define _AP_CAUTIONUPDATE_DIR			"C:\\T1atm\\App\\Swf\\image\\CAUTION"
#define _AP_CAUTIONUPDATEDOWNLOAD_DIR	"C:\\T1atm\\App\\Swf\\image\\CAUTIONDOWNLOAD"
#define _AP_CAUTIONUPDATEBACK_DIR		"C:\\T1atm\\App\\Swf\\image\\CAUTIONBACK"
/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Initialize Function
/////////////////////////////////////////////////////////////////////////////
	void	SetOwner(CWinAtmCtrl *pOwner);
	int		fnAPP_InitializeDS(int SerialTranFlag = FALSE);		// Initialize DS
	int		fnAPP_InitializeSerialTranDS();						// Initialize Serial Transaction DS
/////////////////////////////////////////////////////////////////////////////
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTranCmn)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTranCmn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTranCmn)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
class CTranCmnExit
{
public:
	int		TranExitResult;

public:
	CTranCmnExit(int ExitResult)
	{
		TranExitResult = ExitResult;
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANCMN_H__94D1FADB_96A6_4E42_A156_4886674FB303__INCLUDED_)
