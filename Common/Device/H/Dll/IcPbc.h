/////////////////////////////////////////////////////////////////////////////
#if	!defined(_ICPBC_H_INCLUDED_)
#define	_ICPBC_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

#define	EMVCORE_API	__declspec(dllimport)

/////////////////////////////////////////////////////////////////////////////
//	�������Ͷ���
/////////////////////////////////////////////////////////////////////////////
#define TYPE_CASH			0x8000					//��������(�ֽ�)
#define TYPE_GOODS			0x4000					//��������(����)
#define TYPE_SERVICE		0x2000					//��������(����)
#define TYPE_CASHBACK		0x1000					//��������(����)
#define TYPE_INQUIRY		0x0800					//��������(��ѯ)
#define TYPE_PAYMENT		0x0400					//��������(֧��)
#define TYPE_ADMINISTRATIVE	0x0200					//��������(����)
#define TYPE_TRANSFER		0x0100					//��������(ת��)
#define TYPE_DEPOSIT		0x0080					//��������(���)
	
#define ACCNT_TYPE_DEFAULT	0x00
#define ACCNT_TYPE_SAVING	0x10
#define ACCNT_TYPE_DEBIT	0x20
#define ACCNT_TYPE_CREDIT	0x30

/////////////////////////////////////////////////////////////////////////////
//	��������
/////////////////////////////////////////////////////////////////////////////
#define	MAX_APP_NUM			32						//Ӧ���б����ɴ洢��Ӧ����
#define	MAX_CAPK_NUM		64						//��֤��Կ�����ɴ洢�Ĺ�Կ��
#define	MAX_CAPKREVOKE_NUM	96						//��֤��Կ�����б��������32*3

#define	PART_MATCH			0x00					//ASI(����ƥ��)
#define	FULL_MATCH			0x01					//ASI(��ȫƥ��)

#define	EMV_GET_POSENTRYMODE		0 
#define	EMV_GET_BATCHCAPTUREINFO	1 
#define	EMV_GET_ADVICESUPPORTINFO	2 

#define ENGLISH				0
#define CHINESE				1

#define SUPPORT_YES			0x01					//֧��
#define SUPPORT_NO			0x00					//��֧��

#define SEL_FOR_TRAN		0
#define SEL_FOR_LOG			1	

/////////////////////////////////////////////////////////////////////////////
//	�ļ�ϵͳ����
/////////////////////////////////////////////////////////////////////////////
#define	CO_RDWR				0x01
#define	CO_CREATE			0x02
	
#define	CFILE_SEEK_CUR		0
#define	CFILE_SEEK_SET		1
#define	CFILE_SEEK_END		2 
	
/////////////////////////////////////////////////////////////////////////////
//	���״����������붨��
/////////////////////////////////////////////////////////////////////////////
#define	EMV_OK				0						//�ɹ�
#define	ERR_EMVRSP			(-1)					//���������
#define	ERR_APPBLOCK		(-2)					//Ӧ������
#define	ERR_NOAPP			(-3)					//��Ƭ��û��EMVӦ��
#define	ERR_USERCANCEL		(-4)					//�û�ȡ����ǰ��������
#define	ERR_TIMEOUT			(-5)					//�û�������ʱ
#define	ERR_EMVDATA			(-6)					//��Ƭ���ݴ���
#define	ERR_NOTACCEPT		(-7)					//���ײ�����
#define	ERR_EMVDENIAL		(-8)					//���ױ��ܾ�
#define	ERR_KEYEXP			(-9)					//��Կ����
#define	ERR_NOPINPAD		(-10)					//û��������̻���̲�����
#define	ERR_NOPIN			(-11)					//û��������û���������������
#define	ERR_CAPKCHECKSUM	(-12)					//��֤������ԿУ��ʹ���
#define	ERR_NOTFOUND		(-13)					//û���ҵ�ָ�������ݻ�Ԫ��
#define	ERR_NODATA			(-14)					//ָ��������Ԫ��û������
#define	ERR_OVERFLOW		(-15)					//�ڴ����
#define	ERR_NOTRANSLOG		(-16)					//�޽�����־
#define	ERR_NORECORD		(-17)					//�޼�¼
#define	ERR_NOLOGITEM		(-18)					//Ŀ־��Ŀ����
#define	ERR_ICCRESET		(-19)					//IC����λʧ��
#define	ERR_ICCCMD			(-20)					//IC����ʧ��
#define	ERR_ICCBLOCK		(-21)					//IC������
#define	ERR_ICCNORECORD		(-22)					//IC���޼�¼
#define ERR_APPSELRETRY		(-23)					//ѡ��Ӧ������
#define	ERR_GENAC1_6985		(-24)					//GEN AC�����6985
#define ERR_USECONTACT		(-25)					//�ǽ�ʧ�ܣ����ýӴ�����
#define ERR_APPEXP			(-26)					//qPBOC��Ӧ�ù���
#define ERR_BLACKLIST		(-27)					//qPBOC��������
#define ERR_GPORSP			(-28)					//err from GPO
#define ERR_USEMAG			(-29)					//�ǽӽ�����ˢ�ſ������ôſ�����

#define	REFER_APPROVE		0x01					//�ο�������(ѡ����׼)
#define	REFER_DENIAL		0x02					//�ο�������(ѡ��ܾ�)
#define	ONLINE_APPROVE		0x00					//����������(������׼)
#define	ONLINE_FAILED		0x01					//����������(����ʧ��)
#define	ONLINE_REFER		0x02					//����������(�����ο�)
#define	ONLINE_DENIAL		0x03					//����������(�����ܾ�)
#define	ONLINE_ABORT		0x04					//����PBOC(������ֹ)

#define PATH_PBOC			0x00					//Ӧ��·������׼PBOC
#define PATH_QPBOC			0x01					//Ӧ��·����qPBOC
#define PATH_MSD			0x02					//Ӧ��·����MSD
#define PATH_ECash			0x03					//Ӧ��·���������ֽ�

/////////////////////////////////////////////////////////////////////////////
//	�ṹ�嶨��
/////////////////////////////////////////////////////////////////////////////
typedef	struct{
	BYTE	AppName[33];							//����Ӧ��������'\x00'��β���ַ���
	BYTE	AID[17];								//Ӧ�ñ�־
	BYTE	AidLen;									//AID�ĳ���
	BYTE	SelFlag;								//ѡ���־( ����ƥ��/ȫƥ��)
	BYTE	Priority;			 					//���ȼ���־
	BYTE	TargetPer;		 						//Ŀ��ٷֱ���
	BYTE	MaxTargetPer;		 					//���Ŀ��ٷֱ���
	BYTE	FloorLimitCheck;	 					//�Ƿ�������޶�
	BYTE	RandTransSel;		 					//�Ƿ�����������ѡ��
	BYTE	VelocityCheck;	 						//�Ƿ����Ƶ�ȼ��
	DWORD	FloorLimit;		 						//����޶�
	DWORD	Threshold;		 						//��ֵ
	BYTE	TACDenial[6];		 					//�ն���Ϊ����(�ܾ�)
	BYTE	TACOnline[6];		 					//�ն���Ϊ����(����)
	BYTE	TACDefault[6];	 						//�ն���Ϊ����(ȱʡ)
	BYTE	AcquierId[7];		 					//�յ��б�־
	BYTE	dDOL[256];		 						//�ն�ȱʡDDOL
	BYTE	tDOL[256];		 						//�ն�ȱʡTDOL
	BYTE	Version[3];		 						//Ӧ�ð汾
	BYTE	RiskManData[10];	 					//���չ�������
}EMV_APPLIST, PBCst_APPLIST;

typedef	struct{
	BYTE	MerchName[256];	 						//�̻���
	BYTE	MerchCateCode[2];	 					//�̻������(ûҪ��ɲ�����)
	BYTE	MerchId[15];		 					//�̻���־(�̻���)
	BYTE	TermId[8];		 						//�ն˱�־(POS��)
	BYTE	TerminalType;		 					//�ն�����
	BYTE	Capability[3];	 						//�ն�����
	BYTE	ExCapability[5];	 					//�ն���չ����
	BYTE	TransCurrExp;		 					//���׻��Ҵ���ָ��
	BYTE	ReferCurrExp;		 					//�ο�����ָ��
	BYTE	ReferCurrCode[2];	 					//�ο����Ҵ���
	BYTE	CountryCode[2];	 						//�ն˹��Ҵ���
	BYTE	TransCurrCode[2];	 					//���׻��Ҵ���
	DWORD	ReferCurrCon;		 					//�ο����Ҵ���ͽ��״����ת��ϵ��(���׻��ҶԲο����ҵĻ���*1000)
	WORD	TransType;								//��ǰ��������
	BYTE	ForceOnline;		 					//�̻�ǿ������(1 ��ʾ������������)
	BYTE	GetDataPIN;		 						//������ǰ�Ƿ�����Դ���
	BYTE	SupportPSESel;	 						//�Ƿ�֧��PSEѡ��ʽ
	BYTE	SupportAccountTypeSel;					//�Ƿ�֧���ʻ�����ѡ��
	BYTE	TermTransQuali[4];						//�ն˽�������
	BYTE	ECTSI;									//�����ֽ��ն�֧��ָʾ��
	BYTE	EC_bTermLimitCheck;						//�Ƿ�֧���ն˽����޶�
	DWORD	EC_TermLimit;							//�ն˽����޶�
	BYTE	CL_bStatusCheck;						//�Ƿ�֧��qPBOC״̬���
	DWORD	CL_FloorLimit;							//�ǽӴ��ն�����޶�
	DWORD	CL_TransLimit;							//�ǽӴ��ն˽����޶�
	DWORD	CL_CVMLimit;							//�ǽӴ��ն�CVM�޶�	
}EMV_PARAM, PBCst_TERM;

typedef struct {
	BYTE	Result;									//����������,��ͷ�ļ�����
	BYTE	RspCode[2];								//8583����39��
	int		AuthCodeLen;							//8583����38��
	BYTE	AuthCode[6];							//8583����38��
	int		AuthDataLen;							//Tag "9100" ��������֤���ĳ���
	BYTE	AuthData[16];							//Tag "9100" ��������֤����
	int		ScripDataLen;							//Tag "7100"+Tag "7200" �����нű�����
	BYTE	ScripData[256];							//Tag "7100"+Tag "7200" �����нű� 
}PBCst_HOST_DATA;

typedef struct {
	int		VerLen;									//�ο��汾�ų���
	BYTE	AidType;								//����
	BYTE	Version[18];							//�ο��汾��
}EMV_AIDVer, PBCst_AID_VERSION;

typedef struct {
	BYTE	SaveKeyID;								//��Կ�洢λ��
    BYTE	CapkVer[17];							//��Կ�汾��
}EMV_CAPKEXT, PBCst_CA_PUBKEY_INDEX;

typedef	struct {
	BYTE	RID[5];			 						//Ӧ��ע�������ID
	BYTE	KeyID;			 						//��Կ����
	BYTE	HashInd;			 					//HASH�㷨��־
	BYTE	ArithInd;			 					//RSA�㷨��־
	BYTE	ModulLen;			 					//ģ����
	BYTE	Modul[248];		 						//ģ
	BYTE	ExponentLen;		 					//ָ������
	BYTE	Exponent[3];		 					//ָ��
	BYTE	ExpDate[3];		 						//��Ч��(YYMMDD)
	BYTE	CheckSum[20];		 					//��ԿУ���
}EMV_CAPK, PBCst_CA_PUBKEY;

typedef struct{
	BYTE	Command[4];
	WORD	Lc;
	BYTE	DataIn[512];
	WORD	Le;
}ICC_APDU_SEND, PBCst_APDU_SEND;

typedef struct{
	WORD	LenOut;
	BYTE	DataOut[512];
	BYTE	SWA;
	BYTE	SWB;
}ICC_APDU_RESP, PBCst_APDU_RESP; 

/////////////////////////////////////////////////////////////////////////////
//	��������
/////////////////////////////////////////////////////////////////////////////
//	�ص���������
typedef int		(* P_ACCOUNTTYPESEL)(BYTE* btpAccountType);
typedef	int		(* P_INPUTAMT)(DWORD* dwpAuthAmt, DWORD* dwpCashBackAmt);
typedef	int		(* P_GETDATETIME)(BYTE* btpDateTime);
typedef	int		(* P_GETHOLDERPWD)(int nTryFlag, int nRemainCnt, BYTE* btpPin);
typedef	int		(* P_GETUNKNOWTLV)(WORD wTag, BYTE* btpData, int nDataLen);
typedef	void	(* P_IOCTRL)(BYTE btIoName, BYTE* btpIoValue);
typedef	void	(* P_ADVICEPROC)(void);
typedef	int		(* P_VERIFYCERT)(void);
typedef	int		(* P_REFERPROC)(void);
typedef void	(* P_DISPVERIFYPINRESULT)(BYTE btResult);
typedef void	(* P_READIFDSN)(BYTE* btpIfDsn);
typedef int		(* P_ICCCOMMAND)(BYTE btSlot, ICC_APDU_SEND* stpApduSend, ICC_APDU_RESP* stpApduResp);

typedef	struct _FUNCPTR_PARAM_
{
	P_GETDATETIME			pGetDateTime;
	P_GETUNKNOWTLV			pGetUnknowTLV;
	P_IOCTRL				pIoCtrl;
	P_INPUTAMT				pInputAmt;
	P_ADVICEPROC			pAdviceProc;
	P_VERIFYCERT			pVerifyCert;
	P_GETHOLDERPWD			pGetHolderPwd;
	P_REFERPROC				pReferProc;
	P_DISPVERIFYPINRESULT	pDispVerifyPinResult; 
	P_READIFDSN				pReadIfdSN;
	P_ICCCOMMAND			pIccCommand; 
	P_ACCOUNTTYPESEL		PAccountTypeSel; 
	int*					gPrintReceipt;
	int*					gRandSelectNum;
	BYTE*					POSEntryMode;
	BYTE*					bBatchCapture;
}FUNCPTR_PARAM, PBCst_FUNCPTR_PARAM;

//	�ص�������ʼ��
EMVCORE_API void EmvLib_InitCallBackFunc(PBCst_FUNCPTR_PARAM* stpFuncPtr);

//	��ʼ���ں�
EMVCORE_API int  EmvLib_Init();

//	ѡ��Ӧ��
EMVCORE_API	int  EmvLib_AddApp(PBCst_APPLIST* stpApp);
EMVCORE_API	int  EmvLib_GetApp(int nIndex, PBCst_APPLIST* stpApp);
EMVCORE_API	int  EmvLib_DelApp(BYTE* btpAID, int nAidLen);
EMVCORE_API	int  Emvlib_CreatAppList(BYTE btSlotIn, DWORD dwTransNoIn, BYTE btTryCntIn, PBCst_APPLIST* stpListOut, int* npAppNumOut, int nReadLogFlag);
EMVCORE_API	int  Emvlib_SelectApp(int nSelIndexIn, int nReadLogFlag);

//	���潻�״���
EMVCORE_API	int  EmvLib_ReadAppData(int nGetTag);
EMVCORE_API	int  EmvLib_CardAuth(void);
EMVCORE_API	int  EmvLib_ProcTrans(BYTE* btpIfGoOnline);
EMVCORE_API	int  EmvLib_ProcTransComplete(BYTE btResult, BYTE* btpRspCode, BYTE* btpAuthCode, int nAuthCodeLen, BYTE* btpAuthData, int nAuthDataLen, BYTE* btpScriptData, int nScriptLen);

//	С��֧����ѯ���
EMVCORE_API int  EmvLib_GetBalance(BYTE* btpBcdBalance);
EMVCORE_API int  EmvLib_GetIccData(WORD wTag, BYTE* btpDataOut, int* npOutLen);

//	���׼�¼��ѯ
EMVCORE_API	int	 EmvLib_ReadLogRecord(int nRecordNo);
EMVCORE_API	int	 EmvLib_GetLogItem(WORD wTag, BYTE* btpTagData, int* npTagLen);

//	��Ϣ����
EMVCORE_API	void EmvLib_GetParam(PBCst_TERM* stpParam);
EMVCORE_API	void EmvLib_SetParam(PBCst_TERM* stpParam);
EMVCORE_API	int	 EmvLib_GetTLV(WORD wTag, BYTE* btpDataOut, int* npDataLen);
EMVCORE_API	int	 EmvLib_SetTLV(WORD wTag, BYTE* btpDataIn, int nDataLen);
EMVCORE_API	int	 EmvLib_GetScriptResult(BYTE* btpResult, int* npRetLen);

//	����
EMVCORE_API	int	 EmvLib_GetVer(void);
EMVCORE_API	void EmvLib_ClearTransLog(void);
EMVCORE_API	int	 EmvLib_AddCapk(PBCst_CA_PUBKEY* stpCaPubKey);
EMVCORE_API	int	 EmvLib_GetCapk(int nIndex, PBCst_CA_PUBKEY* stpCaPubKey);
EMVCORE_API	int	 EmvLib_DelCapk(BYTE btKeyID, BYTE* btpRID);
EMVCORE_API	int	 EmvLib_CheckCapk(BYTE* btpKeyID, BYTE* btpRID);

// �ݲ���
EMVCORE_API void  SetAdviceReqSendFlag(int nFlag);
EMVCORE_API int   GetAdviceReqSendFlag(void);
EMVCORE_API void  SetAdviceReqFlag(int nFlag);
EMVCORE_API int   GetAdviceReqFlag(void);
EMVCORE_API int   InputAmt_New(void);
EMVCORE_API void  s_InitTLVData(void);
EMVCORE_API void  vOneTwo(BYTE* btpOne, WORD wLen, BYTE* btTwo);
EMVCORE_API void  vOneTwo_BCD(BYTE* btOne, WORD wLen, BYTE* btTwo);
EMVCORE_API void  LongToStr(DWORD dwLDat, BYTE* btpStr);
EMVCORE_API BYTE  EmvAddIPKRevoke(BYTE* btpRid, BYTE btCaPKey, BYTE* btpCertSerial);
EMVCORE_API BYTE  EmvDelIPKRevoke(BYTE* btpRid, BYTE btCaPKey, BYTE* btpCertSerial);
EMVCORE_API BYTE  EmvLib_GetPath(void);
EMVCORE_API void  EmvLib_TransInit(void);
EMVCORE_API int   EmvLib_ProcCLTrans(void);
EMVCORE_API int   EmvLib_qPBOCPreProcess(void);

//////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#endif
