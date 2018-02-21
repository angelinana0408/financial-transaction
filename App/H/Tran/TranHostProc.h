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
	int		PinTypeFlag;			    // ������֤��־     0���飬1�����飬������1����
	char	CertifType[2];			    // ֤������         01-�������֤02-���ڲ�03-����֤04-���֤ 05-�۰ľ��������ڵ�ͨ��֤ 06-̨�����֤ 07-�������� 08-������� 09-��ʱ���֤ 10-����ʿ��֤ 11-������ְ�ɲ�֤
	int 	SuccessFlag[2];				// �ɹ���־         0:�ɹ� 1:ʧ�� 
	char	CurrType[3];				// ����             000:�����001:�۱�002:��Ԫ003:��Ԫ004:ŷԪ
	int 	AccountType;			    // �˻�����         0:��˽�˺�1:��˽��2:�Թ��˺�3:�Թ���
	int 	BillService;			    // �˵�����         0������ͨ1����ͨ
	int 	ChanType;			    	// ����             0�Զ�����1��̨2�绰������3����4��������5ATM6�����ն�7����
	char	OpenType[8];				// ��ͨ����         �������ַ���ת����10���Ƶ�ֵ 7-0λ��Ӧ7-0������
	char	NoteAwoke[6];				// �Ƿ��������     0������ 1����
	int     PinValiStyle;               // �ͻ�������֤��ʽ 0��֤�ñ��ؾ�̬���룬�������˻�����1��֤�ñ��ؾ�̬���룬�������ֻ��ϵ�����(��̨���ɷ��͵��ͻ��ֻ���)2��֤�ñ��ؾ�̬���룬���������ƶ�̬����
    char    OpenServFlag[8];            // ��ͨ����         Ŀǰ��һλ��Ӧ��ͨ��Լ�����ף�1��ͨ0����ͨ         
    int     NoteFacili;                 // ���ŷ�����	    �ƶ�0��ͨ1����2
	int     AssuType;                   // Լ������	        0�˹�1�Զ�
	char    TranType[4];                // ��������	        4��ַ���5������6��ر����˺Ż��7ת��ת��(ͬ��ͬ��)8ͬ�ǿ���9ת����10��ؿ��� 11����1001�ƶ��ɷ�1002��ͨ�ɷ�
    char     RemitReturnFlag[1];            // ��һ�����־	    0������1����
	int     TranState;                  // ״̬	Ԥ��״̬    0 ���� 1δ���� 2�Ѽ��� 3�Ѹ���
    char    ReturnCode[6];              // ������       	0-���׳ɹ�  �����D�D��"errcode.h"�еĴ����붨��
	char    JHTranCode[6];                // Ӧ�ô���         0��ʾ��ͨ���࣬�����ʾ��ͨ�����µ�ҵ��
	char    TranName[60];               // ��������Ӧ������	        ҵ������
	char    SumLimit[60];               // Ӧ����Ϣ1	    �ɷѵ����޶�
	char    ReservePin[6];              //Ԥ������          ������ʱҪ��
	int     DepTime;                    // һ��ͨ����
	char    TranMoney[19];                  //���׽��
	char    ReserveNum[3];              // Ԥ�����

} ACHostSend;

/////////////////////////////////////////////////////////////////////////////
//	After Resp A/C Manager Host Variable
/////////////////////////////////////////////////////////////////////////////
struct	tagACHostResp
{
    int     ReturnCode;                 // ������       	0-���׳ɹ�  �����D�D��"errcode.h"�еĴ����붨��
	char    ReturnMess[200];            // ������Ϣ	
	char    ClientName[60];             // ����
	char    ClientAdd[80];              // ��ַ
	char    MobilePhone[13];            // �ֻ�
    char	TelePhone[13];              // �绰
	char    PostCode[13];               // �ʱ� 
	int     BillService;                // �˵�����         0:��˽�˺�1:��˽��2:�Թ��˺�3:�Թ���
    char    Email[60];                  // 
	char    BankAccountNo[23];          // �����˻�
	char    CardAccountNo[23];          // ���п���
	char	OpenType[8];				// ��ͨ����         �������ַ���ת����10���Ƶ�ֵ 7-0λ��Ӧ7-0������
    int     PinValiStyle;               // �ͻ�������֤��ʽ 0��֤�ñ��ؾ�̬���룬�������˻�����1��֤�ñ��ؾ�̬���룬�������ֻ��ϵ�����(��̨���ɷ��͵��ͻ��ֻ���)2��֤�ñ��ؾ�̬���룬���������ƶ�̬����
    char    ClientManager[20];          // �ͻ�����
	char    ACState[2];                 // ״̬
	char    ReserveNum[3];              // Ԥ�����
	int     AssuType;                   // Լ������	        0�˹�1�Զ�
	char    AssuRemitMax[19];           // Լ�������     ת���˻������޶�
	char    TranSum[19];                // ���
	char    StartDate[8];               // ��ʼ����
	char    MoneyUse[60];               // �ʽ���;
	                     				// ����Լ������ʱ��
	char    TranType[4];                // ��������	        4��ַ���5������6��ر����˺Ż��7ת��ת��(ͬ��ͬ��)8ͬ�ǿ���9ת����10��ؿ��� 11����1001�ƶ��ɷ�1002��ͨ�ɷ�
	char    PayeeName[60];              // �տ�������     �������ɷ����ơ��������ơ�֤ȯ����
	char    PayeePostCode[6];           // �տ����ʱ�
	char    PayeeAdd[80];               // �տ��˵�ַ
	char    PayeeMess[80];              // �տ�����Ϣ��   ����ַ ���д��� һ��ͨ���� ����Ϊӡˢ��

	char    RemitReturnFlag[1];            // ��һ�����־	    0������1����
	char    PayeeAccount[23];           // �տ����˺�
	char    PayeeBankCode[23];          // ת�����д���  
    char    PayeeBankName[1];              // ת����������
    int     TranTypeAttri;              // ������������ 0:Ԥ��ʵ�ɶ�֧�� 1��ֻ֧��Ԥ�� 2��ֻ֧��ʵ�� 3����֧�ֽɷ�
	char    ReserveSum[19];             // �������
	char    OperType[3];                // ҵ������
	char    OpenCountNode[12];          // ��������
	char    ACTranServiceNo[17];        // ������ˮ��		�˻��ܼ���ˮ��
	char    LiquiDate[8];               // ��������
	char    TranDate[8];                // ��������
	char    TranTime[8];                // ����ʱ��
	char    ClientNum[10];              // �ͻ���	10	�˻��ܼ��ڲ��ͻ���
	char    TranCode[6];                // ������ ����ʱ���˻��ܼҽ�����
	char	CurrType[3];				// ����     Ŀǰֻ�������        000:�����001:�۱�002:��Ԫ003:��Ԫ004:ŷԪ
	char    CommCharge[19];             // ������  �ܹ�
	char    RealCharge[19];             // ʵ��������		��ѵ�
	char    BankTeller[12];             // ���й�Ա  ����ʱ�����й�Ա
	char    Banknode[12];               // ��������  ����ʱ����������
	char    HostServiceNo[22];           // ǰ����ˮ��
    char    HostServiceNo1[22];          // ������ˮ��	
    char    PostalOrderNo[22];          // ��Ʊ����
	char    OperServiceNo[22];	        // Ӧ����ˮ	 �����ˮ	�ɷ���ˮ�ȵ�����ϵͳ��ˮ
	char    OperServiceNo1[22];	        // Ӧ����ˮ	 	�ɷ���ˮ�ȵ�����ϵͳ��ˮ
	char    OperMess[60];               // ������Ϣ
	char    ReserveTableNo[4];          // Ԥ�������		����Ԥ�����ף�����ֶ���ֵ
	char    OraServiceNo[17];           // ԭ������ˮ��   �����˹������ཻ�ף�������Է��ص�������¼����ˮ��
	int     TranChanType;               // ��������	 	����ʱ������
	char    JHNumber[23];                   // �ɷѺ���
	char    JHAmount[19];                   // Ӧ�ɷ��ý��
    int     RecNum;                     // ��¼����
	char    JHTranCode[6];                // Ӧ�ô���         0��ʾ��ͨ���࣬�����ʾ��ͨ�����µ�ҵ��
    char    TranName[60];               // Ӧ������	        ҵ�����ƽ�������
    char    SumLimit[60];               // Ӧ����Ϣ1	    �ɷѵ����޶�
    char    AcBalance[19];                  // �������

} ACHostResp;
	

/////////////////////////////////////////////////////////////////////////////
//	After Resp A/C Manager Host Variable
/////////////////////////////////////////////////////////////////////////////
struct	tagACHostRespACManager
{
  	char    PayeeName[60];              // �տ�������     �������ɷ����ơ��������ơ�֤ȯ����
	char    PayeePostCode[6];           // �տ����ʱ�
	char    PayeeAdd[80];               // �տ��˵�ַ
	char    PayeeAccount[23];           // �տ����˺�
    char    PayeeBankCode[23];          // ת�����д���  
    char    PayeeBankName[1];              // ת����������
	char    ReserveSum[19];             // �������   
	char    JHNumber[23];                   // �ɷѺ���
	char    TranType[4];                // �ɷ�����  //��������
	char    ReserveNum[3];              // Ԥ�����
	int     ArrayNum;                   // ҳ�����ӵı��
	int     TranTypeAttri;              // ������������0:Ԥ��ʵ�ɶ�֧�� 1��ֻ֧��Ԥ�� 2��ֻ֧��ʵ�� 3����֧�ֽɷ�
	char     RemitReturnFlag[1];            // ��һ�����־	    0������1����
	char    TranName[60];               // Ӧ������	        ҵ�����ƽ�������
// 	char    AcBalance;                  // �������
	char    JHTranCode[6];                // Ӧ�ô���         0��ʾ��ͨ���࣬�����ʾ��ͨ�����µ�ҵ��


}ACHostRespACManager[100];
struct	tagACHostRespRemitAssu
{
  	char    PayeeName[60];              // �տ�������     �������ɷ����ơ��������ơ�֤ȯ����
	char    PayeePostCode[6];           // �տ����ʱ�
	char    PayeeAdd[80];               // �տ��˵�ַ
	char    PayeeAccount[23];           // �տ����˺�
    char    PayeeBankCode[23];          // ת�����д���  
    char    PayeeBankName[1];              // ת����������
	char    ReserveSum[19];             // �������   
	char    JHNumber[23];                  // �ɷѺ���
	char    TranType[4];                // �ɷ�����  //��������
	char    ReserveNum[3];              // Ԥ�����
	int     ArrayNum;                   // ҳ�����ӵı��
    int     TranTypeAttri;              // ������������0:Ԥ��ʵ�ɶ�֧�� 1��ֻ֧��Ԥ�� 2��ֻ֧��ʵ�� 3����֧�ֽɷ�
	char     RemitReturnFlag[1];            // ��һ�����־	    0������1����
	char    TranName[60];               // Ӧ������	        ҵ�����ƽ�������
	char    SumLimit[60];               // Ӧ����Ϣ1	    �ɷѵ����޶�
// 	char    AcBalance;                  // �������
}ACHostRespRemitAssu[100];

struct	tagACHostRespPayAssu
{
  	char    PayeeName[60];              // �տ�������     �������ɷ����ơ��������ơ�֤ȯ����
	char    PayeePostCode[6];           // �տ����ʱ�
	char    PayeeAdd[80];               // �տ��˵�ַ
	char    PayeeAccount[23];           // �տ����˺�
    char    PayeeBankCode[23];          // ת�����д���  
    char    PayeeBankName[1];              // ת����������
	char    ReserveSum[19];             // �������   
	char    JHNumber[23];                  // �ɷѺ���
	char    TranType[4];                // �ɷ�����  //��������
	char    ReserveNum[3];              // Ԥ�����
	int     ArrayNum;                   // ҳ�����ӵı��
	char    TranName[60];               // Ӧ������	        ҵ������
    int     TranTypeAttri;              // ������������0:Ԥ��ʵ�ɶ�֧�� 1��ֻ֧��Ԥ�� 2��ֻ֧��ʵ�� 3����֧�ֽɷ�
    char    SumLimit[60];               // Ӧ����Ϣ1	    �ɷѵ����޶�
// 	char    AcBalance;                  // �������
	char    JHAmount[19];                   // Ӧ�ɷ��ý��

}ACHostRespPayAssu[100];

struct	tagACHostRespPayUnAssu
{
  	char    PayeeName[60];              // �տ�������     �������ɷ����ơ��������ơ�֤ȯ����
	char    PayeePostCode[6];           // �տ����ʱ�
	char    PayeeAdd[80];               // �տ��˵�ַ
	char    PayeeAccount[23];           // �տ����˺�
    char    PayeeBankCode[23];          // ת�����д���  
    char    PayeeBankName[1];              // ת����������
	char    ReserveSum[19];             // �������   
	char    JHNumber[23];                   // �ɷѺ���
	char    TranType[4];                // �ɷ�����  //��������
	char    ReserveNum[3];              // Ԥ�����
	int     ArrayNum;                   // ҳ�����ӵı��
	char    TranName[60];               // Ӧ������	        ҵ������
    char    SumLimit[60];               // Ӧ����Ϣ1	    �ɷѵ����޶�
// 	char    AcBalance;                  // �������
    int     TranTypeAttri;              // ������������0:Ԥ��ʵ�ɶ�֧�� 1��ֻ֧��Ԥ�� 2��ֻ֧��ʵ�� 3����֧�ֽɷ�
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
