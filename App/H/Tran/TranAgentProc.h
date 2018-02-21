/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranAgentProc_H_INCLUDED_)
#define _TranAgentProc_H_INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// Attributes
public:

/////////////////////////////////////////////////////////////////////////////
//	Agent Cmd Define
/////////////////////////////////////////////////////////////////////////////
#define TRAN_KIND 	            0
#define CLERK_DOOR_OPEN	     	1						
#define CLERK_SHUTTER_OPEN		2					
#define CLERK_SHUTTER_CLOSE		3						
#define CLERK_ENTER_ADMIN		4						
#define CLERK_ENTER_TRAN	    5					
#define CLERK_DEVICE_INIT	    6						
#define CLERK_ATM_OPEN	        7						
#define CLERK_ATM_POWEROFF	    8	
#define CLERK_DOOR_CLOSE	    9
#define AGENT_OPENFAIL	        10
#define CLERK_OTHER	            99

#define AGENT_WITHDRAWAL        21
#define AGENT_INQUIRY           22
#define AGENT_DETAIL            23
#define AGENT_TRANSFER          24
#define AGENT_CHANGEPIN         25
#define AGENT_ADDCASH           26
#define AGENT_OPEN              27
#define AGENT_ERRORMODE         28
#define AGENT_STATUS            29
#define AGENT_ATMPTOTAL         30
#define AGENT_ADDCASHTOTAL      31
#define AGENT_ATMCTOTAL         32
#define AGENT_CANCEL            33
#define AGENT_PAY               34              // 缴费
#define AGENT_REMIT_ADD         35              // 按址汇款
#define AGENT_AC_PAY            36              // 账户缴费
#define AGENT_AC_TRANSFER       37              // 账户管家转账一本通
#define AGENT_AC_REMIT          38              // 账户管家汇款
#define AGENT_CARD_RETRACT      39				// 吞卡  adde by yusy 08.09.09
#define AGENT_HZT				40				// 绿卡通活期转其他储种		added by yusy 2009.05.13
#define AGENT_TZH				41				// 绿卡通其他储种转活期
#define AGENT_OTHER_INQ			42				// 查询其他储种
#define AGENT_AC_ASSU_TRAN      43				// 约定交易
#define AGENT_AC_CUSASSU_DETAIL 44				// 查询客户约定细项信息  查询转账汇款和约定缴费
#define AGENT_AC_NOASSU_PAY     45				// 非约定缴费
#define AGENT_AC_PAY_INQ        46				// 缴费欠费查询
#define AGENT_REMIT_INF_INQ     47				// 汇票信息查询
#define AGENT_REMIT_IN			48				// 汇兑入账					added by yusy 2009.07.17
#define	AGENT_AC_BALANCE_INQ    49				// 查询账户余额				added by liyi 2009.07.21
#define AGENT_REMIT_PWD			50				// 密码汇款

#define AGENT_SMSPARITY			53				// 短信验证
#define AGENT_ECHANNEL_INQ		54				// 电子渠道查询
#define AGENT_ECHANNEL			55				// 开通电子渠道
#define AGENT_INQUIRY_CheckPin  56              // 插卡验密

////////////////////////////////////////
// Agent Status Type
#define RESOURCE_STATUS         1
#define DEVICE_STATUS           2
/////////////////////////////////////////////////////////////////////////////


//	Library Variable
/////////////////////////////////////////////////////////////////////////////
	typedef struct _tagRcvShareMessage	
	{
		char strMsgLen     [5];       // strData的长度(最大1000)
		char strTxID        [4];      // 6.ATM AP Message Processing sheet的 TXID
		char strTrID        [4];      // 6.ATM AP Message Processing sheet的 TRID
		char strResp        [1];      // 处理结果码(参考远程应答码)
		char strData     [1000];      // 传输数据(最大1000)
	}RcvShareMessage, *LPRcvShareMessage;

	typedef struct _tagSndShareMessage
	{
		char strMsgLen    [5];     // strData的长度(最大 10000)
		char strTxID       [4];    //  6.ATM AP Message Processing sheet的 TXID
		char strTrID       [4];    //  6.ATM AP Message Processing sheet的 TRID
		char strResp      [1];     // 处理结果码(参考远程应答码)
		char strData  [10000];     // 取款机处理结果应答数据(最大 10000)
    }SndShareMessage, *LPSndShareMessage;

	typedef UINT (*GetSharedMessage)(LPSTR, LPSTR, DWORD);
    typedef bool (*DataInitSharedMessage)(LPSTR);
    typedef bool (*PutSharedMessage)(LPSTR, LPSTR, DWORD);

	GetSharedMessage         pGetSharedMessage;
    DataInitSharedMessage    pDataInitSharedMessage;
    PutSharedMessage         pPutSharedMessage;	
	
	typedef struct _tagATMStatus
	{
		char sMTCCode   [11]; // 取款机(ATM)故障码			
		char sStatusDesc [201]; // 取款机故障说明(故障码详细说明)
	}ATMSTATUS, *LPATMSTATUS;

	typedef struct _tagRemainMediaInfo
	{
		char card          [1];           // 读卡器
		char cash          [1];           // 出钞器
		char reciept       [1];           // 凭条打印机			
		char pb            [1];           // 存折打印机				
	}REMAINMEDIA, *LPREMAINMEDIA;
	
	typedef struct _tagDevRestInfo
	{
		char dev_alldev        [1];        //所有设备		
		char dev_card          [1];        //读卡器		
		char dev_cash          [1];        //出钞器		
		char dev_reciept       [1];        //凭条打印机		
		char dev_journal       [1];        //流水打印机		
		char dev_pb            [1];        //存折打印机		

	}DEVREST, *LPDEVREST;


	typedef struct tagAgentOldErrorStatus			// Host Header Message Send
	{
		char	CduStatus;					
		char	SprStatus;			
		char	JprStatus;	
		char	McuStatus;	
		char	PbStatus;
		char	PinStatus;
		char	DvrStatus;	
	} AgentDeviceStatus; 
	
	AgentDeviceStatus szAgentOldErrorStatus;
	AgentDeviceStatus szAgentErrorStatus;

	ATMSTATUS szAtmStatus;
	REMAINMEDIA szRemainMedia;
	DEVREST szDevRest;

	char szAtmRcvBuff[1000+14];   // 接收数据BUFFER
    char szAtmSndBuff[10000+14];  // 处理结果BUFFER


	int bCommRecvFlag;              // 交易完毕标志 0：接收未完成 1：接收完成
	int nAgentErrorDeviceNum;       // 设备故障个数
	int nAgentSaveErrorDeviceNum;   // 设备故障个数
//    int bAgentMsgSendFlg;         // 资源状态是否发生变化标志 0：无变化 1:状态发生变化
//    int bAgentMsgErrorSendFlg;    // 设备故障状态是否发生变化标志 0：无变化 1:状态发生变化

	char szCompanyCode[3];
	char szAgentMachineKind[4];
	char szAgentOldStatus[14];
	char szAgentStatus[14];
//	char szAgentOldErrorStatus[8];
// 	char szAgentErrorStatus[8];
	char szAgentFtpServerIP[16];
	char szAgentMachineCode[21];

	BOOL bAgetDoorFLAG;               // 是否保险柜门状态发生变化
	BOOL bAgentRebootFlag;            // 下载重启标识位 在ATM初始化之前下载重启则重启标识位会被重置
	int bAgentRestResultFlag;         // 初始化成功标志 1：成功，0:失败  2:未完成
	int bAgentWithFlag;               // 取款成功标识   1：成功，0:失败
	int nAgentOperatorMode;           // 是否到操作员模式下
	BOOL nAgnetAtmOpenFistFailFlag;        // 第一次签到失败标志   1：第一次  0:非第一次
 	BOOL nAgentAtmAfterOpenFistFailFlag;   // 签到成功后第一次签到链路断开标志   1：第一次  0:非第一次
    BOOL nAgentBidAtmColseFlag;       // 暂停服务成功标识
	BOOL nAgentAtmDvrErrorFlag;       // DVR故障标志

	char szAgentErrorCode[12];				// added by yusy 08.09.09
	char szTempAtmStatus[512];			// added by yusy 09.09.30
	char szAgentCheckErrorCode[3];		// added by yusy 09.10.28
	int  TranWriteFlag;
	int  ErrorSendFlag;					
    
/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Library Function
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//  fnAPL_AgentCommand   by hyl
/////////////////////////////////////////////////////////////////////////////
	int     fnAPP_AgentLoadLibrary();
	int     fnAPP_TranAgentCmd();
    int     fnAPP_AgentIdxGetData(int nAppRunType);
	int     fnAPP_AgentCompareDeviceStatus(int StatusType);
	char    fnAPP_GetCstStatus();
	
/////////////////////////////////////////////////////////////////////////////
#endif
