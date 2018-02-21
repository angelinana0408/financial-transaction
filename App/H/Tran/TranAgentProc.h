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
#define AGENT_PAY               34              // �ɷ�
#define AGENT_REMIT_ADD         35              // ��ַ���
#define AGENT_AC_PAY            36              // �˻��ɷ�
#define AGENT_AC_TRANSFER       37              // �˻��ܼ�ת��һ��ͨ
#define AGENT_AC_REMIT          38              // �˻��ܼһ��
#define AGENT_CARD_RETRACT      39				// �̿�  adde by yusy 08.09.09
#define AGENT_HZT				40				// �̿�ͨ����ת��������		added by yusy 2009.05.13
#define AGENT_TZH				41				// �̿�ͨ��������ת����
#define AGENT_OTHER_INQ			42				// ��ѯ��������
#define AGENT_AC_ASSU_TRAN      43				// Լ������
#define AGENT_AC_CUSASSU_DETAIL 44				// ��ѯ�ͻ�Լ��ϸ����Ϣ  ��ѯת�˻���Լ���ɷ�
#define AGENT_AC_NOASSU_PAY     45				// ��Լ���ɷ�
#define AGENT_AC_PAY_INQ        46				// �ɷ�Ƿ�Ѳ�ѯ
#define AGENT_REMIT_INF_INQ     47				// ��Ʊ��Ϣ��ѯ
#define AGENT_REMIT_IN			48				// �������					added by yusy 2009.07.17
#define	AGENT_AC_BALANCE_INQ    49				// ��ѯ�˻����				added by liyi 2009.07.21
#define AGENT_REMIT_PWD			50				// ������

#define AGENT_SMSPARITY			53				// ������֤
#define AGENT_ECHANNEL_INQ		54				// ����������ѯ
#define AGENT_ECHANNEL			55				// ��ͨ��������
#define AGENT_INQUIRY_CheckPin  56              // �忨����

////////////////////////////////////////
// Agent Status Type
#define RESOURCE_STATUS         1
#define DEVICE_STATUS           2
/////////////////////////////////////////////////////////////////////////////


//	Library Variable
/////////////////////////////////////////////////////////////////////////////
	typedef struct _tagRcvShareMessage	
	{
		char strMsgLen     [5];       // strData�ĳ���(���1000)
		char strTxID        [4];      // 6.ATM AP Message Processing sheet�� TXID
		char strTrID        [4];      // 6.ATM AP Message Processing sheet�� TRID
		char strResp        [1];      // ��������(�ο�Զ��Ӧ����)
		char strData     [1000];      // ��������(���1000)
	}RcvShareMessage, *LPRcvShareMessage;

	typedef struct _tagSndShareMessage
	{
		char strMsgLen    [5];     // strData�ĳ���(��� 10000)
		char strTxID       [4];    //  6.ATM AP Message Processing sheet�� TXID
		char strTrID       [4];    //  6.ATM AP Message Processing sheet�� TRID
		char strResp      [1];     // ��������(�ο�Զ��Ӧ����)
		char strData  [10000];     // ȡ���������Ӧ������(��� 10000)
    }SndShareMessage, *LPSndShareMessage;

	typedef UINT (*GetSharedMessage)(LPSTR, LPSTR, DWORD);
    typedef bool (*DataInitSharedMessage)(LPSTR);
    typedef bool (*PutSharedMessage)(LPSTR, LPSTR, DWORD);

	GetSharedMessage         pGetSharedMessage;
    DataInitSharedMessage    pDataInitSharedMessage;
    PutSharedMessage         pPutSharedMessage;	
	
	typedef struct _tagATMStatus
	{
		char sMTCCode   [11]; // ȡ���(ATM)������			
		char sStatusDesc [201]; // ȡ�������˵��(��������ϸ˵��)
	}ATMSTATUS, *LPATMSTATUS;

	typedef struct _tagRemainMediaInfo
	{
		char card          [1];           // ������
		char cash          [1];           // ������
		char reciept       [1];           // ƾ����ӡ��			
		char pb            [1];           // ���۴�ӡ��				
	}REMAINMEDIA, *LPREMAINMEDIA;
	
	typedef struct _tagDevRestInfo
	{
		char dev_alldev        [1];        //�����豸		
		char dev_card          [1];        //������		
		char dev_cash          [1];        //������		
		char dev_reciept       [1];        //ƾ����ӡ��		
		char dev_journal       [1];        //��ˮ��ӡ��		
		char dev_pb            [1];        //���۴�ӡ��		

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

	char szAtmRcvBuff[1000+14];   // ��������BUFFER
    char szAtmSndBuff[10000+14];  // ������BUFFER


	int bCommRecvFlag;              // ������ϱ�־ 0������δ��� 1���������
	int nAgentErrorDeviceNum;       // �豸���ϸ���
	int nAgentSaveErrorDeviceNum;   // �豸���ϸ���
//    int bAgentMsgSendFlg;         // ��Դ״̬�Ƿ����仯��־ 0���ޱ仯 1:״̬�����仯
//    int bAgentMsgErrorSendFlg;    // �豸����״̬�Ƿ����仯��־ 0���ޱ仯 1:״̬�����仯

	char szCompanyCode[3];
	char szAgentMachineKind[4];
	char szAgentOldStatus[14];
	char szAgentStatus[14];
//	char szAgentOldErrorStatus[8];
// 	char szAgentErrorStatus[8];
	char szAgentFtpServerIP[16];
	char szAgentMachineCode[21];

	BOOL bAgetDoorFLAG;               // �Ƿ��չ���״̬�����仯
	BOOL bAgentRebootFlag;            // ����������ʶλ ��ATM��ʼ��֮ǰ����������������ʶλ�ᱻ����
	int bAgentRestResultFlag;         // ��ʼ���ɹ���־ 1���ɹ���0:ʧ��  2:δ���
	int bAgentWithFlag;               // ȡ��ɹ���ʶ   1���ɹ���0:ʧ��
	int nAgentOperatorMode;           // �Ƿ񵽲���Աģʽ��
	BOOL nAgnetAtmOpenFistFailFlag;        // ��һ��ǩ��ʧ�ܱ�־   1����һ��  0:�ǵ�һ��
 	BOOL nAgentAtmAfterOpenFistFailFlag;   // ǩ���ɹ����һ��ǩ����·�Ͽ���־   1����һ��  0:�ǵ�һ��
    BOOL nAgentBidAtmColseFlag;       // ��ͣ����ɹ���ʶ
	BOOL nAgentAtmDvrErrorFlag;       // DVR���ϱ�־

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
