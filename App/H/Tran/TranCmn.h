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

	int		iDEBUGFLAG;					// ����ģʽ��־  by zjd
	char	szADDCashCounterNumber[7];	// ������Ĺ�Ա�� by zjd
	char	totalKindFlag;				// ���˷�ʽ
	int     ACInqFlag;                  // �˻��ܼ��Ƿ񷢲�ѯ����֤������ 0:δ���ͣ�1���ѷ���
    int     PayAssuCn;                    // 5507���ص�Ԥ���ɷѼ�¼��
	int     UnPayAssuCn;                  // 5507���صķ�Ԥ���ɷѼ�¼��
	int     RemitCn;                      // 5507���صĻ�����ͼ�¼��
	int     UnAssuPayOpenNum;             // ��ͨ��Լ���ɷѵ���Ŀ�����Ϊ0,����Լ���ɷ���ĿΪ0��5507����ɷѲ���ͨ

	// 20080905 by yaokq
	int			ZzhTotalNum;			//���˻�����Ŀ
	int			ZzhCurNum;				//�Ѳ����˻���Ŀ

	CString	strVersion;											// 060411 �ϲ��汾
	CString	strRebootTimeStart;									// 060411 ����ʱ�����
	CString	strRebootTimeEnd;									// 060411 ����ʱ�����
	CString strRebootTime;										// 060502 �������ʱ��

	int		transInqCnt;					// ת�˲�ѯ��������
	int		LangMode;											// ��ˮ����ģʽ added by liuxl 20120410
	int		iFstPIN;											// �״����ܱ�־

	CString		tacDenial;				//�ն���Ϊ����ܾ�
	CString		tacOnline;				//�ն���Ϊ��������
	CString		tacDefault;				//�ն���Ϊ����ȱʡ
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
	int		KeyInTimeOut;				// Key In Time Out  �޲����ȴ�ʱ��
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
	char	ACCounterNumber[8];		// �˻��ܼң����й�Ա��
    
	// add by hyl
	double	TransferMinAmt;				// Transfer Min Amount
	int     ADSetNum;                   // AD PICTURE NUMBER
	int     ADTime;                     // AD TIME 
	int     DetailNum;                  // Inquire Detail Number
	int     ErrorNoticeTime;            // �쳣֪ͨ����
    int     AutoRetryTime;              // �Զ��ط�����(���)
    double  TranMaxRemittance;          // ���ʻ�����
	double  TranMinRemittance;          // ���ʻ����С

	int     TranDvrMode;

	char    DvrStatus[5];              // DVR ״̬
 	char    DvrChanStatus[5];          // DVR ͨ��״̬
	// 20080903 by yaokq
	double		ZztohMaxNum;			//����ת��������޶�
	double		ZztohMinNum;			//����ת������С�޶�
	double		DhtohMaxNum;			//����ת��������޶�
	double		DhtohMinNum;			//����ת������С�޶�
	double		TzcktohMaxNum;			//֪ͨ���ת��������޶�
	double		TzcktohMinNum;			//֪ͨ���ת������С�޶�
	double		HtodhMaxNum;			//��ת��������޶�
	double		HtodhMinNum;			//��ת������С�޶�
	double		HtotzcqMaxNum;			//��ת֪ͨ�������޶�
	double		HtotzcqMinNum;			//��ת֪ͨ�����С�޶�
	double		HtozzMaxNum;			//��ת��������޶�
	double		HtozzMinNum;			//��ת������С�޶�
	double		SaveKindPara;			//�����������
    
	// added by yusy ic
	double		nIcLoadMinNum;			//Ȧ����С���
	double		nIcLoadMaxNum;			//Ȧ�������

} InitInfo;

struct	tagCwcInfo						// added by liuxl 20110714
{
	char		TranKind[6];			// ԭ���״�����
	char		TranDate[8];			// ԭ��������
	char		TranTime[6];			// ԭ����ʱ��
	char		TranSerialNo[8];		// ԭ������ˮ��
	char		TranAmount[12];			// ԭ���׽��
	char		ICSerialNo[3];			// IC�����к�
	char		CancelSerialNo[8];		// ԭ������ˮ��
	char		TranAccount[128];		// ԭ�����˺�
	int			TranAccountLen;			// �˺ų���
	char		ICCwcData[1024];		// IC����������
	int			ICCwcDataLen;			// ���ݳ���
	char		TranExpInfo[256];		// ��չ��Ϣԭ����
	int			TranExpInfoLen;			// ԭ���볤��
	int			CwcFlag;				// ������־λ
	int			CwcRetryCnt;			// ��������

} CwcInfo;

struct	tagCardRetainData
{
	char		TranDateTime[14];		// ԭ��������ʱ��
	char		CardRetainKind;			// �̿���ʽ Ĭ��Ϊ1
	char		CardRetainInfo[2];		// �̿�ԭ�� 00:��ʱ, 01:����, XX:����������
	char		TranAccount[128];		// ԭ�����˺�
	char		TranType;				// ���뷽ʽ
	int			TranAccountLen;			// �˺ų���
	int			CardRetFlag;			// �̿���־λ
	int			CardRetCnt;				// �̿�����
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
	//���   BY HYL  �Ƶ�TranAtmpProc.h��   by zjd
//	long    SendFileLength;            //�ļ�����
//	long    DownFilelength;
//	int     SendBufferLength;       //�ϴ����ݰ�����
//	long    SetBegin;                  //ƫ����
//	char    DetailTblTemp[400];      //���ݳ���
 	char    szDownLoadFileName[12];
//	char    FileEnd;
    
	char     InputRemainAmount[12];
	
	CString AccountType;				// Custom Select AccountType  1��Ĭ�ϣ�2�����3��֧Ʊ��4������
	CString	strTFRInaccountType;		// ת��ѡ���ת�����ͣ�2Ϊ����1Ϊ��

	CString strTFRSubType;				// liyi add �̿�ͨ ת���ʻ����ͣ�1�����2��������3��֪ͨ
	CString strTFRInData;				// liyi add �̿�ͨ ����
	CString strTFRSubNo;				// liyi add �̿�ͨ �����������ʺ�
// A/C Manager
	CString ACPayType;                  //�ܼ��Žɷ�����

    CString strRemitAddNo;				//�տ������ 2008.10.05
	CString strRemitAddName;			//�տ�������
	CString strRemitAddAdd;				//�տ��˵�ַ

    CString strRemitInNo;

	CString strPayInqRefNo;				// liyi add �ɷ�/��ֵ��ѯ 20110926


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
