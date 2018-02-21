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
		BYTE		CmdCode[3];									// 命令码
		BYTE		Time[16];									// 时间
		BYTE		ChanIdx[4];									// 叠加的画面
		BYTE	    Cardno[19];									// 卡号
		BYTE		TradeType[3];								// 交易类型
		BYTE		Amount[12];									// 交易金额
		BYTE		SerialNo[8];								// 流水号
		BYTE		AtmId[8];									// 终端号
		BYTE		Checksum[4];								// 校验
	}DVR_CMD_DATASEND;
	
	typedef	struct
	{
		BYTE	  StartCode[4];									//
		BYTE	  DvrStatus[4];									// DVR状态
		BYTE	  ChanStatus[4];								// 摄像头状态
		BYTE	  CheckSum[2];									// 校验
		BYTE	  Reserved[2];									// 保留
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
int  fnAPP_DvrDataComm(int DvrCmdCode,int DvrChanIdx,int DvrTimeOut,char UseStat[8]); //命令,叠加通道,叠加时间,位置用途
int  fnAPP_DvrIdel();
#endif 
