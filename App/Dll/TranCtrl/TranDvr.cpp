// TranDvr.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Tran\TranCmn.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �����豸���
HANDLE hComm = INVALID_HANDLE_VALUE;

// �򿪴���
// ����: pPort - �������ƻ��豸·��������"COM1"��"\\.\COM1"���ַ�ʽ�������ú���
//       nBaudRate - ������       9600
//       nParity - ��żУ��       0
//       nByteSize - �����ֽڿ�� 8
//       nStopBits - ֹͣλ       1
BOOL CTranCmn::fnAPP_OpenComm(const char* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
	DCB dcb;		// ���ڿ��ƿ�

	int timeout1 = 100;
	int timeout2 = 1;
	int timeout3 = 3000;
	int timeout4 = 1;
	int timeout5 = 3000;
	
	COMMTIMEOUTS timeouts = {	// ���ڳ�ʱ���Ʋ���
	    	timeout1,				// ���ַ������ʱʱ��: 100 ms
			timeout2,					// ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
			timeout3,				// ������(�����)����ʱʱ��: 500 ms
			timeout4,					// д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
 			timeout5};				// ������(�����)д��ʱʱ��: 100 ms


	if(hComm == INVALID_HANDLE_VALUE)
	{
		hComm = CreateFile(pPort,	// �������ƻ��豸·��
			GENERIC_READ | GENERIC_WRITE,	// ��д��ʽ
			0,				// ����ʽ����ռ
			NULL,			// Ĭ�ϵİ�ȫ������
			OPEN_EXISTING,	// ������ʽ
			0,				// ���������ļ�����
			NULL);			// �������ģ���ļ�
		
		if(hComm == INVALID_HANDLE_VALUE) return FALSE;		// �򿪴���ʧ��
		
		GetCommState(hComm, &dcb);		// ȡDCB
		
		dcb.BaudRate = nBaudRate;
		dcb.ByteSize = nByteSize;
		dcb.Parity = nParity;
		dcb.StopBits = nStopBits;
		
		SetCommState(hComm, &dcb);		// ����DCB
		
		SetupComm(hComm, 4096, 1024);	// �������������������С
		
		SetCommTimeouts(hComm, &timeouts);	// ���ó�ʱ
	}
	return TRUE;
}

// �رմ���
BOOL CTranCmn::fnAPP_CloseComm()
{
	CloseHandle(hComm);
	hComm = INVALID_HANDLE_VALUE;
	return TRUE;
}

// д����
// ����: pData - ��д�����ݻ�����ָ��
//       nLength - ��д�����ݳ���
// ����: ʵ��д������ݳ���
int CTranCmn::fnAPP_WriteComm(void* pData, int nLength)
{
	DWORD dwNumWrite;	// ���ڷ��������ݳ���	
	WriteFile(hComm, pData, (DWORD)nLength, &dwNumWrite, NULL);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_WriteComm()returnNumWrite(%d)",dwNumWrite);
	return (int)dwNumWrite;
}

// ������
// ����: pData - ���������ݻ�����ָ��
//       nLength - ������������ݳ���
// ����: ʵ�ʶ��������ݳ���
int CTranCmn::fnAPP_ReadComm(void* pData, int nLength)
{
	DWORD dwNumRead;	// �����յ������ݳ���
	ReadFile(hComm, pData, (DWORD)nLength, &dwNumRead, NULL);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_ReadComm()returnNumRead(%d)",dwNumRead);
	return (int)dwNumRead;
	
}


//////////////////////////////////////////////////////////////////////////////
//DVR SETDATA BY HYL
int CTranCmn::fnAPP_DvrDataComm(int DvrCmdCode,int DvrChanIdx,int DvrTimeOut,char UseStat[8])
{	
    CString TranType;
	BYTE TempSum;
	BYTE TempSumRe[2];
	BYTE TempDataSend[81];
	BYTE TempDataReceive[16];
	CString m_strPort;
	m_strPort =GetSprintf("\\\\.\\%s",IniGetStr(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TRAN_OPENCOMM,"COM1"));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_DvrDataComm()CommPort[%s]",m_strPort);
	if(fnAPP_OpenComm(m_strPort,9600,NOPARITY,8,ONESTOPBIT) == FALSE)      
	{ 
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_OpenComm() openfail");
	    fnAPP_CloseComm();
		return FALSE;
	}
	memset(&DvrCmdDataSend,0x00, sizeof(DvrCmdDataSend));
	memset(&DvrCmdDataReceive,0x00, sizeof(DvrCmdDataReceive));
	memset(&TempSum,0x00, sizeof(TempSum));
	memset(&TempSumRe,0x00, sizeof(TempSumRe));
	memset(&TempDataSend,0x00, sizeof(TempDataSend));
	memset(&TempDataReceive,0x00, sizeof(TempDataReceive));
	
	if(TranCode == TC_WITHDRAWAL)
	{						
		TranType = "CWD";
	}
	else 
	if(TranCode == TC_CHANGEPIN) 
	{
		TranType = "PIN";
	}
	else 
	if(TranCode == TC_TRANSFER || TranCode == TC_HZT || TranCode == TC_TZH)
	{
		TranType = "TFR";
	}
	else
	if(TranCode == TC_LOAD)
	{
		TranType = "TSAL";
	}
	else
	if(TranCode == TC_BOUNDMDOIFY)
	{
		TranType = "MAL";
	}
	else 
	if(TranCode == TC_INQUIRY || TranCode == TC_OTHER_INQ || TranCode == TC_DETAIL ||  TranCode == TC_INQUIRY_CheckPin) 
	{
		TranType = "INQ";
	}
	else
	if (TranCode == TC_REMIT_ADD || TranCode == TC_REMIT_PWD || TranCode == TC_REMIT_IN)
	{
		TranType = "RMT";
	}
	else
	if (TranCode == TC_PAY)
	{
		TranType = "PAY";
	}
	else
	if (TranCode == TC_ECHANNEL)
	{
		TranType = "ECH";
	}
	else
	{
		TranType = "   ";
	}

	DvrCmdDataSend.StartCode[0] = 0x02;							
	DvrCmdDataSend.StartCode[1] = 'A';
	DvrCmdDataSend.StartCode[2] = 'T';
	DvrCmdDataSend.StartCode[3] = 'M';
	
//	memcpy(DvrCmdDataSend.CmdCode,GetSprintf("%03d",DvrCmdCode),3);          // ������
	if (DvrCmdCode == CMDCODE_INCARD)
	{
		DvrCmdDataSend.CmdCode[0] = 0x00;
		DvrCmdDataSend.CmdCode[1] = 0x03;
		DvrCmdDataSend.CmdCode[2] = 0x01;
	}
	else
	if (DvrCmdCode == CMDCODE_TAKECARD)
	{
		DvrCmdDataSend.CmdCode[0] = 0x00;
		DvrCmdDataSend.CmdCode[1] = 0x03;
		DvrCmdDataSend.CmdCode[2] = 0x02;
	}
	else
	if (DvrCmdCode == CMDCODE_OVERLAY)
	{
		DvrCmdDataSend.CmdCode[0] = 0x00;
		DvrCmdDataSend.CmdCode[1] = 0x03;
		DvrCmdDataSend.CmdCode[2] = 0x03;
	}	
	else
	if (DvrCmdCode == CMDCODE_SETTIME)
	{
		DvrCmdDataSend.CmdCode[0] = 0x00;
		DvrCmdDataSend.CmdCode[1] = 0x03;
		DvrCmdDataSend.CmdCode[2] = 0x04;
	}	
	else
	if (DvrCmdCode == CMDCODE_GETSTATUS)
	{
		DvrCmdDataSend.CmdCode[0] = 0x00;
		DvrCmdDataSend.CmdCode[1] = 0x03;
		DvrCmdDataSend.CmdCode[2] = 0x05;
	}	
	else
	if (DvrCmdCode == CMDCODE_RETAINCARD)
	{
		DvrCmdDataSend.CmdCode[0] = 0x00;
		DvrCmdDataSend.CmdCode[1] = 0x03;
		DvrCmdDataSend.CmdCode[2] = 0x06;
	}	
	else
	if (DvrCmdCode == CMDCODE_RETAINCASH)
	{
		DvrCmdDataSend.CmdCode[0] = 0x00;
		DvrCmdDataSend.CmdCode[1] = 0x03;
		DvrCmdDataSend.CmdCode[2] = 0x07;
	}
	memcpy(DvrCmdDataSend.Time, GetSprintf("%4.4s%2.2s%2.2s%2.2s%2.2s%2.2s00", GetDate().GetBuffer(0),
		&GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6],
		GetTime().GetBuffer(0), &GetTime().GetBuffer(0)[2], &GetTime().GetBuffer(0)[4]),16);	 // ʱ������
	memcpy(DvrCmdDataSend.ChanIdx,GetSprintf("%04.4d",DvrChanIdx),4);		// ���ӻ���


	char	szAccountNo[256];
	memset(szAccountNo, 0, sizeof(szAccountNo));
	memcpy(szAccountNo,CardAccountNo,19);
	CString strAcnt = EditString(szAccountNo, 19, "##############$$$$#", '#', '$', '*', TRUE);
	memcpy(DvrCmdDataSend.Cardno,strAcnt,19);	// ����

	memcpy(DvrCmdDataSend.TradeType,GetSprintf("%03.3s",TranType),3);		// ��������													// ��������
	memcpy(DvrCmdDataSend.Amount,GetSprintf("%012.12s",Accept.Money),12);	// ���׽��
	memcpy(DvrCmdDataSend.SerialNo,GetSprintf("%08.8s",m_pProfile->TRANS.SerialNo),8); // ��ˮ��
	memcpy(DvrCmdDataSend.AtmId,GetSprintf("%08.8s",m_pProfile->NETWORK.AtmSerialNum),8);	// �ն˺�

	if (DvrCmdCode == CMDCODE_INCARD)										
	{
		memcpy(DvrCmdDataSend.TradeType,"   ",3);
		memcpy(DvrCmdDataSend.Amount,"            ",12);
		memcpy(DvrCmdDataSend.SerialNo,"        ",8);
	}
	else
	if (DvrCmdCode == CMDCODE_SETTIME ||
		DvrCmdCode == CMDCODE_GETSTATUS)
	{
		memcpy(DvrCmdDataSend.Cardno,"8888888888888888888",19);
		memcpy(DvrCmdDataSend.TradeType,"   ",3);
		memcpy(DvrCmdDataSend.Amount,"            ",12);
		memcpy(DvrCmdDataSend.SerialNo,"        ",8);
	}

	if (memcmp(UseStat,"DOOROPEN",8) == 0)
	{
		memcpy(DvrCmdDataSend.Cardno,"8888888888888888888",19);
		memcpy(DvrCmdDataSend.SerialNo,"DOOROPEN",8);
	}
	else
	if (memcmp(UseStat,"DOORCLOS",8) == 0)
	{
		memcpy(DvrCmdDataSend.Cardno,"8888888888888888888",19);
		memcpy(DvrCmdDataSend.SerialNo,"DOORCLOS",8);
	}
	
	memcpy(&TempDataSend,&DvrCmdDataSend,81);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DvrCmdDataSend.CmdCode[%s]Time[%s]ChanIdx[%s]Cardno[%s]TradeType[%s]Amount[%s]SerialNo[%s]AtmId[%s]",
		GetSprintf("%3.3d",DvrCmdCode),
		GetSprintf("%16.16s",DvrCmdDataSend.Time),
		GetSprintf("%4.4s",DvrCmdDataSend.ChanIdx),
		GetSprintf("%19.19s",DvrCmdDataSend.Cardno),
		GetSprintf("%3.3s",DvrCmdDataSend.TradeType),
		GetSprintf("%12.12s",DvrCmdDataSend.Amount),
		GetSprintf("%8.8s",DvrCmdDataSend.SerialNo),
		GetSprintf("%8.8s",DvrCmdDataSend.AtmId));

	TempSum = TempDataSend[0];
	for (int j=0; j <77; j++ )
	{
		TempSum = TempSum + TempDataSend[j+1];
	}   

    memcpy(DvrCmdDataSend.Checksum, GetSprintf("%02x00", TempSum),4);
//	if(DvrCmdDataSend.Checksum[0] >= 'a'&&DvrCmdDataSend.Checksum[0] <= 'f')
//		DvrCmdDataSend.Checksum[0] =DvrCmdDataSend.Checksum[0] -0x20;             // ��0x20�����д��ĸ
//	if(DvrCmdDataSend.Checksum[1] >= 'a'&&DvrCmdDataSend.Checksum[1] <= 'f')
// 		DvrCmdDataSend.Checksum[1] =DvrCmdDataSend.Checksum[1] -0x20;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_DvrDataComm():DvrCmdDataSend.Checksum[%s]",GetSprintf("%4.4s",DvrCmdDataSend.Checksum));

    fnAPP_WriteComm(&DvrCmdDataSend,81);

//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_DvrDataComm():DvrCmdDataSend[%s]",GetSprintf("%81.81s",&DvrCmdDataSend));
	if (DvrCmdCode == CMDCODE_GETSTATUS)						// ֻ�л�ȡDVR״̬ʱ���Ż��л�Ӧ����
	{
		fnAPP_ReadComm(&TempDataReceive,16);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_DvrDataComm():TempDataReceive[%s]",TempDataReceive);

 		TempSum = TempDataReceive[0]^TempDataReceive[1]^TempDataReceive[2]^
			TempDataReceive[3]^TempDataReceive[4]^TempDataReceive[5]^
			TempDataReceive[6]^TempDataReceive[7]^TempDataReceive[8]^
			TempDataReceive[9]^TempDataReceive[10]^TempDataReceive[11];

 		memcpy(TempSumRe,GetSprintf("%02d", TempSum),2);
//		if(TempSumRe[0] >= 'a' && TempSumRe[0]<='f')
//			TempSumRe[0] = TempSumRe[0] - 0x20;
//		if(TempSumRe[1] >= 'a' && TempSumRe[1]<='f')
// 			 TempSumRe[1] = TempSumRe[1] - 0x20;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_DvrDataComm():TempSumRe[%s]",GetSprintf("%2.2s",TempSumRe));


		if( TempDataReceive[0] == 0x01			&& 
			TempDataReceive[1] == 'D'			&&
			TempDataReceive[2] == 'V'			&&
			TempDataReceive[3] == 'R'			&&
			TempDataReceive[12]==TempSumRe[0]	&& 
			TempDataReceive[13]==TempSumRe[1]	)
		{
			memcpy(&DvrCmdDataReceive,GetSprintf("%-16.16s",&TempDataReceive),16);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_DvrDataComm():DvrStatus[%s]ChanStatus[%s]",
		GetSprintf("%4.4s",DvrCmdDataReceive.DvrStatus),GetSprintf("%4.4s",DvrCmdDataReceive.ChanStatus));

			memcpy(InitInfo.DvrStatus,DvrCmdDataReceive.DvrStatus,4);
			memcpy(InitInfo.DvrChanStatus,DvrCmdDataReceive.ChanStatus,4);
			return TRUE;
		}
		else 
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_DvrDataComm():fnAPP_ReadComm:return FALSE");
			return FALSE;		
		}
	}
	return TRUE;
}


// Dvr Idel
int CTranCmn::fnAPP_DvrIdel()
{   
    static int DvrErrorTime=0;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_DvrIdel:AtmStatus[%d]",m_pDevCmn->AtmStatus);
    if(m_pDevCmn->AtmStatus == ATM_CUSTOM || m_pDevCmn->AtmStatus == ATM_READY)
	{	
		if(!fnAPP_DvrDataComm(CMDCODE_GETSTATUS,DVRCHAN_IDX0,DVR_GETSTATUS_TIMEOUT,"GETSTATU"))
		{
			DvrErrorTime++;
		}
		else  
			DvrErrorTime = 0;
		if(DvrErrorTime >= 3)
		{
			memcpy(InitInfo.DvrStatus,"0004",4);
			memcpy(InitInfo.DvrChanStatus,"2222",4);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_DvrIdel()DvrErrorTime[%d]",DvrErrorTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_DvrIdel()DvrErroWarning");
		}				
	}
	return T_OK;
	
}