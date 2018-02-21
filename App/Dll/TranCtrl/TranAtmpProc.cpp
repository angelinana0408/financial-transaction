/////////////////////////////////////////////////////////////////////////////
// TranAtmpProc.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include <wininet.h>

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Dll\LcTCPClient.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Recv Analysis Bid Command
int CTranCmn::fnAPL_UnpackAtmpCmd(char* Buffer, char* MessageKind)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnPackAtmpCmd()");

	int		Cnt = 0;
	int		i = 0;
	int		Length1 = 0;
	char	szTmpBit[1024];
	char	szTmpBuff[1024];
	BYTE	TempBuff[NETBUFFSIZE];
	
	memset(szTmpBit, 0, sizeof(szTmpBit));
	memset(szTmpBuff, 0, sizeof(szTmpBuff));
	memset(TempBuff, 0, sizeof(TempBuff));

	memset(&HMEATMPRecv, 0, sizeof(HMEATMPRecv));						// Host Message Edit Recv
	memset(&HMEATMPSend, 0, sizeof(HMEATMPSend));

	// Recv Data Field Length
	HMEATMPRecv.DataLength[0]  = 4;
	HMEATMPRecv.DataLength[1]  = MAXBITMAP/8;
	HMEATMPRecv.DataLength[2]  = -99;
	HMEATMPRecv.DataLength[3]  = 6;
	HMEATMPRecv.DataLength[4]  = 12;
	HMEATMPRecv.DataLength[7]  = 2;
	HMEATMPRecv.DataLength[11] = 8;
	HMEATMPRecv.DataLength[12] = 6;
	HMEATMPRecv.DataLength[13] = 8;
	HMEATMPRecv.DataLength[14] = 8;
	HMEATMPRecv.DataLength[15] = 8;
	HMEATMPRecv.DataLength[16] = 2;
	HMEATMPRecv.DataLength[18] = 4;
	HMEATMPRecv.DataLength[20] = 8;
	HMEATMPRecv.DataLength[21] = 36;
	HMEATMPRecv.DataLength[22] = 3;
	HMEATMPRecv.DataLength[24] = 80;
	HMEATMPRecv.DataLength[25] = 2;
	HMEATMPRecv.DataLength[28] = 8;
	HMEATMPRecv.DataLength[29] = 8;
	HMEATMPRecv.DataLength[30] = 8;
	HMEATMPRecv.DataLength[35] = -99;
	HMEATMPRecv.DataLength[36] = -999;
	HMEATMPRecv.DataLength[37] = 12;
	HMEATMPRecv.DataLength[38] = 16;
	HMEATMPRecv.DataLength[39] = 2;
	HMEATMPRecv.DataLength[40] = -99;
	HMEATMPRecv.DataLength[41] = 8;
	HMEATMPRecv.DataLength[42] = 15;
	HMEATMPRecv.DataLength[45] = -999;					//20140124,jl
	HMEATMPRecv.DataLength[46] = 10;
	HMEATMPRecv.DataLength[49] = 3;
	HMEATMPRecv.DataLength[51] = 7;
	HMEATMPRecv.DataLength[52] = 8;
	HMEATMPRecv.DataLength[53] = 16;
	HMEATMPRecv.DataLength[54] = 39;
	HMEATMPRecv.DataLength[60] = 4;
	HMEATMPRecv.DataLength[61] = -99;
	HMEATMPRecv.DataLength[62] = 8;
	HMEATMPRecv.DataLength[63] = -99;
	HMEATMPRecv.DataLength[72] = 4;
	HMEATMPRecv.DataLength[90] = 40;
	HMEATMPRecv.DataLength[93] = 3;
	HMEATMPRecv.DataLength[96] = -99;
	HMEATMPRecv.DataLength[102] = -99;
	HMEATMPRecv.DataLength[103] = -99;
	HMEATMPRecv.DataLength[120] = -999;
	HMEATMPRecv.DataLength[121] = -999;
	HMEATMPRecv.DataLength[122] = -999;
	HMEATMPRecv.DataLength[123] = -999;
	HMEATMPRecv.DataLength[128] = 8;

	memcpy(HMEATMPRecv.Data[0], &Buffer[Cnt], HMEATMPRecv.DataLength[0]);
	Cnt += HMEATMPRecv.DataLength[0];								// 0 Transaction Kind

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnpackAtmpCmd()HMEATMPRecv.Data[0][%s]", HMEATMPRecv.Data[0]);
	if (memcmp(HMEATMPRecv.Data[0], "0820", HMEATMPRecv.DataLength[0]) == 0)
          ;
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnpackAtmpCmd() INFORMATION TYPE MISMATCH");
		return FALSE;
	}

	memcpy(HMEATMPRecv.Data[1], &Buffer[Cnt], HMEATMPRecv.DataLength[1]);
																// 1 Bitmap
	memset(szTmpBit, 0, sizeof(szTmpBit));
	memcpy(szTmpBit, HMEATMPRecv.Data[1], HMEATMPRecv.DataLength[1]);

	
	for (i = 1; i < MAXBITMAP; i++)
	{
		BYTE BT = 0x01;
		if ((szTmpBit[(i-1)/8] & (BT<<(7-(i-1)%8))) >> (7-(i-1)%8))
		{
			HMEATMPRecv.DataRecvBit[i] = TRUE;
			if (-99 == HMEATMPRecv.DataLength[i])
			{
				Length1 = 0;
				memset(szTmpBuff, 0, sizeof(szTmpBuff));
				memcpy(szTmpBuff, Buffer + Cnt, 2);	
				Cnt += 2;
				memcpy(HMEATMPRecv.Data[i], szTmpBuff, 2);			// LL
				Length1 = Asc2Int(szTmpBuff, 2);
				memcpy(HMEATMPRecv.Data[i] + 2, &Buffer[Cnt], Length1);
				HMEATMPRecv.DataLength[i] = 2 + Length1;
				Cnt += Length1;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnpackAtmpCmd()HMEATMPRecv.Data[%d][%s], Length[%d]", i, HMEATMPRecv.Data[i], Length1);
				continue;
			}
			else
			if (-999 == HMEATMPRecv.DataLength[i])
			{
				Length1 = 0;
				memset(szTmpBuff, 0, sizeof(szTmpBuff));
				memcpy(szTmpBuff, Buffer + Cnt, 3);
				Cnt += 3;
				memcpy(HMEATMPRecv.Data[i], szTmpBuff, 3);			// LLL
				Length1 = Asc2Int(szTmpBuff, 3);
				memcpy(HMEATMPRecv.Data[i] + 3, &Buffer[Cnt], Length1);
				HMEATMPRecv.DataLength[i] = 3 + Length1;
				Cnt += Length1;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnpackAtmpCmd()HMEATMPRecv.Data[%d][%s], Length[%d]", i, HMEATMPRecv.Data[i], Length1);
				continue;
			}

			memcpy(HMEATMPRecv.Data[i], &Buffer[Cnt], HMEATMPRecv.DataLength[i]);
			Cnt += HMEATMPRecv.DataLength[i];
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnpackAtmpCmd()HMEATMPRecv.Data[%d][%s]", i, HMEATMPRecv.Data[i]);
		}
	}
	memcpy(MessageKind, HMEATMPRecv.Data[3], 6);

	if (memcmp(HMEATMPRecv.Data[41], m_pProfile->NETWORK.AtmSerialNum, HMEATMPRecv.DataLength[41]) == 0)
		;
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnpackAtmpCmd() ATM ID MISMATCH");
		return FALSE;
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnpackAtmpCmd(TranCode[%6.6s]):UnPack OVER!", HMEATMPRecv.Data[3]);
	return TRUE;
}

// Pack Bid Command
int CTranCmn::fnAPL_PackAtmpCmd(char* SendBuffer, bool bRejectFlag)
{

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_PackAtmpCmd ");
	int		Cnt = 0;
	int		i = 0;
	char	szTmpBit[1024];
	CString strATMPBitMap("");
	
    strATMPBitMap.Format("%0129.129s", "");
/////////////////////////以下开始进行打包    by zjd   ////////////处理BITMAP
		if ((memcmp(HMEATMPRecv.Data[3], "200100", 6) == 0)	||			//TC_ATMOPEN
			(memcmp(HMEATMPRecv.Data[3], "200200", 6) == 0)	||			//TC_ATMCLOSE
			(memcmp(HMEATMPRecv.Data[3], "200300", 6) == 0)	||			//TC_ATMREBOOT
			(memcmp(HMEATMPRecv.Data[3], "200400", 6) == 0)	||			//TC_ATMSTATUS
			(memcmp(HMEATMPRecv.Data[3], "200500", 6) == 0))			//TC_ATMRESETALL
		{

			strATMPBitMap.SetAt(0,  '1');								// Transaction Kind
			strATMPBitMap.SetAt(1,  '1');								// Bit Map
		    strATMPBitMap.SetAt(3,  '1');								// Process Code
			strATMPBitMap.SetAt(11, '1');				     			// Serial No
			strATMPBitMap.SetAt(12, '1');								// Time
			strATMPBitMap.SetAt(13, '1');								// Date
			strATMPBitMap.SetAt(39, '1');								// Response No
			strATMPBitMap.SetAt(41, '1');								// Atm Serial Number
		}
		else
		if ((memcmp(HMEATMPRecv.Data[3], "200600", 6) == 0)	||			//TC_ATMBIDADDOWNLOAD  实际上是通知  by zjd
			(memcmp(HMEATMPRecv.Data[3], "200700", 6) == 0)	||			//TC_ATMBIDADDOWNLOAD
			(memcmp(HMEATMPRecv.Data[3], "200800", 6) == 0)	||			//TC_ATMBIDSENDDETAIL  实际上是通知  by zjd
			(memcmp(HMEATMPRecv.Data[3], "200900", 6) == 0)	||			//TC_ATMBIDSENDDETAIL
			(memcmp(HMEATMPRecv.Data[3], "201000", 6) == 0)	||			//TC_ATMBIDSOFTUPDATE  实际上是通知  by zjd
			(memcmp(HMEATMPRecv.Data[3], "201100", 6) == 0))			//TC_ATMBIDSOFTUPDATE
		{
			strATMPBitMap.SetAt(0,  '1');								// Transaction Kind
			strATMPBitMap.SetAt(1,  '1');								// Bit Map
			strATMPBitMap.SetAt(3,  '1');								// Process Code
			strATMPBitMap.SetAt(11, '1');								// Serial No
			strATMPBitMap.SetAt(12, '1');								// Time
			strATMPBitMap.SetAt(13, '1');								// Date
			strATMPBitMap.SetAt(39, '1');								// Response No
			strATMPBitMap.SetAt(41, '1');								// Atm Serial Number
			strATMPBitMap.SetAt(122, '1');								// Send Information
			strATMPBitMap.SetAt(123, '1');								// Receive Information
		}
		else
			return FALSE;

/////////////////////////////打包头
		HHMSendHeader* pHHMSendData = (HHMSendHeader*)SendBuffer;
		memset(pHHMSendData, '0', sizeof(HHMSendHeader));
		memcpy(pHHMSendData->TPDU, "00000", 5);
		memcpy(pHHMSendData->NetworkFlag, "01", 2);
		memcpy(pHHMSendData->TransSerialNo, HMEATMPRecv.Data[11], 8);
		memcpy(pHHMSendData->ErrorCode, "0000", 4);

/////////////////////////////数据准备
        
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "bRejectFlag[%d]",bRejectFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HMEATMPRecv.Data[3][%s]",HMEATMPRecv.Data[3]);

		int		nTmp = 1;
		int		TempLength = 0;
		char sztemp[8];

		memset(szTmpBit, 0, sizeof(szTmpBit));

		if (HMEATMPRecv.DataLength[0] > 0)
		{
			memcpy(HMEATMPSend.Data[0], "0830", 4);
 			HMEATMPSend.DataLength[0] = 4;		
		}

		HMEATMPSend.DataLength[1] = 16;
		memset(HMEATMPSend.Data[1], '0', HMEATMPSend.DataLength[1]);
		
		if (HMEATMPRecv.DataLength[3] > 0)
		{
			memcpy(HMEATMPSend.Data[3], HMEATMPRecv.Data[3], HMEATMPRecv.DataLength[3]);
			HMEATMPSend.DataLength[3] = HMEATMPRecv.DataLength[3];
		}
		
		if (HMEATMPRecv.DataLength[11] > 0)
		{
			memcpy(HMEATMPSend.Data[11], HMEATMPRecv.Data[11], HMEATMPRecv.DataLength[11]);
			HMEATMPSend.DataLength[11] = HMEATMPRecv.DataLength[11];
		}
		
		if (HMEATMPRecv.DataLength[12] > 0)
		{
			memcpy(HMEATMPSend.Data[12], HMEATMPRecv.Data[12], HMEATMPRecv.DataLength[12]);
			HMEATMPSend.DataLength[12] = HMEATMPRecv.DataLength[12];
		}

		if (HMEATMPRecv.DataLength[13] > 0)
		{
			memcpy(HMEATMPSend.Data[13], HMEATMPRecv.Data[13], HMEATMPRecv.DataLength[13]);
			HMEATMPSend.DataLength[13] = HMEATMPRecv.DataLength[13];
		}


		if ((bRejectFlag == TRUE)  &&
			((memcmp(HMEATMPRecv.Data[3], "200200", 6) == 0) ||
			 (memcmp(HMEATMPRecv.Data[3], "200300", 6) == 0) ||
			 (memcmp(HMEATMPRecv.Data[3], "200400", 6) == 0) ||
			 (memcmp(HMEATMPRecv.Data[3], "200500", 6) == 0)))					//立即拒绝   by zjd
		{
			memcpy(HMEATMPSend.Data[39], "H4", 2);
			HMEATMPSend.DataLength[39] = 2;
		}
		else
		if ((bRejectFlag == TRUE)  && (memcmp(HMEATMPRecv.Data[3], "200800", 6) == 0))
		{
			memcpy(HMEATMPSend.Data[39], "H5", 2);
			HMEATMPSend.DataLength[39] = 2;
			strATMPBitMap.SetAt(123, '0');								// Receive Information
		}
		else
		{
			memcpy(HMEATMPSend.Data[39], "00", 2);
			HMEATMPSend.DataLength[39] = 2;
		}

		if (HMEATMPRecv.DataLength[41] > 0)
		{
			memcpy(HMEATMPSend.Data[41], HMEATMPRecv.Data[41], HMEATMPRecv.DataLength[41]);
			HMEATMPSend.DataLength[41] = HMEATMPRecv.DataLength[41];
		}
		
		if (strATMPBitMap.GetAt(122) == '1')
		{
			if (memcmp(HMEATMPRecv.Data[3], "200600", 6) == 0)
			{
				 HMEATMPSend.DataLength[122] = HMEATMPRecv.DataLength[122];
				 memset(HMEATMPSend.Data[122], '0', HMEATMPSend.DataLength[122]);
				 memcpy(HMEATMPSend.Data[122], HMEATMPRecv.Data[122], HMEATMPSend.DataLength[122]);
			}
			else
			if (memcmp(HMEATMPRecv.Data[3], "200700", 6) == 0)
			{
				HMEATMPSend.DataLength[122] = 28;
				memset(HMEATMPSend.Data[122], '0', HMEATMPSend.DataLength[123]);
				memcpy(&HMEATMPSend.Data[122][0],"025",3);
				memcpy(&HMEATMPSend.Data[122][3], strDownLoadFileName.GetBuffer(0), strDownLoadFileName.GetLength() > 12 ? 12 : strDownLoadFileName.GetLength());                                  //每次固定下载长度400
				memcpy(&HMEATMPSend.Data[122][15],GetSprintf("%08s",Int2Asc(SetBegin)),8);                 //起始位置
				memcpy(&HMEATMPSend.Data[122][23],&FileEnd,1);                                             //文件长度   BY HYL
				memcpy(&HMEATMPSend.Data[122][24],"0400",4);                                               //每次固定下载长度400
			}
			else
			if (memcmp(HMEATMPRecv.Data[3], "200800", 6) == 0)
			{
				HMEATMPSend.DataLength[122] = HMEATMPRecv.DataLength[122];
				memset(HMEATMPSend.Data[122], '0', HMEATMPSend.DataLength[122]);
				memcpy(HMEATMPSend.Data[122], HMEATMPRecv.Data[122], HMEATMPSend.DataLength[122]);
			}
			else
			if (memcmp(HMEATMPRecv.Data[3], "200900", 6) == 0)
			{
				HMEATMPSend.DataLength[122] = 11;
				memset(HMEATMPSend.Data[122], '0', HMEATMPSend.DataLength[122]);
				memcpy(HMEATMPSend.Data[122], HMEATMPRecv.Data[122], HMEATMPSend.DataLength[122]);
			}
			else
			if (memcmp(HMEATMPRecv.Data[3], "201000", 6) == 0)
			{
				HMEATMPSend.DataLength[122] = HMEATMPRecv.DataLength[122];
				memset(HMEATMPSend.Data[122], '0', HMEATMPSend.DataLength[122]);
				memcpy(HMEATMPSend.Data[122], HMEATMPRecv.Data[122], HMEATMPSend.DataLength[122]);
			}
			else
			if (memcmp(HMEATMPRecv.Data[3], "201100", 6) == 0)
			{
				HMEATMPSend.DataLength[122] = 28;
				memset(HMEATMPSend.Data[122], '0', HMEATMPSend.DataLength[123]);
				memcpy(&HMEATMPSend.Data[122][0],"025",3);
				memcpy(&HMEATMPSend.Data[122][3],strDownLoadFileName.GetBuffer(0), strDownLoadFileName.GetLength() > 12 ? 12 : strDownLoadFileName.GetLength());                                  //每次固定下载长度400
				memcpy(&HMEATMPSend.Data[122][15],GetSprintf("%08s",Int2Asc(SetBegin)),8);               //起始位置
				memcpy(&HMEATMPSend.Data[122][23],&FileEnd,1);                                        //文件长度   BY HYL
				memcpy(&HMEATMPSend.Data[122][24],"0400",4); 
			}
			else
			{
				HMEATMPSend.DataLength[122] = 3;
				memset(HMEATMPSend.Data[122], '0', HMEATMPSend.DataLength[122]);
				memcpy(HMEATMPSend.Data[122], "000", 3);
			}
		}
	
		if (strATMPBitMap.GetAt(123) == '1')								// 123 应答自定义数据  ans999
		{   
			if (memcmp(HMEATMPSend.Data[3], "200600", 6) == 0)
			{
				HMEATMPSend.DataLength[123] = 27;
				memset(HMEATMPSend.Data[123], '0', HMEATMPSend.DataLength[123]);
				memcpy(&HMEATMPSend.Data[123][0],"024",3);
				memcpy(&HMEATMPSend.Data[123][3],"0400",4);                                  //每次固定下载长度400
				memcpy(&HMEATMPSend.Data[123][7],GetSprintf("%08s",Int2Asc(SetBegin)),8);  //文件长度   BY HYL
				memcpy(&HMEATMPSend.Data[123][15],"            ", 12);
				memcpy(&HMEATMPSend.Data[123][15],strDownLoadFileName.GetBuffer(0), strDownLoadFileName.GetLength() > 12 ? 12 : strDownLoadFileName.GetLength());
			}
			else
			if (memcmp(HMEATMPSend.Data[3], "200700", 6) == 0)
			{
				HMEATMPSend.DataLength[123] = 3;
				memset(HMEATMPSend.Data[123], '0', HMEATMPSend.DataLength[123]);
 			}
 			else
			if (memcmp(HMEATMPSend.Data[3], "200800", 6) == 0)
			{
				HMEATMPSend.DataLength[123] = 34;
				memset(HMEATMPSend.Data[123], '0', HMEATMPSend.DataLength[123]);
				memcpy(&HMEATMPSend.Data[123][0],"031",3);
				memcpy(&HMEATMPSend.Data[123][3],"024",3);
				memcpy(&HMEATMPSend.Data[123][6],GetSprintf("%09d",SendFileLength),9);  //文件长度   BY HYL
				memcpy(&HMEATMPSend.Data[123][15],&HMEATMPSend.Data[122][3],14);
				memcpy(&HMEATMPSend.Data[123][29],&HMEATMPSend.Data[122][17],1);
				memcpy(&HMEATMPSend.Data[123][30],"0400",4);                            //主机上传明细报文固定为400
			}
			else
			if (memcmp(HMEATMPSend.Data[3], "200900", 6) == 0)
			{
//			 	 SendBufferLength = (SendFileLength - SetBegin) < 400 ? (SendFileLength - SetBegin ):400;
//			 	 strATMPBitMap.DataLength[123] = 411;  
//			 	 memset(strATMPBitMap.Data[123], '0', strATMPBitMap.DataLength[123]);
//			 	 memcpy(&strATMPBitMap.Data[123][0],"408",3);
//			 	 memcpy(&strATMPBitMap.Data[123][3],&FileEnd,1);
//			 	 memcpy(&strATMPBitMap.Data[123][4],"0400",4);
//			 	 memcpy(&strATMPBitMap.Data[123][8],"400",3);
//			 	 memcpy(&strATMPBitMap.Data[123][11],DetailTblTemp, SendBufferLength);
			}
			else
			if (memcmp(HMEATMPSend.Data[3], "201000", 6) == 0)
			{
				HMEATMPSend.DataLength[123] = 27;
				memset(HMEATMPSend.Data[123], '0', HMEATMPSend.DataLength[123]);
				memcpy(&HMEATMPSend.Data[123][0],"024",3);
				memcpy(&HMEATMPSend.Data[123][3],"0400",4);                                  //每次固定下载长度400
				memcpy(&HMEATMPSend.Data[123][7],GetSprintf("%08s",Int2Asc(SetBegin)),8);  //文件长度   BY HYL
				memcpy(&HMEATMPSend.Data[123][15],"            ", 12);
				memcpy(&HMEATMPSend.Data[123][15],strDownLoadFileName.GetBuffer(0), strDownLoadFileName.GetLength() > 12 ? 12 : strDownLoadFileName.GetLength());
			}
			else
			if (memcmp(HMEATMPSend.Data[3], "201100", 6) == 0)
			{
				HMEATMPSend.DataLength[123] = 3;
				memset(HMEATMPSend.Data[123], '0', HMEATMPSend.DataLength[123]);
 			}
			else
			{
				HMEATMPSend.DataLength[123] = 3;
				memset(HMEATMPSend.Data[123], '0', HMEATMPSend.DataLength[123]);
				memcpy(HMEATMPSend.Data[123], "000", 3);
			}
		}

/////////////////////////////数据打包
		Cnt = sizeof(HHMSendHeader);

		for (i = 0; i < MAXBITMAP; i++)
		{
			if (strATMPBitMap.GetAt(i) == '1')
			{
				if (i > 0)
					szTmpBit[(i - 1) / 8] |= (nTmp << (7 - (i - 1) % 8));
				memcpy(&SendBuffer[Cnt], HMEATMPSend.Data[i], HMEATMPSend.DataLength[i]);
				Cnt += HMEATMPSend.DataLength[i];
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_packAtmpCmd() HMEATMPSend.Data[%d][%s]", i, HMEATMPSend.Data[i]);
			}
		}
		
		memcpy(HMEATMPSend.Data[1], szTmpBit, HMEATMPSend.DataLength[1]);
		memcpy(&SendBuffer[HMEATMPSend.DataLength[0] + sizeof(HHMSendHeader)], HMEATMPSend.Data[1], HMEATMPSend.DataLength[1]);

		MakePack(GetSprintf("%04X", Cnt - 2).GetBuffer(0), sztemp, 4);
		memcpy(pHHMSendData->Length, sztemp, 2);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_packAtmpCmd():return(%d)", Cnt);
	return Cnt;
}

// Atm Bid Send Detail Notice
int CTranCmn::fnAPP_TranATMPSendDetailNotice()
{   
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPSendDetailNotice()");
	
	char		SendHostIp[15];
	char        iHostPort[5];
	int			nTempCount = 0;
	char        szFileType;
	FILE*		TempFp;
	CFileStatus	TempFileStatus;
	char        szDetailFileName[14];

	
	SetBegin=0;
	memset(szDetailFileName,0,sizeof(szDetailFileName));
	memset(szUploadFileNamePath,0,sizeof(szUploadFileNamePath));
    //200800获取主机下发的信息
	memcpy(szDetailFileName,&HMEATMPRecv.Data[122][3],14);
	memcpy(&szFileType,&HMEATMPRecv.Data[122][17],1);
	SetBegin = Asc2Int(&HMEATMPRecv.Data[122][18], 8);	
	memcpy(SendHostIp,&HMEATMPRecv.Data[122][26],15);
	memcpy(iHostPort,&HMEATMPRecv.Data[122][41],5);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szDetailFileName(%14.14s)",szDetailFileName);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szFileType(%c)",szFileType);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetBegin(%d)",SetBegin);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SendHostIp(%s)",SendHostIp);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "iHostPort(%s)",iHostPort);

	
	if(szFileType == 'J')
	{
		sprintf(szUploadFileNamePath, "%s\\%14.14s.LOG", _EJR_DETAILSEND, szDetailFileName);
	}
	else
	if(szFileType == 'R')
	{
		sprintf(szUploadFileNamePath, "%s\\%14.14s.SPR.LOG", _EJR_DETAILSEND, szDetailFileName);
	}
	else
	if(szFileType == 'S')
	{
	    sprintf(szUploadFileNamePath, "%s\\%14.14s.LINE.LOG", _EJR_DETAILSEND, szDetailFileName);
	}
    else
	{   
    	return T_EXIT;
	}

	TempFp = fopen(szUploadFileNamePath, "r");
	if(TempFp)
	{
		fseek(TempFp,0L,SEEK_END);   
		SendFileLength = ftell(TempFp);
		fclose(TempFp);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SendFileLength[%d]",SendFileLength);
    }
	else
	{   
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Open File[%s] Fail ",szUploadFileNamePath); 
		return T_EXIT;
	}

	if(SendFileLength < SetBegin)
		return T_EXIT;

	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPSendDetailnotice():return");
	return T_OK;
}

// Atm Bid Send Detail
int CTranCmn::fnAPP_TranATMPSendDetail()
{   
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPSendDetail()");
	
	char    	DetailTblTemp[400];      //数据长度
	int			nTempCount = 0;
	FILE*		TempFp;
	CFileStatus	TempFileStatus;
	char        FileEndFlag;
	char        szDetailFileName[14];
	int			iCurrLength;
	
	SetBegin=0;
	memset(szDetailFileName,0,sizeof(szDetailFileName));

	SetBegin = Asc2Int(&HMEATMPRecv.Data[122][4],8);                //获取偏移量

//	if(SetBegin >= SendFileLength)                                  //偏移量大于文件长度则不发报文
//	{   
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetBegin > = SendFileLentch");
//		return T_EXIT;
// 	} 
 	   
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetBegin(%d)",SetBegin);
	
	if (CFile::GetStatus(szUploadFileNamePath, TempFileStatus))
	{   
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CFile::GetStatus[%d]",CFile::GetStatus(szUploadFileNamePath, TempFileStatus));
		TempFp = fopen(szUploadFileNamePath, "rb");			
		if (TempFp)
		{
            if((SendFileLength - SetBegin) > 400)
			{
				memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
				fseek(TempFp,SetBegin,0);
				fread(&DetailTblTemp, 400, 1, TempFp);
				fclose(TempFp);
				FileEndFlag = '1';
			}
			else
			if((SendFileLength - SetBegin) == 400)
			{   
				memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
				fseek(TempFp,SetBegin,0);
				fread(&DetailTblTemp, 400, 1, TempFp);
				fclose(TempFp);
				FileEndFlag = '0';	
			}
			else
            if((SendFileLength - SetBegin) <  400 && (SendFileLength - SetBegin) > 0 )
			{
				memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
				fseek(TempFp,SetBegin,0);
				fread(&DetailTblTemp, SendFileLength - SetBegin, 1, TempFp);
				fclose(TempFp);
				FileEndFlag = '0';	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DetailTblTemp[%s]",DetailTblTemp);
			}
			
		}
		iCurrLength = (SendFileLength - SetBegin) < 400 ? (SendFileLength - SetBegin ):400;
		HMEATMPSend.DataLength[123] = iCurrLength + 11;  
		memset(HMEATMPSend.Data[123], ' ', HMEATMPSend.DataLength[123]);
		memcpy(&HMEATMPSend.Data[123][0],GetSprintf("%03.3d", iCurrLength + 8).GetBuffer(0),3);
		memcpy(&HMEATMPSend.Data[123][3],&FileEndFlag,1);
		memcpy(&HMEATMPSend.Data[123][4],GetSprintf("%04.4d", iCurrLength).GetBuffer(0),4);
		memcpy(&HMEATMPSend.Data[123][8],GetSprintf("%03.3d", iCurrLength).GetBuffer(0),3);
		memcpy(&HMEATMPSend.Data[123][11],DetailTblTemp, iCurrLength);

		if(SetBegin >= SendFileLength)                                  //偏移量大于文件长度则不发报文
		{   
			FileEndFlag = '0';
			HMEATMPSend.DataLength[123] = 11;  
			memset(HMEATMPSend.Data[123], ' ', HMEATMPSend.DataLength[123]);
			memcpy(&HMEATMPSend.Data[123][0],GetSprintf("%03.3d", 8).GetBuffer(0),3);
			memcpy(&HMEATMPSend.Data[123][3],&FileEndFlag,1);
			memcpy(&HMEATMPSend.Data[123][4],GetSprintf("%04.4d", 0).GetBuffer(0),4);
			memcpy(&HMEATMPSend.Data[123][8],GetSprintf("%03.3d", 0).GetBuffer(0),3);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetBegin > = SendFileLentch");
		}
		
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPSendDetail():return");
	return T_OK;
}


int CTranCmn::fnAPP_TranATMPAdDownloadNotice()
{   
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranATMPAdDownloadNotice()");
// 	fnAPP_InitializeDS();
	
//    char		FileVersion[4];
	char    	EquipmentBrand[3];      
	int         EquipmentType;
	char        OperatingSystem[3];
	int         DataLength;	
	CFileStatus	TempFileStatus;	
	int         Filelength;
//	char        TempNameList2[21]; 
//	int         FNameLength;
	SHFILEOPSTRUCT   FileOp;   
	char fPath[256],tPath[256];
	FileOp.wFunc = FO_COPY;
	FileOp.fFlags= FOF_NOCONFIRMATION;
	
    //200600获取主机下发的信息
	ADFileUseTime = GetSprintf("%12.12s",&HMEATMPRecv.Data[122][3]);
	ADFileEndTime = GetSprintf("%12.12s",&HMEATMPRecv.Data[122][15]);
	ADFileType = Asc2Int(&HMEATMPRecv.Data[122][27], 1);
	memcpy(EquipmentBrand,&HMEATMPRecv.Data[122][28], 3);
	EquipmentType = Asc2Int(&HMEATMPRecv.Data[122][31],1);
	memcpy(OperatingSystem,&HMEATMPRecv.Data[122][32],3);
	DataLength = Asc2Int(&HMEATMPRecv.Data[122][35],3);
   	ADFileList = GetSprintf("%s", &HMEATMPRecv.Data[122][38]);
	
	SplitString(ADFileList, "|", ADFileNameList);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FileList[%s]",ADFileList);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FileNameList.GetSize()[%d]",ADFileNameList.GetSize());
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::EquipmentType[%d]",EquipmentType);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FileType[%d]",ADFileType);

    _mkdir(_AP_ADUPDATEDOWNLOAD_DIR);
	_mkdir(_AP_ADUPDATEBACK_DIR);
	_mkdir(_AP_NOTICEUPDATEDOWNLOAD_DIR);
	_mkdir(_AP_NOTICEUPDATEBACK_DIR);
	_mkdir(_AP_NOTICEUPDATE_DIR);
	_mkdir(_AP_CAUTIONUPDATEDOWNLOAD_DIR);
	_mkdir(_AP_CAUTIONUPDATEBACK_DIR);
	_mkdir(_AP_CAUTIONUPDATE_DIR);
	
	Filelength = Asc2Int(ADFileNameList[0], 9);
	
    if( ADFileType ==1 )                                 //广告下发
	{	
		//原图片为永久广告进行备份 BY HYL
	    for(int count=1;count<=10;count++)
		{
			if(memcmp(IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO,GetSprintf("%s%d","ADPICTUREENDTIME",count)),"999999999999",12) == 0)
			{	                                                      
				DeleteFile(GetSprintf("%s\\%s%d%s",_AP_ADUPDATEBACK_DIR,"AD",count,".jpg"));
				memset(fPath,0,sizeof(fPath));
				memset(tPath,0,sizeof(tPath));
				strcpy(fPath,GetSprintf("%s\\%s%d%s",_AP_ADUPDATE_DIR,"AD",count,".jpg"));   
				strcpy(tPath,_AP_ADUPDATEBACK_DIR);   
				FileOp.pFrom = fPath;   
				FileOp.pTo = tPath;   
				if(!SHFileOperation(&FileOp))
				{		  
//					int ADNum=IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADPICTURENUM);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::[AD%d.jpg]ADBACKFile Move Success!",count);
				}
				else 
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::[AD%d.jpg]ADBACKFile Move Failed!",count);
					return T_EXIT;
				}
			}
		}
		
		//判断是否存在未下完的文件by hyl
		if(IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILELENGTH) !=0 
			&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAME) !=""
			&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELIST) == ADFileList)     //判断是否存在未下完的文件
		{
			SetBegin = IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILELENGTH,0);
			strDownLoadFileName = IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAME);
		}
		else
		{
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILELENGTH, 0);
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELISTNUM, 0);
			IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAME, "");
			IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELIST, ADFileList);
			SetBegin = 0;
			strDownLoadFileName.Format("%s",&ADFileNameList[0].GetBuffer(0)[9]);
		}
		
		IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELIST,ADFileList);               //正在下载的文件列表
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::strDownLoadFileName[%s]",strDownLoadFileName);
	}
	else
	if(ADFileType == 2 )                            
	{
		
	//原图片为永久公告进行备份 BY HYL
	  if(memcmp(IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREENDTIME),"999999999999",12) == 0)
	  {	                                                      
		   
		  DeleteFiles(_AP_NOTICEUPDATEBACK_DIR, -1);
		  memset(fPath,0,sizeof(fPath));
		  memset(tPath,0,sizeof(tPath));
		  strcpy(fPath,GetSprintf("%s\\%s",_AP_NOTICEUPDATE_DIR,"NOTICE*.*")); 
		  strcpy(tPath,_AP_NOTICEUPDATEBACK_DIR);   
		  FileOp.pFrom = fPath;   
		  FileOp.pTo = tPath;   
		  if(!SHFileOperation(&FileOp))
		  {		  
			  int NoticeNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTURENUM);
 			  IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTRUEBACKNUM,NoticeNum);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NOTICEBACKFile Move Success!");
		  }
		  else 
		  {
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NOTICEBACKFile Move Failed!");
			  return T_EXIT;
		  }
	  }	  
			
		//判断是否存在未下完的文件by hyl
		if(IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILELENGTH) !=0 
			&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAME) !=""
			&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTURETEMPLIST) == ADFileList)     //判断是否存在未下完的文件
		{
			SetBegin = IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILELENGTH,0);
			strDownLoadFileName = IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAME);
		}
		else
		{
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILELENGTH, 0);
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAMELISTNUM, 0);
			IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTURETEMPLIST, "");
			IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAME, ADFileList);
			SetBegin = 0;
			strDownLoadFileName.Format("%s",&ADFileNameList[0].GetBuffer(0)[9]);
		}
		
		IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTURETEMPLIST,ADFileList);               //正在下载的文件列表
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::strDownLoadFileName[%s]",strDownLoadFileName);	
	}
	else
	if(ADFileType == 3 )                            
	{
		
		//原图片为安全提示永久进行备份
		if(memcmp(IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREENDTIME),"999999999999",12) == 0)
		{	                                                      
			
			DeleteFiles(_AP_CAUTIONUPDATEBACK_DIR, -1);
			memset(fPath,0,sizeof(fPath));
			memset(tPath,0,sizeof(tPath));
			strcpy(fPath,GetSprintf("%s\\%s",_AP_CAUTIONUPDATE_DIR,"CAUTION*.*")); 
			strcpy(tPath,_AP_CAUTIONUPDATEBACK_DIR);   
			FileOp.pFrom = fPath;   
			FileOp.pTo = tPath;   
			if(!SHFileOperation(&FileOp))
			{		  
				int CautionNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTURENUM);
				IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTRUEBACKNUM,CautionNum);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CAUTIONBACKFile Move Success!");
			}
			else 
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CAUTIONBACKFile Move Failed!");
				return T_EXIT;
			}
		}	  
		
		//判断是否存在未下完的文件
		if(IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILELENGTH) !=0 
			&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAME) !=""
			&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTURETEMPLIST) == ADFileList)     //判断是否存在未下完的文件
		{
			SetBegin = IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILELENGTH,0);
			strDownLoadFileName = IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAME);
		}
		else
		{
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILELENGTH, 0);
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELISTNUM, 0);
			IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTURETEMPLIST, "");
			IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAME, ADFileList);
			SetBegin = 0;
			strDownLoadFileName.Format("%s",&ADFileNameList[0].GetBuffer(0)[9]);
		}
		
		IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTURETEMPLIST,ADFileList);               //正在下载的文件列表
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::strDownLoadFileName[%s]",strDownLoadFileName);	
	}
	else 
		return T_EXIT;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranATMPAdDownloadNotice(): return");
	return T_OK;
}

int CTranCmn::fnAPP_TranATMPAdDownload()
{   
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranATMPAdDownload()");
	int         FNameLength;
	bool		bOverFlag = FALSE;

	FNameLength = ADFileNameList[0].GetLength() - 9;
    if( ADFileType == 1 )                                 //广告下发
	{	
		int FileListNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELISTNUM, 0);          //如果存在未下完的文件,为列表第几个
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPAdDownload():ADPic1[%d]", FileListNum);
		if (FileListNum > ADFileNameList.GetSize())
		{
			FileListNum = 0;
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELISTNUM, 0);
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILELENGTH,0);                    //下载完一个文件把文件偏移量清空
		}
		fnAPP_TranATMPAdverDownLoadFileProc(ADFileNameList[FileListNum], &bOverFlag );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPAdDownload():bOverFlag[%d]", bOverFlag);
		FileListNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELISTNUM); 
		if( FileListNum + 1 == ADFileNameList.GetSize() && bOverFlag == TRUE)
		{
//			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADPICTURELIST, ADFileList);                        //完成后保存下载列表
//			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELISTNUM,0);              //如果存在未下完的文件,为列表第几个

			SplitString(ADFileList ,"|", ADFileNameList);
			char TempFileName[12];
			char ADFileName[12];
			for (int ADDOWNCOUNT=0; ADDOWNCOUNT < ADFileNameList.GetSize();ADDOWNCOUNT++)
			{
				FNameLength = ADFileNameList[ADDOWNCOUNT].GetLength() - 9;
				memcpy(&TempFileName,GetSprintf("%s",ADFileNameList[ADDOWNCOUNT]),ADFileNameList[ADDOWNCOUNT].GetLength());
				memcpy(ADFileName,GetSprintf("%s",&TempFileName[9]),FNameLength);
				for(int ADNUM=1;ADNUM<=10;ADNUM++)
				{
					if(memcmp(ADFileName,GetSprintf("%s%d%s","AD",ADNUM,".jpg").GetBuffer(0),FNameLength) == 0)
					{
					 IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREUSETIME",ADNUM),ADFileUseTime);
					 IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREENDTIME",ADNUM),ADFileEndTime);
					}				 
				}
			}		 
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELIST,"");                          //完成后清除临时下载列表
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPAdDownload():ADPic");
		}
	}
	else
	if(ADFileType == 2 )                            
	{
		int	FileListNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAMELISTNUM,0);          //如果存在未下完的文件,为列表第几个
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPNoticeDownload():NoticePic1[%d]", FileListNum);
		if (FileListNum > ADFileNameList.GetSize())
		{
			FileListNum = 0;
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAMELISTNUM, 0);
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILELENGTH,0);                    //下载完一个文件把文件偏移量清空
		}
		fnAPP_TranATMPNoticeDownLoadFileProc(ADFileNameList[FileListNum], &bOverFlag );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPNoticeDownload():bOverFlag[%d]", bOverFlag);
		FileListNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAMELISTNUM); 
		if( FileListNum + 1 == ADFileNameList.GetSize() && bOverFlag == TRUE)
		{
			SplitString(ADFileList ,"|", ADFileNameList);
//			char TempFileName[12];
// 			char ADFileName[12];
//			for (int ADDOWNCOUNT=0; ADDOWNCOUNT < ADFileNameList.GetSize();ADDOWNCOUNT++)
//			{
//				FNameLength = ADFileNameList[ADDOWNCOUNT].GetLength() - 9;
//				memcpy(&TempFileName,GetSprintf("%s",ADFileNameList[ADDOWNCOUNT]),ADFileNameList[ADDOWNCOUNT].GetLength());
//				memcpy(ADFileName,GetSprintf("%s",&TempFileName[9]),FNameLength);
//				for(int ADNUM=1;ADNUM<=10;ADNUM++)
//				{
//					if(memcmp(ADFileName,GetSprintf("%s%d%s","NOTICE",ADNUM,".jpg").GetBuffer(0),FNameLength) == 0)
//					{
//						IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","NOTICEPICTUREUSETIME",ADNUM),ADFileUseTime);
//						IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","NOTICEPICTUREENDTIME",ADNUM),ADFileEndTime);
//						
//					}				 
// 				}
// 			}
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREDOWNLOADNUM, FileListNum+1);
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREUSETIME,ADFileUseTime);
            IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREENDTIME,ADFileEndTime);
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAMELIST,""); 
            
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPAdDownload():Notice");
		}
	}
	else
	if(ADFileType == 3 )                        // added by yusy 2010.03.22
	{
		int	FileListNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELISTNUM,0);          //如果存在未下完的文件,为列表第几个
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPCautionDownload():CautionPic1[%d]", FileListNum);
		if (FileListNum > ADFileNameList.GetSize())
		{
			FileListNum = 0;
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELISTNUM, 0);
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILELENGTH,0);                    //下载完一个文件把文件偏移量清空
		}
		fnAPP_TranATMPCautionDownLoadFileProc(ADFileNameList[FileListNum], &bOverFlag );
		
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPCautionDownload():bOverFlag[%d]", bOverFlag);
		FileListNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELISTNUM); 
		if( FileListNum + 1 == ADFileNameList.GetSize() && bOverFlag == TRUE)
		{
			SplitString(ADFileList ,"|", ADFileNameList);
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREDOWNLOADNUM, FileListNum+1);
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREUSETIME,ADFileUseTime);
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREENDTIME,ADFileEndTime);
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELIST,""); 
			
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPAdDownload():Caution");
		}
	}
	else 
		return T_EXIT;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranATMPAdDownload():return");
	return T_OK;
}

int CTranCmn::fnAPP_TranATMPSoftUpdateNotice()
{   
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranATMPSoftUpdateNotice()");
// 	fnAPP_InitializeDS();
	

	char    	EquipmentBrand[3];      
	char        EquipmentType;
	char        OperatingSystem[3];
	int         DataLength;	
	CFileStatus	TempFileStatus;	
	int         Filelength;
	int         FNameLength;
	char        TempNameList2[21]; 

    _mkdir(_AP_SOFTUPDATE_DIR);
	_mkdir(_AP_SOFTBACKUPDATE_DIR);
	
    //201000获取主机下发的信息
	SWFileVersion = GetSprintf("%4.4s",&HMEATMPRecv.Data[122][3]);
	SWUpdateFileUseTime = GetSprintf("%12.12s",&HMEATMPRecv.Data[122][7]);
	memcpy(EquipmentBrand,&HMEATMPRecv.Data[122][19], 3);
	EquipmentType = Asc2Int(&HMEATMPRecv.Data[122][22],1);
	memcpy(OperatingSystem,&HMEATMPRecv.Data[122][23],3);
	DataLength = Asc2Int(&HMEATMPRecv.Data[122][26],3);
	SWFileList = GetSprintf("%s", &HMEATMPRecv.Data[122][29]);

	SplitString(SWFileList, "|", SWFileNameList);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::UpdateFileUseTime[%s]",SWUpdateFileUseTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FileList[%s]",SWFileList);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FileNameList.GetSize()[%d]",SWFileNameList.GetSize());
// 	SetBegin = 0;

	Filelength = Asc2Int(SWFileNameList[0].GetBuffer(0), 9);
	FNameLength = SWFileNameList[0].GetLength() - 9;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FNameLength[%d]",FNameLength);
	if (FNameLength > 12)											//liyi add 2008.10.05 防止软件更新文件名超过12字节
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FNameLength[%d] > 12, return T_EXIT",FNameLength);
		return T_EXIT;
	}
	memcpy(TempNameList2,GetSprintf("%s",SWFileNameList[0]).GetBuffer(0),SWFileNameList[0].GetLength());
	strDownLoadFileName.Format("%s",&TempNameList2[9]);
	
    ///////////////////////////////////////////////////////////////////////////////

	//判断是否存在未下完的文件by hyl
	if(IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILELENGTH) !=0                         //已下完的文件长度
		&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAME) !=""                      //正在下载的文件名
		&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELIST) == SWFileList)        //判断是否存在未下完的文件
	{
		SetBegin = IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILELENGTH,0);
		strDownLoadFileName = IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAME);
	}
	else
	{
		IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILELENGTH, 0);
		IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELISTNUM, 0);
		IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAME, "");
		IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELIST, SWFileList);
		SetBegin = 0;
        strDownLoadFileName.Format("%s",&SWFileNameList[0].GetBuffer(0)[9]);
	}
	
	IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELIST,SWFileList); //保存正在下载的文件列表

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::strDownLoadFileName[%s]",strDownLoadFileName);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPSoftUpdateNotice()");
  return T_OK;	
}

int CTranCmn::fnAPP_TranATMPSoftUpdate()
{   
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranATMPSoftUpdate()");
    int         SWNameLength;
	bool		SWbOverFlag = FALSE;
    
    SWNameLength = SWFileNameList[0].GetLength() - 9;
	
	int FileListNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELISTNUM, 0);          //如果存在未下完的文件,为列表第几个
	if (FileListNum > SWFileNameList.GetSize())
	{
		FileListNum = 0;
		IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELISTNUM, 0);
		IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILELENGTH,0);                    //下载完一个文件把文件偏移量清空
	}
	fnAPP_TranATMPDownloadFileProc(SWFileNameList[FileListNum], &SWbOverFlag );
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPSoftUpdate():SWbOverFlag[%d]", SWbOverFlag);
		FileListNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELISTNUM); 

	if(FileListNum + 1 == SWFileNameList.GetSize() && SWbOverFlag == TRUE)      
	{
		IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO,_INIKEY_TRANSACTION_HOSTDOWNLOADVERSION, SWFileVersion);
		IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO,_INIKEY_TRANSACTION_SOFTUPDATEFILEUSETIME, SWUpdateFileUseTime);
		IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO,_INIKEY_TRANSACTION_SOFTUPDATEFILELIST, SWFileList);                      //完成后保存下载列表
		IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELIST,"");                          //完成后清除临时下载列表
// 		IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELISTNUM,0);                        //如果存在未下完的文件,为列表第几个
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "insert UpdateFileUseTime",SWUpdateFileUseTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "insert FileList",SWFileList);

    } 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPSoftUpdate()");
	return T_OK;	
}


int CTranCmn::fnAPP_TranATMPDownloadFileProc(CString TempNameList, bool* SWbOverFlag)
{   
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranATMPDownloadFileProc()");
    FILE*		TempFp;
	int        TempDownLength;
	int        TempDownLength1;            //前置下送数据包长度
	CFileStatus	TempFileStatus;
	char        szTempFileNamePath[256];    //路径文件名
	BYTE        DetailTblTemp[1024];
	char        TempNameList1[21];          
	long        RecvSetbegin;
	char        szRecvFileName[24];
	char        OneFileEnd;
	int         DownFileNameLength;
	
//	SetBegin = 0;

	memset(szTempFileNamePath,0,sizeof(szTempFileNamePath));
	memset(TempNameList1,0,sizeof(TempNameList1));
	DownFilelength = Asc2Int(TempNameList, 9);
	DownFileNameLength = TempNameList.GetLength() - 9;
 	memcpy(&TempNameList1,GetSprintf("%s",TempNameList),TempNameList.GetLength());
	strDownLoadFileName.Format("%s",&TempNameList1[9]);

	memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
	memset(szRecvFileName,0,sizeof(szRecvFileName));
	memcpy(szRecvFileName,GetSprintf("%-12s",&HMEATMPRecv.Data[122][3]).GetBuffer(0), DownFileNameLength);	
	strDownLoadFileName.Format("%s",&TempNameList1[9]);
	RecvSetbegin = Asc2Int(GetSprintf("%08s", &HMEATMPRecv.Data[122][15]),8);
	memcpy(&FileEnd,&HMEATMPRecv.Data[122][23],1);
	TempDownLength = Asc2Int(&HMEATMPRecv.Data[122][24],4);
    TempDownLength1 = Asc2Int(&HMEATMPRecv.Data[122][28],3);
    memcpy(&DetailTblTemp,&HMEATMPRecv.Data[122][31],TempDownLength1);	

    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szRecvFileName[%s]",szRecvFileName);
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strDownLoadFileName[%s]",strDownLoadFileName);

	if (memcmp(strDownLoadFileName.GetBuffer(0), szRecvFileName, strDownLoadFileName.GetLength()) != 0)
	{
		strDownLoadFileName.Format("%s", szRecvFileName);
		IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELISTNUM, IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELISTNUM, 0) + 1);
		SetBegin = 0;
		RecvSetbegin = 0;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strDownLoadFileName-M[%s]",strDownLoadFileName);
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvSetbegin[%d]",RecvSetbegin);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Setbegin[%d]",SetBegin);
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TempDownLength1[%d]",TempDownLength1);

    sprintf(szTempFileNamePath, "%s\\%s", _AP_SOFTBACKUPDATE_DIR, strDownLoadFileName);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strDownLoadFileName[%s]",strDownLoadFileName);	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DownFilelength[%d]",DownFilelength);
	
	TempFp = fopen(szTempFileNamePath, "r");
	
	long SWFileLength;
	if(TempFp)
	{
		fseek(TempFp,0L,SEEK_END);   
		SWFileLength = ftell(TempFp);
		fclose(TempFp);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SendFileLength[%d]",SendFileLength);
		
    }
	else
		SWFileLength = 0;
	
	if (SetBegin == 0)
		TempFp = fopen(szTempFileNamePath, "wb");
	else
		TempFp = fopen(szTempFileNamePath, "ab");
	
	if(TempFp)
	{
		fclose(TempFp);
	}
	else
	{
		return T_EXIT;
	}
			   

	if (CFile::GetStatus(szTempFileNamePath, TempFileStatus))
	{   
		TempFp = fopen(szTempFileNamePath, "ab");
		if (TempFp)
		{   
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownFilelength - SetBegin[%d]",DownFilelength - SetBegin);
			if(DownFilelength - SetBegin <= 400)                                //每次固定传400字节
			{
				OneFileEnd = '0';
			}
			fseek(TempFp,SetBegin,0);
			fwrite(&DetailTblTemp, TempDownLength1, 1, TempFp);
			fclose(TempFp);
		}

		if (memcmp(strDownLoadFileName.GetBuffer(0), szRecvFileName, strDownLoadFileName.GetLength()) != 0)
			SetBegin = 0;
		else
			SetBegin += TempDownLength1;                                    //设置偏移量
		
		if(DownFilelength - SetBegin <= 400)                                //每次固定传400字节
		{
			OneFileEnd = '0';
			*SWbOverFlag = TRUE;
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILELENGTH,0);    //文件长度初始化
		    IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAME,"");    //当前下载文件名初始化
		}
		else
		{
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILELENGTH,SetBegin);   //文件已传送长度
            IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAME,strDownLoadFileName);    //当前下载文件名赋值
		}
		if ((FileEnd == '0')||(OneFileEnd == '0'))        
		{
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAME,"");                     //下载完一个文件把文件名清空
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILELENGTH,0);                    //下载完一个文件把文件偏移量清空
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CFile::GetStatus[%d]",CFile::GetStatus(szTempFileNamePath, TempFileStatus));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPDownloadFileProc():return");
    return T_OK;  
}

int CTranCmn::fnAPP_TranATMPAdverDownLoadFileProc(CString TempNameList,bool* bOverFlag)
{   
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranATMPAdverDownLoadFileProc(%s)", TempNameList);
    FILE*		TempFp;
	int        TempDownLength;
	int        TempDownLength1;            //前置下送数据包长度
	CFileStatus	TempFileStatus;
	char        szTempFileNamePath[256];    //路径文件名
	BYTE        DetailTblTemp[1024];
	char        TempNameList1[21];          
	long        RecvSetbegin;
	char        szRecvFileName[24];
// 	char        AdNewFileName[24];
	char        OneFileEnd;
	int         DownFileNameLength;
	

// 	SetBegin = 0;

	memset(szTempFileNamePath,0,sizeof(szTempFileNamePath));
// 	memset(AdNewFileName,0,sizeof(AdNewFileName));
	DownFilelength = Asc2Int(TempNameList, 9);
	DownFileNameLength = TempNameList.GetLength() - 9;
 	memcpy(&TempNameList1,GetSprintf("%s",TempNameList),TempNameList.GetLength());
	strDownLoadFileName.Format("%s",&TempNameList1[9]);

	memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
	memset(szRecvFileName,0,sizeof(szRecvFileName));
	memcpy(szRecvFileName,GetSprintf("%-12s",&HMEATMPRecv.Data[122][3]).GetBuffer(0), DownFileNameLength);	 
	RecvSetbegin = Asc2Int(GetSprintf("%08s", &HMEATMPRecv.Data[122][15]),8);
	memcpy(&FileEnd,&HMEATMPRecv.Data[122][23],1);
	TempDownLength = Asc2Int(&HMEATMPRecv.Data[122][24],4);
    TempDownLength1 = Asc2Int(&HMEATMPRecv.Data[122][28],3);
    memcpy(&DetailTblTemp,&HMEATMPRecv.Data[122][31],TempDownLength1);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szRecvFileName[%s]",szRecvFileName);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strDownLoadFileName[%s]",strDownLoadFileName);
	if (memcmp(strDownLoadFileName.GetBuffer(0), szRecvFileName, strDownLoadFileName.GetLength()) != 0)
	{
		strDownLoadFileName.Format("%s", szRecvFileName);
		IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELISTNUM, IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELISTNUM, 0) + 1);
		SetBegin = 0;
		RecvSetbegin = 0;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strDownLoadFileName-M[%s]",strDownLoadFileName);
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvSetbegin[%d]",RecvSetbegin);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Setbegin[%d]",SetBegin);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TempDownLength1[%d]",TempDownLength1);

	if(RecvSetbegin != SetBegin )
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvFileName or RecvSetbegin error");
		return T_EXIT;
    }			   
	
	////////////////////////////////////////////////////////////////////////////////////////
	//	memcpy(&AdNewFileName,GetSprintf("%2s%s%s","AD",Int2Asc(i + 1),".jpg"),6+strlen(Int2Asc(i)));
    sprintf(szTempFileNamePath, "%s\\%s", _AP_ADUPDATEDOWNLOAD_DIR, strDownLoadFileName);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strDownLoadFileName[%s]",strDownLoadFileName);	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DownFilelength[%d]",DownFilelength);
	
	
	TempFp = fopen(szTempFileNamePath, "r");
	
	long ADFileLength;
	if(TempFp)
	{
		fseek(TempFp,0L,SEEK_END);   
		ADFileLength = ftell(TempFp);
		fclose(TempFp);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SendFileLength[%d]",SendFileLength);
		
    }
	else
		ADFileLength = 0;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ADFileLength[%d]DownFilelength[%d]",ADFileLength,DownFilelength);

	if (SetBegin == 0)
		TempFp = fopen(szTempFileNamePath, "wb");
	else
		TempFp = fopen(szTempFileNamePath, "ab");
	
	if(TempFp)
	{
		fclose(TempFp);
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Success:[%s]",strDownLoadFileName);	
	}
	else
	{
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Fail:[%s] ",strDownLoadFileName);
		return T_EXIT;
	}
    
	if (CFile::GetStatus(szTempFileNamePath, TempFileStatus))
	{   
		TempFp = fopen(szTempFileNamePath, "ab");
		if (TempFp)
		{   
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownFilelength - SetBegin[%d]",DownFilelength - SetBegin);
            if(DownFilelength - SetBegin <= 400)                                //每次固定传400字节
				OneFileEnd = '0';
			fseek(TempFp,SetBegin,0);
			fwrite(&DetailTblTemp, TempDownLength1, 1, TempFp);
			fclose(TempFp);
			
		}
		
		if (memcmp(strDownLoadFileName.GetBuffer(0), szRecvFileName, strDownLoadFileName.GetLength()) != 0)
			SetBegin = 0;
		else
			SetBegin += TempDownLength1;                                    //设置偏移量

		if(DownFilelength - SetBegin <= 400)                                //每次固定传400字节
		{
			OneFileEnd = '0';
			*bOverFlag = TRUE;
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILELENGTH,0);    //文件长度初始化
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAME,"");    //当前下载文件名初始化
		}
		else
		{

			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILELENGTH,SetBegin);   //文件已传送长度
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAME,strDownLoadFileName);    //当前下载文件名赋值
		}

		if ((FileEnd == '0')||(OneFileEnd == '0'))
		{
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAME,"");                     //下载完一个文件把文件名清空
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILELENGTH,0);                    //下载完一个文件把文件偏移量清空
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CFile::GetStatus[%d]",CFile::GetStatus(szTempFileNamePath, TempFileStatus));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPAdverDownLoadFileProc():return");
    return T_OK;  
}


int CTranCmn::fnAPP_TranATMPNoticeDownLoadFileProc(CString TempNameList,bool* bOverFlag)
{   
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranATMPNoticeDownLoadFileProc(%s)", TempNameList);
    FILE*		TempFp;
	int         TempDownLength;
	int         TempDownLength1;            //前置下送数据包长度
	CFileStatus	TempFileStatus;
	char        szTempFileNamePath[256];    //路径文件名
	BYTE        DetailTblTemp[1024];
	char        TempNameList1[21];          
	long        RecvSetbegin;
	char        szRecvFileName[24];
//  	char        NoticeNewFileName[24];
	char        OneFileEnd;
	int         DownFileNameLength;
	
//	SetBegin = 0;
	memset(szTempFileNamePath,0,sizeof(szTempFileNamePath));
	memset(TempNameList1,0,sizeof(TempNameList));
	DownFilelength = Asc2Int(TempNameList, 9);
	DownFileNameLength = TempNameList.GetLength() - 9;
 	memcpy(&TempNameList1,GetSprintf("%s",TempNameList),TempNameList.GetLength());
	strDownLoadFileName.Format("%s",&TempNameList1[9]);

	memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
	memset(szRecvFileName,0,sizeof(szRecvFileName));
	memcpy(szRecvFileName,GetSprintf("%-12s",&HMEATMPRecv.Data[122][3]).GetBuffer(0), DownFileNameLength);	 
	RecvSetbegin = Asc2Int(GetSprintf("%08s", &HMEATMPRecv.Data[122][15]),8);
	memcpy(&FileEnd,&HMEATMPRecv.Data[122][23],1);
	TempDownLength = Asc2Int(&HMEATMPRecv.Data[122][24],4);
	TempDownLength1 = Asc2Int(&HMEATMPRecv.Data[122][28],3);
	memcpy(&DetailTblTemp,&HMEATMPRecv.Data[122][31],TempDownLength1);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szRecvFileName[%s]",szRecvFileName);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strDownLoadFileName[%s]",strDownLoadFileName);
	if (memcmp(strDownLoadFileName.GetBuffer(0), szRecvFileName, strDownLoadFileName.GetLength()) != 0)
	{
		strDownLoadFileName.Format("%s", szRecvFileName);
		IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAMELISTNUM, IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAMELISTNUM, 0) + 1);
		SetBegin = 0;
		RecvSetbegin = 0;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strDownLoadFileName-M[%s]",strDownLoadFileName);
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvSetbegin[%d]",RecvSetbegin);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Setbegin[%d]",SetBegin);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TempDownLength1[%d]",TempDownLength1);

	if(RecvSetbegin != SetBegin )
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvFileName or RecvSetbegin error");
		return T_EXIT;
    }			   
	
	////////////////////////////////////////////////////////////////////////////////////////
	//	memcpy(&AdNewFileName,GetSprintf("%2s%s%s","AD",Int2Asc(i + 1),".jpg"),6+strlen(Int2Asc(i)));
    sprintf(szTempFileNamePath, "%s\\%s", _AP_NOTICEUPDATEDOWNLOAD_DIR, strDownLoadFileName);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strDownLoadFileName[%s]",strDownLoadFileName);	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DownFilelength[%d]",DownFilelength);
	
	
	TempFp = fopen(szTempFileNamePath, "r");
	
	long ADFileLength;
	if(TempFp)
	{
		fseek(TempFp,0L,SEEK_END);   
		ADFileLength = ftell(TempFp);
		fclose(TempFp);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SendFileLength[%d]",SendFileLength);
		
    }
	else
		ADFileLength = 0;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ADFileLength[%d]DownFilelength[%d]",ADFileLength,DownFilelength);

	if (SetBegin == 0)
		TempFp = fopen(szTempFileNamePath, "wb");
	else
		TempFp = fopen(szTempFileNamePath, "ab");
	
	if(TempFp)
	{
		fclose(TempFp);
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Success:[%s]",strDownLoadFileName);	
	}
	else
	{
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Fail:[%s] ",strDownLoadFileName);
		return T_EXIT;
	}
    
	if (CFile::GetStatus(szTempFileNamePath, TempFileStatus))
	{   
		TempFp = fopen(szTempFileNamePath, "ab");
		if (TempFp)
		{   
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownFilelength - SetBegin[%d]",DownFilelength - SetBegin);
            if(DownFilelength - SetBegin <= 400)                                //每次固定传400字节
				OneFileEnd = '0';
			fseek(TempFp,SetBegin,0);
			fwrite(&DetailTblTemp, TempDownLength1, 1, TempFp);
			fclose(TempFp);
			
		}
		
		if (memcmp(strDownLoadFileName.GetBuffer(0), szRecvFileName, strDownLoadFileName.GetLength()) != 0)
			SetBegin = 0;
		else
			SetBegin += TempDownLength1;                                    //设置偏移量

		if(DownFilelength - SetBegin <= 400)                                //每次固定传400字节
		{
			OneFileEnd = '0';
			*bOverFlag = TRUE;
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILELENGTH,0);    //文件长度初始化
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAME,"");    //当前下载文件名初始化
		}
		else
		{

			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILELENGTH,SetBegin);   //文件已传送长度
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAME,strDownLoadFileName);    //当前下载文件名赋值
		}

		if ((FileEnd == '0')||(OneFileEnd == '0'))
		{
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAME,"");                     //下载完一个文件把文件名清空
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILELENGTH,0);                    //下载完一个文件把文件偏移量清空
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CFile::GetStatus[%d]",CFile::GetStatus(szTempFileNamePath, TempFileStatus));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPNoticeDownLoadFileProc():return");
    return T_OK;    
}

int CTranCmn::fnAPP_TranATMPCautionDownLoadFileProc(CString TempNameList,bool* bOverFlag)
{   
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranATMPCautionDownLoadFileProc(%s)", TempNameList);
    FILE*		TempFp;
	int         TempDownLength;
	int         TempDownLength1;            //前置下送数据包长度
	CFileStatus	TempFileStatus;
	char        szTempFileNamePath[256];    //路径文件名
	BYTE        DetailTblTemp[1024];
	char        TempNameList1[21];          
	long        RecvSetbegin;
	char        szRecvFileName[24];
//  	char        NoticeNewFileName[24];
	char        OneFileEnd;
	int         DownFileNameLength;
	
//	SetBegin = 0;
	memset(szTempFileNamePath,0,sizeof(szTempFileNamePath));
	memset(TempNameList1,0,sizeof(TempNameList));
	DownFilelength = Asc2Int(TempNameList, 9);
	DownFileNameLength = TempNameList.GetLength() - 9;
 	memcpy(&TempNameList1,GetSprintf("%s",TempNameList),TempNameList.GetLength());
	strDownLoadFileName.Format("%s",&TempNameList1[9]);

	memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
	memset(szRecvFileName,0,sizeof(szRecvFileName));
	memcpy(szRecvFileName,GetSprintf("%-12s",&HMEATMPRecv.Data[122][3]).GetBuffer(0), DownFileNameLength);	 
	RecvSetbegin = Asc2Int(GetSprintf("%08s", &HMEATMPRecv.Data[122][15]),8);
	memcpy(&FileEnd,&HMEATMPRecv.Data[122][23],1);
	TempDownLength = Asc2Int(&HMEATMPRecv.Data[122][24],4);
	TempDownLength1 = Asc2Int(&HMEATMPRecv.Data[122][28],3);
	memcpy(&DetailTblTemp,&HMEATMPRecv.Data[122][31],TempDownLength1);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szRecvFileName[%s]",szRecvFileName);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strDownLoadFileName[%s]",strDownLoadFileName);
	if (memcmp(strDownLoadFileName.GetBuffer(0), szRecvFileName, strDownLoadFileName.GetLength()) != 0)
	{
		strDownLoadFileName.Format("%s", szRecvFileName);
		IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELISTNUM, IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELISTNUM, 0) + 1);
		SetBegin = 0;
		RecvSetbegin = 0;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strDownLoadFileName-M[%s]",strDownLoadFileName);
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvSetbegin[%d]",RecvSetbegin);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Setbegin[%d]",SetBegin);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TempDownLength1[%d]",TempDownLength1);

	if(RecvSetbegin != SetBegin )
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvFileName or RecvSetbegin error");
		return T_EXIT;
    }			   
	
	////////////////////////////////////////////////////////////////////////////////////////
	//	memcpy(&AdNewFileName,GetSprintf("%2s%s%s","AD",Int2Asc(i + 1),".jpg"),6+strlen(Int2Asc(i)));
    sprintf(szTempFileNamePath, "%s\\%s", _AP_CAUTIONUPDATEDOWNLOAD_DIR, strDownLoadFileName);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strDownLoadFileName[%s]",strDownLoadFileName);	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DownFilelength[%d]",DownFilelength);
	
	
	TempFp = fopen(szTempFileNamePath, "r");
	
	long ADFileLength;
	if(TempFp)
	{
		fseek(TempFp,0L,SEEK_END);   
		ADFileLength = ftell(TempFp);
		fclose(TempFp);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SendFileLength[%d]",SendFileLength);
		
    }
	else
		ADFileLength = 0;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ADFileLength[%d]DownFilelength[%d]",ADFileLength,DownFilelength);

	if (SetBegin == 0)
		TempFp = fopen(szTempFileNamePath, "wb");
	else
		TempFp = fopen(szTempFileNamePath, "ab");
	
	if(TempFp)
	{
		fclose(TempFp);
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Success:[%s]",strDownLoadFileName);	
	}
	else
	{
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Fail:[%s] ",strDownLoadFileName);
		return T_EXIT;
	}
    
	if (CFile::GetStatus(szTempFileNamePath, TempFileStatus))
	{   
		TempFp = fopen(szTempFileNamePath, "ab");
		if (TempFp)
		{   
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownFilelength - SetBegin[%d]",DownFilelength - SetBegin);
            if(DownFilelength - SetBegin <= 400)                                //每次固定传400字节
				OneFileEnd = '0';
			fseek(TempFp,SetBegin,0);
			fwrite(&DetailTblTemp, TempDownLength1, 1, TempFp);
			fclose(TempFp);
			
		}
		
		if (memcmp(strDownLoadFileName.GetBuffer(0), szRecvFileName, strDownLoadFileName.GetLength()) != 0)
			SetBegin = 0;
		else
			SetBegin += TempDownLength1;                                    //设置偏移量

		if(DownFilelength - SetBegin <= 400)                                //每次固定传400字节
		{
			OneFileEnd = '0';
			*bOverFlag = TRUE;
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILELENGTH,0);    //文件长度初始化
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAME,"");    //当前下载文件名初始化
		}
		else
		{

			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILELENGTH,SetBegin);   //文件已传送长度
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAME,strDownLoadFileName);    //当前下载文件名赋值
		}

		if ((FileEnd == '0')||(OneFileEnd == '0'))
		{
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAME,"");                     //下载完一个文件把文件名清空
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILELENGTH,0);                    //下载完一个文件把文件偏移量清空
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CFile::GetStatus[%d]",CFile::GetStatus(szTempFileNamePath, TempFileStatus));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranATMPCautionDownLoadFileProc():return");
    return T_OK;    
}
