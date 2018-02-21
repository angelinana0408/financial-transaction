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

#define TYPE_CASH            0x8000    //��������(�ֽ�)
#define TYPE_GOODS           0x4000    //��������(����)
#define TYPE_SERVICE         0x2000    //��������(����)
#define TYPE_CASHBACK        0x1000    //��������(����)
#define TYPE_INQUIRY         0x0800    //��������(��ѯ)
#define TYPE_PAYMENT         0x0400    //��������(֧��)
#define TYPE_ADMINISTRATIVE  0x0200    //��������(����)
#define TYPE_TRANSFER        0x0100    //��������(ת��) 
#define TYPE_DEPOSIT         0x0080    //��������(���)

#define MAX_APP_NUM          32      //Ӧ���б����ɴ洢��Ӧ����
#define MAX_CAPK_NUM         64      //��֤��Կ�����ɴ洢�Ĺ�Կ��
#define MAX_CAPKREVOKE_NUM   96      //��֤��Կ�����б��������32*3

#define PART_MATCH           0x00    //ASI(����ƥ��)
#define FULL_MATCH           0x01    //ASI(��ȫƥ��)

#define SUPPORT_YES          0x01    //֧��
#define SUPPORT_NO           0x00    //��֧��

#define ACCNT_TYPE_DEFAULT		0x00
#define ACCNT_TYPE_SAVING		0x10
#define ACCNT_TYPE_DEBIT		0x20
#define ACCNT_TYPE_CREDIT		0x30

#define ICC_USER         0
#define ICC_CL           1         //added by gubh for contactless card

//���״����������붨��
#define EMV_OK              0      //�ɹ�  
#define ERR_EMVRSP         (-1)      //���������
#define ERR_APPBLOCK       (-2)      //Ӧ������
#define ERR_NOAPP          (-3)      //��Ƭ��û��EMVӦ��
#define ERR_USERCANCEL     (-4)      //�û�ȡ����ǰ��������
#define ERR_TIMEOUT        (-5)      //�û�������ʱ
#define ERR_EMVDATA        (-6)      //��Ƭ���ݴ���
#define ERR_NOTACCEPT      (-7)      //���ײ�����
#define ERR_EMVDENIAL      (-8)      //���ױ��ܾ�
#define ERR_KEYEXP         (-9)      //��Կ����
#define ERR_NOPINPAD       (-10)     //û��������̻���̲�����
#define ERR_NOPIN          (-11)     //û��������û���������������
#define ERR_CAPKCHECKSUM   (-12)     //��֤������ԿУ��ʹ���
#define ERR_NOTFOUND       (-13)     //û���ҵ�ָ�������ݻ�Ԫ��
#define ERR_NODATA         (-14)     //ָ��������Ԫ��û������
#define ERR_OVERFLOW       (-15)     //�ڴ����
#define ERR_NOTRANSLOG     (-16)     //�޽�����־
#define ERR_NORECORD       (-17)     //�޼�¼
#define ERR_NOLOGITEM      (-18)     //Ŀ־��Ŀ����
#define ERR_ICCRESET       (-19)     //IC����λʧ��
#define ERR_ICCCMD         (-20)     //IC����ʧ��
#define ERR_ICCBLOCK       (-21)     //IC������ 
#define ERR_ICCNORECORD    (-22)     //IC���޼�¼
//////////////////////////////////////////////////////////////////////////
#define ERR_APPSELRETRY    (-23)     //ѡ��Ӧ������
#define ERR_GENAC1_6985    (-24)     //GEN AC�����6985
#define ERR_USECONTACT     (-25)     //�ǽ�ʧ�ܣ����ýӴ�����
#define ERR_APPEXP         (-26)     //qPBOC��Ӧ�ù���
#define ERR_BLACKLIST      (-27)     //qPBOC��������
#define ERR_GPORSP         (-28)     //err from GPO
#define ERR_USEMAG         (-29)     //�ǽӽ�����ˢ�ſ������ôſ�����

#define REFER_APPROVE     0x01     //�ο�������(ѡ����׼)
#define REFER_DENIAL      0x02     //�ο�������(ѡ��ܾ�)
#define ONLINE_APPROVE    0x00     //����������(������׼)     
#define ONLINE_FAILED     0x01     //����������(����ʧ��) 
#define ONLINE_REFER      0x02     //����������(�����ο�)
#define ONLINE_DENIAL     0x03     //����������(�����ܾ�)
#define ONLINE_ABORT      0x04     //����PBOC(������ֹ)

#define PATH_PBOC             0x00 //Ӧ��·������׼PBOC
#define PATH_QPBOC            0x01 //Ӧ��·����qPBOC
#define PATH_MSD              0x02 //Ӧ��·����MSD
#define PATH_ECash            0x03 //Ӧ��·���������ֽ�
	
typedef struct{
    unsigned char AppName[33];       //����Ӧ��������'\x00'��β���ַ���
    unsigned char AID[17];           //Ӧ�ñ�־
    unsigned char AidLen;            //AID�ĳ���
    unsigned char SelFlag;           //ѡ���־( ����ƥ��/ȫƥ��)      
    unsigned char Priority;          //���ȼ���־
    unsigned char TargetPer;         //Ŀ��ٷֱ���
    unsigned char MaxTargetPer;      //���Ŀ��ٷֱ���
    unsigned char FloorLimitCheck;   //�Ƿ�������޶�
    unsigned char RandTransSel;      //�Ƿ�����������ѡ��
    unsigned char VelocityCheck;     //�Ƿ����Ƶ�ȼ��
    unsigned long FloorLimit;        //����޶�
    unsigned long Threshold;         //��ֵ
    unsigned char TACDenial[6];      //�ն���Ϊ����(�ܾ�)
    unsigned char TACOnline[6];      //�ն���Ϊ����(����)
    unsigned char TACDefault[6];     //�ն���Ϊ����(ȱʡ)
    unsigned char AcquierId[7];      //�յ��б�־ [6]
    unsigned char dDOL[256];         //�ն�ȱʡDDOL
    unsigned char tDOL[256];         //�ն�ȱʡTDOL
    unsigned char Version[3];        //Ӧ�ð汾
    unsigned char RiskManData[10];   //���չ�������
}EMV_APPLIST;

typedef struct {
    unsigned char RID[5];            //Ӧ��ע�������ID
    unsigned char KeyID;             //��Կ����
    unsigned char HashInd;           //HASH�㷨��־
    unsigned char ArithInd;          //RSA�㷨��־
    unsigned char ModulLen;          //ģ����
    unsigned char Modul[248];        //ģ
    unsigned char ExponentLen;       //ָ������
    unsigned char Exponent[3];       //ָ��
    unsigned char ExpDate[3];        //��Ч��(YYMMDD)
    unsigned char CheckSum[20];      //��ԿУ���
}EMV_CAPK;

typedef struct{
    unsigned char MerchName[256];    //�̻���
    unsigned char MerchCateCode[2];  //�̻������(ûҪ��ɲ�����)
    unsigned char MerchId[15];       //�̻���־(�̻���)
    unsigned char TermId[8];         //�ն˱�־(POS��)
    unsigned char TerminalType;      //�ն�����
    unsigned char Capability[3];     //�ն�����
    unsigned char ExCapability[5];   //�ն���չ����
    unsigned char TransCurrExp;      //���׻��Ҵ���ָ��
    unsigned char ReferCurrExp;      //�ο�����ָ��
    unsigned char ReferCurrCode[2];  //�ο����Ҵ���
    unsigned char CountryCode[2];    //�ն˹��Ҵ���
    unsigned char TransCurrCode[2];  //���׻��Ҵ���
    unsigned long ReferCurrCon;      //�ο����Ҵ���ͽ��״����ת��ϵ��(���׻��ҶԲο����ҵĻ���*1000)
    //unsigned char TransType;         //��ǰ�������� 
	unsigned short TransType;         //��ǰ�������� 
    unsigned char ForceOnline;       //�̻�ǿ������(1 ��ʾ������������)
    unsigned char GetDataPIN;        //������ǰ�Ƿ�����Դ���
    unsigned char SupportPSESel;		//�Ƿ�֧��PSEѡ��ʽ
	unsigned char SupportAccountTypeSel; //�Ƿ�֧���ʻ�����ѡ��
	unsigned char TermTransQuali[4]; //�ն˽�������
	unsigned char ECTSI;             //�����ֽ��ն�֧��ָʾ��
	unsigned char EC_bTermLimitCheck;      //�Ƿ�֧���ն˽����޶�
	unsigned long EC_TermLimit;            //�ն˽����޶
	unsigned char CL_bStatusCheck;         //�Ƿ�֧��qPBOC״̬���
	unsigned long CL_FloorLimit;        //�ǽӴ��ն�����޶�
	unsigned long CL_TransLimit;        //�ǽӴ��ն˽����޶�
	unsigned long CL_CVMLimit;          //�ǽӴ��ն�CVM�޶�
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
//�������� ԭ�ں����̲� ȥ�������ص�����
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
//��������EMV��ĺ��� ����EMV L2����Ӧ��ֱ�ӵ�����һЩ �ֶ����ų���
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