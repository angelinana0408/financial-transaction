/////////////////////////////////////////////////////////////////////////////
// TranDev.cpp : implementation file
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

//#include "nb30.h"
// #pragma comment(lib,"netapi32.lib")

/////////////////////////////////////////////////////////////////////////////
//	Library Function
/////////////////////////////////////////////////////////////////////////////
// Get Error Code Kind
int	CTranCmn::fnAPL_GetErrorCodeKind(LPCTSTR szHsErrorCode)
{
	int		nTempErrorCodeKind = 0;
	int		nTempCmpLength = 0;

	for (int i = 1; i < itemof(ErrorConvTbl); i++) 
	{
		if (memcmp(szHsErrorCode, ErrorConvTbl[i].MTC, ErrorConvTbl[i].CmpLength) == 0)
		{
			if (ErrorConvTbl[i].CmpLength >= nTempCmpLength)	// Find(Max Length)
			{
				nTempCmpLength = ErrorConvTbl[i].CmpLength;
				nTempErrorCodeKind = ErrorConvTbl[i].Kind;
			}
		}
	}

	if (nTempErrorCodeKind == 0)								// Not Define
		nTempErrorCodeKind = 1;									// Device Error

	return nTempErrorCodeKind;
}

// Get Bank Error Code
CString	CTranCmn::fnAPL_GetBankErrorCode(LPCTSTR szHsErrorCode)
{
	CString		strTempLC("????");
	int			nTempCmpLength = 0;

	for (int i = 0; i < itemof(ErrorConvTbl); i++)				// 2005.04.03
	{
		if (memcmp(szHsErrorCode, ErrorConvTbl[i].MTC, ErrorConvTbl[i].CmpLength) == 0)
		{
			if (ErrorConvTbl[i].CmpLength >= nTempCmpLength)	// Find(Length Max)
			{
				nTempCmpLength = ErrorConvTbl[i].CmpLength;
				Strcpy(strTempLC, ErrorConvTbl[i].BankErrorCode, 4);
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	"2002",	1,		// JOURNAL JAM
//	"2002",	1,		// SLIP JAM
//	"2002",	1,		// JPR TIMEOVER(EVENT)
//	"2002",	1,		// SPR TIMEOVER(EVENT)
/////////////////////////////////////////////////////////////////////////////
	if (strTempLC == "2002")									// 2004.11.05
	{
		if (m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)
			Strcpy(strTempLC, "2002", 4);
	}
	if (strTempLC == "2002")
	{
		if (m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)
			Strcpy(strTempLC, "2002", 4);
	}

	return strTempLC;
}

// Format Mac Data 2008.10.13 计算MAC，需要删除两端空格
int	CTranCmn::fnAPL_FormatMacData(LPCTSTR SourceData, int& Length, LPSTR DeData)
{
	int		i = 0; 
	int		j = 0;
	char	MacBuffer1[NETBUFFSIZE];
	char	MacBuffer2[NETBUFFSIZE];
	CString	strTemp;

	memset(MacBuffer1, 0, sizeof(MacBuffer1));
	memset(MacBuffer2, 0, sizeof(MacBuffer2));
	
	memcpy(MacBuffer1, SourceData, Length);

	strTemp.Format("%s", MacBuffer1);
	strTemp.TrimLeft();
	strTemp.TrimRight();
	
	memset(MacBuffer1, 0, sizeof(MacBuffer1));
	memcpy(MacBuffer1, strTemp.GetBuffer(0), strTemp.GetLength());


	for (i = 0 ; i < Length ; i++)
	{
		if (MacBuffer1[i] >= 'a' && MacBuffer1[i] <= 'z')
			MacBuffer2[j++] = MacBuffer1[i] - 32;
		else 
		if ((MacBuffer1[i] >= 'A' && MacBuffer1[i] <= 'Z')	||
			(MacBuffer1[i] >= '0' && MacBuffer1[i] <= '9')	||
			(MacBuffer1[i] == ',')							||  
			(MacBuffer1[i] == '.'))
				MacBuffer2[j++] = MacBuffer1[i];
		else 
		if (MacBuffer1[i] == ' ')
		{
			MacBuffer2[j] = MacBuffer1[i];
			if (MacBuffer1[i+1] != ' ')
				j++;
		}
	}

	sprintf(DeData, "%s ", MacBuffer2);
	Length = j + 1 ;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPL_FormatMacData():InData[%s], Length[%d], OutData[%s]", SourceData, Length, DeData);
	return TRUE;
}

// Set Total Date
int	CTranCmn::fnAPL_SetTotalDate()
{
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALDATE, GetDate());

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetTotalDate():_REGKEY_DEVICE_TOTALDATE[%s]", GetDate());
	return TRUE;
}

// Add Day Total Count
int	CTranCmn::fnAPL_AddDayTotalCount()
{
	int		nTempCnt = 0;

	if ((RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALDATE).GetLength() == 8)	&&
		(memcmp(RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALDATE), &m_pProfile->TRANS.YYYYMMDD[0], 8) == 0))
	{
		nTempCnt = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DAYTOTALCNT) + 1;
	}

	RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DAYTOTALCNT, nTempCnt);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AddDayTotalCount():_REGKEY_DEVICE_DAYTOTALCNT[%d]", nTempCnt);
	return TRUE;
}

// Write Transaction Ejr
int	CTranCmn::fnAPL_WriteTransactonEjr(int nEjrKind)
{
	if (m_pProfile->DEVICE.EjrDevice == 1)						// CUP(SH) : 2006.04.19
		;
	else
		return TRUE;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_WriteTransactonEjr(nEjrKind[%d])", nEjrKind);

	FILE*		TempFp;
	char		szTempFileName[1024];
	int			nFirstWrite = FALSE;
	CString		strTemp("");
	CFileStatus	TempFileStatus;

	_mkdir(_EJR_FTP);
	memset(szTempFileName, 0, sizeof(szTempFileName));
	sprintf(szTempFileName, "%s\\%s", _EJR_FTP, _AP_EJRTEMP);
	
	if (CFile::GetStatus(szTempFileName, TempFileStatus))
		nFirstWrite = FALSE;
	else
		nFirstWrite = TRUE;

	TempFp = fopen(szTempFileName, "ac");
	if (TempFp)
	{
		switch (nEjrKind)
		{
			case  1:
				if (nFirstWrite)
					;											// File Create
				else
				{
					fprintf(TempFp, "\n");						// New Record
				}
				
				strTemp.Format("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100);
				if (TranCode == TC_WITHDRAWAL)
				{
					fprintf(TempFp, "%8.8s%6.6s %19.19s  CWD  %13.13s  %8.8s  001",
									  GetDate(),
									  GetTime(),
									  CardAccountNo,
									  strTemp,
									  &m_pProfile->TRANS.SerialNo[0]);
				}
				else
				if (TranCode == TC_TRANSFER)
				{
					fprintf(TempFp, "%8.8s%6.6s %19.19s  TRF  %13.13s  %8.8s  %19.19s    001",
									  GetDate(),
									  GetTime(),
									  CardAccountNo,
									  strTemp,
									  &m_pProfile->TRANS.SerialNo[0],
									  Accept.InAccountNum);
				}
				else
				if (TranCode == TC_HZT)							//liyi add 绿卡通
				{
					fprintf(TempFp, "%8.8s%6.6s %19.19s  HZT  %13.13s  %8.8s  %19.19s    001",
									  GetDate(),
									  GetTime(),
									  CardAccountNo,
									  strTemp,
									  &m_pProfile->TRANS.SerialNo[0],
									  Accept.InAccountNum);
				}
				else
				if (TranCode == TC_TZH)
				{
					fprintf(TempFp, "%8.8s%6.6s %19.19s  TZH  %13.13s  %8.8s  %19.19s    001",
									  GetDate(),
									  GetTime(),
									  CardAccountNo,
									  strTemp,
									  &m_pProfile->TRANS.SerialNo[0],
									  Accept.InAccountNum);
				}
				else
				if (TranCode == TC_REMIT_ADD)
				{
					fprintf(TempFp, "%8.8s%6.6s %19.19s  RMT  %13.13s  %8.8s  %19.19s    001",
									  GetDate(),
									  GetTime(),
									  CardAccountNo,
									  strTemp,
									  &m_pProfile->TRANS.SerialNo[0],
									  Accept.InAccountNum);
				}
				else
				if (TranCode == TC_REMIT_IN)
				{
					fprintf(TempFp, "%8.8s%6.6s %19.19s  RMT  %13.13s  %8.8s  %19.19s    001",
									  GetDate(),
									  GetTime(),
									  CardAccountNo,
									  strTemp,
									  &m_pProfile->TRANS.SerialNo[0],
									  Accept.InAccountNum);
				}
				else
				if (TranCode == TC_REMIT_PWD)
				{
					fprintf(TempFp, "%8.8s%6.6s %19.19s  RMT  %13.13s  %8.8s  %19.19s    001",
									  GetDate(),
									  GetTime(),
									  CardAccountNo,
									  strTemp,
									  &m_pProfile->TRANS.SerialNo[0],
									  Accept.InAccountNum);
				}
				else
				if (TranCode == TC_PAY)
				{
					fprintf(TempFp, "%8.8s%6.6s %19.19s  PAY  %13.13s  %8.8s  %19.19s    001",
									  GetDate(),
									  GetTime(),
									  CardAccountNo,
									  strTemp,
									  &m_pProfile->TRANS.SerialNo[0],
									  Accept.InAccountNum);
				}
				break;

			case  2:
			case  3:
			case  4:
			case  5:
			case  6:
			case  7:
			case  8:
			case  9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
				fprintf(TempFp, "  %03d", nEjrKind);
				break;

			case 20:
				fprintf(TempFp, "  R%2.2s", HostResp.RejectCode);
				break;
				
			default:
				break;
		}
		fflush(TempFp);
		fclose(TempFp);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_WriteTransactonEjr():return");
	return TRUE;
}

// Ftp Send Ejr Procedure
int CTranCmn::fnAPL_FtpSendEjrProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_FtpSendEjrProc()");

	char		szTempFileName[1024];
	FILE*		TempFp;
	CFileStatus	TempFileStatus;
	CString		strEjrFileName("");
	CString		strEjrFilePath("");

	if (m_pProfile->DEVICE.EjrDevice == 1)						// CUP(SH) : 2006.04.19
		;
	else
		return TRUE;

	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// Host Local Mode
		return TRUE;

	memset(szTempFileName, 0, sizeof(szTempFileName));
	sprintf(szTempFileName, "%s\\%s", _EJR_FTP, _AP_EJRTEMP);
	
	strEjrFileName.Format("%8.8s_%2.2s_%2.2s_%2.2s.%02d", 
							&m_pProfile->NETWORK.AtmSerialNum[0],
							&m_pProfile->TRANS.DeviceYYYYMMDD[2],
							&m_pProfile->TRANS.DeviceYYYYMMDD[4],
							&m_pProfile->TRANS.DeviceYYYYMMDD[6],
							RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DAYTOTALCNT));
	strEjrFilePath.Format("%s\\%s", _EJR_FTP, strEjrFileName);

	if (CFile::GetStatus(szTempFileName, TempFileStatus))
	{
		if (CopyFile(szTempFileName, strEjrFilePath, 0))
			DeleteFile(szTempFileName);
		else
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_FtpSendEjrProc():Copy File Fail");
			return FALSE;
		}
	}
	else
	{
		_mkdir(_EJR_FTP);
		TempFp = fopen(strEjrFilePath, "a");
		if (TempFp)
			fclose(TempFp);
		else
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_FtpSendEjrProc():Create File Fail");
			return FALSE;
		}
	}

	for (int i = 0; i < 30; i++)
	{
		if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME1) == "")
		{
			RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND1, 0);
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME1, "/ATMClient/");
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME1, strEjrFilePath);
			break;
		}
		else
		if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME2) == "")
		{
			RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND2, 0);
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME2, "/ATMClient/");
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME2, strEjrFilePath);
			break;
		}
		else
		if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME3) == "")
		{
			RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND3, 0);
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME3, "/ATMClient/");
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME3, strEjrFilePath);
			break;
		}
		else
		if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME4) == "")
		{
			RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND4, 0);
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME4, "/ATMClient/");
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME4, strEjrFilePath);
			break;
		}
		else
		{
			Delay_Msg(1000);
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_FtpSendEjrProc():return");
	return TRUE;
}

// Ftp Send Check Procedure
int CTranCmn::fnAPL_FtpSetFileNameProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_FtpSetFileNameProc()");

	char			szTempFileName[1024];
	CFileStatus		TempFileStatus;
	CStringArray	strTempArray;
	
	if (m_pProfile->DEVICE.EjrDevice == 1)						// CUP(SH) : 2006.04.19
		;
	else
		return TRUE;

	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// Host Local Mode
		return TRUE;

	if (HMERecv.DataRecvBit[48])
		;
	else 
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_FtpSetFileNameProc():DataRecvBit[48] Recv Error");
		return FALSE;
	}

	memset(szTempFileName, 0, sizeof(szTempFileName));
	memcpy(szTempFileName, &HMERecv.Data[48][11], Asc2Int(&HMERecv.Data[48][8], 3));

	if (CFile::GetStatus(szTempFileName, TempFileStatus))
		;
	else 
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_FtpSetFileNameProc():File Not Found");
		return FALSE;
	}

	if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME1) == "")
	{
		RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND1, 0);
		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME1, "/FTPTemp/");
		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME1, GetSprintf("%s", szTempFileName));
	}
	else
	if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME2) == "")
	{
		RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND2, 0);
		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME2, "/FTPTemp/");
		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME2, GetSprintf("%s", szTempFileName));
	}
	else
	if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME3) == "")
	{
		RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND3, 0);
		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME3, "/FTPTemp/");
		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME3, GetSprintf("%s", szTempFileName));
	}
	else
	if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME4) == "")
	{
		RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND4, 0);
		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME4, "/FTPTemp/");
		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME4, GetSprintf("%s", szTempFileName));
	}
	else
		return FALSE;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_FtpSetFileNameProc():return");
	return TRUE;
}

// Dvr Send Data Proc
int CTranCmn::fnAPL_DvrSendDataProc(LPCTSTR strSendKind)
{
//	SEND : 
//	POLLING			0x93 Length[2] Date[8] Time[6] ST BCC[2]
//	CARD INSERT		0x93 Length[2] Date[8] Time[6] CI CARDNO[19] BCC[2]
//	TRANSACITON		0x93 Length[2] Date[8] Time[6] TT CWD BCC[2]
//	MONEY PRESENT	0x93 Length[2] Date[8] Time[6] CE BCC[2]	=> 15 SEC WAIT
//	CARD PRESENT	0x93 Length[2] Date[8] Time[6] C0 BCC[2]	=> 15 SEC WAIT
//
//	RECV :
//	RESPONSE		0x83 Length[2] DVR DVRSTATUS[1] CAMERASTATUS[4] BCC[2]

	int		nReturn = FALSE;

	if (m_pProfile->DEVICE.DvrDevice)
		;
	else
		return nReturn;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_DvrSendDataProc(strSendKind[%s])", strSendKind);

	BYTE	szSendBuff[COMBUFFSIZE];
	int		nSendLength = 0;
	int		nDataLength = 0;
	int		nBccDataLength = 0;
	int		nDelayTime = 0;

	memset(szSendBuff, 0, sizeof(szSendBuff));

	szSendBuff[nSendLength] = 0x93;								// STX
	nSendLength += 1;

	nSendLength += 2;											// Length

	memcpy(&szSendBuff[nSendLength], GetDate().GetBuffer(0), 8);// Date
	nSendLength += 8;
	nDataLength += 8;
	nBccDataLength += 8;

	memcpy(&szSendBuff[nSendLength], GetTime().GetBuffer(0), 6);// Time
	nSendLength += 6;
	nDataLength += 6;
	nBccDataLength += 6;

	memcpy(&szSendBuff[nSendLength], strSendKind, 2);			// Send Kind
	nSendLength += 2;
	nDataLength += 2;
	nBccDataLength += 2;

	if (strSendKind == "CI")									// Card Insert
	{
		memcpy(&szSendBuff[nSendLength], CardAccountNo, strlen(CardAccountNo));
		nSendLength += strlen(CardAccountNo);
		nDataLength += strlen(CardAccountNo);
		nBccDataLength += strlen(CardAccountNo);				// Card Account No
	}
	else
	if (strSendKind == "TT")									// Transaction
	{
		if (m_pDevCmn->TranStatus == TRAN_TRAN)
		{
			switch (TranCode)
			{
				case TC_INQUIRY:
				case TC_OTHER_INQ:
				case TC_CUS_INF_INQ:
				case TC_CUS_INF_INQ_OTHERBANK:
				case TC_ECHANNEL_INQ:
				case TC_INQUIRY_CheckPin:
				
					memcpy(&szSendBuff[nSendLength], "INQ", 3);
					break;
				case TC_LOAD:
					memcpy(&szSendBuff[nSendLength], "TSAL", 4);
					break;
				case TC_BOUNDMDOIFY:
					memcpy(&szSendBuff[nSendLength], "MAL", 3);
					break;
				case TC_TRANSFER:
				case TC_HZT:								//liyi add 绿卡通
				case TC_TZH:
					memcpy(&szSendBuff[nSendLength], "TRF", 3);
					break;

				case TC_WITHDRAWAL:
					memcpy(&szSendBuff[nSendLength], "CWD", 3);
					break;

				case TC_CHANGEPIN:
					memcpy(&szSendBuff[nSendLength], "PIN", 3);
					break;
					
				case TC_REMIT_ADD:
					memcpy(&szSendBuff[nSendLength], "TRF", 3);
					break;

				case TC_REMIT_IN:
					memcpy(&szSendBuff[nSendLength], "TRF", 3);
					break;

				case TC_REMIT_PWD:
					memcpy(&szSendBuff[nSendLength], "TRF", 3);
					break;

				case TC_PAY:
					memcpy(&szSendBuff[nSendLength], "TRF", 3);
					break;

				case TC_SMSPARITY:
					memcpy(&szSendBuff[nSendLength], "SMS", 3);

				case TC_ECHANNEL:
					memcpy(&szSendBuff[nSendLength], "ECH", 3);

				default:
					return nReturn;
			}
			nSendLength += 3;
			nDataLength += 3;
			nBccDataLength += 3;
		}
		else
			return nReturn;
	}
	else
	if ((strSendKind == "CE")	||								// Cash Taken (15), Retract
		(strSendKind == "CO"))									// Card Taken (15), Retract
	{
		nDelayTime = 15;
	}
	else
	if ((strSendKind == "CB")	||								// Cash Dispense
		(strSendKind == "OD")	||								// Open Door
		(strSendKind == "CD")	||								// Close Door
		(strSendKind == "RS"))									// Reset
		;
	else
	{
		return nReturn;
	}

	nDataLength += 2;											// BCC Length
	nBccDataLength += 2;										// Length

	memcpy(&szSendBuff[1], GetSprintf("%02d", nDataLength), 2);
	BYTE Bcc = MakeBcc(&szSendBuff[1], nBccDataLength);
	MakeUnPack(&Bcc, &szSendBuff[nSendLength], 1);
	nSendLength += 2;

	nReturn = m_pDevCmn->fnDVR_SendData(MakeUnPack(szSendBuff, nSendLength), nDelayTime);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_DvrSendDataProc():return(%d)", nReturn);
	return nReturn;
}

// Ftp Send Ejm Proc
int CTranCmn::fnAPL_FtpSendEjmProc(int nEjmType)
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnAPL_FtpSendEjmProc()");

	CString			strInputStartDate("");						// 2005.02.24
	CString			strInputEndDate("");
	CString			strInputDate("");
	int				nInputStartDate = 0;
	int				nInputEndDate = 0;
	int				nInputDate = 0;
	CString			strTempEjrFileName("");
	CString			strEjrFilePath("");

	if (m_pProfile->DEVICE.EjrDevice == 1)						// CUP(SH) : 2006.04.19
		;
	else
		return TRUE;

	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// Host Local Mode
		return TRUE;

	if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALDATE).GetLength() == 8)
		strInputStartDate = RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALDATE);
	else
		strInputStartDate = GetDate();

	nInputStartDate = Asc2Int(strInputStartDate);

	strInputEndDate.Format("%s", GetDate());
	nInputEndDate = Asc2Int(strInputEndDate);

	_mkdir(_AP_TEMP_DIR);
	DeleteFiles(_AP_TEMP_DIR, -1);

	for (nInputDate = nInputStartDate; nInputDate <= nInputEndDate; nInputDate++)
	{
		if (((nInputDate % 100) > 31)		||
			(((nInputDate % 10000) / 100) > 12))
			continue;
		strInputDate = Int2Asc(nInputDate);
		fnAPL_MergeTmpFile(_EJR_DIR, ".TXT", strInputDate, nEjmType);
		Delay_Msg(100);
	}

	strTempEjrFileName.Format("%s\\TEMPEJM.TXT", _AP_TEMP_DIR);
	if (nEjmType == 11)											// Send Withdrawal
		strEjrFilePath.Format("%s\\EJRWITH%s(%s-%s).TXT", _AP_TEMP_DIR, m_pProfile->NETWORK.AtmSerialNum, strInputStartDate, strInputEndDate);
	else
	if (nEjmType == 12)											// Send All
		strEjrFilePath.Format("%s\\EJRALL%s(%s-%s).TXT", _AP_TEMP_DIR, m_pProfile->NETWORK.AtmSerialNum, strInputStartDate, strInputEndDate);
	else
	if (nEjmType == 13)											// Send User
		strEjrFilePath.Format("%s\\EJRUSER%s(%s-%s).TXT", _AP_TEMP_DIR, m_pProfile->NETWORK.AtmSerialNum, strInputStartDate, strInputEndDate);

	if (CopyFile(strTempEjrFileName, strEjrFilePath, 0))
		DeleteFile(strTempEjrFileName);
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_FtpSendEjmProc():Copy File Fail");
		return FALSE;
	}

	for (int i = 0; i < 30; i++)
	{
		if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME1) == "")
		{
			RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND1, 0);
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME1, "/ATMClient/");
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME1, strEjrFilePath);
			break;
		}
		else
		if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME2) == "")
		{
			RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND2, 0);
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME2, "/ATMClient/");
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME2, strEjrFilePath);
			break;
		}
		else
		if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME3) == "")
		{
			RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND3, 0);
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME3, "/ATMClient/");
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME3, strEjrFilePath);
			break;
		}
		else
		if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME4) == "")
		{
			RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND4, 0);
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME4, "/ATMClient/");
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME4, strEjrFilePath);
			break;
		}
		else
		{
			Delay_Msg(1000);
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "fnAPL_FtpSendEjmProc():return");
	return TRUE;
}

// Merge File In Temp Directory
int CTranCmn::fnAPL_MergeTmpFile(CString strPath, CString strFileType, CString strInputDate, int nEjmType)
{
	CFileFind		finder;
	CString			dir, file;
	CFileStatus		fileStatus;
	CString			strSrcFileName("");
	CString			strDesFileName("");
	FILE*			SrcFileNameFp;
	FILE*			DesFileNameFp;
	CString			strTemp("");

	EjrTbl			TempEjr;
	int				nTempFReadNum = 0;
	char			szTemp[1024];
	BOOL			bHaveWrite = FALSE;
	BOOL			bHaveWriteLine = FALSE;

	dir.Format("%s\\%s", strPath, "*.*");
	BOOL bWorking = finder.FindFile(dir);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		
		dir  = finder.GetFilePath();
		file = finder.GetFileName();
		
		if (file == _T(".") ||
			file == _T(".."))	
			continue;
		
		if (finder.IsDirectory())
		{
			if (!fnAPL_MergeTmpFile(dir, strFileType, strInputDate, nEjmType))
			{
				finder.Close();
				return FALSE;
			}
		}
		else
		{
			if (!CFile::GetStatus(dir, fileStatus))
			{
				finder.Close();
				return FALSE;
			}
			
			CTime t1(fileStatus.m_ctime.GetYear(), fileStatus.m_ctime.GetMonth(), 
				fileStatus.m_ctime.GetDay(), 0, 0, 0);

			if ((strFileType.CompareNoCase(file.Right(strFileType.GetLength())) == 0) && 
				(t1.Format("%Y%m%d") == strInputDate))			// 2006.01.04
			{
				strSrcFileName = dir;
				SrcFileNameFp = fopen(strSrcFileName, "rb");
				if (!SrcFileNameFp)
					break;

				strDesFileName.Format("%s\\TEMPEJM.TXT", _AP_TEMP_DIR);
				DesFileNameFp = fopen(strDesFileName, "a");
				if (!DesFileNameFp)
				{
					fclose(SrcFileNameFp);
					break;
				}

				while (TRUE)
				{				
					memset(&TempEjr, 0, sizeof(TempEjr));
					nTempFReadNum = fread(&TempEjr , sizeof(TempEjr), 1, SrcFileNameFp);
					sprintf(szTemp, "%%%d.%ds", sizeof(TempEjr), sizeof(TempEjr));
					if (nTempFReadNum)
						strTemp.Format(szTemp, (LPSTR)&TempEjr);
					else
						break;
					
					bHaveWrite = FALSE;
					bHaveWriteLine = FALSE;
					switch (nEjmType) 
					{
						case 11:								// Withdrawal Print
							if ((strTemp.Find("TRAN TYPE     : WITHDRAWAL")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : CWD")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : TRANSFER")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : TFR")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : TRF")		>= 0))
								bHaveWrite = TRUE;
							if (strTemp.Find("TRAN TYPE     : ") >= 0)
								bHaveWriteLine = TRUE;
							break;

						case 12:								// All Print
							bHaveWrite = TRUE;
							if (strTemp.Find("TRAN TYPE     : ") >= 0)
								bHaveWriteLine = TRUE;
							break;

						case 13:								// User Print
							if ((strTemp.Find("TRAN TYPE     : WITHDRAWAL")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : CWD")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : INQUIRY")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : INQ")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : DETAIL")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : TRANSFER")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : TFR")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : TRF")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : CHANGEPIN")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : PIN")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : CHECKPIN")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : CWC")		>= 0))
								bHaveWrite = TRUE;
							if (strTemp.Find("TRAN TYPE     : ") >= 0)
								bHaveWriteLine = TRUE;
							break;
					}

					if (bHaveWrite)
					{
						for (int i = 0; i < JPR_MAX_ROW; i++)
						{
							strTemp = GetString(TempEjr.Data[i], JPR_MAX_COL, JPR_MAX_COL);
							strTemp.TrimRight();
							if (strTemp.GetLength())
							{
								if ((nEjmType == 11)	||		// Print
									(nEjmType == 12)	||
									(nEjmType == 13))
									fprintf(DesFileNameFp, "%s\n", strTemp);
							}
						}
						
						if (bHaveWriteLine)
						{
							if ((nEjmType == 11)	||			// Print
								(nEjmType == 12)	||
								(nEjmType == 13))
								fprintf(DesFileNameFp, "********************************\n");
						}
					}
				}
				
				fclose(SrcFileNameFp);
				fclose(DesFileNameFp);
			}
		}
	}
	finder.Close();

	return TRUE;
}

// Save Detail Data
int CTranCmn::fnAPL_SaveDetailData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SaveDetailData()");

	char		szTempFileName[256];
	char		szTempSerialNo[256];
	CString		strBatchNo("");
	FILE*		TempFp;
	CFileStatus	TempFileStatus;

	memset(szTempFileName, 0, sizeof(szTempFileName));
	memset(szTempSerialNo, 0, sizeof(szTempSerialNo));

	memset(&DetailData, '0', sizeof(DetailData));
	if (strlen(CardAccountNo) <= 19)
		memcpy(&DetailData.AccountNo[19 - strlen(CardAccountNo)], CardAccountNo, strlen(CardAccountNo));
	else
		memcpy(DetailData.AccountNo, CardAccountNo, 19);
	
	memcpy(DetailData.TranHandleCode, HMESend.Data[3], 6);
	memcpy(DetailData.TranAmount, Accept.Money, sizeof(Accept.Money));
	memcpy(DetailData.SerialNo, m_pProfile->TRANS.SerialNo, 8);
	memcpy(DetailData.TranTime, HMESend.Data[12], 6);
	memcpy(DetailData.TranDate, HMESend.Data[13], 8);

	strBatchNo = GetSprintf("%04.4s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, "0001"));
	memcpy(DetailData.DetailBatchNo, strBatchNo, 4);

	memcpy(szTempSerialNo, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILWRITEP, "0000"), 4);
	
	if (memcmp(szTempSerialNo, "9999", 4) == 0)
		memset(szTempSerialNo, '0', 4);
	AddString(szTempSerialNo, "0001", 4);
		
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SaveDetailData() szTempSerialNo[%s]", szTempSerialNo);
	_mkdir(_EJR_DETAILSEND);
	sprintf(szTempFileName, "%s\\%4.4s.DAT", _EJR_DETAILSEND, GetSprintf("%4.4s", szTempSerialNo));
																// 2005.12.01
	TempFp = fopen(szTempFileName, "wc");
	if (TempFp)
	{
		fwrite(&DetailData, sizeof(DetailTbl), 1, TempFp);
		fflush(TempFp);
		fclose(TempFp);
	}
	
	IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILWRITEP, GetSprintf("%4.4s", szTempSerialNo));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SaveDetailData(): return()");
	return T_OK;
}

// Get Detail Data
int CTranCmn::fnAPL_GetDetailData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDetailData()");

	DetailTbl	DetailTblTemp;
	char		szTempFileName[256];
	char		szTempSerialNo[256];
	char		szTempDetailData[256];
	int			nTempCount = 0;
	int			nTotalDetailCnt = 0;
	
	CString		strDetailBatchNo("");
	CString		strDetailSendP("");
	FILE*		TempFp;
	CFileStatus	TempFileStatus;

/////////////////////////////////////////////////////////////////////////////
	DetailBeginPSetFlag = FALSE;
	strDetailData = "";
/////////////////////////////////////////////////////////////////////////////

	memset(&DetailTblTemp, ' ', sizeof(DetailTbl));
	memset(szTempFileName, 0, sizeof(szTempFileName));
	memset(szTempSerialNo, 0, sizeof(szTempSerialNo));
	memset(szTempDetailData, 0, sizeof(szTempDetailData));

	nTotalDetailCnt = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDCNT, 0);
	strDetailSendP = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDP, "0000");
	memcpy(szTempSerialNo, strDetailSendP, 4);

	strDetailBatchNo = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILBATCHNO, "0001");

	while (TRUE)
	{
		if (memcmp(szTempSerialNo, "9999", 4) == 0)
			memset(szTempSerialNo, '0', 4);
		AddString(szTempSerialNo, "0001", 4);
		
		sprintf(szTempFileName, "%s\\%4.4s.DAT", _EJR_DETAILSEND, GetSprintf("%4.4s", szTempSerialNo));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDetailData() szTempFileName[%s]", szTempFileName);		
																// 2005.12.01
		if (CFile::GetStatus(szTempFileName, TempFileStatus))
		{
			TempFp = fopen(szTempFileName, "r");						
			if (TempFp)
			{
				memset(&DetailTblTemp, ' ', sizeof(DetailTbl));
				fread(&DetailTblTemp, sizeof(DetailTbl), 1, TempFp);
				fclose(TempFp);
			}
			else 
				continue;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDetailData() strDetailBatchNo[%s]", strDetailBatchNo);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDetailData() DetailTblTemp.DetailBatchNo[%s]", DetailTblTemp.DetailBatchNo);
			if (memcmp(strDetailBatchNo.GetBuffer(0), DetailTblTemp.DetailBatchNo, 4) == 0)
			{
				if (DetailBeginPSetFlag)
					;
				else
				{
					DetailBeginPSetFlag = TRUE;
					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILBEGINP, GetSprintf("%4.4s", szTempSerialNo));
				}

				memset(szTempDetailData, 0, sizeof(szTempDetailData));
				memcpy(szTempDetailData, &DetailTblTemp, (sizeof(DetailTbl) - sizeof(DetailData.DetailBatchNo)));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDetailData() szTempDetailData[%s]", szTempDetailData);					
				strDetailData += GetSprintf("%s", szTempDetailData);
			}
			else 
				break;
		}
		else 
			break;
		
		nTempCount++;
		if (nTempCount >= 10)
			break;
	}
	
	IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDP, GetSprintf("%4.4s", szTempSerialNo));
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDetailData(): return(%d) strDetailData[%s]", nTempCount, strDetailData);

	if (strDetailData.GetLength())
	{
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDCNT, nTempCount + nTotalDetailCnt);
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDCURRCNT, nTempCount);
		return nTempCount;
	}
	else 
		return FALSE;
}
	
	// Get Detail Count
	int CTranCmn::fnAPL_GetDetailCount()
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDetailCount()");
		
		DetailTbl	DetailTblTemp;
		char		szTempFileName[256];
		char		szTempSerialNo[256];
		char		szBeginSerialNo[256];
		char		szTempDetailData[256];
		int			nTempCount = 0;
		
		CString		strDetailBatchNo("");
		CString		strDetailSendP("");
		FILE*		TempFp;
		CFileStatus	TempFileStatus;
		
		memset(&DetailTblTemp, ' ', sizeof(DetailTbl));
		
		memset(szTempFileName, 0, sizeof(szTempFileName));
		memset(szTempSerialNo, 0, sizeof(szTempSerialNo));
		memset(szBeginSerialNo, 0, sizeof(szBeginSerialNo));
		memset(szTempDetailData, 0, sizeof(szTempDetailData));
		
		strDetailSendP = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDP, "0000");
		memcpy(szTempSerialNo, strDetailSendP, 4);
		if (memcmp(szTempSerialNo, "9999", 4) == 0)
		{
			memset(szTempSerialNo, '0', 4);
			AddString(szTempSerialNo, "0001", 4);
			memcpy(szBeginSerialNo, szTempSerialNo, 4);
		}
		
		strDetailBatchNo = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILBATCHNO, "0001");
		
		while (TRUE)
		{
			if (memcmp(szTempSerialNo, "9999", 4) == 0)
				memset(szTempSerialNo, '0', 4);
			AddString(szTempSerialNo, "0001", 4);
			
			if (memcmp(szTempSerialNo, szBeginSerialNo, 4) == 0)
				break;
			
			sprintf(szTempFileName, "%s\\%4.4s.DAT", _EJR_DETAILSEND, GetSprintf("%4.4s", szTempSerialNo));
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDetailCount() szTempFileName[%s]", szTempFileName);		
			// 2005.12.01
			if (CFile::GetStatus(szTempFileName, TempFileStatus))
			{
				TempFp = fopen(szTempFileName, "r");						
				if (TempFp)
				{
					memset(&DetailTblTemp, ' ', sizeof(DetailTbl));
					fread(&DetailTblTemp, sizeof(DetailTbl), 1, TempFp);
					fclose(TempFp);
				}
				else 
					continue;
				
				if (memcmp(strDetailBatchNo, DetailTblTemp.DetailBatchNo, 4) == 0)
					;
				else 
					break;
			}
		else 
			break;
		
		nTempCount++;
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDetailCount(): return(%d)", nTempCount);
	return nTempCount;
}

// Bid Set Work Param Proc
int CTranCmn::fnAPL_BidSetWorkParamProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BidSetWorkParamProc()");
	int		nWithMaxAmt = 0;									// Withdraw Max Amount
	int		nWithMinAmt = 0;									// Withdraw Min Amount
	double	dTransferMaxAmt = 0;								// Transfer Max Amount
	int		nMsrSendTime = LINE_MSR_SEND_TIME;					// Msr Send Time

	if (HMERecv.DataRecvBit[48])
		;
	else
		return T_OK;

	if (IsNum(HMERecv.Data[48], 3))
	{
		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WORKPARAMETER, 
				  MakeUnPack(&HMERecv.Data[48][3], Asc2Int(HMERecv.Data[48], 3)));
	}

	if (Asc2Int(HMERecv.Data[48], 3) >= 92)						// 2005.09.08
		;
	else
		return T_OK;

	nMsrSendTime = Asc2Int(&HMERecv.Data[48][5], 6);			// 2005.09.08
	nWithMaxAmt = Asc2Int(&HMERecv.Data[48][15], 6);
	nWithMinAmt = Asc2Int(&HMERecv.Data[48][27], 6);
	dTransferMaxAmt = Asc2Float(&HMERecv.Data[48][35], 12) / 100;

	if ((m_pDevCmn->FstCSTValue == CASH_CHINA_100) ||
		(m_pDevCmn->FstCSTValue == CASH_CHINA_50))
	{
		if ((nWithMaxAmt < CSH_M10050_MINOUT) ||
			(nWithMaxAmt > CSH_M10050_MAXOUT))
			InitInfo.WithMaxAmt = CSH_M10050_MAXOUT;			// Withdraw Max Amount : 2004.11.05
		else
			InitInfo.WithMaxAmt = nWithMaxAmt;
	}
	else
	if ((m_pDevCmn->FstCSTValue == CASH_CHINA_20) ||
		(m_pDevCmn->FstCSTValue == CASH_CHINA_10))
	{
		if ((nWithMaxAmt < CSH_M2010_MINOUT) ||
			(nWithMaxAmt > CSH_M2010_MAXOUT))
			InitInfo.WithMaxAmt = CSH_M2010_MAXOUT;				// Withdraw Max Amount : 2004.11.05
		else
			InitInfo.WithMaxAmt = nWithMaxAmt;
	}

	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMAXAMOUNT, InitInfo.WithMaxAmt);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BidSetWorkParamProc() _INIKEY_TIMEDEFINE_WITHMAXAMOUNT[%d]", InitInfo.WithMaxAmt);

//	InitInfo.TransferMaxAmt = dTransferMaxAmt;					// Transfer Max Amount : 2005.09.11
//	IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMAXAMOUNT, GetSprintf("%12.12s", &HMERecv.Data[48][35]));
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BidSetWorkParamProc() _INIKEY_TIMEDEFINE_TRANMAXAMOUNT[%s]", GetSprintf("%12.12s", &HMERecv.Data[48][35]));

	if (nMsrSendTime < 30)										// Msr Send Time
		InitInfo.MsrSendTime = 30;
	else
	if (nMsrSendTime > 3600)									// Msr Send Time
		InitInfo.MsrSendTime = 3600;
	else
		InitInfo.MsrSendTime = nMsrSendTime;

	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_IDLERETRYTIME, InitInfo.MsrSendTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BidSetWorkParamProc() _INIKEY_TIMEDEFINE_IDLERETRYTIME[%d]", InitInfo.MsrSendTime);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BidSetWorkParamProc(): return()");
	return T_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Nms Funtion : 2006.07.03
/////////////////////////////////////////////////////////////////////////////
// Send Host
int CTranCmn::fnAPL_SendHost()
{
	if (m_pProfile->DEVICE.NmsDevice)
		;
	else
		return TRUE;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SendHost()TranCode[%d]NmsSendFlag[%d]", TranCode, NmsSendFlag);

	if (NmsSendFlag)
		return T_OK;

	if ((m_pDevCmn->TranStatus == TRAN_TRAN)	&&
		(TranCode == 0))
	{
		return T_OK;											// User Not Selected Transaction
	}

	if (!AddSerialFlag)
	{
		AddSerialFlag = TRUE;									// Add Serial Flag
		m_pDevCmn->fnAPL_AddSerialNo();							// Add Serial No
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SendHost() m_pProfile->TRANS.SerialNo(%8.8s)", m_pProfile->TRANS.SerialNo);
	}

	int	nReturn = -1;

/////////////////////////////////////////////////////////////////////////////
	NmsSendFlag = FALSE;										// 2006.07.20
	StatusEtcLength = 0;
	memset(StatusEtcBuffer, 0, sizeof(StatusEtcBuffer));
	NmsSendLength = 0;
	memset(NmsSendBuffer, 0, sizeof(NmsSendBuffer));
/////////////////////////////////////////////////////////////////////////////

	fnAPL_SendMake();

	if (!NmsSendLength)
	{
		return T_OK;
	}

	if (m_pProfile->DEVICE.NmsDevice == 1)						// ZL NMS : 2006.04.19
	{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SendHost() NmsSendBuffer", (char*)NmsSendBuffer, NmsSendLength);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SendHost() StatusEtcBuffer", (char*)StatusEtcBuffer, StatusEtcLength);
		nReturn = Send2AMAS((LPCSTR)NmsSendBuffer, NmsSendLength, (LPCSTR)StatusEtcBuffer, StatusEtcLength);
	}
	else
	if (m_pProfile->DEVICE.NmsDevice == 2)						// DC NMS : 2006.06.16
	{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SendHost() NmsSendBuffer", (char*)NmsSendBuffer, NmsSendLength);
		nReturn = m_pDevCmn->fnNMS_SendData(MakeUnPack(NmsSendBuffer, NmsSendLength));
	}
	
/////////////////////////////////////////////////////////////////////////////
	NmsSendFlag = TRUE;											// 2006.07.20
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SendHost():return(%d)", nReturn);
	return T_OK;
}

// Send Make
int CTranCmn::fnAPL_SendMake()
{
	if (m_pProfile->DEVICE.NmsDevice)
		;
	else
		return TRUE;
	
	int				nTempLen = 0;
	int				nRemainAmount = 0;
	int				nTranStatus = 0;
	CString			strTemp("");
	char			szTempBuff[1024];
	static char		TempBuffer[256];
	CStringArray	strTempArray;

	if ((TranCode == TC_CANCEL)	||
		(TranCode == TC_ERROR))
	{
		SplitString(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCARD), ",", strTempArray);
		if (strTempArray.GetSize() == 6)
		{
			MakePack(strTempArray[0].GetBuffer(0), &CardTran,		sizeof(CardTran) * 2);
			MakePack(strTempArray[1].GetBuffer(0), &CardBank,		sizeof(CardBank) * 2);
			MakePack(strTempArray[2].GetBuffer(0), &CardService,	sizeof(CardService) * 2);
			MakePack(strTempArray[3].GetBuffer(0), &CardData,		sizeof(CardData) * 2);
			MakePack(strTempArray[4].GetBuffer(0), &CardAccountNo,	sizeof(CardAccountNo) * 2);
			MakePack(strTempArray[5].GetBuffer(0), &CardPan,		sizeof(CardPan) * 2);
		}
		else
		{
			CardTran = MENU_NULL;								// Card Transaction
			CardBank = 0;										// Card Bank
			CardService = 0;									// Card Service
			memset(&CardData, 0, sizeof(CardData));				// Card Data 
			memset(CardAccountNo, 0, sizeof(CardAccountNo));	// Card Account No
			memset(CardPan, 0, sizeof(CardPan));				// Card Pan
		}
	}

/////////////////////////////////////////////////////////////////////////////
	StatusEtcLength = 0;
	memset(StatusEtcBuffer, 0, sizeof(StatusEtcBuffer));
	NmsSendLength = 0;
	memset(NmsSendBuffer, 0, sizeof(NmsSendBuffer));
/////////////////////////////////////////////////////////////////////////////

	memset(szTempBuff, 0, sizeof(szTempBuff));
	nTranStatus = m_pDevCmn->TranStatus;

	if (m_pProfile->DEVICE.NmsDevice == 1)						// ZL NMS : 2006.04.19
	{
		if ((nTranStatus == TRAN_ERROR)		||
			(nTranStatus == TRAN_AEXSTATUS)	||
			(nTranStatus == TRAN_OEXSTATUS)	||
			(nTranStatus == TRAN_MSRSTATUS))
		{
			memset(szTempBuff, 0, sizeof(szTempBuff));
			strTemp = m_pDevCmn->fstrDVR_GetStatus();
			MakePack(strTemp.GetBuffer(0), szTempBuff, strTemp.GetLength());

			memset(StatusEtcBuffer, 0, sizeof(StatusEtcBuffer));
			memset(StatusEtcBuffer, '0', 17);

			if (IsNum(szTempBuff, 5))
			{
				memcpy(&StatusEtcBuffer[0], Int2Asc(DvrStatusReturn, 1), 1);
				memcpy(&StatusEtcBuffer[1], &szTempBuff[1], 4);
			}

			nRemainAmount =     (m_pDevCmn->FstCSTCnt * m_pDevCmn->FstCSTValue)
							+	(m_pDevCmn->SndCSTCnt * m_pDevCmn->SndCSTValue)
							+	(m_pDevCmn->TrdCSTCnt * m_pDevCmn->TrdCSTValue)
							+	(m_pDevCmn->FthCSTCnt * m_pDevCmn->FthCSTValue);
			memcpy(&StatusEtcBuffer[7], GetSprintf("%8.8d", nRemainAmount), 8);

			if (!m_pDevCmn->fnAPL_CheckError())					// ErrorCode
			{
				strTemp = GetSprintf("%5.5s(%2.2s)%1.1c", 
								  m_pDevCmn->CurErrBuff.ErrorCode,
								  &m_pDevCmn->CurErrBuff.ErrorCode[5],
								  m_pDevCmn->CurErrBuff.ProcCount[0]);
				memcpy(&StatusEtcBuffer[17], GetSprintf("%-16.16s", strTemp), 16);
			}
			else
			{
				strTemp.Format("%16.16s", "");
				memcpy(&StatusEtcBuffer[17], strTemp, 16);
			}

			memcpy(&StatusEtcBuffer[33], "00", 2);

			if (nTranStatus == TRAN_ERROR)						// Card Retract Code
			{
				strTemp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANERROR);
				if ((fnAPL_GetErrorCodeKind(strTemp) == 2)	&&
					(memcmp(fnAPL_GetBankErrorCode(strTemp), "1004", 4) == 0))
					memcpy(&StatusEtcBuffer[33], "FF", 2);
				else
				{
					strTemp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_RESPONSENO);
					if ((strTemp.GetLength() != 2)	||
						(strTemp == "  "))
						memcpy(&StatusEtcBuffer[33], "FF", 2);
					else
						memcpy(&StatusEtcBuffer[33], strTemp, 2);
					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_RESPONSENO, "");
				}
			}

			if (memcmp(TempBuffer, StatusEtcBuffer, 35) != 0)
			{
				memcpy(TempBuffer, StatusEtcBuffer, 35);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SendMake()StatusEtcBuffer[%35.35s]", StatusEtcBuffer);
			}

			StatusEtcLength = 17 + 16 + 2;

			NmsSendLength = SendLength;
			memcpy(NmsSendBuffer, SendBuffer, SendLength);
		}
		else
		if (nTranStatus == TRAN_OPEN)
		{
			memset(StatusEtcBuffer, 0, sizeof(StatusEtcBuffer));
			memset(StatusEtcBuffer, '0', 15);

			strTemp.Format("%s", VERSIONHOST);
			memcpy(&StatusEtcBuffer[8], strTemp.Right(6), 6);

			if (m_pDevCmn->TranResult)
				memcpy(&StatusEtcBuffer[14], "0", 1);
			else
				memcpy(&StatusEtcBuffer[14], "1", 1);
			
			StatusEtcLength = 15;

			if (RecvOkFlag)
			{
				memset(NmsSendBuffer, 0, sizeof(szTempBuff));
				memset(NmsSendBuffer, '0', 12);
				memcpy(&NmsSendBuffer[0], "85", 2);
				memcpy(&NmsSendBuffer[2], "01", 2);
				memcpy(&NmsSendBuffer[8], GetSprintf("%4.4d", RecvLength), 4);
				memcpy(&NmsSendBuffer[12], RecvBuffer, RecvLength);

				NmsSendLength = RecvLength + 12;
				memcpy(&NmsSendBuffer[12], RecvBuffer, RecvLength);
			}
			else
			{
				NmsSendLength = SendLength;
				memcpy(NmsSendBuffer, SendBuffer, SendLength);
			}
		}
		else
		if (nTranStatus == TRAN_TOTAL)
		{
			if (RecvOkFlag)
			{
				memset(NmsSendBuffer, 0, sizeof(NmsSendBuffer));
				memset(NmsSendBuffer, '0', 12);
				memcpy(&NmsSendBuffer[0], "85", 2);
				memcpy(&NmsSendBuffer[2], "01", 2);
				memcpy(&NmsSendBuffer[8], GetSprintf("%4.4d", RecvLength), 4);
				memcpy(&NmsSendBuffer[12], RecvBuffer, RecvLength);
				
				NmsSendLength = RecvLength + 12;
				memcpy(&NmsSendBuffer[12], RecvBuffer, RecvLength);
			}
			else
			{
				NmsSendLength = SendLength;
				memcpy(NmsSendBuffer, SendBuffer, SendLength);
			}
		}
		else													// 2006.08.23
		if ((nTranStatus == TRAN_ADDCASH)		||
			(nTranStatus == TRAN_SUBCASH)		||
			(nTranStatus == TRAN_CLEARCASH))
		{
			NmsSendLength = SendLength;
			memcpy(NmsSendBuffer, SendBuffer, SendLength);
		}
		else
		if ((nTranStatus == TRAN_CANCEL)		||
			(nTranStatus == TRAN_TRAN)			||
			(nTranStatus == TRAN_CONFIRMWITH))
		{
			if (RecvOkFlag)
			{
				memset(NmsSendBuffer, 0, sizeof(szTempBuff));
				memset(NmsSendBuffer, '0', 12);
				memcpy(&NmsSendBuffer[0], "65", 2);
				memcpy(&NmsSendBuffer[2], "01", 2);
				memcpy(&NmsSendBuffer[8], GetSprintf("%4.4d", RecvLength), 4);
				memcpy(&NmsSendBuffer[12], RecvBuffer, RecvLength);
				
				NmsSendLength = RecvLength + 12;
				memcpy(&NmsSendBuffer[12], RecvBuffer, RecvLength);
			}
			else
			{
				NmsSendLength = SendLength;
				memcpy(NmsSendBuffer, SendBuffer, SendLength);
			}
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_pProfile->DEVICE.NmsDevice == 2)						// DC NMS : 2006.06.16
	{
		if (nTranStatus == TRAN_ERROR)
		{
			if ((m_pDevCmn->DeviceRetract & DEV_MCU)	||		// Card Retract(Send Transaction)
				(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_RESPONSENO).GetLength()))
				;
			else
				nTranStatus = TRAN_MSRSTATUS;					// Send Status
		}

		if ((nTranStatus == TRAN_AEXSTATUS)		||
			(nTranStatus == TRAN_OEXSTATUS)		||
			(nTranStatus == TRAN_MSRSTATUS))
		{
			nTempLen = 0;
			memset(NmsSendBuffer, '0', 16);
			nTempLen += 16;										// Send Length
			memcpy(&NmsSendBuffer[nTempLen], "S", 1);
			nTempLen += 1;										// Message Kind
			memcpy(&NmsSendBuffer[nTempLen], m_pProfile->NETWORK.AtmSerialNum, 8);
			nTempLen += 8;										// Atm Id
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			memset(&NmsSendBuffer[nTempLen], '0', 11 * 4);		// Cst Remain Count
			if (m_pDevCmn->CSTCnt >= 1)
			{
				memcpy(&NmsSendBuffer[nTempLen], "156", 3);
				memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FstCSTValue), 4);
				memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
			}
			nTempLen += 11;
			if (m_pDevCmn->CSTCnt >= 2)
			{
				memcpy(&NmsSendBuffer[nTempLen], "156", 3);
				memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->SndCSTValue), 4);
				memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
			}
			nTempLen += 11;
			if (m_pDevCmn->CSTCnt >= 3)
			{
				memcpy(&NmsSendBuffer[nTempLen], "156", 3);
				memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->TrdCSTValue), 4);
				memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
			}
			nTempLen += 11;
			if (m_pDevCmn->CSTCnt >= 4)
			{
				memcpy(&NmsSendBuffer[nTempLen], "156", 3);
				memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FthCSTValue), 4);
				memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
			}
			nTempLen += 11;
			memcpy(&NmsSendBuffer[nTempLen], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_REJECT)), 4);		// Cst Reject Count
			nTempLen += 4;
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator

			memcpy(&NmsSendBuffer[nTempLen], "0000", 4);
			nTempLen += 4;										// Atm Status
			memcpy(&NmsSendBuffer[nTempLen], "00", 2);			// Default: Normal
			if ((InitInfo.BidAtmColseFlag)	&&					// 2005.09.11
				(!m_pDevCmn->OpenKey))
				memcpy(&NmsSendBuffer[nTempLen], "08", 2);		// Clerk Mode
			else
			if (!m_pDevCmn->OpenKey)
				memcpy(&NmsSendBuffer[nTempLen], "09", 2);		// Clerk Mode
			else
			if ((m_pDevCmn->AtmStatus == ATM_INIT)		||
				(m_pDevCmn->AtmStatus == ATM_READY)		||
				(m_pDevCmn->AtmStatus == ATM_DOWN)		||
				(m_pDevCmn->AtmStatus == ATM_ERROR))
				memcpy(&NmsSendBuffer[nTempLen], "08", 2);		// Error Mode
			nTempLen += 2;
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator

			memcpy(&NmsSendBuffer[nTempLen], "0001", 4);
			nTempLen += 4;										// Line Status
			memcpy(&NmsSendBuffer[nTempLen], "00", 2);			// Default: Normal
			if (!m_pDevCmn->fnAPL_CheckHostLine())				// Check Host Line
				memcpy(&NmsSendBuffer[nTempLen], "07", 2);
			nTempLen += 2;
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator

			memcpy(&NmsSendBuffer[nTempLen], "0002", 4);
			nTempLen += 4;										// Mcu Status
			memcpy(&NmsSendBuffer[nTempLen], "00", 2);			// Default: Normal
			if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_MCU))
				memcpy(&NmsSendBuffer[nTempLen], "04", 2);
			else
			if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MCU))
				memcpy(&NmsSendBuffer[nTempLen], "03", 2);
			nTempLen += 2;
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator

			memcpy(&NmsSendBuffer[nTempLen], "0003", 4);
			nTempLen += 4;										// Jpr Status
			memcpy(&NmsSendBuffer[nTempLen], "00", 2);			// Default: Normal
			if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_JPR))
				memcpy(&NmsSendBuffer[nTempLen], "04", 2);		// Missing
			else
			if (m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)
				memcpy(&NmsSendBuffer[nTempLen], "06", 2);		// Empty
			else
			if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR))
				memcpy(&NmsSendBuffer[nTempLen], "03", 2);		// Error
			else
			if (m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)
				memcpy(&NmsSendBuffer[nTempLen], "03", 2);		// Error
			else
			if (m_pDevCmn->JnlStatus == JNL_NORMAL)
				memcpy(&NmsSendBuffer[nTempLen], "00", 2);		// Normal
			else
			if (m_pDevCmn->JnlStatus == JNL_LOW_END)
				memcpy(&NmsSendBuffer[nTempLen], "05", 2);		// Low
			nTempLen += 2;
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator

			memcpy(&NmsSendBuffer[nTempLen], "0004", 4);
			nTempLen += 4;										// Spr Status
			memcpy(&NmsSendBuffer[nTempLen], "00", 2);			// Default: Normal
			if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR))
				memcpy(&NmsSendBuffer[nTempLen], "04", 2);		// Missing
			else
			if (m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)
				memcpy(&NmsSendBuffer[nTempLen], "06", 2);		// Empty
			else
			if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))
				memcpy(&NmsSendBuffer[nTempLen], "03", 2);		// Error
			else
			if (m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)
				memcpy(&NmsSendBuffer[nTempLen], "03", 2);		// Error
			else
			if (m_pDevCmn->SlipStatus == SLIP_NORMAL)
				memcpy(&NmsSendBuffer[nTempLen], "00", 2);		// Normal
			else
			if (m_pDevCmn->SlipStatus == SLIP_LOW_END)
				memcpy(&NmsSendBuffer[nTempLen], "05", 2);		// Low
			nTempLen += 2;
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			
			memcpy(&NmsSendBuffer[nTempLen], "0005", 4);
			nTempLen += 4;										// Cdu Status
			memcpy(&NmsSendBuffer[nTempLen], "00", 2);			// Default: Normal
			if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_CSH))
				memcpy(&NmsSendBuffer[nTempLen], "04", 2);		// Missing
			else
			if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))
				memcpy(&NmsSendBuffer[nTempLen], "03", 2);		// Error
			nTempLen += 2;
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			
			memcpy(&NmsSendBuffer[nTempLen], "0007", 4);
			nTempLen += 4;										// Door Status
			memcpy(&NmsSendBuffer[nTempLen], "00", 2);			// Default: Normal
			if (m_pDevCmn->DoorStatus == DOOR_OPENED)
				memcpy(&NmsSendBuffer[nTempLen], "11", 2);		// Door Open
			nTempLen += 2;
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator

			if (m_pDevCmn->CSTCnt >= 1)
			{
				memcpy(&NmsSendBuffer[nTempLen], "0011", 4);
				nTempLen += 4;									// Cst1 Status
				memcpy(&NmsSendBuffer[nTempLen], "00", 2);		// Default: Normal
				if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))					
					memcpy(&NmsSendBuffer[nTempLen], "03", 2);	// Error
				else
				if (m_pDevCmn->FstCSTLoc != NORMAL_POS)
					memcpy(&NmsSendBuffer[nTempLen], "04", 2);	// Missing
				else
				if (m_pDevCmn->FstCSTStatus == CST_NORMAL)
					memcpy(&NmsSendBuffer[nTempLen], "00", 2);	// Normal
				else
				if (m_pDevCmn->FstCSTStatus == CST_NEAR)
					memcpy(&NmsSendBuffer[nTempLen], "01", 2);	// Low
				else
				if (m_pDevCmn->FstCSTStatus == CST_EMPTY)
					memcpy(&NmsSendBuffer[nTempLen], "02", 2);	// Empty
				else
				if (m_pDevCmn->FstCSTStatus == CST_FULL)
					memcpy(&NmsSendBuffer[nTempLen], "00", 2);	// Normal
				nTempLen += 2;
				memcpy(&NmsSendBuffer[nTempLen], "^", 1);
				nTempLen += 1;									// Separator
			}
			if (m_pDevCmn->CSTCnt >= 2)
			{
				memcpy(&NmsSendBuffer[nTempLen], "0012", 4);
				nTempLen += 4;									// Cst2 Status
				memcpy(&NmsSendBuffer[nTempLen], "00", 2);		// Default: Normal
				if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))					
					memcpy(&NmsSendBuffer[nTempLen], "03", 2);	// Error
				else
				if (m_pDevCmn->SndCSTLoc != NORMAL_POS)
					memcpy(&NmsSendBuffer[nTempLen], "04", 2);	// Missing
				else
				if (m_pDevCmn->SndCSTStatus == CST_NORMAL)
					memcpy(&NmsSendBuffer[nTempLen], "00", 2);	// Normal
				else
				if (m_pDevCmn->SndCSTStatus == CST_NEAR)
					memcpy(&NmsSendBuffer[nTempLen], "01", 2);	// Low
				else
				if (m_pDevCmn->SndCSTStatus == CST_EMPTY)
					memcpy(&NmsSendBuffer[nTempLen], "02", 2);	// Empty
				else
				if (m_pDevCmn->SndCSTStatus == CST_FULL)
					memcpy(&NmsSendBuffer[nTempLen], "00", 2);	// Normal
				nTempLen += 2;
				memcpy(&NmsSendBuffer[nTempLen], "^", 1);
				nTempLen += 1;									// Separator
			}
			if (m_pDevCmn->CSTCnt >= 3)
			{
				memcpy(&NmsSendBuffer[nTempLen], "0013", 4);
				nTempLen += 4;									// Cst3 Status
				memcpy(&NmsSendBuffer[nTempLen], "00", 2);		// Default: Normal
				if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))					
					memcpy(&NmsSendBuffer[nTempLen], "03", 2);	// Error
				else
				if (m_pDevCmn->TrdCSTLoc != NORMAL_POS)
					memcpy(&NmsSendBuffer[nTempLen], "04", 2);	// Missing
				else
				if (m_pDevCmn->TrdCSTStatus == CST_NORMAL)
					memcpy(&NmsSendBuffer[nTempLen], "00", 2);	// Normal
				else
				if (m_pDevCmn->TrdCSTStatus == CST_NEAR)
					memcpy(&NmsSendBuffer[nTempLen], "01", 2);	// Low
				else
				if (m_pDevCmn->TrdCSTStatus == CST_EMPTY)
					memcpy(&NmsSendBuffer[nTempLen], "02", 2);	// Empty
				else
				if (m_pDevCmn->TrdCSTStatus == CST_FULL)
					memcpy(&NmsSendBuffer[nTempLen], "00", 2);	// Normal
				nTempLen += 2;
				memcpy(&NmsSendBuffer[nTempLen], "^", 1);
				nTempLen += 1;									// Separator
			}
			if (m_pDevCmn->CSTCnt >= 4)
			{
				memcpy(&NmsSendBuffer[nTempLen], "0013", 4);
				nTempLen += 4;									// Cst4 Status
				memcpy(&NmsSendBuffer[nTempLen], "00", 2);		// Default: Normal
				if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))						
					memcpy(&NmsSendBuffer[nTempLen], "03", 2);	// Error
				else
				if (m_pDevCmn->FthCSTLoc != NORMAL_POS)
					memcpy(&NmsSendBuffer[nTempLen], "04", 2);	// Missing
				else
				if (m_pDevCmn->FthCSTStatus == CST_NORMAL)
					memcpy(&NmsSendBuffer[nTempLen], "00", 2);	// Normal
				else
				if (m_pDevCmn->FthCSTStatus == CST_NEAR)
					memcpy(&NmsSendBuffer[nTempLen], "01", 2);	// Low
				else
				if (m_pDevCmn->FthCSTStatus == CST_EMPTY)
					memcpy(&NmsSendBuffer[nTempLen], "02", 2);	// Empty
				else
				if (m_pDevCmn->FthCSTStatus == CST_FULL)
					memcpy(&NmsSendBuffer[nTempLen], "00", 2);	// Normal
				nTempLen += 2;
				memcpy(&NmsSendBuffer[nTempLen], "^", 1);
				nTempLen += 1;									// Separator
			}
			memcpy(&NmsSendBuffer[nTempLen], "0043", 4);
			nTempLen += 4;										// Shutter Status
			memcpy(&NmsSendBuffer[nTempLen], "00", 2);			// Default: Normal
			if (m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM)
				memcpy(&NmsSendBuffer[nTempLen], "03", 2);		// Error
			nTempLen += 2;
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator

			memcpy(&NmsSendBuffer[nTempLen], "0045", 4);
			nTempLen += 4;										// Reject Cst Status
			memcpy(&NmsSendBuffer[nTempLen], "00", 2);			// Default: Normal
			if (m_pDevCmn->RejectCSTLoc != NORMAL_POS)
				memcpy(&NmsSendBuffer[nTempLen], "04", 2);		// Missing
			else
			if (m_pDevCmn->RejectCSTStatus == CST_FULL)
				memcpy(&NmsSendBuffer[nTempLen], "03", 2);		// Error
			nTempLen += 2;
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			
			NmsSendLength = nTempLen;
			memcpy(&NmsSendBuffer[8], GetSprintf("%8.8d", nTempLen - 16), 8);
		}
		else
		if (nTranStatus == TRAN_BIDCOMMAND)
		{
			nTempLen = 0;
			memset(NmsSendBuffer, '0', 16);
			nTempLen += 16;										// Send Length
			switch (TranCode)
			{
				case TC_ATMOPENDC:
				case TC_ATMCLOSEDC:
				case TC_ATMSTATUSDC:
					memcpy(&NmsSendBuffer[nTempLen], "B", 1);
					nTempLen += 1;
					break;
				case TC_ATMOPENSYNC:
				case TC_ATMCLOSESYNC:
				case TC_ATMSTATUSSYNC:
					memcpy(&NmsSendBuffer[nTempLen], "A", 1);
					nTempLen += 1;
					break;
				default:
					break;
			}
			memcpy(&NmsSendBuffer[nTempLen], m_pProfile->NETWORK.AtmSerialNum, 8);
			nTempLen += 8;										// Atm Id
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			switch (TranCode)
			{
				case TC_ATMOPENDC:
					memcpy(&NmsSendBuffer[nTempLen], "01", 2);
					nTempLen += 2;
					break;
				case TC_ATMCLOSEDC:
					memcpy(&NmsSendBuffer[nTempLen], "02", 2);
					nTempLen += 2;
					break;
				case TC_ATMSTATUSDC:
					memcpy(&NmsSendBuffer[nTempLen], "66", 2);
					nTempLen += 2;
					break;
				case TC_ATMOPENSYNC:
					memcpy(&NmsSendBuffer[nTempLen], "01", 2);
					nTempLen += 2;
					break;
				case TC_ATMCLOSESYNC:
					memcpy(&NmsSendBuffer[nTempLen], "02", 2);
					nTempLen += 2;
					break;
				case TC_ATMSTATUSSYNC:
					memcpy(&NmsSendBuffer[nTempLen], "66", 2);
					nTempLen += 2;
					break;
				default:
					break;
			}
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			switch (TranCode)
			{
				case TC_ATMOPENDC:
				case TC_ATMCLOSEDC:
					memcpy(&NmsSendBuffer[nTempLen], "01", 2);
					nTempLen += 2;
					break;
				case TC_ATMSTATUSDC:
					memcpy(&NmsSendBuffer[nTempLen], "01", 2);
					nTempLen += 2;
					break;
				case TC_ATMOPENSYNC:
				case TC_ATMCLOSESYNC:
					memcpy(&NmsSendBuffer[nTempLen], "01", 2);
					nTempLen += 2;
					break;
				case TC_ATMSTATUSSYNC:
					memcpy(&NmsSendBuffer[nTempLen], "01", 2);
					nTempLen += 2;
					break;
				default:
					break;
			}
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			memcpy(&NmsSendBuffer[nTempLen], DCNmsDateTime, sizeof(DCNmsDateTime));
			nTempLen += sizeof(DCNmsDateTime);
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			switch (TranCode)
			{
				case TC_ATMSTATUSDC:
				case TC_ATMSTATUSSYNC:
					strTemp.Format("%s", VERSION);
					strTemp.Remove('V');
					strTemp.Replace("-", ".");
					strTemp += ".0";
					memcpy(&NmsSendBuffer[nTempLen], strTemp, strTemp.GetLength());
					nTempLen += strTemp.GetLength();
					break;
				default:
					break;
			}
			NmsSendLength = nTempLen;
			memcpy(&NmsSendBuffer[8], GetSprintf("%8.8d", nTempLen - 16), 8);
		}
		else
		if ((nTranStatus == TRAN_OPEN)			||				// Transaction
			(nTranStatus == TRAN_CANCEL)		||
			(nTranStatus == TRAN_ERROR)			||
			(nTranStatus == TRAN_TRAN)			||
			(nTranStatus == TRAN_TOTAL)			||
			(nTranStatus == TRAN_ADDCASH)		||
			(nTranStatus == TRAN_SUBCASH)		||
			(nTranStatus == TRAN_CLEARCASH)		||
			(nTranStatus == TRAN_DOWNFIT)		||
			(m_pDevCmn->TranStatus == TRAN_DOWNICFIT) ||
			(m_pDevCmn->TranStatus == TRAN_DOWNAIDLIST))			// added by yusy ic
		{
			nTempLen = 0;
			memset(NmsSendBuffer, '0', 16);
			nTempLen += 16;										// Send Length
			memcpy(&NmsSendBuffer[nTempLen], "R", 1);
			nTempLen += 1;										// Message Kind
			memcpy(&NmsSendBuffer[nTempLen], m_pProfile->NETWORK.AtmSerialNum, 8);
			nTempLen += 8;										// Atm Id
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			switch (nTranStatus)
			{
				case TRAN_OPEN:
					memcpy(&NmsSendBuffer[nTempLen], "201", 3);
					break;
				case TRAN_CANCEL:
					memcpy(&NmsSendBuffer[nTempLen], "011", 3);
					break;
				case TRAN_ERROR:
					memcpy(&NmsSendBuffer[nTempLen], "203", 3);
					break;
				case TRAN_TOTAL:
					memcpy(&NmsSendBuffer[nTempLen], "309", 3);
					break;
				case TRAN_ADDCASH:
				case TRAN_SUBCASH:
				case TRAN_CLEARCASH:
					memcpy(&NmsSendBuffer[nTempLen], "202", 3);
					break;
				case TRAN_DOWNFIT:
				case TRAN_DOWNICFIT:		// added by yusy ic
				case TRAN_DOWNAIDLIST:
					memcpy(&NmsSendBuffer[nTempLen], "309", 3);
					break;
				case TRAN_TRAN:
					switch (TranCode)
					{
						case TC_INQUIRY:
						
						case TC_OTHER_INQ:
						case TC_CUS_INF_INQ:
						case TC_CUS_INF_INQ_OTHERBANK:
						case TC_ECHANNEL_INQ:
						case TC_SMSPARITY:
						case TC_ECHANNEL:
						case TC_INQUIRY_CheckPin:
							memcpy(&NmsSendBuffer[nTempLen], "001", 3);
							break;
						case TC_TRANSFER:
						case TC_HZT:						//liyi add 绿卡通
						case TC_TZH:
							memcpy(&NmsSendBuffer[nTempLen], "003", 3);
							break;
						case TC_WITHDRAWAL:
							memcpy(&NmsSendBuffer[nTempLen], "002", 3);
							break;
						case TC_CHANGEPIN:
							memcpy(&NmsSendBuffer[nTempLen], "004", 3);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			nTempLen += 3;										// Tran Kind
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			memcpy(&NmsSendBuffer[nTempLen], GetSprintf("%8.8s%6.6s", GetDate(), GetTime()), 14);
			nTempLen += 14;										// Date & Time
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			memcpy(&NmsSendBuffer[nTempLen], m_pProfile->TRANS.SerialNo, 8);
			nTempLen += 8;										// Serial No
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			strTemp.Format("%12.12s", HostResp.ReferenceNo);
			strTemp.Remove(' ');
			if (strTemp.GetLength())
			{
				if (strTemp.GetLength() <= 12)
				{
					memcpy(&NmsSendBuffer[nTempLen], strTemp, strTemp.GetLength());
					nTempLen += strTemp.GetLength();			// Reference No
				}
				else
				{
					memcpy(&NmsSendBuffer[nTempLen], strTemp, 12);
					nTempLen += 12;								// Reference No
				}
			}
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			if (strlen(CardAccountNo))
			{
				if (strlen(CardAccountNo) <= 19)
				{
					memcpy(&NmsSendBuffer[nTempLen], CardAccountNo, strlen(CardAccountNo));
					nTempLen += strlen(CardAccountNo);			// Account No
				}
				else
				{
					memcpy(&NmsSendBuffer[nTempLen], CardAccountNo, 19);
					nTempLen += 19;								// Account No
				}
			}
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			if ((TranCode == TC_ADDCASH)	||
				(TranCode == TC_SUBCASH)	||
				(TranCode == TC_CLEARCASH))
			{
				nRemainAmount =		(m_pDevCmn->FstCSTSetCash * m_pDevCmn->FstCSTValue)
								+	(m_pDevCmn->SndCSTSetCash * m_pDevCmn->SndCSTValue)
								+	(m_pDevCmn->TrdCSTSetCash * m_pDevCmn->TrdCSTValue)
								+	(m_pDevCmn->FthCSTSetCash * m_pDevCmn->FthCSTValue);
				memset(szTempBuff, '0', 12);
				
				memcpy(szTempBuff, GetSprintf("%10.10d", nRemainAmount), 10);
				Zero2Space(szTempBuff, sizeof(Accept.Money));
				strTemp.Format("%12.12s", szTempBuff);
				strTemp.Remove(' ');
				memcpy(&NmsSendBuffer[nTempLen], strTemp, strTemp.GetLength());
				nTempLen += strTemp.GetLength();				// Amount
			}
			else
			{
				if (!IsZero(Accept.Money, sizeof(Accept.Money)))
				{
					memset(szTempBuff, 0, sizeof(szTempBuff));
					memcpy(szTempBuff, Accept.Money, sizeof(Accept.Money));
					Zero2Space(szTempBuff, sizeof(Accept.Money));
					strTemp.Format("%12.12s", szTempBuff);
					strTemp.Remove(' ');
					memcpy(&NmsSendBuffer[nTempLen], strTemp, strTemp.GetLength());
					nTempLen += strTemp.GetLength();			// Amount
				}
			}
			
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			if ((nTranStatus == TRAN_CANCEL)	||
				(nTranStatus == TRAN_TRAN))
			{
				memcpy(&NmsSendBuffer[nTempLen], "156", 3);
				nTempLen += 3;									// Currency Code
			}
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator

			strTemp = GetString(HostResp.RejectCode, sizeof(HostResp.RejectCode), sizeof(HostResp.RejectCode));
			if (IsSpace(strTemp.GetBuffer(0), strTemp.GetLength()))
			{
				memset(&NmsSendBuffer[nTempLen], 'F', 4);
			}
			else
			{
				strTemp.Remove(' ');
				memcpy(&NmsSendBuffer[nTempLen], GetSprintf("%04.4s", strTemp), 4);
			}
			nTempLen += 4;										// Reject Code
			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator
			if ((nTranStatus == TRAN_ERROR)		||
				(nTranStatus == TRAN_CANCEL)	||
				(nTranStatus == TRAN_TRAN))
			{
				if (CardBank == 1)
					memcpy(&NmsSendBuffer[nTempLen], "0", 1);
				else
					memcpy(&NmsSendBuffer[nTempLen], "1", 3);
				nTempLen += 1;									// Card Type: 0-Mother, 1-Other
			}

			memcpy(&NmsSendBuffer[nTempLen], "^", 1);
			nTempLen += 1;										// Separator

			switch (TranCode)
			{
				case TC_OPEN:
					break;
				case TC_CANCEL:
					memcpy(&NmsSendBuffer[nTempLen], OrgSerialNo, sizeof(OrgSerialNo));
					nTempLen += sizeof(OrgSerialNo);			// Org Serial No
					break;
				case TC_ERROR:
					strTemp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_RESPONSENO);
					if (strTemp.GetLength())
					{
						memcpy(&NmsSendBuffer[nTempLen], strTemp, strTemp.GetLength());
						nTempLen += strTemp.GetLength();		// Card Retract Reason: Host Order
					}
					else
					{
						memset(szTempBuff, 'F', sizeof(HostResp.RejectCode));
						memcpy(&NmsSendBuffer[nTempLen], szTempBuff, sizeof(HostResp.RejectCode));
						nTempLen += sizeof(HostResp.RejectCode);// Card Retract Reason: Lost Card
					}
					break;
				case TC_TOTAL:
				case TC_TOTALPRE:
					break;
				case TC_ADDCASH:
				case TC_SUBCASH:
				case TC_CLEARCASH:
//					memset(&NmsSendBuffer[nTempLen], '0', 11 * 4);
//					if (m_pDevCmn->CSTCnt >= 1)					// Cst Remain Count
//					{
//						memcpy(&NmsSendBuffer[nTempLen], "156", 3);
//						memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FstCSTValue), 4);
//						memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
//					}
//					nTempLen += 11;
//					if (m_pDevCmn->CSTCnt >= 2)
//					{
//						memcpy(&NmsSendBuffer[nTempLen], "156", 3);
//						memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->SndCSTValue), 4);
//						memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
//					}
//					nTempLen += 11;
//					if (m_pDevCmn->CSTCnt >= 3)
//					{
//						memcpy(&NmsSendBuffer[nTempLen], "156", 3);
//						memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->TrdCSTValue), 4);
//						memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
//					}
//					nTempLen += 11;
//					if (m_pDevCmn->CSTCnt >= 4)
//					{
//						memcpy(&NmsSendBuffer[nTempLen], "156", 3);
//						memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FthCSTValue), 4);
//						memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
//					}
//					nTempLen += 11;
//					memset(&NmsSendBuffer[nTempLen], '0', 11 * 4);
//					if (m_pDevCmn->CSTCnt >= 1)					// Cst Dispense Count
//					{
//						memcpy(&NmsSendBuffer[nTempLen], "156", 3);
//						memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FstCSTValue), 4);
//						memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_1)), 4);
//					}
//					nTempLen += 11;
//					if (m_pDevCmn->CSTCnt >= 2)
//					{
//						memcpy(&NmsSendBuffer[nTempLen], "156", 3);
//						memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->SndCSTValue), 4);
//						memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_2)), 4);
//					}
//					nTempLen += 11;
//					if (m_pDevCmn->CSTCnt >= 3)
//					{
//						memcpy(&NmsSendBuffer[nTempLen], "156", 3);
//						memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->TrdCSTValue), 4);
//						memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_3)), 4);
//					}
//					nTempLen += 11;
//					if (m_pDevCmn->CSTCnt >= 4)
//					{
//						memcpy(&NmsSendBuffer[nTempLen], "156", 3);
//						memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FthCSTValue), 4);
//						memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_4)), 4);
//					}
//					nTempLen += 11;
//					memset(&NmsSendBuffer[nTempLen], '0', 11 * 4);
//					if (m_pDevCmn->CSTCnt >= 1)					// Cst Set Cash Count
//					{
//						memcpy(&NmsSendBuffer[nTempLen], "156", 3);
//						memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FstCSTValue), 4);
//						memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->FstCSTSetCash), 4);
//					}
//					nTempLen += 11;
//					if (m_pDevCmn->CSTCnt >= 2)
//					{
//						memcpy(&NmsSendBuffer[nTempLen], "156", 3);
//						memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->SndCSTValue), 4);
//						memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->SndCSTSetCash), 4);
//					}
//					nTempLen += 11;
//					if (m_pDevCmn->CSTCnt >= 3)
//					{
//						memcpy(&NmsSendBuffer[nTempLen], "156", 3);
//						memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->TrdCSTValue), 4);
//						memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->TrdCSTSetCash), 4);
//					}
//					nTempLen += 11;
//					if (m_pDevCmn->CSTCnt >= 4)
//					{
//						memcpy(&NmsSendBuffer[nTempLen], "156", 3);
//						memcpy(&NmsSendBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FthCSTValue), 4);
//						memcpy(&NmsSendBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->FthCSTSetCash), 4);
//					}
//					nTempLen += 11;
//					memcpy(&NmsSendBuffer[nTempLen], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_REJECT)), 4);
//					nTempLen += 4;
					break;
				case TC_DOWNFIT:
				case TC_DOWNICFIT:		// added by yusy ic
				case TC_DOWNAIDLIST:
				case TC_DOWNPBOCPARA:
				case TC_DOWNPARM:
				case TC_DOWNPAY:
				case TC_DOWNECHANNELPARA:
				case TC_DOWNECHANNELPROT:
					break;
				case TC_INQUIRY:
				
				case TC_OTHER_INQ:
				case TC_CUS_INF_INQ:
				case TC_CUS_INF_INQ_OTHERBANK:
				case TC_ECHANNEL_INQ:
				case TC_SMSPARITY:
				case TC_ECHANNEL:
					case TC_INQUIRY_CheckPin:
					break;
				case TC_TRANSFER:
					memcpy(&NmsSendBuffer[nTempLen ], Accept.InAccountNum, Accept.InAccountNumSize);
					nTempLen += Accept.InAccountNumSize;		// To Account No
					break;
				case TC_HZT:									// liyi add 绿卡通
				case TC_TZH:
					break;
				case TC_WITHDRAWAL:
					if (m_pDevCmn->TranResult)
					{
						if (m_pDevCmn->DeviceLost & DEV_CSH)
							memcpy(&NmsSendBuffer[nTempLen], "1", 1);
						else
						if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))
							memcpy(&NmsSendBuffer[nTempLen], "2", 1);
						else
							memcpy(&NmsSendBuffer[nTempLen], "0", 1);
					}
					else
						memcpy(&NmsSendBuffer[nTempLen], "2", 1);
					nTempLen += 1;								// Dispense Status
					break;
				case TC_CHANGEPIN:
					break;
				default:
					break;
			}
			
			NmsSendLength = nTempLen;
			memcpy(&NmsSendBuffer[8], GetSprintf("%8.8d", nTempLen - 16), 8);
		}
		else
		if (nTranStatus == TRAN_DCSENDEJR)						// Send Detail
		{
			nTempLen = 0;
			memcpy(&NmsSendBuffer[nTempLen], GetSprintf("%04d", DCEjrSendData.GetLength() + 8), 4);
			nTempLen += 4;
			memcpy(&NmsSendBuffer[nTempLen], GetSprintf("%04d", DCEjrSendCnt), 4);
			nTempLen += 4;
			if (DCEjrSendData.GetLength() <= sizeof(NmsSendBuffer))
			{
				memcpy(&NmsSendBuffer[nTempLen], DCEjrSendData, DCEjrSendData.GetLength());
				nTempLen += DCEjrSendData.GetLength();
			}
			else
			{
				memcpy(&NmsSendBuffer[nTempLen], DCEjrSendData, sizeof(NmsSendBuffer));
				nTempLen += sizeof(NmsSendBuffer);
			}
			NmsSendLength = nTempLen;
		}
		else
		if (nTranStatus == TRAN_DCEJREND)						// Send Detail End
		{
			NmsSendLength = 4;
			memset(NmsSendBuffer, '0', 4);
		}
	}
/////////////////////////////////////////////////////////////////////////////

	return T_OK;
}

// Get Status
int CTranCmn::fnAPL_GetUseStatus()
{
	int	nReturn = FALSE;

	if (m_pProfile->DEVICE.NmsDevice == 1)						// ZL NMS : 2006.04.19
		;
	else
	if (m_pProfile->DEVICE.NmsDevice == 2)						// DC NMS : 2006.06.16
		nReturn = m_pDevCmn->fnNMS_GetUseStatus();

	return nReturn;
}

// End Process
int CTranCmn::fnAPL_EndProcess()
{
	int iret = -1;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_EndProcess()NmsDevice[%d]", m_pProfile->DEVICE.NmsDevice);

	if (m_pProfile->DEVICE.NmsDevice == 1)						// ZL NMS : 2006.04.19
	{
		iret = CloseAgentService();
		Delay_Msg(3000);
	}
	else
	if (m_pProfile->DEVICE.NmsDevice == 2)						// DC NMS : 2006.06.16
	{
		m_pDevCmn->fnNMS_Deinitialize();
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_EndProcess():return");
	return T_OK;
}

// Write DC Transaction Ejr
int CTranCmn::fnAPL_WriteDCTransactionEjr()
{
	if (m_pProfile->DEVICE.EjrDevice == 2)						// DC NMS : 2006.06.16
		;
	else
		return TRUE;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_WriteDCTransactionEjr()m_pDevCmn->TranStatus[%d]TranCode[%d]DCEjrWriteFlag[%d]", m_pDevCmn->TranStatus, TranCode, DCEjrWriteFlag);
	if (DCEjrWriteFlag)
		return T_OK;

	if ((m_pDevCmn->TranStatus == TRAN_TRAN)	&&
		(TranCode == 0))
	{
		return T_OK;											// User Not Selected Transaction
	}

	if (!AddSerialFlag)
	{
		AddSerialFlag = TRUE;									// Add Serial Flag
		m_pDevCmn->fnAPL_AddSerialNo();							// Add Serial No
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_WriteDCTransactionEjr() m_pProfile->TRANS.SerialNo(%8.8s)", m_pProfile->TRANS.SerialNo);
	}

	DCEjrWriteFlag = TRUE;

	int		nTempLen = 0;
	int		nTranStatus = 0;
	int		nRemainAmount = 0;
	CString	strTemp("");
	char	szTempBuff[1024];
	char	DCTranEjrBuffer[1024];

	memset(szTempBuff, 0, sizeof(szTempBuff));

	memset(DCTranEjrBuffer, ' ', sizeof(DCTranEjrBuffer));		// DC Tran Ejr Buffer

	nTranStatus = m_pDevCmn->TranStatus;

	if (nTranStatus == TRAN_ERROR)
	{
		if ((m_pDevCmn->DeviceRetract & DEV_MCU)	||			// Card Retract(Send Transaction)
			(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_RESPONSENO).GetLength()))
			;
		else
			return T_OK;
	}

	if ((nTranStatus == TRAN_OPEN)			||
		(nTranStatus == TRAN_CANCEL)		||
		(nTranStatus == TRAN_ERROR)			||
		(nTranStatus == TRAN_TRAN)			||
		(nTranStatus == TRAN_TOTAL)			||
		(nTranStatus == TRAN_TOTALPRE)		||
		(nTranStatus == TRAN_ADDCASH)		||
		(nTranStatus == TRAN_SUBCASH)		||
		(nTranStatus == TRAN_CLEARCASH)		||
		(nTranStatus == TRAN_DOWNFIT)		||
		(m_pDevCmn->TranStatus == TRAN_DOWNICFIT) ||
		(m_pDevCmn->TranStatus == TRAN_DOWNAIDLIST))		// added by yusy ic
		;
	else
		return TRUE;

	nTempLen = 0;
	memcpy(&DCTranEjrBuffer[nTempLen], ":", 1);
	nTempLen += 1;												// STX
	memcpy(&DCTranEjrBuffer[nTempLen], m_pProfile->NETWORK.AtmSerialNum, 8);
	nTempLen += 8;												// Atm Id
	memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
	nTempLen += 1;												// Separator
	switch (nTranStatus)
	{
		case TRAN_OPEN:
			memcpy(&DCTranEjrBuffer[nTempLen], "201", 3);
			break;
		case TRAN_CANCEL:
			memcpy(&DCTranEjrBuffer[nTempLen], "011", 3);
			break;
		case TRAN_ERROR:
			memcpy(&DCTranEjrBuffer[nTempLen], "203", 3);
			break;
		case TRAN_TOTAL:
		case TRAN_TOTALPRE:
			memcpy(&DCTranEjrBuffer[nTempLen], "309", 3);
			break;
		case TRAN_ADDCASH:
		case TRAN_SUBCASH:
		case TRAN_CLEARCASH:
			memcpy(&DCTranEjrBuffer[nTempLen], "202", 3);
			break;
		case TRAN_DOWNFIT:
		case TRAN_DOWNICFIT:		// added by yusy ic
		case TRAN_DOWNAIDLIST:
			memcpy(&DCTranEjrBuffer[nTempLen], "309", 3);
			break;
		case TRAN_TRAN:
			switch (TranCode)
			{
				case TC_INQUIRY:
				
				case TC_OTHER_INQ:
				case TC_CUS_INF_INQ:
				case TC_CUS_INF_INQ_OTHERBANK:
				case TC_ECHANNEL_INQ:
				case TC_SMSPARITY:
				case TC_ECHANNEL:
				case TC_INQUIRY_CheckPin:
					memcpy(&DCTranEjrBuffer[nTempLen], "001", 3);
					break;
				case TC_TRANSFER:
				case TC_HZT:							//liyi add 绿卡通
				case TC_TZH:
					memcpy(&DCTranEjrBuffer[nTempLen], "003", 3);
					break;
				case TC_WITHDRAWAL:
					memcpy(&DCTranEjrBuffer[nTempLen], "002", 3);
					break;
				case TC_CHANGEPIN:
					memcpy(&DCTranEjrBuffer[nTempLen], "004", 3);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	nTempLen += 3;												// Tran Kind
	memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
	nTempLen += 1;												// Separator
	memcpy(&DCTranEjrBuffer[nTempLen], GetSprintf("%8.8s%6.6s", GetDate(), GetTime()), 14);
	nTempLen += 14;												// Date & Time
	memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
	nTempLen += 1;												// Separator
	memcpy(&DCTranEjrBuffer[nTempLen], m_pProfile->TRANS.SerialNo, 8);
	nTempLen += 8;												// Serial No
	memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
	nTempLen += 1;												// Separator
	strTemp.Format("%12.12s", HostResp.ReferenceNo);
	strTemp.Remove(' ');
	if (strTemp.GetLength())
	{
		if (strTemp.GetLength() <= 12)
		{
			memcpy(&DCTranEjrBuffer[nTempLen], strTemp, strTemp.GetLength());
			nTempLen += strTemp.GetLength();					// Reference No
		}
		else
		{
			memcpy(&DCTranEjrBuffer[nTempLen], strTemp, 12);
			nTempLen += 12;										// Reference No
		}
	}
	memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
	nTempLen += 1;												// Separator
	if (strlen(CardAccountNo))
	{
		if (strlen(CardAccountNo) <= 19)
		{
			memcpy(&DCTranEjrBuffer[nTempLen], CardAccountNo, strlen(CardAccountNo));
			nTempLen += strlen(CardAccountNo);					// Account No
		}
		else
		{
			memcpy(&DCTranEjrBuffer[nTempLen], CardAccountNo, 19);
			nTempLen += 19;										// Account No
		}
	}
	memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
	nTempLen += 1;												// Separator
	if (!IsZero(Accept.Money, sizeof(Accept.Money)))
	{
		memset(szTempBuff, 0, sizeof(szTempBuff));
		memcpy(szTempBuff, Accept.Money, sizeof(Accept.Money));
		Zero2Space(szTempBuff, sizeof(Accept.Money));
		strTemp.Format("%12.12s", szTempBuff);
		strTemp.Remove(' ');
		memcpy(&DCTranEjrBuffer[nTempLen], strTemp, strTemp.GetLength());
		nTempLen += strTemp.GetLength();						// Amount
	}
	
	memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
	nTempLen += 1;												// Separator
	if ((nTranStatus == TRAN_CANCEL)	||
		(nTranStatus == TRAN_TRAN))
	{
		memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
		nTempLen += 3;											// Currency Code
	}
	memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
	nTempLen += 1;												// Separator
	strTemp = GetString(HostResp.RejectCode, sizeof(HostResp.RejectCode), sizeof(HostResp.RejectCode));
	if (IsSpace(strTemp.GetBuffer(0), strTemp.GetLength()))
	{
		memset(&DCTranEjrBuffer[nTempLen], 'F', 4);
	}
	else
	{
		strTemp.Remove(' ');
		memcpy(&DCTranEjrBuffer[nTempLen], GetSprintf("%04.4s", strTemp), 4);
	}

	if (nTranStatus == TRAN_ERROR)
	{
		memset(&DCTranEjrBuffer[nTempLen], '0', 4);
	}

	nTempLen += 4;												// Reject Code
	memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
	nTempLen += 1;												// Separator
	if ((nTranStatus == TRAN_ERROR)		||
		(nTranStatus == TRAN_CANCEL)	||
		(nTranStatus == TRAN_TRAN))
	{
		if (CardBank == 1)
			memcpy(&DCTranEjrBuffer[nTempLen], "0", 1);
		else
			memcpy(&DCTranEjrBuffer[nTempLen], "1", 3);
		nTempLen += 1;											// Card Type: 0-Mother, 1-Other
	}

	memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
	nTempLen += 1;												// Separator
	switch (TranCode)
	{
		case TC_OPEN:
			break;
		case TC_CANCEL:
			memcpy(&DCTranEjrBuffer[nTempLen], OrgSerialNo, sizeof(OrgSerialNo));
			nTempLen += sizeof(OrgSerialNo);					// Org Serial No
			memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
			nTempLen += 1;										// Separator
			break;
		case TC_ERROR:
			strTemp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_RESPONSENO);
			if (strTemp.GetLength())
			{
				memcpy(&DCTranEjrBuffer[nTempLen], strTemp, strTemp.GetLength());
				nTempLen += strTemp.GetLength();				// Card Retract Reason: Host Order
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_RESPONSENO, "");
			}
			else
			{
				memset(szTempBuff, 'F', sizeof(HostResp.RejectCode));
				memcpy(&DCTranEjrBuffer[nTempLen], szTempBuff, sizeof(HostResp.RejectCode));
				nTempLen += sizeof(HostResp.RejectCode);		// Card Retract Reason: Lost Card
			}
			memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
			nTempLen += 1;										// Separator
			break;
		case TC_TOTAL:
		case TC_TOTALPRE:
			break;
		case TC_ADDCASH:
		case TC_SUBCASH:
		case TC_CLEARCASH:
			memset(&DCTranEjrBuffer[nTempLen], '0', 11 * 4);	// Cst Remain Count
			if (m_pDevCmn->CSTCnt >= 1)
			{
				memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
				memcpy(&DCTranEjrBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FstCSTValue), 4);
				memcpy(&DCTranEjrBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
			}
			nTempLen += 11;
			if (m_pDevCmn->CSTCnt >= 2)
			{
				memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
				memcpy(&DCTranEjrBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->SndCSTValue), 4);
				memcpy(&DCTranEjrBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
			}
			nTempLen += 11;
			if (m_pDevCmn->CSTCnt >= 3)
			{
				memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
				memcpy(&DCTranEjrBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->TrdCSTValue), 4);
				memcpy(&DCTranEjrBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
			}
			nTempLen += 11;
			if (m_pDevCmn->CSTCnt >= 4)
			{
				memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
				memcpy(&DCTranEjrBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FthCSTValue), 4);
				memcpy(&DCTranEjrBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
			}
			nTempLen += 11;
/////////////////////////////////////////////////////////////////////////////
			memset(&DCTranEjrBuffer[nTempLen], '0', 11 * 4);	// Cst Dispense Count
			if (m_pDevCmn->CSTCnt >= 1)
			{
				memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
				memcpy(&DCTranEjrBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FstCSTValue), 4);
				memcpy(&DCTranEjrBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_1)), 4);
			}
			nTempLen += 11;
			if (m_pDevCmn->CSTCnt >= 2)
			{
				memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
				memcpy(&DCTranEjrBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->SndCSTValue), 4);
				memcpy(&DCTranEjrBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_2)), 4);
			}
			nTempLen += 11;
			if (m_pDevCmn->CSTCnt >= 3)
			{
				memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
				memcpy(&DCTranEjrBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->TrdCSTValue), 4);
				memcpy(&DCTranEjrBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_3)), 4);
			}
			nTempLen += 11;
			if (m_pDevCmn->CSTCnt >= 4)
			{
				memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
				memcpy(&DCTranEjrBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FthCSTValue), 4);
				memcpy(&DCTranEjrBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_4)), 4);
			}
			nTempLen += 11;
/////////////////////////////////////////////////////////////////////////////
			memset(&DCTranEjrBuffer[nTempLen], '0', 11 * 4);	// Cst Set Cash Count
			if (m_pDevCmn->CSTCnt >= 1)
			{
				memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
				memcpy(&DCTranEjrBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FstCSTValue), 4);
				memcpy(&DCTranEjrBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->FstCSTSetCash), 4);
			}
			nTempLen += 11;
			if (m_pDevCmn->CSTCnt >= 2)
			{
				memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
				memcpy(&DCTranEjrBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->SndCSTValue), 4);
				memcpy(&DCTranEjrBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->SndCSTSetCash), 4);
			}
			nTempLen += 11;
			if (m_pDevCmn->CSTCnt >= 3)
			{
				memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
				memcpy(&DCTranEjrBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->TrdCSTValue), 4);
				memcpy(&DCTranEjrBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->TrdCSTSetCash), 4);
			}
			nTempLen += 11;
			if (m_pDevCmn->CSTCnt >= 4)
			{
				memcpy(&DCTranEjrBuffer[nTempLen], "156", 3);
				memcpy(&DCTranEjrBuffer[nTempLen + 3], GetSprintf("%04d", m_pDevCmn->FthCSTValue), 4);
				memcpy(&DCTranEjrBuffer[nTempLen + 7], GetSprintf("%04d", m_pDevCmn->FthCSTSetCash), 4);
			}
			nTempLen += 11;
/////////////////////////////////////////////////////////////////////////////
			memcpy(&DCTranEjrBuffer[nTempLen], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_REJECT)), 4);
			nTempLen += 4;										// Reject Count
			memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
			nTempLen += 1;										// Separator
			break;
		case TC_DOWNFIT:
		case TC_DOWNICFIT:										// added by yusy ic
		case TC_DOWNAIDLIST:
		case TC_DOWNPBOCPARA:
		case TC_DOWNPARM:
		case TC_DOWNPAY:
		case TC_DOWNECHANNELPARA:
		case TC_DOWNECHANNELPROT:
			break;
		case TC_INQUIRY:
		case TC_OTHER_INQ:
		case TC_CUS_INF_INQ:
		case TC_CUS_INF_INQ_OTHERBANK:
		case TC_ECHANNEL_INQ:
		case TC_SMSPARITY:
		case TC_ECHANNEL:
		case TC_INQUIRY_CheckPin:
			break;
		case TC_HZT:											//liyi add 绿卡通
		case TC_TZH:
			break;
		case TC_TRANSFER:
			memcpy(&DCTranEjrBuffer[nTempLen], Accept.InAccountNum, Accept.InAccountNumSize);
			nTempLen += Accept.InAccountNumSize;				// To Account No
			memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
			nTempLen += 1;										// Separator
			break;

		case TC_WITHDRAWAL:
			if (m_pDevCmn->TranResult)
			{
				if (m_pDevCmn->DeviceLost & DEV_CSH)
					memcpy(&DCTranEjrBuffer[nTempLen], "1", 1);
				else
				if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))
					memcpy(&DCTranEjrBuffer[nTempLen], "2", 1);
				else
					memcpy(&DCTranEjrBuffer[nTempLen], "0", 1);
			}
			else
				memcpy(&DCTranEjrBuffer[nTempLen], "2", 1);
			nTempLen += 1;										// Dispense Status
			memcpy(&DCTranEjrBuffer[nTempLen], "|", 1);
			nTempLen += 1;										// Separator
			break;
		case TC_CHANGEPIN:
			break;
		default:
			break;
	}
	
	memcpy(&DCTranEjrBuffer[nTempLen], ";", 1);
	nTempLen += 1;												// ETX

	FILE*		TempFp;
	CString		strTempFileName("");
	int			nFirstWrite = FALSE;
	CFileStatus	TempFileStatus;

	_mkdir(_EJR_DCNMS);
	strTempFileName.Format("%s\\%s.TXT", _EJR_DCNMS, GetDate());
	
	strTemp.Format("%-256.256s", DCTranEjrBuffer);
	TempFp = fopen(strTempFileName, "ac");
	if (TempFp)
	{
		fprintf(TempFp, strTemp);
		fprintf(TempFp, "\n");
		fflush(TempFp);
		fclose(TempFp);
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_WriteDCTransactionEjr():return");
	return T_OK;
}

// Get DC Transaction Ejr
int CTranCmn::fnAPL_GetDCTransactionEjr()
{
	if (m_pProfile->DEVICE.EjrDevice == 2)						// DC NMS : 2006.06.16
		;
	else
		return TRUE;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCTransactionEjr()");

	FILE*			TempFp;
	int				nCount = 0;
	int				nTempPosition = 0;
	char			szBuff[1024];
	CString			strTempFileName("");
	CString			strPrintData("");
	CString			strPrintDataSave("");
	CString			strTempData("");
	CFileStatus		TempFileStatus;

	memset(szBuff, 0, sizeof(szBuff));

/////////////////////////////////////////////////////////////////////////////
	DCEjrSendCnt = 0;
	DCEjrSendData = "";
/////////////////////////////////////////////////////////////////////////////

	strTempFileName.Format("%s\\%8.8s.TXT", _EJR_DCNMS, DCBidEjrDate);
	
	if (CFile::GetStatus(strTempFileName, TempFileStatus))		// File Check
	{
		TempFp = fopen(strTempFileName, "rb");
		if (TempFp)
		{
			while (TRUE)
			{
				nCount = 0;
				nTempPosition = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DCEJRSENDP);
				fseek(TempFp, nTempPosition * 258, SEEK_SET);
				
				memset(szBuff, 0, sizeof(szBuff));
				if (fgets(szBuff, sizeof(szBuff), TempFp))		// 1 Line Read
				{
					nCount++;
					nTempPosition += nCount;
					IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DCEJRSENDP, nTempPosition);
					strTempData.Format("%s", szBuff);
					strTempData.TrimLeft();
					strTempData.TrimRight();
					if ((strTempData.Left(1) == ":")	&&
						(strTempData.Right(1) == ";"))
						;
					else
					{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCTransactionEjr() Data Format Error!!!");
						continue;
					}

					strPrintDataSave = strTempData + '\n';

					if (strTempData.GetLength() <= 258)
					{
						strPrintData += strPrintDataSave;
					}
					else
					{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCTransactionEjr() Data Format OverSize!!!");
						continue;
					}

					if (strPrintData.GetLength() <= 1000)
					{
						DCEjrSendCnt++;
						continue;
					}
					else
					{
						nTempPosition -= 1;
						IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DCEJRSENDP, nTempPosition);
						DCEjrSendData = strPrintData.Left(strPrintData.GetLength() - strPrintDataSave.GetLength());
						break;
					}
				}
				else
				{
					DCEjrSendData = strPrintData;
					break;
				}
			}
			fclose(TempFp);
		}
		else
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCTransactionEjr() File Open Failed!!!");
		}
	}
	else
	{
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DCEJRSENDP, 0);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCTransactionEjr() File Not Found!!!");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCTransactionEjr() DCEjrSendData[%s]", DCEjrSendData);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCTransactionEjr() nTempPosition(%d)", nTempPosition);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCTransactionEjr():return(%d)", DCEjrSendCnt);
	return DCEjrSendCnt;
}

// Get DC Ejr Count
int CTranCmn::fnAPL_GetDCEjrCount()
{
	if (m_pProfile->DEVICE.EjrDevice == 2)						// DC NMS : 2006.06.16
		;
	else
		return TRUE;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCEjrCount()");

	FILE*			TempFp;
	int				nCount = 0;
	int				nTempPosition = 0;
	char			szBuff[1024];
	CString			strTempFileName("");
	CString			strPrintData("");
	CString			strPrintDataSave("");
	CString			strTempData("");
	CFileStatus		TempFileStatus;

	memset(szBuff, 0, sizeof(szBuff));

/////////////////////////////////////////////////////////////////////////////
	DCEjrSendCnt = 0;
	IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DCEJRSENDP, 0);
/////////////////////////////////////////////////////////////////////////////

	strTempFileName.Format("%s\\%8.8s.TXT", _EJR_DCNMS, DCBidEjrDate);
	
	if (CFile::GetStatus(strTempFileName, TempFileStatus))		// File Check
	{
		TempFp = fopen(strTempFileName, "rb");
		if (TempFp)
		{
			while (TRUE)
			{
				nCount = 0;
				nTempPosition = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DCEJRSENDP);
				fseek(TempFp, nTempPosition * 258, SEEK_SET);	// 256 + CR + LF
				
				memset(szBuff, 0, sizeof(szBuff));
				if (fgets(szBuff, sizeof(szBuff), TempFp))		// 1 Line Read
				{
					nCount++;
					nTempPosition += nCount;
					IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DCEJRSENDP, nTempPosition);
					strTempData.Format("%s", szBuff);
					strTempData.TrimLeft();
					strTempData.TrimRight();
					if ((strTempData.Left(1) == ":")	&&
						(strTempData.Right(1) == ";"))
						;
					else
					{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCEjrCount() Data Format Error!!!");
						continue;
					}

					if (strTempData.GetLength() <= 258)
						;
					else
					{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCEjrCount() Data Format OverSize!!!");
						continue;
					}

					DCEjrSendCnt++;
				}
				else
					break;
			}
			fclose(TempFp);
		}
		else
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCEjrCount() File Open Failed!!!");
		}
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCEjrCount() File Not Found!!!");
	}

	IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DCEJRSENDP, 0);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDCEjrCount():return(%d)", DCEjrSendCnt);
	return DCEjrSendCnt;
}

// Get Dvr Status
int CTranCmn::fnAPL_GetDvrStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDvrStatus()");

	CString strReturn("");
	char	szTempBuff[256];
	int		nReturn = 0;
	
	memset(szTempBuff, 0, sizeof(szTempBuff));
	strReturn = m_pDevCmn->fstrDVR_GetStatus();

	if (strReturn.GetLength() >= 10)
	{
		MakePack(strReturn.GetBuffer(0), szTempBuff, strReturn.GetLength());
		nReturn = Asc2Int(&szTempBuff[0], 1);
		if (nReturn > DvrStatusReturn)
		{
			DvrStatusReturn = nReturn;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDvrStatus(): return(%d)", DvrStatusReturn);
		}
		else
		if ((DvrStatusReturn == 3)	&&
			(nReturn == 0))
		{
			DvrStatusReturn = nReturn;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDvrStatus(): return(%d)", DvrStatusReturn);
		}
	}

	return DvrStatusReturn;
}

// Des Encrypt Decrypt(Mode 0:Decrypt, 1:Encrypt)
int CTranCmn::fnAPL_DesEncryptDecrypt(UCHAR*  szKey, UCHAR* szData, int Mode)
{
	int     valindex;
	int		i, j, k, iter;
	UCHAR   bufout[64];
	UCHAR 	kwork[56];
	UCHAR   worka[48];
	UCHAR   kn[48];
	UCHAR   buffer[64];
	UCHAR   key[64];
	UCHAR   nbrofshift;
	UCHAR   temp1;
	UCHAR   temp2;

																// Initialize The Tables
	static UCHAR	s1[4][16] = {								// Table - S1
		14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
		0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
		4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
		15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13  };

	static UCHAR	s2[4][16] = {								// Table - S2
		15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
		3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
		0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
		13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9  };

	static UCHAR	s3[4][16] = {								// Table - S3
		10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
		13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
		13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
		1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12  };

	static UCHAR	s4[4][16] = {								// Table - S4
		7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
		13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
		10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
		3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14  };

	static UCHAR	s5[4][16] = {								// Table - S5
		2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
		14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
		4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
		11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3  };

	static UCHAR	s6[4][16] = {								// Table - S6
		12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
		10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
		9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
		4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13  };

	static UCHAR	s7[4][16] = {								// Table - S7
		4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
		13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
		1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
		6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12  };

	static UCHAR	s8[4][16] = {								// Table - S8
		13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
		1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
		7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
		2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11  };

	static UCHAR	shift[16] = {								// Table - Shift
		1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1	      };


	static UCHAR	binary[64] = {								// Table - Binary
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1,
		0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1,
		1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1,
		1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1		  };

																// Main Process
	for (i = 0; i < 8; i++)										// Convert From 64-Bit Key Into 64-Byte Key
	{
		key[8*i]   = ((j = *(szKey + i)) / 128) % 2;
		key[8*i+1] = (j / 64) % 2;
		key[8*i+2] = (j / 32) % 2;
		key[8*i+3] = (j / 16) % 2;
		key[8*i+4] = (j / 8) % 2;
		key[8*i+5] = (j / 4) % 2;
		key[8*i+6] = (j / 2) % 2;
		key[8*i+7] = (j % 2);
	}

	for (i = 0; i < 8; i++)										// Convert From 64-Bit Data Into 64-Byte Data
	{
		buffer[8*i]   = ((j = *(szData + i)) / 128) % 2;
		buffer[8*i+1] = (j / 64) % 2;
		buffer[8*i+2] = (j / 32) % 2;
		buffer[8*i+3] = (j / 16) % 2;
		buffer[8*i+4] = (j / 8) % 2;
		buffer[8*i+5] = (j / 4) % 2;
		buffer[8*i+6] = (j / 2) % 2;
		buffer[8*i+7] = (j % 2);
	}

	bufout[ 0] = buffer[57];									// Initial Permutation of Data
	bufout[ 1] = buffer[49];
	bufout[ 2] = buffer[41];
	bufout[ 3] = buffer[33];
	bufout[ 4] = buffer[25];
	bufout[ 5] = buffer[17];
	bufout[ 6] = buffer[ 9];
	bufout[ 7] = buffer[ 1];
	bufout[ 8] = buffer[59];
	bufout[ 9] = buffer[51];
	bufout[10] = buffer[43];
	bufout[11] = buffer[35];
	bufout[12] = buffer[27];
	bufout[13] = buffer[19];
	bufout[14] = buffer[11];
	bufout[15] = buffer[ 3];
	bufout[16] = buffer[61];
	bufout[17] = buffer[53];
	bufout[18] = buffer[45];
	bufout[19] = buffer[37];
	bufout[20] = buffer[29];
	bufout[21] = buffer[21];
	bufout[22] = buffer[13];
	bufout[23] = buffer[ 5];
	bufout[24] = buffer[63];
	bufout[25] = buffer[55];
	bufout[26] = buffer[47];
	bufout[27] = buffer[39];
	bufout[28] = buffer[31];
	bufout[29] = buffer[23];
	bufout[30] = buffer[15];
	bufout[31] = buffer[ 7];
	bufout[32] = buffer[56];
	bufout[33] = buffer[48];
	bufout[34] = buffer[40];
	bufout[35] = buffer[32];
	bufout[36] = buffer[24];
	bufout[37] = buffer[16];
	bufout[38] = buffer[ 8];
	bufout[39] = buffer[ 0];
	bufout[40] = buffer[58];
	bufout[41] = buffer[50];
	bufout[42] = buffer[42];
	bufout[43] = buffer[34];
	bufout[44] = buffer[26];
	bufout[45] = buffer[18];
	bufout[46] = buffer[10];
	bufout[47] = buffer[ 2];
	bufout[48] = buffer[60];
	bufout[49] = buffer[52];
	bufout[50] = buffer[44];
	bufout[51] = buffer[36];
	bufout[52] = buffer[28];
	bufout[53] = buffer[20];
	bufout[54] = buffer[12];
	bufout[55] = buffer[ 4];
	bufout[56] = buffer[62];
	bufout[57] = buffer[54];
	bufout[58] = buffer[46];
	bufout[59] = buffer[38];
	bufout[60] = buffer[30];
	bufout[61] = buffer[22];
	bufout[62] = buffer[14];
	bufout[63] = buffer[ 6];

	kwork[ 0] = key[56];										// Initial Permutation of Key
	kwork[ 1] = key[48];
	kwork[ 2] = key[40];
	kwork[ 3] = key[32];
	kwork[ 4] = key[24];
	kwork[ 5] = key[16];
	kwork[ 6] = key[ 8];
	kwork[ 7] = key[ 0];
	kwork[ 8] = key[57];
	kwork[ 9] = key[49];
	kwork[10] = key[41];
	kwork[11] = key[33];
	kwork[12] = key[25];
	kwork[13] = key[17];
	kwork[14] = key[ 9];
	kwork[15] = key[ 1];
	kwork[16] = key[58];
	kwork[17] = key[50];
	kwork[18] = key[42];
	kwork[19] = key[34];
	kwork[20] = key[26];
	kwork[21] = key[18];
	kwork[22] = key[10];
	kwork[23] = key[ 2];
	kwork[24] = key[59];
	kwork[25] = key[51];
	kwork[26] = key[43];
	kwork[27] = key[35];
	kwork[28] = key[62];
	kwork[29] = key[54];
	kwork[30] = key[46];
	kwork[31] = key[38];
	kwork[32] = key[30];
	kwork[33] = key[22];
	kwork[34] = key[14];
	kwork[35] = key[ 6];
	kwork[36] = key[61];
	kwork[37] = key[53];
	kwork[38] = key[45];
	kwork[39] = key[37];
	kwork[40] = key[29];
	kwork[41] = key[21];
	kwork[42] = key[13];
	kwork[43] = key[ 5];
	kwork[44] = key[60];
	kwork[45] = key[52];
	kwork[46] = key[44];
	kwork[47] = key[36];
	kwork[48] = key[28];
	kwork[49] = key[20];
	kwork[50] = key[12];
	kwork[51] = key[ 4];
	kwork[52] = key[27];
	kwork[53] = key[19];
	kwork[54] = key[11];
	kwork[55] = key[ 3];

	for (iter = 1; iter < 17; iter++)							// 16 Iterations
	{
		for (i = 0; i < 32; i++)
			buffer[i] = bufout[32+i];
																// Calculation of F(R, K)
		worka[ 0] = buffer[31];									// Permute - E
		worka[ 1] = buffer[ 0];
		worka[ 2] = buffer[ 1];
		worka[ 3] = buffer[ 2];
		worka[ 4] = buffer[ 3];
		worka[ 5] = buffer[ 4];
		worka[ 6] = buffer[ 3];
		worka[ 7] = buffer[ 4];
		worka[ 8] = buffer[ 5];
		worka[ 9] = buffer[ 6];
		worka[10] = buffer[ 7];
		worka[11] = buffer[ 8];
		worka[12] = buffer[ 7];
		worka[13] = buffer[ 8];
		worka[14] = buffer[ 9];
		worka[15] = buffer[10];
		worka[16] = buffer[11];
		worka[17] = buffer[12];
		worka[18] = buffer[11];
		worka[19] = buffer[12];
		worka[20] = buffer[13];
		worka[21] = buffer[14];
		worka[22] = buffer[15];
		worka[23] = buffer[16];
		worka[24] = buffer[15];
		worka[25] = buffer[16];
		worka[26] = buffer[17];
		worka[27] = buffer[18];
		worka[28] = buffer[19];
		worka[29] = buffer[20];
		worka[30] = buffer[19];
		worka[31] = buffer[20];
		worka[32] = buffer[21];
		worka[33] = buffer[22];
		worka[34] = buffer[23];
		worka[35] = buffer[24];
		worka[36] = buffer[23];
		worka[37] = buffer[24];
		worka[38] = buffer[25];
		worka[39] = buffer[26];
		worka[40] = buffer[27];
		worka[41] = buffer[28];
		worka[42] = buffer[27];
		worka[43] = buffer[28];
		worka[44] = buffer[29];
		worka[45] = buffer[30];
		worka[46] = buffer[31];
		worka[47] = buffer[ 0];
																// KS Function Begin
		if (Mode == 1)											// Encrypt
		{
			nbrofshift = shift[iter-1];
			for (i = 0; i < nbrofshift; i++)
			{
				temp1 = kwork[0];
				temp2 = kwork[28];
				for (j = 0; j < 27; j++)
				{
					kwork[j]    = kwork[j+ 1];
					kwork[j+28] = kwork[j+29];
				}
				kwork[27] = temp1;
				kwork[55] = temp2;
			}
		}
		else													// Decrypt
		if (iter > 1)
		{
			nbrofshift = shift[17-iter];
			for (i = 0; i < nbrofshift; i++)
			{
				temp1 = kwork[27];
				temp2 = kwork[55];
				for (j = 27; j > 0; j--)
				{
					kwork[j]    = kwork[j- 1];
					kwork[j+28] = kwork[j+27];
				}
				kwork[ 0] = temp1;
				kwork[28] = temp2;
			}
		}

		kn[ 0] = kwork[13];										// Permute kwork - PC2
		kn[ 1] = kwork[16];
		kn[ 2] = kwork[10];
		kn[ 3] = kwork[23];
		kn[ 4] = kwork[ 0];
		kn[ 5] = kwork[ 4];
		kn[ 6] = kwork[ 2];
		kn[ 7] = kwork[27];
		kn[ 8] = kwork[14];
		kn[ 9] = kwork[ 5];
		kn[10] = kwork[20];
		kn[11] = kwork[ 9];
		kn[12] = kwork[22];
		kn[13] = kwork[18];
		kn[14] = kwork[11];
		kn[15] = kwork[ 3];
		kn[16] = kwork[25];
		kn[17] = kwork[ 7];
		kn[18] = kwork[15];
		kn[19] = kwork[ 6];
		kn[20] = kwork[26];
		kn[21] = kwork[19];
		kn[22] = kwork[12];
		kn[23] = kwork[ 1];
		kn[24] = kwork[40];
		kn[25] = kwork[51];
		kn[26] = kwork[30];
		kn[27] = kwork[36];
		kn[28] = kwork[46];
		kn[29] = kwork[54];
		kn[30] = kwork[29];
		kn[31] = kwork[39];
		kn[32] = kwork[50];
		kn[33] = kwork[44];
		kn[34] = kwork[32];
		kn[35] = kwork[47];
		kn[36] = kwork[43];
		kn[37] = kwork[48];
		kn[38] = kwork[38];
		kn[39] = kwork[55];
		kn[40] = kwork[33];
		kn[41] = kwork[52];
		kn[42] = kwork[45];
		kn[43] = kwork[41];
		kn[44] = kwork[49];
		kn[45] = kwork[35];
		kn[46] = kwork[28];
		kn[47] = kwork[31];
																// KS Function End

		for (i = 0; i < 48; i++)								// worka XOR kn
			worka[i] = worka[i] ^ kn[i];
																// 8 S-Functions
		valindex = s1[(2 * worka[ 0] + worka[ 5])][(2 * (2 * (2 * worka[ 1] + worka[ 2]) + worka[ 3]) + worka[ 4])];
		valindex = valindex * 4;
		kn[ 0] = binary[0+valindex];
		kn[ 1] = binary[1+valindex];
		kn[ 2] = binary[2+valindex];
		kn[ 3] = binary[3+valindex];

		valindex = s2[(2 * worka[ 6] + worka[11])][(2 * (2 * (2 * worka[ 7] + worka[ 8]) + worka[ 9]) + worka[10])];
		valindex = valindex * 4;
		kn[ 4] = binary[0+valindex];
		kn[ 5] = binary[1+valindex];
		kn[ 6] = binary[2+valindex];
		kn[ 7] = binary[3+valindex];

		valindex = s3[(2 * worka[12] + worka[17])][(2 * (2 * (2 * worka[13] + worka[14]) + worka[15]) + worka[16])];
		valindex = valindex * 4;
		kn[ 8] = binary[0+valindex];
		kn[ 9] = binary[1+valindex];
		kn[10] = binary[2+valindex];
		kn[11] = binary[3+valindex];

		valindex = s4[(2 * worka[18] + worka[23])][(2 * (2 * (2 * worka[19] + worka[20]) + worka[21]) + worka[22])];
		valindex = valindex * 4;
		kn[12] = binary[0+valindex];
		kn[13] = binary[1+valindex];
		kn[14] = binary[2+valindex];
		kn[15] = binary[3+valindex];

		valindex = s5[(2 * worka[24] + worka[29])][(2 * (2 * (2 * worka[25] + worka[26]) + worka[27]) + worka[28])];
		valindex = valindex * 4;
		kn[16] = binary[0+valindex];
		kn[17] = binary[1+valindex];
		kn[18] = binary[2+valindex];
		kn[19] = binary[3+valindex];

		valindex = s6[(2 * worka[30] + worka[35])][(2 * (2 * (2 * worka[31] + worka[32]) + worka[33]) + worka[34])];
		valindex = valindex * 4;
		kn[20] = binary[0+valindex];
		kn[21] = binary[1+valindex];
		kn[22] = binary[2+valindex];
		kn[23] = binary[3+valindex];
		
		valindex = s7[(2 * worka[36] + worka[41])][(2 * (2 * (2 * worka[37] + worka[38]) + worka[39]) + worka[40])];
		valindex = valindex * 4;
		kn[24] = binary[0+valindex];
		kn[25] = binary[1+valindex];
		kn[26] = binary[2+valindex];
		kn[27] = binary[3+valindex];
		
		valindex = s8[(2 * worka[42] + worka[47])][(2 * (2 * (2 * worka[43] + worka[44]) + worka[45]) + worka[46])];
		valindex = valindex * 4;
		kn[28] = binary[0+valindex];
		kn[29] = binary[1+valindex];
		kn[30] = binary[2+valindex];
		kn[31] = binary[3+valindex];

		worka[ 0] = kn[15];										// Permute - P
		worka[ 1] = kn[ 6];
		worka[ 2] = kn[19];
		worka[ 3] = kn[20];
		worka[ 4] = kn[28];
		worka[ 5] = kn[11];
		worka[ 6] = kn[27];
		worka[ 7] = kn[16];
		worka[ 8] = kn[ 0];
		worka[ 9] = kn[14];
		worka[10] = kn[22];
		worka[11] = kn[25];
		worka[12] = kn[ 4];
		worka[13] = kn[17];
		worka[14] = kn[30];
		worka[15] = kn[ 9];
		worka[16] = kn[ 1];
		worka[17] = kn[ 7];
		worka[18] = kn[23];
		worka[19] = kn[13];
		worka[20] = kn[31];
		worka[21] = kn[26];
		worka[22] = kn[ 2];
		worka[23] = kn[ 8];
		worka[24] = kn[18];
		worka[25] = kn[12];
		worka[26] = kn[29];
		worka[27] = kn[ 5];
		worka[28] = kn[21];
		worka[29] = kn[10];
		worka[30] = kn[ 3];
		worka[31] = kn[24];

		for (i = 0; i < 32; i++)
		{
			bufout[i+32] = bufout[i] ^ worka[i];
			bufout[i]    = buffer[i];
		}
	}															// End of Iter

	for (i = 0; i < 32; i++)									// Prepare Output
	{
		j = bufout[i];
		bufout[i]    = bufout[32+i];
		bufout[32+i] = j;
	}

	buffer[ 0] = bufout[39];									// Inverse Initial Permutation
	buffer[ 1] = bufout[ 7];
	buffer[ 2] = bufout[47];
	buffer[ 3] = bufout[15];
	buffer[ 4] = bufout[55];
	buffer[ 5] = bufout[23];
	buffer[ 6] = bufout[63];
	buffer[ 7] = bufout[31];
	buffer[ 8] = bufout[38];
	buffer[ 9] = bufout[ 6];
	buffer[10] = bufout[46];
	buffer[11] = bufout[14];
	buffer[12] = bufout[54];
	buffer[13] = bufout[22];
	buffer[14] = bufout[62];
	buffer[15] = bufout[30];
	buffer[16] = bufout[37];
	buffer[17] = bufout[ 5];
	buffer[18] = bufout[45];
	buffer[19] = bufout[13];
	buffer[20] = bufout[53];
	buffer[21] = bufout[21];
	buffer[22] = bufout[61];
	buffer[23] = bufout[29];
	buffer[24] = bufout[36];
	buffer[25] = bufout[ 4];
	buffer[26] = bufout[44];
	buffer[27] = bufout[12];
	buffer[28] = bufout[52];
	buffer[29] = bufout[20];
	buffer[30] = bufout[60];
	buffer[31] = bufout[28];
	buffer[32] = bufout[35];
	buffer[33] = bufout[ 3];
	buffer[34] = bufout[43];
	buffer[35] = bufout[11];
	buffer[36] = bufout[51];
	buffer[37] = bufout[19];
	buffer[38] = bufout[59];
	buffer[39] = bufout[27];
	buffer[40] = bufout[34];
	buffer[41] = bufout[ 2];
	buffer[42] = bufout[42];
	buffer[43] = bufout[10];
	buffer[44] = bufout[50];
	buffer[45] = bufout[18];
	buffer[46] = bufout[58];
	buffer[47] = bufout[26];
	buffer[48] = bufout[33];
	buffer[49] = bufout[ 1];
	buffer[50] = bufout[41];
	buffer[51] = bufout[ 9];
	buffer[52] = bufout[49];
	buffer[53] = bufout[17];
	buffer[54] = bufout[57];
	buffer[55] = bufout[25];
	buffer[56] = bufout[32];
	buffer[57] = bufout[ 0];
	buffer[58] = bufout[40];
	buffer[59] = bufout[ 8];
	buffer[60] = bufout[48];
	buffer[61] = bufout[16];
	buffer[62] = bufout[56];
	buffer[63] = bufout[24];

	for (i = 0, j = 0; i < 8; i++)
	{
		*(szData + i) = 0x00;

		for (k = 0; k < 7; k++)
			*(szData + i) = ((*(szData + i)) + buffer[j+k]) * 2;

		*(szData + i) = *(szData + i) + buffer[j+7];

		j += 8;
	}

	return TRUE;
}

// Encrypt PIN
int CTranCmn::fnAPL_EncryptPIN(LPSTR szPin, LPSTR szEncryptedPin)
{
	char	szBlock1[256];
	char	szTempBuff[256];
	char	TPinKey[8];

	memset(szBlock1, 0, sizeof(szBlock1));
	memset(szTempBuff, 0, sizeof(szTempBuff));

	if (strlen(szPin) < 8)
		memcpy(&szBlock1[8 - strlen(szPin)], szPin, strlen(szPin));
	else
		memcpy(szBlock1, szPin, 8);

	MakePack(PinKey, TPinKey, 16);
	fnAPL_DesEncryptDecrypt((UCHAR *)TPinKey, (UCHAR *)szBlock1, 1);

	memcpy(szEncryptedPin, szBlock1, 8);

	return T_OK;
}

// Get Fit File Content Save
int CTranCmn::fnAPL_GetFitFileContentSave(LPCTSTR FitFileName)
{
	int			nFileResult = 0;
	FILE*		fpStream = NULL;

	memset(FitFileContent, 0, sizeof(FitFileContent));

	fpStream = fopen(FitFileName, "r");
	if (fpStream == NULL)
		return T_MSG;											// File Open Fail
	
	while (!feof(fpStream))										// Save Fit File
	{
		fgets(FitFileContent[nFileResult++], itemof(FitFileContent[0]) - 1 , fpStream);
		if (nFileResult >= (itemof(FitFileContent) - 1))			// The Last Item
			break;
	}

	fclose(fpStream);
	memcpy(FitFileContent[nFileResult], "AAAAAA", 6);			// End Flag

	return T_OK;
}

// Get Fit File Content Save
int CTranCmn::fnAPL_GetFitFileContent()
{
	memset(FITContent_POST, 0, sizeof(FITContent_POST));
	CString strTmp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_FITPARM, "FITTNUM", "");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetFitFileContent()FITTNUM[%d]", Asc2Int(strTmp));
	if (strTmp == "" || Asc2Int(strTmp) <= 0)
		memcpy(FITContent_POST[0], "AAAAAA", 6);			// End Flag
	else
	{
		CString strFITtemp;
		int iFITNum = 0;

		for (int i = 1; i <= Asc2Int(strTmp); i++)			// 注意卡表记录序号是否为1开始   by zjd
		{
			strFITtemp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_FITPARM, GetSprintf("%s%d","FITT",i), "");
			if (strFITtemp != "")
			{
				memcpy(FITContent_POST[iFITNum], strFITtemp.GetBuffer(0), strlen(strFITtemp));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetFitFileContent()FITContent_POST[%d][%s]", iFITNum, FITContent_POST[iFITNum]);
				iFITNum ++;
			}
		}
		memcpy(FITContent_POST[iFITNum], "AAAAAA", 6);			// End Flag
	}
	return T_OK;
}

// Get IC Fit File Content Save
int CTranCmn::fnAPL_GetICFitFileContent()
{
	memset(ICFITContent_POST, 0, sizeof(ICFITContent_POST));
	CString strTmp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ICFITPARM, "FITTNUM", "");
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetICFitFileContent()FITTNUM[%d]", Asc2Int(strTmp));
	if (strTmp == "" || Asc2Int(strTmp) <= 0)
		memcpy(ICFITContent_POST[0], "AAAAAA", 6);			// End Flag
	else
	{
		CString strFITtemp;
		int iFITNum = 0;
		
		for (int i = 1; i <= Asc2Int(strTmp); i++)			// 注意卡表记录序号是否为1开始   by zjd
		{
			strFITtemp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ICFITPARM, GetSprintf("%s%d","FITT",i), "");
			if (strFITtemp != "")
			{
				memcpy(ICFITContent_POST[iFITNum], strFITtemp.GetBuffer(0), strlen(strFITtemp));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetICFitFileContent()ICFITContent_POST[%d][%s]", iFITNum, ICFITContent_POST[iFITNum]);
				iFITNum ++;
			}
		}
		memcpy(ICFITContent_POST[iFITNum], "AAAAAA", 6);			// End Flag
	}
	return T_OK;
}

// Get IC Fit File Content Save
int CTranCmn::fnAPL_GetAIDListFileContent(PBCst_APPLIST *PbcAppList, int *len)
{
	CStringArray	strArrayData;
	BYTE			bTemp[256];
	int				nlen = 0;
	CString strTmp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_AIDLISTPARM, "AIDNUM", "");
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetAIDListFileContent()AIDNUM[%d]", Asc2Int(strTmp));
	*len = Asc2Int(strTmp);
	for (int i = 0; i < *len; i++)
	{
		memset(bTemp, 0, sizeof(bTemp));
		strTmp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_AIDLISTPARM, GetSprintf("AID%d", i+1), "");
		SplitString(strTmp, ",", strArrayData);
		memcpy(&PbcAppList[i], PbcAppTable, sizeof(PbcAppTable));
		nlen = MakePack(strArrayData[0].GetBuffer(0), bTemp, strArrayData[0].GetLength());
		memcpy(PbcAppList[i].AID, bTemp, nlen);
		PbcAppList[i].AidLen = nlen;
		if (memcmp(strArrayData[1].GetBuffer(0), "1", 1) == 0)
			PbcAppList[i].SelFlag = 0x01;
		else
			PbcAppList[i].SelFlag = 0x00;

		memset(bTemp, 0, sizeof(bTemp));						// added by liuxl 20110523
		nlen = MakePack(strArrayData[2].GetBuffer(0), bTemp, strArrayData[2].GetLength());
		memcpy(PbcAppList[i].Version, bTemp, nlen);

		memset(bTemp, 0, sizeof(bTemp));
		nlen = MakePack(tacDenial.GetBuffer(0), bTemp, tacDenial.GetLength());
		memcpy(PbcAppList[i].TACDenial, bTemp, nlen);

		memset(bTemp, 0, sizeof(bTemp));
		nlen = MakePack(tacOnline.GetBuffer(0), bTemp, tacOnline.GetLength());
		memcpy(PbcAppList[i].TACOnline, bTemp, nlen);

		memset(bTemp, 0, sizeof(bTemp));
		nlen = MakePack(tacDefault.GetBuffer(0), bTemp, tacDefault.GetLength());
		memcpy(PbcAppList[i].TACDefault, bTemp, nlen);
	}
	return T_OK;
}

// Get Fit File Content Save
int CTranCmn::fnAPL_GetEchannelFileContent()
{
	memset(ECHANNELContent_POST, 0, sizeof(ECHANNELContent_POST));
	memset(ECHANNELSEL, 0, sizeof(ECHANNELSEL));
	CString strTmp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ECHANNELPARM, "ECHANNELNUM", "0");
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetEchannelFileContent()ECHANNELNUM[%d]", Asc2Int(strTmp));
	if (strTmp == "" || Asc2Int(strTmp) <= 0)
		memcpy(ECHANNELContent_POST[0], "AAAAAA", 6);			// End Flag
	else
	{
		CString strFITtemp;
		int iFITNum = 0;
		
		for (int i = 1; i <= Asc2Int(strTmp); i++)			// 注意卡表记录序号是否为1开始   by zjd
		{
			strFITtemp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ECHANNELPARM, GetSprintf("%s%d","ECHANNEL",i), "");
			if (strFITtemp != "")
			{
				memcpy(ECHANNELContent_POST[iFITNum], strFITtemp.GetBuffer(0), strlen(strFITtemp));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetEchannelFileContent()ECHANNELContent_POST[%d][%s]", iFITNum, ECHANNELContent_POST[iFITNum]);
				iFITNum ++;
			}
		}
		memcpy(ECHANNELContent_POST[iFITNum], "AAAAAA", 6);			// End Flag
	}
	return T_OK;
}
BOOL CTranCmn::fnAPL_checkOBTRANINQ(CString pAccountNo)
{
	int				nFitPos = 0;
	CStringArray	strFitItemsArray;


	memset(CurrTranINQCanDO, '0', sizeof(CurrTranCanDO));

	while (nFitPos < itemof(FITContent_POST))
	{

		if (memcmp(FITContent_POST[nFitPos], "AAAAAA", 6) == 0)	// End Check
			break;

		SplitString(CString(FITContent_POST[nFitPos++]), ",", strFitItemsArray);
		if (strFitItemsArray.GetSize() != 15)					// Item Check
			continue;

		// 处理外行卡的特殊记录，一般该记录应该为最后的一条记录  by zjd
		if ((Asc2Int(strFitItemsArray[2].GetBuffer(0)) == 0) &&
			(Asc2Int(strFitItemsArray[3].GetBuffer(0)) == 0))
		{
			if ((strFitItemsArray[13].GetLength() != 0) &&
			(strFitItemsArray[14].GetLength() != 0))
			{
				memcpy(CurrTranINQCanDO, strFitItemsArray[14], __min(strFitItemsArray[14].GetLength(), 16));
			
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CurrTranINQCanDO=[%d]",CurrTranINQCanDO);
			}
			break;
		}
	}
return T_MSG;
}

BOOL CTranCmn::fnAPL_checkLocalCard(CString pAccountNo)
{
	int				nFitPos = 0;
	CStringArray	strFitItemsArray;

	while (nFitPos < itemof(FITContent_POST))
	{
		if (memcmp(FITContent_POST[nFitPos], "AAAAAA", 6) == 0)	// End Check
			break;

		SplitString(CString(FITContent_POST[nFitPos++]), ",", strFitItemsArray);
		if (strFitItemsArray.GetSize() != 15)					// Item Check
			continue;

		if(strFitItemsArray[13].GetLength() != 0 && strFitItemsArray[13] != "3")
		{
			int len = Asc2Int(strFitItemsArray[3].GetBuffer(0));
			if(memcmp(strFitItemsArray[4], pAccountNo.Mid(0, len), len) == 0)
				return TRUE;
		}
		else
		if(pAccountNo[0] == '6' && pAccountNo[1] == '0')
		{
			return TRUE;
		}
	}

	nFitPos = 0;

	while (nFitPos < itemof(ICFITContent_POST))
	{
		if (memcmp(ICFITContent_POST[nFitPos], "AAAAAA", 6) == 0)	// End Check
			break;
		
		SplitString(CString(ICFITContent_POST[nFitPos++]), ",", strFitItemsArray);
		if (strFitItemsArray.GetSize() != 15)					// Item Check
			continue;
		
		if(strFitItemsArray[13].GetLength() != 0 && strFitItemsArray[13] != "3")
		{
			int len = Asc2Int(strFitItemsArray[3].GetBuffer(0));
			if(memcmp(strFitItemsArray[4], pAccountNo.Mid(0, len), len) == 0)
				return TRUE;
		}
		else
		if(pAccountNo[0] == '6' && pAccountNo[1] == '0')
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

int CTranCmn::fnAPL_GetICCardAvailData(char* pTrack2, char* pAccountNo, int* pCardType)
{
	int				nFitPos = 0;
	char*			szTrack = NULL;
	CStringArray	strFitItemsArray;

	memset(pAccountNo, 0, 20);									// Account No Initialize
	*pCardType = 0;												// Card Type Initialize
	memset(CurrTranCanDO, '0', sizeof(CurrTranCanDO));
	memset(TempTranCanDO, '0', sizeof(TempTranCanDO));
	LocalCardFlag = 3;

	char icdata[256];
	memset(icdata, 0, sizeof(icdata));
	int len = 0;
	m_pDevCmn->fnPBC_TlvGetValue(0x5A, (BYTE*)icdata, &len);

	while (nFitPos < itemof(ICFITContent_POST))
	{
		if (memcmp(ICFITContent_POST[nFitPos], "AAAAAA", 6) == 0)	// End Check
			break;

		SplitString(CString(ICFITContent_POST[nFitPos++]), ",", strFitItemsArray);
		if (strFitItemsArray.GetSize() != 15)					// Item Check
			continue;

		// 处理外行卡的特殊记录，一般该记录应该为最后的一条记录  by zjd
		if ((Asc2Int(strFitItemsArray[2].GetBuffer(0)) == 0) &&
			(Asc2Int(strFitItemsArray[3].GetBuffer(0)) == 0))
		{
			if ((strFitItemsArray[13].GetLength() != 0) &&
				(strFitItemsArray[14].GetLength() != 0))
			{
				memcpy(pAccountNo, MakeUnPack(icdata, len).GetBuffer(0), len*2);
				POSTICType = Asc2Int(strFitItemsArray[1].GetBuffer(0), 1);
				CurrTypes = Asc2Int(strFitItemsArray[12].GetBuffer(0), 2);
				memcpy(CurrTranCanDO, strFitItemsArray[14], __min(strFitItemsArray[14].GetLength(), 16));
				memcpy(TempTranCanDO, strFitItemsArray[14], __min(strFitItemsArray[14].GetLength(), 16));
				LocalCardFlag = Asc2Int(strFitItemsArray[13].GetBuffer(0), 1);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetICCardAvailData():LocalCardFlag=[%d]",LocalCardFlag);
			}
			break;
		}

		if ((strFitItemsArray[2].GetLength() != 0)				&&
			(Asc2Int(strFitItemsArray[3].GetBuffer(0), 2) != 0) &&
			(strFitItemsArray[4].GetLength() != 0))
		{
			szTrack = MakeUnPack(icdata, len).GetBuffer(0);
			if ((int)strlen(szTrack) < (Asc2Int(strFitItemsArray[2].GetBuffer(0)) + strFitItemsArray[4].GetLength()))
				continue;
			if (memcmp(szTrack + Asc2Int(strFitItemsArray[2].GetBuffer(0)), strFitItemsArray[4].GetBuffer(0),
				strFitItemsArray[4].GetLength()) == 0)			// CardFlag Data Check
			{
				*pCardType = 1;										// Save Card Type
			}
			else
				continue;
		}
		else
			continue;

		if ((strFitItemsArray[1].GetLength() != 0) &&			// CardNo Item Check
			(Asc2Int(strFitItemsArray[12].GetBuffer(0)) != 0) &&
			(strFitItemsArray[13].GetLength() != 0) &&
			(strFitItemsArray[14].GetLength() != 0))
		{
			memcpy(pAccountNo, MakeUnPack(icdata, len).GetBuffer(0), len*2);
			POSTICType = Asc2Int(strFitItemsArray[1].GetBuffer(0), 1);
			CurrTypes = Asc2Int(strFitItemsArray[12].GetBuffer(0), 2);
			memcpy(CurrTranCanDO, strFitItemsArray[14], __min(strFitItemsArray[14].GetLength(), 16));
			memcpy(TempTranCanDO, strFitItemsArray[14], __min(strFitItemsArray[14].GetLength(), 16));
			LocalCardFlag = Asc2Int(strFitItemsArray[13].GetBuffer(0), 1);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetICCardAvailData():CurrTranCanDO=[%s]",CurrTranCanDO);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetICCardAvailData():LocalCardFlag=[%d]",LocalCardFlag);
			return T_OK;
		}
	}

	return T_MSG;
}

// Get Card Avail Data 需要设置当前卡的可交易列表（CurrTranCanDO）和本省的标志（LocalCardFlag）  by zjd
int CTranCmn::fnAPL_GetPOSTCardAvailData(char* pTrack2, char* pTrack3, char* pAccountNo, int* pCardType)
{
	int				nFitPos  = 0;
	char*			szTrack = NULL;
	CStringArray	strFitItemsArray;
	
	memset(pAccountNo, 0, 20);									// Account No Initialize
	*pCardType = 0;												// Card Type Initialize
	memset(CurrTranCanDO, '0', sizeof(CurrTranCanDO));
	LocalCardFlag = 3;

	while (nFitPos < itemof(FITContent_POST))
	{
		if (memcmp(FITContent_POST[nFitPos], "AAAAAA", 6) == 0)	// End Check
			break;

		SplitString(CString(FITContent_POST[nFitPos++]), ",", strFitItemsArray);
		if (strFitItemsArray.GetSize() != 15)					// Item Check
			continue;

		// 处理外行卡的特殊记录，一般该记录应该为最后的一条记录  by zjd
		if ((Asc2Int(strFitItemsArray[2].GetBuffer(0)) == 0) &&
			(Asc2Int(strFitItemsArray[3].GetBuffer(0)) == 0))
		{
			if ((strFitItemsArray[13].GetLength() != 0) &&
			(strFitItemsArray[14].GetLength() != 0))
			{
				memcpy(CurrTranCanDO, strFitItemsArray[14], __min(strFitItemsArray[14].GetLength(), 16));
				LocalCardFlag = Asc2Int(strFitItemsArray[13].GetBuffer(0), 1);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetPOSTCardAvailData():LocalCardFlag=[%d]",LocalCardFlag);
			}
			break;
		}

		if ((strFitItemsArray[1].GetLength() != 0) &&			
			(strFitItemsArray[2].GetLength() != 0) &&			
			(Asc2Int(strFitItemsArray[3].GetBuffer(0), 2) != 0) &&
			(strFitItemsArray[4].GetLength() != 0))
		{
			if (strFitItemsArray[1] == "2")						// 2Tr Check
				szTrack = pTrack2;
			else
			if (strFitItemsArray[1] == "3")						// 3Tr Check
				szTrack = pTrack3;
			else
				continue;

			if ((int)strlen(szTrack) < (Asc2Int(strFitItemsArray[2].GetBuffer(0)) + strFitItemsArray[4].GetLength()))
				continue;										// CardFlag Size Check
			if (memcmp(szTrack + Asc2Int(strFitItemsArray[2].GetBuffer(0)), strFitItemsArray[4].GetBuffer(0),
				strFitItemsArray[4].GetLength()) == 0)			// CardFlag Data Check
			{
				if ((strFitItemsArray[1].GetLength() != 0) &&			
					(strFitItemsArray[5].GetLength() != 0) &&			
					(Asc2Int(strFitItemsArray[6].GetBuffer(0), 2) != 0) &&
					(strFitItemsArray[7].GetLength() != 0))
				{
					if (strFitItemsArray[1] == "2")						// 2Tr Check
						szTrack = pTrack2;
					else
					if (strFitItemsArray[1] == "3")						// 3Tr Check
						szTrack = pTrack3;
					else
						continue;

					if ((int)strlen(szTrack) < (Asc2Int(strFitItemsArray[5].GetBuffer(0)) + strFitItemsArray[7].GetLength()))
						continue;										// CardFlag Size Check
					if (memcmp(szTrack + Asc2Int(strFitItemsArray[5].GetBuffer(0)), strFitItemsArray[7].GetBuffer(0),
						strFitItemsArray[7].GetLength()) == 0)			// CardFlag Data Check
					{
						if ((strFitItemsArray[1].GetLength() != 0) &&			
							(strFitItemsArray[8].GetLength() != 0) &&			
							(Asc2Int(strFitItemsArray[9].GetBuffer(0), 2) != 0) &&
							(strFitItemsArray[10].GetLength() != 0))
						{
							if (strFitItemsArray[1] == "2")						// 2Tr Check
								szTrack = pTrack2;
							else
							if (strFitItemsArray[1] == "3")						// 3Tr Check
								szTrack = pTrack3;
							else
								continue;

							if ((int)strlen(szTrack) < (Asc2Int(strFitItemsArray[8].GetBuffer(0)) + strFitItemsArray[10].GetLength()))
								continue;										// CardFlag Size Check
							if (memcmp(szTrack + Asc2Int(strFitItemsArray[8].GetBuffer(0)), strFitItemsArray[10].GetBuffer(0),
								strFitItemsArray[10].GetLength()) == 0)			// CardFlag Data Check
								*pCardType = 1;										// Save Card Type
							else
								continue;
						}
						else
							*pCardType = 1;										// Save Card Type
					}
					else
						continue;
				}
				else
					*pCardType = 1;										// Save Card Type
			}
			else
				continue;
		}
		else
			continue;

		if ((strFitItemsArray[1].GetLength() != 0) &&			// CardNo Item Check
			(strFitItemsArray[11].GetLength() != 0) &&
			(Asc2Int(strFitItemsArray[12].GetBuffer(0)) != 0) &&
			(strFitItemsArray[13].GetLength() != 0) &&
			(strFitItemsArray[14].GetLength() != 0))
		{
			if (strFitItemsArray[1] == "2")						// 2Tr Check
			{
				if ((int)strlen(pTrack2) >= (Asc2Int(strFitItemsArray[11].GetBuffer(0)) + strFitItemsArray[12].GetLength()))
				{	
					memcpy(pAccountNo, (pTrack2 + Asc2Int(strFitItemsArray[11].GetBuffer(0))), 
							__min(Asc2Int(strFitItemsArray[12].GetBuffer(0)), 19));
					memcpy(CurrTranCanDO, strFitItemsArray[14], __min(strFitItemsArray[14].GetLength(), 16));
					LocalCardFlag = Asc2Int(strFitItemsArray[13].GetBuffer(0), 1);
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetPOSTCardAvailData():LocalCardFlag=[%d]",LocalCardFlag);
					return T_OK;
				}
			}
			else
			if (strFitItemsArray[1] == "3")						// 3Tr Check
			{
				if ((int)strlen(pTrack3) >= (Asc2Int(strFitItemsArray[11].GetBuffer(0)) + strFitItemsArray[12].GetLength()))
				{	
					memcpy(pAccountNo, (pTrack3 + Asc2Int(strFitItemsArray[11].GetBuffer(0))), 
							__min(Asc2Int(strFitItemsArray[12].GetBuffer(0)), 19));
					memcpy(CurrTranCanDO, strFitItemsArray[14], __min(strFitItemsArray[14].GetLength(), 16));
					LocalCardFlag = Asc2Int(strFitItemsArray[13].GetBuffer(0), 1);
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetPOSTCardAvailData():LocalCardFlag=[%d]",LocalCardFlag);
					return T_OK;
				}
			}
		}
	}
	
	return T_MSG;
}

int CTranCmn::fnAPL_GetCardAvailData(char* pTrack2, char* pTrack3, char* pAccountNo, int* pCardType)
{
	int				nFitPos  = 0;
	int             nBmemcmp = 0, nEmemcmp = 0;
	char*			szTrack = NULL;
	CStringArray	strFitItemsArray;
	
	memset(pAccountNo, 0, 20);									// Account No Initialize
	*pCardType = 0;												// Card Type Initialize

	while (nFitPos < itemof(FitFileContent))
	{
		if (memcmp(FitFileContent[nFitPos], "AAAAAA", 6) == 0)	// End Check
			break;

		SplitString(CString(FitFileContent[nFitPos++]), ",", strFitItemsArray);
		if (strFitItemsArray.GetSize() != 13)					// Item Check
			continue;
		

		if ((strFitItemsArray[0].GetLength() == 4) &&			// CardType Item Check
			(strFitItemsArray[4].GetLength() != 0) &&			// CardFlag Item Check
			(strFitItemsArray[5].GetLength() != 0) &&
			(strFitItemsArray[6].GetLength() != 0))
		{
			if (strFitItemsArray[4] == "2")						// 2Tr Check
				szTrack = pTrack2;
			else
			if (strFitItemsArray[4] == "3")						// 3Tr Check
				szTrack = pTrack3;
			else
				continue;

			if ((int)strlen(szTrack) < (Asc2Int(strFitItemsArray[6].GetBuffer(0)) + strFitItemsArray[5].GetLength()))
				continue;										// CardFlag Size Check
			if (memcmp(szTrack + Asc2Int(strFitItemsArray[6].GetBuffer(0)), strFitItemsArray[5].GetBuffer(0),
				strFitItemsArray[5].GetLength()) != 0)			// CardFlag Data Check
				continue;

			*pCardType = Asc2Int(&strFitItemsArray[0].GetBuffer(0)[1], 1);
																// Save Card Type
		}
		else
			continue;


		if ((strFitItemsArray[4].GetLength() != 0) &&			// CardFlag Item Check
			(strFitItemsArray[7].GetLength() != 0) &&			// LocalFlag Item Check
			(strFitItemsArray[8].GetLength() != 0) &&
			(strFitItemsArray[9].GetLength() != 0) &&
			(strFitItemsArray[10].GetLength() != 0))
		{
			if (strFitItemsArray[4] == "2")						// 2Tr Check
				szTrack = pTrack2;
			else
			if (strFitItemsArray[4] == "3")						// 3Tr Check
				szTrack = pTrack3;
			else
				continue;

			if (((int)strlen(szTrack) < (Asc2Int(strFitItemsArray[7].GetBuffer(0)) + strFitItemsArray[8].GetLength())) || 
				((int)strlen(szTrack) < (Asc2Int(strFitItemsArray[7].GetBuffer(0)) + strFitItemsArray[9].GetLength())))
				continue;										// LocalFlag Size Check

			nBmemcmp = memcmp(szTrack + Asc2Int(strFitItemsArray[7].GetBuffer(0)), strFitItemsArray[8].GetBuffer(0), strFitItemsArray[8].GetLength());
			nEmemcmp = memcmp(szTrack + Asc2Int(strFitItemsArray[7].GetBuffer(0)), strFitItemsArray[9].GetBuffer(0), strFitItemsArray[9].GetLength());
			if ((nBmemcmp < 0) || (nEmemcmp > 0))					// LocalFlag Begin End Check
				continue;
		}


		if ((strFitItemsArray[1].GetLength() != 0) &&			// CardNo Item Check
			(strFitItemsArray[2].GetLength() != 0) &&
			(strFitItemsArray[3].GetLength() != 0))
		{
			if (strFitItemsArray[1] == "2")						// 2Tr Check
			{
				if ((int)strlen(pTrack2) >= (Asc2Int(strFitItemsArray[2].GetBuffer(0)) + strFitItemsArray[3].GetLength()))
				{	
					memcpy(pAccountNo, (pTrack2 + Asc2Int(strFitItemsArray[2].GetBuffer(0))), 
							__min(Asc2Int(strFitItemsArray[3].GetBuffer(0)), 19));
					return T_OK;
				}
			}
			else
			if (strFitItemsArray[1] == "3")						// 3Tr Check
			{
				if ((int)strlen(pTrack3) >= (Asc2Int(strFitItemsArray[2].GetBuffer(0)) + strFitItemsArray[3].GetLength()))
				{	
					memcpy(pAccountNo, (pTrack3 + Asc2Int(strFitItemsArray[2].GetBuffer(0))), 
							__min(Asc2Int(strFitItemsArray[3].GetBuffer(0)), 19));
					return T_OK;
				}
			}
		}
	}
	
	return T_MSG;
}

// Add Batch No
int CTranCmn::fnAPL_AddBatchNo(int nBatchLength)
{
	char	szTempBatchNo[256];
	
	memset(szTempBatchNo, 0, sizeof(szTempBatchNo));
//	memcpy(szTempBatchNo, GetSprintf("%06.6s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADDCASHBATCHNO, "000000")), 6);
	memcpy(szTempBatchNo, GetSprintf("%04.4s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, "0000")), 4);

	if (memcmp(szTempBatchNo, "9999", 4) == 0)
		memset(szTempBatchNo, '0', 4);
	AddString(szTempBatchNo, "0001", 4);

//	IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADDCASHBATCHNO, GetSprintf("%6.6s", szTempBatchNo));
	IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, GetSprintf("%04.4s", szTempBatchNo));
	
	return TRUE;
}

int	CTranCmn::fnAPL_GetMACData(CString strMacData, UCHAR* szMacKey, UCHAR* szResult)
{
	char sztemp[8];
	char szdata[8];
	int  MacDataLen = strMacData.GetLength();

// 	if (iDEBUGFLAG)
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetMACData strMacData[%d][%s]", MacDataLen, strMacData);

	memset(szdata, 0, sizeof(szdata));
	for (int i = 0; i < (MacDataLen + 7) / 8; i++)
	{
		memset(sztemp, 0, sizeof(sztemp));
		if ((i + 1) * 8 <= MacDataLen)
			memcpy(sztemp, &(strMacData.GetBuffer(0)[i * 8]), 8);
		else
			memcpy(sztemp, &(strMacData.GetBuffer(0)[i * 8]), MacDataLen % 8);
		for (int j = 0; j < 8; j++)
			szdata[j] ^= sztemp[j];
//		if (iDEBUGFLAG)
//		{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetMACData sztemp    [%s]", MakeUnPack(sztemp, 8));
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetMACData szdata  in[%s]", MakeUnPack(szdata, 8));
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetMACData szMacKey  [%s]", MakeUnPack(szMacKey, 8));
// 		}
		
		DesEncryptDecrypt((UCHAR *)szMacKey, (UCHAR *)szdata, 1);

//		if (iDEBUGFLAG)
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetMACData szdata out[%s]", MakeUnPack(szdata, 8));

	}
	memcpy(szResult, szdata, 8);	
	return T_OK;
}


