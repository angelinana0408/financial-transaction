/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranHostProc_H_INCLUDED_)
#define _TranHostProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Send Host/Recv Host Variable
/////////////////////////////////////////////////////////////////////////////
	int		AddSerialFlag;				// Add Serial Flag
	int		TranSend;					// N Times Transaction Send Flag
	char	OrgSerialNo[8];				// Ogr Serial No
	char	WithSerialNo[8];			// Withdrawal Serial No
	
	int		DownloadParmFlag;			// If Download Parm
	char	NewParmVersion[8];
	int		DownloadFITFlag;			// If Download FIT
	char	NewFITVersion[8];
	int		DownloadPayFlag;			// If Download Pay
	char	NewPayVersion[8];
	int		DownloadICFITFlag;			// If Download IC FIT added by yusy ic
	char	NewICFITVersion[8];
	int		DownloadAIDListFlag;		// If Download AID List
	char	NewAIDListVersion[8];
	int		DownloadPBOCParaFlag;		// If Download PBOC para
	char	NewPBOCParaVersion[8];
	int		DownloadEchannelParaFlag;	// If Download Echannel Parameter
	int		DownloadEchannelNum;
	int		DownloadEchannelMtd;		// Download Echannel Method
	char	NewEchannelParaVersion[8];
	int		DownloadOverFlag;			// Download Over Flag
	int		DownloadCurrNum;			// Download Current Number
	int		DownloadCurrProtLen;
	int		DownloadEchannelProtCurrNum;
	int		EProtRowNum;
	char	EchannelProtContent[1024];
	int		TotalAddFlag;

	CString	strBitMap;					// Bit Map
	int		SendLength;					// Send Length
	int		MacLength;					// Mac Length
	int		RecvLength;					// Recv Length

	BYTE	SendBuffer[NETBUFFSIZE];	// Send Buffer
	BYTE	MacBuffer[NETBUFFSIZE];		// Mac Buffer
	BYTE	RecvBuffer[NETBUFFSIZE];	// Recv Buffer
	
	HMESEND		HMESend;				// Host Message Edit Send
	HMERECV		HMERecv;				// Host Message Edit Recv
	
	ATMSTATUSINFO	AtmStatusInfo;		// Atm Status Info
	ATMSTATUSINFO127	AtmStatusInfo127;		// Atm Status Info
	ATMTOTALINFO	AtmTotalInfo;		// Atm Total Info
	ATMADDCASHINFO	AtmAddCashInfo;		// Atm AddCash Info
	
	PSPRecv		PSPRecvData;			// Parameter Slip Print Recv Data
	PPPRecv		PPPRecvData;			// Parameter Passbook Print Recv Data
	PPWRecv		PPWRecvData;			// Parameter Passbook Write Recv Data
	PCWRecv		PCWRecvData;			// Parameter Card Write Recv Data
	PMERecv		PMERecvData;			// Parameter Message Edit Recv Data
	PMDRecv		PMDRecvData;			// Parameter Message Display Recv Data

	TRANRESPONSE	TranResponseData;	// Transaction Response
	RQKRESPONSE		RQKResponseData;	// RQK Response

	BYTE		IsuAuthData[256];
	int			IsuAuthDataLen;
	BYTE		IcScriptData[256];
	int			IcScriptDataLen;
	BYTE		IcScriptData2[256];
	int			IcScriptDataLen2;
/////////////////////////////////////////////////////////////////////////////
//	After Recv Host Variable
/////////////////////////////////////////////////////////////////////////////
struct	tagHostResp
{
	int		CardRetractFlag;			// Card Retract Flag
	int		CardEjectFlag;				// Card Eject Flag
	char	RejectCode[2];				// Reject Code 
	char	MacData[8];					// Mac Data
	char	ReferenceNo[12];			// Reference No	: 2006.07.03
	char	AccountNo[19];				// Account No
	char	AtmID[4];					// ATM ID
	char	AtmCycle[4];				// ATM Cycle
	char	Atm_ser[6];					// ATM Serial No

} HostResp;

/////////////////////////////////////////////////////////////////////////////
//	After Send A/C Manager Host Variable
/////////////////////////////////////////////////////////////////////////////
struct	tagACHostSend
{
	int		PinTypeFlag;			    // 密码验证标志     0：验，1：不验，不等于1：验
	char	CertifType[2];			    // 证件类型         01-居民身份证02-户口簿03-军官证04-武官证 05-港澳居民往来内地通行证 06-台湾回乡证 07-本国护照 08-外国护照 09-临时身份证 10-军人士兵证 11-军人文职干部证
	int 	SuccessFlag[2];				// 成功标志         0:成功 1:失败 
	char	CurrType[3];				// 币种             000:人民币001:港币002:美元003:日元004:欧元
	int 	AccountType;			    // 账户类型         0:对私账号1:对私卡2:对公账号3:对公卡
	int 	BillService;			    // 账单服务         0：不开通1：开通
	int 	ChanType;			    	// 渠道             0自动交易1柜台2电话储蓄所3短信4网上渠道5ATM6自助终端7备用
	char	OpenType[8];				// 开通渠道         二进制字符串转换成10进制的值 7-0位对应7-0的渠道
	char	NoteAwoke[6];				// 是否短信提醒     0不进醒 1提醒
	int     PinValiStyle;               // 客户密码验证方式 0验证用本地静态密码，交易用账户密码1验证用本地静态密码，交易用手机上的密码(后台生成发送到客户手机上)2验证用本地静态密码，交易用令牌动态密码
    char    OpenServFlag[8];            // 开通服务         目前第一位对应开通非约定交易，1开通0不开通         
    int     NoteFacili;                 // 短信服务商	    移动0联通1电信2
	int     AssuType;                   // 约定类型	        0人工1自动
	char    TranType[4];                // 交易类型	        4地址汇款5密码汇款6异地本行账号汇款7转账转出(同城同行)8同城跨行9转定期10异地跨行 11基金1001移动缴费1002联通缴费
    char     RemitReturnFlag[1];            // 汇兑回音标志	    0不回音1回音
	int     TranState;                  // 状态	预留状态    0 正常 1未激活 2已激活 3已复核
    char    ReturnCode[6];              // 返回码       	0-交易成功  其他――见"errcode.h"中的错误码定义
	char    JHTranCode[6];                // 应用代码         0表示开通大类，否则表示开通大类下的业务
	char    TranName[60];               // 交易名称应用名称	        业务名称
	char    SumLimit[60];               // 应用信息1	    缴费当日限额
	char    ReservePin[6];              //预留密码          密码汇款时要填
	int     DepTime;                    // 一本通存期
	char    TranMoney[19];                  //交易金额
	char    ReserveNum[3];              // 预留序号

} ACHostSend;

/////////////////////////////////////////////////////////////////////////////
//	After Resp A/C Manager Host Variable
/////////////////////////////////////////////////////////////////////////////
struct	tagACHostResp
{
    int     ReturnCode;                 // 返回码       	0-交易成功  其他――见"errcode.h"中的错误码定义
	char    ReturnMess[200];            // 返回信息	
	char    ClientName[60];             // 姓名
	char    ClientAdd[80];              // 地址
	char    MobilePhone[13];            // 手机
    char	TelePhone[13];              // 电话
	char    PostCode[13];               // 邮编 
	int     BillService;                // 账单服务         0:对私账号1:对私卡2:对公账号3:对公卡
    char    Email[60];                  // 
	char    BankAccountNo[23];          // 银行账户
	char    CardAccountNo[23];          // 银行卡号
	char	OpenType[8];				// 开通渠道         二进制字符串转换成10进制的值 7-0位对应7-0的渠道
    int     PinValiStyle;               // 客户密码验证方式 0验证用本地静态密码，交易用账户密码1验证用本地静态密码，交易用手机上的密码(后台生成发送到客户手机上)2验证用本地静态密码，交易用令牌动态密码
    char    ClientManager[20];          // 客户经理
	char    ACState[2];                 // 状态
	char    ReserveNum[3];              // 预留序号
	int     AssuType;                   // 约定类型	        0人工1自动
	char    AssuRemitMax[19];           // 约定汇款额度     转入账户当日限额
	char    TranSum[19];                // 金额
	char    StartDate[8];               // 起始日期
	char    MoneyUse[60];               // 资金用途
	                     				// 短信约定提醒时间
	char    TranType[4];                // 交易类型	        4地址汇款5密码汇款6异地本行账号汇款7转账转出(同城同行)8同城跨行9转定期10异地跨行 11基金1001移动缴费1002联通缴费
	char    PayeeName[60];              // 收款人姓名     姓名、缴费名称、基金名称、证券名称
	char    PayeePostCode[6];           // 收款人邮编
	char    PayeeAdd[80];               // 收款人地址
	char    PayeeMess[80];              // 收款人信息；   汇款地址 银行代码 一本通存期 基金为印刷号

	char    RemitReturnFlag[1];            // 汇兑回音标志	    0不回音1回音
	char    PayeeAccount[23];           // 收款人账号
	char    PayeeBankCode[23];          // 转入银行代码  
    char    PayeeBankName[1];              // 转入银行名称
    int     TranTypeAttri;              // 交易类型属性 0:预缴实缴都支持 1：只支持预缴 2：只支持实缴 3：不支持缴费
	char    ReserveSum[19];             // 保留金额
	char    OperType[3];                // 业务类型
	char    OpenCountNode[12];          // 开户网点
	char    ACTranServiceNo[17];        // 本地流水号		账户管家流水号
	char    LiquiDate[8];               // 清算日期
	char    TranDate[8];                // 交易日期
	char    TranTime[8];                // 交易时间
	char    ClientNum[10];              // 客户号	10	账户管家内部客户号
	char    TranCode[6];                // 交易码 交易时的账户管家交易码
	char	CurrType[3];				// 币种     目前只是人民币        000:人民币001:港币002:美元003:日元004:欧元
	char    CommCharge[19];             // 手续费  总共
	char    RealCharge[19];             // 实际手续费		汇费等
	char    BankTeller[12];             // 银行柜员  交易时的银行柜员
	char    Banknode[12];               // 银行网点  交易时的银行网点
	char    HostServiceNo[22];           // 前置流水号
    char    HostServiceNo1[22];          // 主机流水号	
    char    PostalOrderNo[22];          // 汇票号码
	char    OperServiceNo[22];	        // 应用流水	 汇兑流水	缴费流水等第三方系统流水
	char    OperServiceNo1[22];	        // 应用流水	 	缴费流水等第三方系统流水
	char    OperMess[60];               // 处理信息
	char    ReserveTableNo[4];          // 预留表序号		若是预留交易，则此字段有值
	char    OraServiceNo[17];           // 原本地流水号   若是人工返回类交易，则是针对返回的那条记录的流水号
	int     TranChanType;               // 交易渠道	 	交易时的渠道
	char    JHNumber[23];                   // 缴费号码
	char    JHAmount[19];                   // 应缴费用金额
    int     RecNum;                     // 记录条数
	char    JHTranCode[6];                // 应用代码         0表示开通大类，否则表示开通大类下的业务
    char    TranName[60];               // 应用名称	        业务名称交易名称
    char    SumLimit[60];               // 应用信息1	    缴费当日限额
    char    AcBalance[19];                  // 可用余额

} ACHostResp;
	

/////////////////////////////////////////////////////////////////////////////
//	After Resp A/C Manager Host Variable
/////////////////////////////////////////////////////////////////////////////
struct	tagACHostRespACManager
{
  	char    PayeeName[60];              // 收款人姓名     姓名、缴费名称、基金名称、证券名称
	char    PayeePostCode[6];           // 收款人邮编
	char    PayeeAdd[80];               // 收款人地址
	char    PayeeAccount[23];           // 收款人账号
    char    PayeeBankCode[23];          // 转入银行代码  
    char    PayeeBankName[1];              // 转入银行名称
	char    ReserveSum[19];             // 保留金额   
	char    JHNumber[23];                   // 缴费号码
	char    TranType[4];                // 缴费类型  //交易类型
	char    ReserveNum[3];              // 预留序号
	int     ArrayNum;                   // 页面显视的编号
	int     TranTypeAttri;              // 交易类型属性0:预缴实缴都支持 1：只支持预缴 2：只支持实缴 3：不支持缴费
	char     RemitReturnFlag[1];            // 汇兑回音标志	    0不回音1回音
	char    TranName[60];               // 应用名称	        业务名称交易名称
// 	char    AcBalance;                  // 可用余额
	char    JHTranCode[6];                // 应用代码         0表示开通大类，否则表示开通大类下的业务


}ACHostRespACManager[100];
struct	tagACHostRespRemitAssu
{
  	char    PayeeName[60];              // 收款人姓名     姓名、缴费名称、基金名称、证券名称
	char    PayeePostCode[6];           // 收款人邮编
	char    PayeeAdd[80];               // 收款人地址
	char    PayeeAccount[23];           // 收款人账号
    char    PayeeBankCode[23];          // 转入银行代码  
    char    PayeeBankName[1];              // 转入银行名称
	char    ReserveSum[19];             // 保留金额   
	char    JHNumber[23];                  // 缴费号码
	char    TranType[4];                // 缴费类型  //交易类型
	char    ReserveNum[3];              // 预留序号
	int     ArrayNum;                   // 页面显视的编号
    int     TranTypeAttri;              // 交易类型属性0:预缴实缴都支持 1：只支持预缴 2：只支持实缴 3：不支持缴费
	char     RemitReturnFlag[1];            // 汇兑回音标志	    0不回音1回音
	char    TranName[60];               // 应用名称	        业务名称交易名称
	char    SumLimit[60];               // 应用信息1	    缴费当日限额
// 	char    AcBalance;                  // 可用余额
}ACHostRespRemitAssu[100];

struct	tagACHostRespPayAssu
{
  	char    PayeeName[60];              // 收款人姓名     姓名、缴费名称、基金名称、证券名称
	char    PayeePostCode[6];           // 收款人邮编
	char    PayeeAdd[80];               // 收款人地址
	char    PayeeAccount[23];           // 收款人账号
    char    PayeeBankCode[23];          // 转入银行代码  
    char    PayeeBankName[1];              // 转入银行名称
	char    ReserveSum[19];             // 保留金额   
	char    JHNumber[23];                  // 缴费号码
	char    TranType[4];                // 缴费类型  //交易类型
	char    ReserveNum[3];              // 预留序号
	int     ArrayNum;                   // 页面显视的编号
	char    TranName[60];               // 应用名称	        业务名称
    int     TranTypeAttri;              // 交易类型属性0:预缴实缴都支持 1：只支持预缴 2：只支持实缴 3：不支持缴费
    char    SumLimit[60];               // 应用信息1	    缴费当日限额
// 	char    AcBalance;                  // 可用余额
	char    JHAmount[19];                   // 应缴费用金额

}ACHostRespPayAssu[100];

struct	tagACHostRespPayUnAssu
{
  	char    PayeeName[60];              // 收款人姓名     姓名、缴费名称、基金名称、证券名称
	char    PayeePostCode[6];           // 收款人邮编
	char    PayeeAdd[80];               // 收款人地址
	char    PayeeAccount[23];           // 收款人账号
    char    PayeeBankCode[23];          // 转入银行代码  
    char    PayeeBankName[1];              // 转入银行名称
	char    ReserveSum[19];             // 保留金额   
	char    JHNumber[23];                   // 缴费号码
	char    TranType[4];                // 缴费类型  //交易类型
	char    ReserveNum[3];              // 预留序号
	int     ArrayNum;                   // 页面显视的编号
	char    TranName[60];               // 应用名称	        业务名称
    char    SumLimit[60];               // 应用信息1	    缴费当日限额
// 	char    AcBalance;                  // 可用余额
    int     TranTypeAttri;              // 交易类型属性0:预缴实缴都支持 1：只支持预缴 2：只支持实缴 3：不支持缴费
}ACHostRespPayUnAssu[100];
/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Send Host/Recv Host Fucntion
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_SendHost();									// Send Host
	int		fnAPP_GetAtmStatusInfo();							// Get Atm Status Info
	int		fnAPP_FilterAtmStatusInfo();						// Filter Atm Status Info
	int		fnAPP_GetAtmStatusInfo127();						// Get Atm Status Info
	int		fnAPP_FilterAtmStatusInfo127();						// Filter Atm Status Info
	int		fnAPP_GetAtmTotalInfo();							// Get Atm Total Info
	int		fnAPP_GetAtmAddCashInfo();							// Get Atm AddCash Info
	int		fnAPP_SendHostMakeHeader(int RealFlag, void* MakeArea);				
																// Send Host Make Header
	int		fnAPP_SendHostMakeSet(int RealFlag);				// Send Host Make Set
	int		fnAPP_SendHostMakeMsg(int RealFlag);				// Send Host Make Message
	int		fnAPP_SendHostMake(int RealFlag = FALSE);			// Send Host Make
	int		fnAPP_SendHostMakeBuffer(int RealFlag);
																// Send Host Make Buffer
	int		fnAPP_SendHostMakeMac(int RealFlag, void* MakeArea, int SendBuffLength, int MacDataLength);
																// Send Host Make Mac
	int		fnAPP_ACSendHost();							// A/C Send Host BY HYL


/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_RecvHost();									// Recv Host
	int		fnAPP_RecvAnal();									// Recv Analysis
	int		fnAPP_RecvAnalBidCommand();							// Recv Analysis Bid Command
	int		fnAPP_RecvAnalDCBidCommand();						// Recv Analysis DC Bid Command

	int		fnAPP_RecvPSPGet(void* RecvArea, PSPRecv* DataArea, int Length);
																// Recv Parameter Slip Print Get
	int		fnAPP_RecvPSPMove(char Data, PSPRecv* pDataArea, int Length);
																// Recv Parameter Slip Print Move

	int		fnAPP_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length);
																// Recv Parameter Passbook Print Get
	int		fnAPP_RecvPPPMove(char Data, PPPRecv* pDataArea, int Length);
																// Recv Parameter Passbook Print Move

	int		fnAPP_RecvPPWGet(void* RecvArea, PPWRecv* DataArea, int Length);
																// Recv Parameter Passbook Write Get
	int		fnAPP_RecvPPWMove(char Data, PPWRecv* pDataArea, int Length);
																// Recv Parameter Passbook Write Move

	int		fnAPP_RecvPCWGet(void* RecvArea, PCWRecv* DataArea, int Length);
																// Recv Parameter Card Write Get
	int		fnAPP_RecvPCWMove(char Data, PCWRecv* pDataArea, int Length);
																// Recv Parameter Card Write Move

	int		fnAPP_RecvPMEGet(void* RecvArea, PMERecv* DataArea, int Length);
																// Recv Parameter Message Edit Get
	int		fnAPP_RecvPMESubGet(void* RecvArea, PMERecv* DataArea, int Cnt, int Length);
																// Recv Parameter Message Edit Sub Get
	int		fnAPP_RecvPMEMove(char Data, PMERecv* pDataArea, int Length);
																// Recv Parameter Message Edit Move

	int		fnAPP_RecvPMDGet(void* RecvArea, PMDRecv* DataArea, int Length);
																// Recv Parameter Message Display Get
	int		fnAPP_RecvPMDMove(char Data, PMDRecv* pDataArea, int Length);
																// Recv Parameter Message Display Get
    int		fnAPP_ACRecvHost();								// A/C Recv Host
	int		fnAPP_SaveECTempData(char* strData, int Length);
/////////////////////////////////////////////////////////////////////////////
#endif
