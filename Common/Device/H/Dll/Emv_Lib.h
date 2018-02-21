#ifndef _EMV_LIB_H
#define _EMV_LIB_H

#define ENGLISH                 0
#define CHINESE                 1
#include "StdAfx.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EMVCORE_API
#ifdef EMVCORE_EXPORTS
#define EMVCORE_API __declspec(dllexport)
#else
#define EMVCORE_API __declspec(dllimport)
#endif
#endif

#define TYPE_CASH            0x8000    //交易类型(现金)
#define TYPE_GOODS           0x4000    //交易类型(购物)
#define TYPE_SERVICE         0x2000    //交易类型(服务)
#define TYPE_CASHBACK        0x1000    //交易类型(反现)
#define TYPE_INQUIRY         0x0800    //交易类型(查询)
#define TYPE_PAYMENT         0x0400    //交易类型(支付)
#define TYPE_ADMINISTRATIVE  0x0200    //交易类型(管理)
#define TYPE_TRANSFER        0x0100    //交易类型(转账) 
#define TYPE_DEPOSIT         0x0080    //交易类型(存款)

#define MAX_APP_NUM          32      //应用列表最多可存储的应用数
#define MAX_CAPK_NUM         64      //认证公钥表最多可存储的公钥数
#define MAX_CAPKREVOKE_NUM   96      //认证公钥撤回列表的最大个数32*3

#define PART_MATCH           0x00    //ASI(部分匹配)
#define FULL_MATCH           0x01    //ASI(完全匹配)

#define SUPPORT_YES          0x01    //支持
#define SUPPORT_NO           0x00    //不支持

#define ACCNT_TYPE_DEFAULT		0x00
#define ACCNT_TYPE_SAVING		0x10
#define ACCNT_TYPE_DEBIT		0x20
#define ACCNT_TYPE_CREDIT		0x30

#define ICC_USER         0
#define ICC_CL           1         //added by gubh for contactless card

//交易处理函数返回码定义
#define EMV_OK              0      //成功  
#define ERR_EMVRSP         (-1)      //返回码错误
#define ERR_APPBLOCK       (-2)      //应用已锁
#define ERR_NOAPP          (-3)      //卡片里没有EMV应用
#define ERR_USERCANCEL     (-4)      //用户取消当前操作或交易
#define ERR_TIMEOUT        (-5)      //用户操作超时
#define ERR_EMVDATA        (-6)      //卡片数据错误
#define ERR_NOTACCEPT      (-7)      //交易不接受
#define ERR_EMVDENIAL      (-8)      //交易被拒绝
#define ERR_KEYEXP         (-9)      //密钥过期
#define ERR_NOPINPAD       (-10)     //没有密码键盘或键盘不可用
#define ERR_NOPIN          (-11)     //没有密码或用户忽略了密码输入
#define ERR_CAPKCHECKSUM   (-12)     //认证中心密钥校验和错误
#define ERR_NOTFOUND       (-13)     //没有找到指定的数据或元素
#define ERR_NODATA         (-14)     //指定的数据元素没有数据
#define ERR_OVERFLOW       (-15)     //内存溢出
#define ERR_NOTRANSLOG     (-16)     //无交易日志
#define ERR_NORECORD       (-17)     //无记录
#define ERR_NOLOGITEM      (-18)     //目志项目错误
#define ERR_ICCRESET       (-19)     //IC卡复位失败
#define ERR_ICCCMD         (-20)     //IC命令失败
#define ERR_ICCBLOCK       (-21)     //IC卡锁卡 
#define ERR_ICCNORECORD    (-22)     //IC卡无记录
//////////////////////////////////////////////////////////////////////////
#define ERR_APPSELRETRY    (-23)     //选择应用重试
#define ERR_GENAC1_6985    (-24)     //GEN AC命令返回6985
#define ERR_USECONTACT     (-25)     //非接失败，改用接触界面
#define ERR_APPEXP         (-26)     //qPBOC卡应用过期
#define ERR_BLACKLIST      (-27)     //qPBOC黑名单卡
#define ERR_GPORSP         (-28)     //err from GPO
#define ERR_USEMAG         (-29)     //非接交易中刷磁卡，改用磁卡界面

#define REFER_APPROVE     0x01     //参考返回码(选择批准)
#define REFER_DENIAL      0x02     //参考返回码(选择拒绝)
#define ONLINE_APPROVE    0x00     //联机返回码(联机批准)     
#define ONLINE_FAILED     0x01     //联机返回码(联机失败) 
#define ONLINE_REFER      0x02     //联机返回码(联机参考)
#define ONLINE_DENIAL     0x03     //联机返回码(联机拒绝)
#define ONLINE_ABORT      0x04     //兼容PBOC(交易终止)

#define PATH_PBOC             0x00 //应用路径：标准PBOC
#define PATH_QPBOC            0x01 //应用路径：qPBOC
#define PATH_MSD              0x02 //应用路径：MSD
#define PATH_ECash            0x03 //应用路径：电子现金
	
typedef struct{
    unsigned char AppName[33];       //本地应用名，以'\x00'结尾的字符串
    unsigned char AID[17];           //应用标志
    unsigned char AidLen;            //AID的长度
    unsigned char SelFlag;           //选择标志( 部分匹配/全匹配)      
    unsigned char Priority;          //优先级标志
    unsigned char TargetPer;         //目标百分比数
    unsigned char MaxTargetPer;      //最大目标百分比数
    unsigned char FloorLimitCheck;   //是否检查最低限额
    unsigned char RandTransSel;      //是否进行随机交易选择
    unsigned char VelocityCheck;     //是否进行频度检测
    unsigned long FloorLimit;        //最低限额
    unsigned long Threshold;         //阀值
    unsigned char TACDenial[6];      //终端行为代码(拒绝)
    unsigned char TACOnline[6];      //终端行为代码(联机)
    unsigned char TACDefault[6];     //终端行为代码(缺省)
    unsigned char AcquierId[7];      //收单行标志 [6]
    unsigned char dDOL[256];         //终端缺省DDOL
    unsigned char tDOL[256];         //终端缺省TDOL
    unsigned char Version[3];        //应用版本
    unsigned char RiskManData[10];   //风险管理数据
}EMV_APPLIST;

typedef struct {
    unsigned char RID[5];            //应用注册服务商ID
    unsigned char KeyID;             //密钥索引
    unsigned char HashInd;           //HASH算法标志
    unsigned char ArithInd;          //RSA算法标志
    unsigned char ModulLen;          //模长度
    unsigned char Modul[248];        //模
    unsigned char ExponentLen;       //指数长度
    unsigned char Exponent[3];       //指数
    unsigned char ExpDate[3];        //有效期(YYMMDD)
    unsigned char CheckSum[20];      //密钥校验和
}EMV_CAPK;

typedef struct{
    unsigned char MerchName[256];    //商户名
    unsigned char MerchCateCode[2];  //商户类别码(没要求可不设置)
    unsigned char MerchId[15];       //商户标志(商户号)
    unsigned char TermId[8];         //终端标志(POS号)
    unsigned char TerminalType;      //终端类型
    unsigned char Capability[3];     //终端性能
    unsigned char ExCapability[5];   //终端扩展性能
    unsigned char TransCurrExp;      //交易货币代码指数
    unsigned char ReferCurrExp;      //参考货币指数
    unsigned char ReferCurrCode[2];  //参考货币代码
    unsigned char CountryCode[2];    //终端国家代码
    unsigned char TransCurrCode[2];  //交易货币代码
    unsigned long ReferCurrCon;      //参考货币代码和交易代码的转换系数(交易货币对参考货币的汇率*1000)
    //unsigned char TransType;         //当前交易类型 
	unsigned short TransType;         //当前交易类型 
    unsigned char ForceOnline;       //商户强制联机(1 表示总是联机交易)
    unsigned char GetDataPIN;        //密码检测前是否读重试次数
    unsigned char SupportPSESel;		//是否支持PSE选择方式
	unsigned char SupportAccountTypeSel; //是否支持帐户类型选择
	unsigned char TermTransQuali[4]; //终端交易限制
	unsigned char ECTSI;             //电子现金终端支持指示器
	unsigned char EC_bTermLimitCheck;      //是否支持终端交易限额
	unsigned long EC_TermLimit;            //终端交易限额，
	unsigned char CL_bStatusCheck;         //是否支持qPBOC状态检查
	unsigned long CL_FloorLimit;        //非接触终端最低限额
	unsigned long CL_TransLimit;        //非接触终端交易限额
	unsigned long CL_CVMLimit;          //非接触终端CVM限额
}EMV_PARAM;

//===========================================================
// Defined for swapping data between terminal and smart card
//===========================================================
typedef struct{
     unsigned char  Command[4];
     unsigned short  Lc;
     unsigned char  DataIn[512];
     unsigned short  Le;
}ICC_APDU_SEND;

typedef struct{
     unsigned short  LenOut;
     unsigned char  DataOut[512];
     unsigned char  SWA;
     unsigned char  SWB;
}ICC_APDU_RESP; 


//FOR CALLBACK FUNCTION
typedef int  (*P_GETDATETIME)(unsigned char *ucDatetime);
typedef int  (*P_GETUNKNOWTLV)(unsigned short Tag, unsigned char *dat, int len);
typedef void (*P_IOCTRL)(unsigned char ioname,unsigned char *piovalue);
typedef int  (*P_INPUTAMT)(unsigned long *AuthAmt, unsigned long *CashBackAmt);
typedef int  (*P_WAITAPPSEL)(int TryCnt, EMV_APPLIST List[], int AppNum);
typedef void (*P_ADVICEPROC)(void);
typedef int  (*P_VERIFYCERT)(void);
typedef int  (*P_GETHOLDERPWD)(int TryFlag, int RemainCnt, unsigned char *pin); 
typedef int  (*P_REFERPROC)(void);
typedef int  (*P_ONLINEPROC)(unsigned char *RspCode, unsigned char *AuthCode, 
								   int *AuthCodeLen, unsigned char *IAuthData, 
								   int *IAuthDataLen, unsigned char *Script, int *ScriptLen);
typedef int  (*P_ACCOUNTYPESEL)(unsigned char *account_type); 
typedef void (*P_DISPVERIFYPINRESULT)(unsigned char result); 
typedef void (*P_READIFDSN)(unsigned char *ifdsn);  
typedef int (*P_ACCOUNTTYPESEL)(unsigned char *account_type);  

typedef int  (*P_ICCRESET)(unsigned char slot,unsigned char VCC_Mode,unsigned char *ATR);
typedef int  (*P_ICCCLOSE)(unsigned char slot); 
typedef int  (*P_ICCCOMMAND)(unsigned char slot,ICC_APDU_SEND * ApduSend,ICC_APDU_RESP * ApduResp); 
 
//===========================================
//        Defined for file system
//
//============================================ 
#define   CO_RDWR                   0x01
#define   CO_CREATE                 0x02

#define   CFILE_SEEK_CUR            0
#define   CFILE_SEEK_SET            1
#define   CFILE_SEEK_END            2 
 


typedef struct _FUNCPTR_PARAM_
{
	P_GETDATETIME		pGetDateTime;
	P_GETUNKNOWTLV		pGetUnknowTLV;
	P_IOCTRL			pIoCtrl;
	P_INPUTAMT			pInputAmt;
// 	P_WAITAPPSEL		pWaitAppSel;
	P_ADVICEPROC		pAdviceProc;
	P_VERIFYCERT		pVerifyCert;
	P_GETHOLDERPWD		pGetHolderPwd; 
	P_REFERPROC			pReferProc;
// 	P_ONLINEPROC		pOnlineProc; 
	P_DISPVERIFYPINRESULT	pDispVerifyPinResult; 
	P_READIFDSN			pReadIfdSN; 
// 	P_ICCRESET			pIccReset;
// 	P_ICCCLOSE			pIccClose;
	P_ICCCOMMAND		pIccCommand; 
	P_ACCOUNTTYPESEL    PAccountTypeSel; 
	int					*gPrintReceipt;
	int					*gRandSelectNum;
	unsigned char		*POSEntryMode;
	unsigned char		*bBatchCapture;
}FUNCPTR_PARAM;


EMVCORE_API void  EmvLib_InitCallBackFunc(FUNCPTR_PARAM *ptrFunc);
EMVCORE_API int  EmvLib_Init(void);
EMVCORE_API int  EmvLib_GetVer(void);
EMVCORE_API void EmvLib_ClearTransLog(void);
EMVCORE_API void EmvLib_GetParam(EMV_PARAM *tParam);
EMVCORE_API void EmvLib_SetParam(EMV_PARAM *tParam);
EMVCORE_API int  EmvLib_GetTLV(unsigned short Tag, unsigned char *DataOut, int *OutLen);
EMVCORE_API int  EmvLib_SetTLV(unsigned short Tag, unsigned char *DataIn, int DataLen);
EMVCORE_API int  EmvLib_GetScriptResult(unsigned char *Result, int *RetLen);
EMVCORE_API int  EmvLib_AddApp(EMV_APPLIST *App);
EMVCORE_API int  EmvLib_GetApp(int Index, EMV_APPLIST *App);
EMVCORE_API int  EmvLib_DelApp(unsigned char *AID, int AidLen);
EMVCORE_API int  EmvLib_AddCapk(EMV_CAPK  *capk );
EMVCORE_API int  EmvLib_GetCapk(int Index, EMV_CAPK  *capk );
EMVCORE_API int  EmvLib_DelCapk(unsigned char KeyID, unsigned char *RID);
EMVCORE_API int  EmvLib_CheckCapk(unsigned char *KeyID, unsigned char *RID);
EMVCORE_API int  EmvLib_ReadLogRecord(int RecordNo);
EMVCORE_API int  EmvLib_GetLogItem(unsigned short Tag, unsigned char *TagData, int *TagLen);
EMVCORE_API int  EmvLib_ReadAppData(int iGetTag); 
EMVCORE_API int  EmvLib_CardAuth(void);  
EMVCORE_API unsigned char  EmvLib_GetPath(void);
EMVCORE_API int  EmvLib_ProcCLTrans(void);
EMVCORE_API int  EmvLib_GetBalance(unsigned char* BcdBalance);
EMVCORE_API int  EmvLib_qPBOCPreProcess(void);
EMVCORE_API void EmvLib_TransInit(void);
EMVCORE_API int  EmvLib_GetIccData(unsigned short tag, unsigned char *DataOut, int *OutLen);
//////////////////////////////////////////////////////////////////////////
//晓星需求 原内核流程拆开 去掉两个回调函数
// EMVCORE_API int  EmvLib_AppSelForLog(int Slot);
// EMVCORE_API int  EmvLib_AppSel(int Slot,unsigned long TransNo);
EMVCORE_API int Emvlib_CreatAppList(unsigned char slotIn, unsigned long TransNoIn, unsigned char TryCntIn,
									EMV_APPLIST *pstListOut, int *pAppNumOut, int ReadLogFlag);
EMVCORE_API int Emvlib_SelectApp(int piSelIndexIn, int ReadLogFlag);
//EMVCORE_API int  EmvLib_ProcTrans(void); 
EMVCORE_API int EmvLib_ProcTrans(unsigned char *bIfGoOnline);
EMVCORE_API int EmvLib_ProcTransComplete(unsigned char ucResult, unsigned char *RspCode, 
										 unsigned char *AuthCode, int AuthCodeLen, 
										 unsigned char *IAuthData, int IAuthDataLen, 
										 unsigned char *script, int ScriptLen);
//////////////////////////////////////////////////////////////////////////
//本来不是EMV库的函数 但是EMV L2测试应用直接调用了一些 现都开放出来
EMVCORE_API void SetAdviceReqSendFlag(int flag);
EMVCORE_API int  GetAdviceReqSendFlag(void);
EMVCORE_API void SetAdviceReqFlag(int flag);
EMVCORE_API int  GetAdviceReqFlag(void);
EMVCORE_API int InputAmt_New(void);
EMVCORE_API void s_InitTLVData(void);
EMVCORE_API void vOneTwo(unsigned char *One, unsigned short len, unsigned char *Two);
EMVCORE_API void vOneTwo_BCD(unsigned char *One,unsigned short len,unsigned char *Two);
EMVCORE_API void LongToStr(unsigned long ldat, unsigned char *str);
EMVCORE_API unsigned long StrToLong(unsigned char *str);
EMVCORE_API unsigned char EmvAddIPKRevoke(unsigned char *rid, unsigned char capki,unsigned char *certserial);
EMVCORE_API unsigned char EmvDelIPKRevoke(unsigned char *rid, unsigned char capki,unsigned char *certserial);
EMVCORE_API void s_InitTransLog(int ClearFlag);
#ifndef EMVCORE_EXPORTS
int  cEmvLib_WaitAppSel(int TryCnt, EMV_APPLIST List[], int AppNum);  
int  cEmvLib_InputAmt(unsigned long *AuthAmt, unsigned long *CashBackAmt);
int  cEmvLib_GetHolderPwd(int TryFlag, int RemainCnt, unsigned char *pin);
int  cEmvLib_ReferProc(void);
int  cEmvLib_GetUnknowTLV(unsigned short Tag, unsigned char *dat, int len);
int  cEmvLib_OnlineProc(unsigned char *RspCode, unsigned char *AuthCode, int *AuthCodeLen, unsigned char *IAuthData, int *IAuthDataLen, unsigned char *Script, int *ScriptLen);
void cEmvLib_AdviceProc(void); 
int  cEmvLib_VerifyCert(void);
int  cEmvLib_GetDateTime(unsigned char *datetime);
void cEmvLib_IoCtrl(unsigned char ioname,unsigned char *iovalue);
void cEmvLib_LcdPrintxyCE(unsigned char col, unsigned char row, unsigned char mode, char *pCHN , char *pEN); 
void cEmvLib_DispVerifyPinResult(unsigned char result); 
void cEmvLib_ReadIfdSN(unsigned char *ifdsn); 
// int  cEmvLib_IccReset(unsigned char slot,unsigned char VCC_Mode,unsigned char *ATR);
// int  cEmvLib_IccClose(unsigned char slot); 
int  cEmvLib_IccCommand(unsigned char slot,ICC_APDU_SEND * ApduSend,ICC_APDU_RESP * ApduResp);  
int  cEmvLib_AccountTypeSel(unsigned char *account_type);

#endif

#define   EMV_GET_POSENTRYMODE			0 
#define   EMV_GET_BATCHCAPTUREINFO		1 
#define   EMV_GET_ADVICESUPPORTINFO		2 

#ifdef __cplusplus
}
#endif

#endif