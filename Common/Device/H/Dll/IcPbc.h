/////////////////////////////////////////////////////////////////////////////
#if	!defined(_ICPBC_H_INCLUDED_)
#define	_ICPBC_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

#define	EMVCORE_API	__declspec(dllimport)

/////////////////////////////////////////////////////////////////////////////
//	交易类型定义
/////////////////////////////////////////////////////////////////////////////
#define TYPE_CASH			0x8000					//交易类型(现金)
#define TYPE_GOODS			0x4000					//交易类型(购物)
#define TYPE_SERVICE		0x2000					//交易类型(服务)
#define TYPE_CASHBACK		0x1000					//交易类型(反现)
#define TYPE_INQUIRY		0x0800					//交易类型(查询)
#define TYPE_PAYMENT		0x0400					//交易类型(支付)
#define TYPE_ADMINISTRATIVE	0x0200					//交易类型(管理)
#define TYPE_TRANSFER		0x0100					//交易类型(转账)
#define TYPE_DEPOSIT		0x0080					//交易类型(存款)
	
#define ACCNT_TYPE_DEFAULT	0x00
#define ACCNT_TYPE_SAVING	0x10
#define ACCNT_TYPE_DEBIT	0x20
#define ACCNT_TYPE_CREDIT	0x30

/////////////////////////////////////////////////////////////////////////////
//	其他定义
/////////////////////////////////////////////////////////////////////////////
#define	MAX_APP_NUM			32						//应用列表最多可存储的应用数
#define	MAX_CAPK_NUM		64						//认证公钥表最多可存储的公钥数
#define	MAX_CAPKREVOKE_NUM	96						//认证公钥撤回列表的最大个数32*3

#define	PART_MATCH			0x00					//ASI(部分匹配)
#define	FULL_MATCH			0x01					//ASI(完全匹配)

#define	EMV_GET_POSENTRYMODE		0 
#define	EMV_GET_BATCHCAPTUREINFO	1 
#define	EMV_GET_ADVICESUPPORTINFO	2 

#define ENGLISH				0
#define CHINESE				1

#define SUPPORT_YES			0x01					//支持
#define SUPPORT_NO			0x00					//不支持

#define SEL_FOR_TRAN		0
#define SEL_FOR_LOG			1	

/////////////////////////////////////////////////////////////////////////////
//	文件系统定义
/////////////////////////////////////////////////////////////////////////////
#define	CO_RDWR				0x01
#define	CO_CREATE			0x02
	
#define	CFILE_SEEK_CUR		0
#define	CFILE_SEEK_SET		1
#define	CFILE_SEEK_END		2 
	
/////////////////////////////////////////////////////////////////////////////
//	交易处理函数返回码定义
/////////////////////////////////////////////////////////////////////////////
#define	EMV_OK				0						//成功
#define	ERR_EMVRSP			(-1)					//返回码错误
#define	ERR_APPBLOCK		(-2)					//应用已锁
#define	ERR_NOAPP			(-3)					//卡片里没有EMV应用
#define	ERR_USERCANCEL		(-4)					//用户取消当前操作或交易
#define	ERR_TIMEOUT			(-5)					//用户操作超时
#define	ERR_EMVDATA			(-6)					//卡片数据错误
#define	ERR_NOTACCEPT		(-7)					//交易不接受
#define	ERR_EMVDENIAL		(-8)					//交易被拒绝
#define	ERR_KEYEXP			(-9)					//密钥过期
#define	ERR_NOPINPAD		(-10)					//没有密码键盘或键盘不可用
#define	ERR_NOPIN			(-11)					//没有密码或用户忽略了密码输入
#define	ERR_CAPKCHECKSUM	(-12)					//认证中心密钥校验和错误
#define	ERR_NOTFOUND		(-13)					//没有找到指定的数据或元素
#define	ERR_NODATA			(-14)					//指定的数据元素没有数据
#define	ERR_OVERFLOW		(-15)					//内存溢出
#define	ERR_NOTRANSLOG		(-16)					//无交易日志
#define	ERR_NORECORD		(-17)					//无记录
#define	ERR_NOLOGITEM		(-18)					//目志项目错误
#define	ERR_ICCRESET		(-19)					//IC卡复位失败
#define	ERR_ICCCMD			(-20)					//IC命令失败
#define	ERR_ICCBLOCK		(-21)					//IC卡锁卡
#define	ERR_ICCNORECORD		(-22)					//IC卡无记录
#define ERR_APPSELRETRY		(-23)					//选择应用重试
#define	ERR_GENAC1_6985		(-24)					//GEN AC命令返回6985
#define ERR_USECONTACT		(-25)					//非接失败，改用接触界面
#define ERR_APPEXP			(-26)					//qPBOC卡应用过期
#define ERR_BLACKLIST		(-27)					//qPBOC黑名单卡
#define ERR_GPORSP			(-28)					//err from GPO
#define ERR_USEMAG			(-29)					//非接交易中刷磁卡，改用磁卡界面

#define	REFER_APPROVE		0x01					//参考返回码(选择批准)
#define	REFER_DENIAL		0x02					//参考返回码(选择拒绝)
#define	ONLINE_APPROVE		0x00					//联机返回码(联机批准)
#define	ONLINE_FAILED		0x01					//联机返回码(联机失败)
#define	ONLINE_REFER		0x02					//联机返回码(联机参考)
#define	ONLINE_DENIAL		0x03					//联机返回码(联机拒绝)
#define	ONLINE_ABORT		0x04					//兼容PBOC(交易终止)

#define PATH_PBOC			0x00					//应用路径：标准PBOC
#define PATH_QPBOC			0x01					//应用路径：qPBOC
#define PATH_MSD			0x02					//应用路径：MSD
#define PATH_ECash			0x03					//应用路径：电子现金

/////////////////////////////////////////////////////////////////////////////
//	结构体定义
/////////////////////////////////////////////////////////////////////////////
typedef	struct{
	BYTE	AppName[33];							//本地应用名，以'\x00'结尾的字符串
	BYTE	AID[17];								//应用标志
	BYTE	AidLen;									//AID的长度
	BYTE	SelFlag;								//选择标志( 部分匹配/全匹配)
	BYTE	Priority;			 					//优先级标志
	BYTE	TargetPer;		 						//目标百分比数
	BYTE	MaxTargetPer;		 					//最大目标百分比数
	BYTE	FloorLimitCheck;	 					//是否检查最低限额
	BYTE	RandTransSel;		 					//是否进行随机交易选择
	BYTE	VelocityCheck;	 						//是否进行频度检测
	DWORD	FloorLimit;		 						//最低限额
	DWORD	Threshold;		 						//阀值
	BYTE	TACDenial[6];		 					//终端行为代码(拒绝)
	BYTE	TACOnline[6];		 					//终端行为代码(联机)
	BYTE	TACDefault[6];	 						//终端行为代码(缺省)
	BYTE	AcquierId[7];		 					//收单行标志
	BYTE	dDOL[256];		 						//终端缺省DDOL
	BYTE	tDOL[256];		 						//终端缺省TDOL
	BYTE	Version[3];		 						//应用版本
	BYTE	RiskManData[10];	 					//风险管理数据
}EMV_APPLIST, PBCst_APPLIST;

typedef	struct{
	BYTE	MerchName[256];	 						//商户名
	BYTE	MerchCateCode[2];	 					//商户类别码(没要求可不设置)
	BYTE	MerchId[15];		 					//商户标志(商户号)
	BYTE	TermId[8];		 						//终端标志(POS号)
	BYTE	TerminalType;		 					//终端类型
	BYTE	Capability[3];	 						//终端性能
	BYTE	ExCapability[5];	 					//终端扩展性能
	BYTE	TransCurrExp;		 					//交易货币代码指数
	BYTE	ReferCurrExp;		 					//参考货币指数
	BYTE	ReferCurrCode[2];	 					//参考货币代码
	BYTE	CountryCode[2];	 						//终端国家代码
	BYTE	TransCurrCode[2];	 					//交易货币代码
	DWORD	ReferCurrCon;		 					//参考货币代码和交易代码的转换系数(交易货币对参考货币的汇率*1000)
	WORD	TransType;								//当前交易类型
	BYTE	ForceOnline;		 					//商户强制联机(1 表示总是联机交易)
	BYTE	GetDataPIN;		 						//密码检测前是否读重试次数
	BYTE	SupportPSESel;	 						//是否支持PSE选择方式
	BYTE	SupportAccountTypeSel;					//是否支持帐户类型选择
	BYTE	TermTransQuali[4];						//终端交易限制
	BYTE	ECTSI;									//电子现金终端支持指示器
	BYTE	EC_bTermLimitCheck;						//是否支持终端交易限额
	DWORD	EC_TermLimit;							//终端交易限额
	BYTE	CL_bStatusCheck;						//是否支持qPBOC状态检查
	DWORD	CL_FloorLimit;							//非接触终端最低限额
	DWORD	CL_TransLimit;							//非接触终端交易限额
	DWORD	CL_CVMLimit;							//非接触终端CVM限额	
}EMV_PARAM, PBCst_TERM;

typedef struct {
	BYTE	Result;									//联机返回码,见头文件定义
	BYTE	RspCode[2];								//8583报文39域
	int		AuthCodeLen;							//8583报文38域
	BYTE	AuthCode[6];							//8583报文38域
	int		AuthDataLen;							//Tag "9100" 发卡行认证密文长度
	BYTE	AuthData[16];							//Tag "9100" 发卡行认证密文
	int		ScripDataLen;							//Tag "7100"+Tag "7200" 发卡行脚本长度
	BYTE	ScripData[256];							//Tag "7100"+Tag "7200" 发卡行脚本 
}PBCst_HOST_DATA;

typedef struct {
	int		VerLen;									//参考版本号长度
	BYTE	AidType;								//类型
	BYTE	Version[18];							//参考版本号
}EMV_AIDVer, PBCst_AID_VERSION;

typedef struct {
	BYTE	SaveKeyID;								//公钥存储位置
    BYTE	CapkVer[17];							//密钥版本号
}EMV_CAPKEXT, PBCst_CA_PUBKEY_INDEX;

typedef	struct {
	BYTE	RID[5];			 						//应用注册服务商ID
	BYTE	KeyID;			 						//密钥索引
	BYTE	HashInd;			 					//HASH算法标志
	BYTE	ArithInd;			 					//RSA算法标志
	BYTE	ModulLen;			 					//模长度
	BYTE	Modul[248];		 						//模
	BYTE	ExponentLen;		 					//指数长度
	BYTE	Exponent[3];		 					//指数
	BYTE	ExpDate[3];		 						//有效期(YYMMDD)
	BYTE	CheckSum[20];		 					//密钥校验和
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
//	函数定义
/////////////////////////////////////////////////////////////////////////////
//	回调函数定义
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

//	回调函数初始化
EMVCORE_API void EmvLib_InitCallBackFunc(PBCst_FUNCPTR_PARAM* stpFuncPtr);

//	初始化内核
EMVCORE_API int  EmvLib_Init();

//	选择应用
EMVCORE_API	int  EmvLib_AddApp(PBCst_APPLIST* stpApp);
EMVCORE_API	int  EmvLib_GetApp(int nIndex, PBCst_APPLIST* stpApp);
EMVCORE_API	int  EmvLib_DelApp(BYTE* btpAID, int nAidLen);
EMVCORE_API	int  Emvlib_CreatAppList(BYTE btSlotIn, DWORD dwTransNoIn, BYTE btTryCntIn, PBCst_APPLIST* stpListOut, int* npAppNumOut, int nReadLogFlag);
EMVCORE_API	int  Emvlib_SelectApp(int nSelIndexIn, int nReadLogFlag);

//	常规交易处理
EMVCORE_API	int  EmvLib_ReadAppData(int nGetTag);
EMVCORE_API	int  EmvLib_CardAuth(void);
EMVCORE_API	int  EmvLib_ProcTrans(BYTE* btpIfGoOnline);
EMVCORE_API	int  EmvLib_ProcTransComplete(BYTE btResult, BYTE* btpRspCode, BYTE* btpAuthCode, int nAuthCodeLen, BYTE* btpAuthData, int nAuthDataLen, BYTE* btpScriptData, int nScriptLen);

//	小额支付查询余额
EMVCORE_API int  EmvLib_GetBalance(BYTE* btpBcdBalance);
EMVCORE_API int  EmvLib_GetIccData(WORD wTag, BYTE* btpDataOut, int* npOutLen);

//	交易记录查询
EMVCORE_API	int	 EmvLib_ReadLogRecord(int nRecordNo);
EMVCORE_API	int	 EmvLib_GetLogItem(WORD wTag, BYTE* btpTagData, int* npTagLen);

//	信息传递
EMVCORE_API	void EmvLib_GetParam(PBCst_TERM* stpParam);
EMVCORE_API	void EmvLib_SetParam(PBCst_TERM* stpParam);
EMVCORE_API	int	 EmvLib_GetTLV(WORD wTag, BYTE* btpDataOut, int* npDataLen);
EMVCORE_API	int	 EmvLib_SetTLV(WORD wTag, BYTE* btpDataIn, int nDataLen);
EMVCORE_API	int	 EmvLib_GetScriptResult(BYTE* btpResult, int* npRetLen);

//	其他
EMVCORE_API	int	 EmvLib_GetVer(void);
EMVCORE_API	void EmvLib_ClearTransLog(void);
EMVCORE_API	int	 EmvLib_AddCapk(PBCst_CA_PUBKEY* stpCaPubKey);
EMVCORE_API	int	 EmvLib_GetCapk(int nIndex, PBCst_CA_PUBKEY* stpCaPubKey);
EMVCORE_API	int	 EmvLib_DelCapk(BYTE btKeyID, BYTE* btpRID);
EMVCORE_API	int	 EmvLib_CheckCapk(BYTE* btpKeyID, BYTE* btpRID);

// 暂不用
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
