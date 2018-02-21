//////////////////////////////////////////////////////////////////////
// Comm.h: interface for the CComm class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(COMM_H_INCLUDED_)
#define COMM_H_INCLUDED_
/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Library Variable
/////////////////////////////////////////////////////////////////////////////

	typedef struct
	{
		BYTE    	StartCode[4];								// 
		BYTE		CmdCode[3];									// ������
		BYTE		Time[16];									// ʱ��
		BYTE		ChanIdx[4];									// ���ӵĻ���
		BYTE	    Cardno[19];									// ����
		BYTE		TradeType[3];								// ��������
		BYTE		Amount[12];									// ���׽��
		BYTE		SerialNo[8];								// ��ˮ��
		BYTE		AtmId[8];									// �ն˺�
		BYTE		Checksum[4];								// У��
	}DVR_CMD_DATASEND;
	
	typedef	struct
	{
		BYTE	  StartCode[4];									//
		BYTE	  DvrStatus[4];									// DVR״̬
		BYTE	  ChanStatus[4];								// ����ͷ״̬
		BYTE	  CheckSum[2];									// У��
		BYTE	  Reserved[2];									// ����
	} DVR_CMD_DATARECEIVE;
	
	DVR_CMD_DATASEND     DvrCmdDataSend;
	DVR_CMD_DATARECEIVE  DvrCmdDataReceive;

	
/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Library Function
/////////////////////////////////////////////////////////////////////////////

BOOL fnAPP_OpenComm(const char* pPort, int nBaudRate=57600, int nParity=NOPARITY, int nByteSize=8, int nStopBits=ONESTOPBIT);
BOOL fnAPP_CloseComm();
int  fnAPP_ReadComm(void* pData, int nLength);
int  fnAPP_WriteComm(void* pData, int nLength);
int  fnAPP_DvrDataComm(int DvrCmdCode,int DvrChanIdx,int DvrTimeOut,char UseStat[8]); //����,����ͨ��,����ʱ��,λ����;
int  fnAPP_DvrIdel();
#endif 
