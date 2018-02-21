/////////////////////////////////////////////////////////////////////////////
// SnaCtrl.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\SnaCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSnaCtrl
/////////////////////////////////////////////////////////////////////////////
CSnaCtrl::CSnaCtrl(NETWORKINFO NETWORK)
{
	m_ConfBuff		= NETWORK;
	
	m_FirstFlag		= TRUE;										// 2004.07.20
	m_Instance		= AfxGetInstanceHandle();					// 2004.07.17 : Get Instance Handle
	m_nLuNo			= 1;										// 2004.07.17 : Default
	memset(&m_OIAInfo, 0, sizeof(m_OIAInfo));					// 2004.07.17 : Default Initialize

	InitSna();													// Variable Initialize
}

CSnaCtrl::~CSnaCtrl()
{
	int		nReturn = FALSE;
	
	if (m_FirstFlag != TRUE)									// 2004.07.20
	{
		nReturn = SysIsEmuConnect();							// Check is Emulation connect to HOST
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysIsEmuConnect():nReturn[%s]", GetSnaReturnMsg(nReturn));
		if (nReturn == TRUE)
		{
			nReturn = SysEmuDisConnect();						// Request 3270 Emulation Program disconnect host
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysEmuDisConnect():nReturn[%s]", GetSnaReturnMsg(nReturn));
		}

		nReturn = SysIsEmuExist();								// Check is Emulation Program existed
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysIsEmuExist():nReturn[%s]", GetSnaReturnMsg(nReturn));
		if (nReturn == TRUE)
		{
			nReturn = SysEndEmuModule();						// Terminate 3270 emulation program
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysEndEmuModule():nReturn[%s]", GetSnaReturnMsg(nReturn));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSnaCtrl member functions
/////////////////////////////////////////////////////////////////////////////
int CSnaCtrl::InitSna()
{
	SNAConnected = FALSE;										// 2004.07.20
	SNAEnabled = FALSE;

	memset(&SNASendBuffer, 0, sizeof(SNASendBuffer));			// Sna Buffer
	memset(&SNARecvBuffer, 0, sizeof(SNARecvBuffer));

	DATASendLength = 0;											// Tran Buffer
	DATARecvLength = 0;
	memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
	memset(DATARecvBuffer, 0, sizeof(DATARecvBuffer));

	pDATASendBuffer = (SNAHeader *)DATASendBuffer;
	pDATARecvBuffer = (SNAHeader *)DATARecvBuffer;
	return TRUE;
}

int CSnaCtrl::OpenSna()
{
	int		nReturn = FALSE;
	int		i = 0;

/////////////////////////////////////////////////////////////////////////////
	if (m_FirstFlag)											// 2004.07.20
	{
		nReturn = SysIsEmuExist();								// Check Is Emulation Program existed
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysIsEmuExist():nReturn[%s]", GetSnaReturnMsg(nReturn));
		if (nReturn != TRUE)										
		{
			nReturn = SysStartEmuModule(SW_HIDE);				// Execute 3270 Emulation program
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysStartEmuModule():nReturn[%s]", GetSnaReturnMsg(nReturn));
			if (nReturn != TRUE)
				return FALSE;
		}

		nReturn = SysIsEmuConnect();							// Check is Emulation connect to HOST
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysIsEmuConnect():nReturn[%s]", GetSnaReturnMsg(nReturn));
		if (nReturn != TRUE)										
		{
			for (i = 0; i < 5; i++)
			{
				Delay_Msg(500);									// Performance Adjust 2004.07.19
				nReturn = SysEmuConnect();						// Request 3270 Emulation Program connect to host
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysEmuConnect():nReturn[%s]", GetSnaReturnMsg(nReturn));
				if (nReturn == TRUE)
					break;
			}
			if (nReturn != TRUE)
				return FALSE;
		}

		nReturn = SysIsLuCorrect(m_nLuNo);						// Check is Lu No correct
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysIsLuCorrect():nReturn[%s]", GetSnaReturnMsg(nReturn));
		if (nReturn != TRUE)
			return FALSE;
		
		m_FirstFlag = FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	nReturn = SysApLinkLu(m_Instance, m_nLuNo);					// Application build connection to a specified Lu
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysApLinkLu():nReturn[%s]", GetSnaReturnMsg(nReturn));
	if (nReturn != TRUE)
		return FALSE;

	return TRUE;
}

int CSnaCtrl::CloseSna()
{
	int		nReturn = FALSE;

	nReturn = SysApUnlinkLu(m_Instance, m_nLuNo);				// Application unlink connection to a specified Lu
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysApUnlinkLu():nReturn[%s]", GetSnaReturnMsg(nReturn));
	if (nReturn != TRUE)
	{
		nReturn = SysForceApUnlinkLu(m_nLuNo);					// Force Application unlink connection to a specified Lu
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysForceApUnlinkLu():nReturn[%s]", GetSnaReturnMsg(nReturn));
	}
	
	SNAConnected = FALSE;										// 2004.07.20
	SNAEnabled = FALSE;
	return TRUE;
}

int CSnaCtrl::SendSna(BYTE *SendBuff, int Length, int SendSec)
{
	int		nReturn = FALSE;

	DATASendLength = 0;											// SEND LENGTH CLEAR
	memset(DATASendBuffer, 0, SNABUFFSIZE);						// SEND BUFFER CLEAR
	
	pDATASendBuffer->Data[0] = 0x02;							// STX
	pDATASendBuffer->Data[1] = 0x00;							// TA1
	pDATASendBuffer->Data[2] = 0x00;							// TA2
	memcpy(&pDATASendBuffer->Data[3], SendBuff, Length);
	SysTransAsc2Ebc(&pDATASendBuffer->Data[3], Length);			// transfer a ASCII string to a EBCDIC string
	pDATASendBuffer->Data[3 + Length] = 0x03;					// ETX
	DATASendLength = Length + 4;								// DATA + 4

	memset(&SNASendBuffer, 0, sizeof(SNASendBuffer));
	SNASendBuffer.wTimeOut = SendSec * 10;						// 100 milisecond
	SNASendBuffer.wSendLen = DATASendLength;
	SNASendBuffer.lpData = (LPSTR)pDATASendBuffer;
	nReturn = SysSendTransparent(m_Instance, m_nLuNo, &SNASendBuffer);
																// send data to HOST in the mode of Transparent
	if (nReturn != TRUE)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysSendTransparent():nReturn[%s]", GetSnaReturnMsg(nReturn));
		return FALSE;
	}
	
	return TRUE;
}

int CSnaCtrl::RecvSna(BYTE *RecvBuff, int *Length, int RecvSec)
{
	int		nReturn = FALSE;

	DATARecvLength = 0;											// RECV LENGTH CLEAR
	memset(DATARecvBuffer, 0, SNABUFFSIZE);						// RECV BUFFER CLEAR

	memset(&SNARecvBuffer, 0, sizeof(SNARecvBuffer));
	SNARecvBuffer.wTimeOut = RecvSec * 10;						// 100 milisecond
	SNARecvBuffer.wMaxSize = SNABUFFSIZE;
	SNARecvBuffer.lpData = (LPSTR)pDATARecvBuffer;
	nReturn = SysReceiveTransparent(m_nLuNo, &SNARecvBuffer);
																// receive data from HOST in the mode of Transparent
	if (nReturn != TRUE)
	{
		if (RecvSec)
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysReceiveTransparent():nReturn[%s]", GetSnaReturnMsg(nReturn));
		}
		*Length = 0;
		return FALSE;
	}

	if (SNARecvBuffer.wRcvLen < 3)								// STX ST1 DATA
	{
		if (RecvSec)
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysReceiveTransparent():nReturn[%s]", GetSnaReturnMsg(nReturn));
		}
		*Length = 0;
		return FALSE;
	}

	DATARecvLength = SNARecvBuffer.wRcvLen;						// STX ST1 DATA 
	SysTransEbc2Asc(&pDATARecvBuffer->Data[2], SNARecvBuffer.wRcvLen - 2);
																// transfer a EBCDIC string to a ASCII string
	memcpy(RecvBuff, &pDATARecvBuffer->Data[2], SNARecvBuffer.wRcvLen - 2);
	*Length = SNARecvBuffer.wRcvLen - 2;
	if ((*Length) <= 0)
	{
		if (RecvSec)
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysReceiveTransparent():nReturn[%s]", GetSnaReturnMsg(nReturn));
		}
		*Length = 0;
		return FALSE;
	}

	return TRUE;
}

int CSnaCtrl::GetStatusSna() 
{
	int		nReturn = FALSE;

	memset(&m_OIAInfo, 0, sizeof(m_OIAInfo));					// 2004.07.17
	nReturn = SysGetOIAInfo(m_nLuNo, &m_OIAInfo);				// Read OIA information
	if (m_OIAInfo.ucSessBound == OIA_SESS_UNBOUND)
	{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "SysGetOIAInfo()==OIA_SESS_UNBOUND:nReturn[%s]", GetSnaReturnMsg(nReturn));
		SNAConnected = FALSE;
		SNAEnabled = FALSE;
		return FALSE;
	}

	if (SNAEnabled != TRUE)										// 2004.07.20
	{
		memset(&m_OIAInfo, 0, sizeof(m_OIAInfo));				// 2004.07.17
		nReturn = SysEnableTransparent(m_Instance, m_nLuNo, TRUE, SNABUFFSIZE, &m_OIAInfo);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SysEnableTransparent():nReturn[%s]", GetSnaReturnMsg(nReturn));
		if (nReturn != TRUE)
		{
			SNAConnected = FALSE;
			SNAEnabled = FALSE;
			return FALSE;
		}
	}

	SNAConnected = TRUE;										// 2004.07.20
	SNAEnabled = TRUE;
	return TRUE;
}

CString CSnaCtrl::GetSnaReturnMsg(int nReturn)
{
	CString	strReturnMsg("");

	switch	(nReturn)
	{
		case 1:
			strReturnMsg = "SUCCEFUL";
			break;

		case API_SYSTEM_ERROR:
			strReturnMsg = "API_SYSTEM_ERROR";
			break;
		case API_EMU_NOTEXIST:
			strReturnMsg = "API_EMU_NOTEXIST";
			break;
		case API_EMU_DISCONNECT:
			strReturnMsg = "API_EMU_DISCONNECT";
			break;
		case API_CONNECT_FAILURE:
			strReturnMsg = "API_CONNECT_FAILURE";
			break;
		case API_SESS_AP_UNLINK:
			strReturnMsg = "API_SESS_AP_UNLINK";
			break;
		case API_SESS_ISUSED:
			strReturnMsg = "API_SESS_ISUSED";
			break;
		case API_SESS_ISUNUSED:
			strReturnMsg = "API_SESS_ISUNUSED";
			break;
		case API_INSTANCE_ERROR:
			strReturnMsg = "API_INSTANCE_ERROR";
			break;
		case API_LUNO_ERROR:
			strReturnMsg = "API_LUNO_ERROR";
			break;
		case API_NAME_ERROR:
			strReturnMsg = "API_NAME_ERROR";
			break;
		case API_ROW_ERROR:
			strReturnMsg = "API_ROW_ERROR";
			break;
		case API_COL_ERROR:
			strReturnMsg = "API_COL_ERROR";
			break;
		case API_FILE_OPEN_ERROR:
			strReturnMsg = "API_FILE_OPEN_ERROR";
			break;
		case API_OPTION_CODE_ERROR:
			strReturnMsg = "API_OPTION_CODE_ERROR";
			break;
		case API_STATUS_ERROR:
			strReturnMsg = "API_STATUS_ERROR";
			break;
		case API_TIMEOUT:
			strReturnMsg = "API_TIMEOUT";
			break;
		case API_INTERNAL_ERROR:
			strReturnMsg = "API_INTERNAL_ERROR";
			break;
		case API_DISK_ERROR:
			strReturnMsg = "API_DISK_ERROR";
			break;
		case API_AIDKEY_ERROR:
			strReturnMsg = "API_AIDKEY_ERROR";
			break;
		case API_SEND_LENGTH_ERROR:
			strReturnMsg = "API_SEND_LENGTH_ERROR";
			break;
		case API_CMD_ERROR:
			strReturnMsg = "API_CMD_ERROR";
			break;
		case API_INP_INHIBIT:
			strReturnMsg = "API_INP_INHIBIT";
			break;
		case API_TEXT_ERROR:
			strReturnMsg = "API_TEXT_ERROR";
			break;
		case API_PRINTER_SESS:
			strReturnMsg = "API_PRINTER_SESS";
			break;
		case API_CURSOR_ERROR:
			strReturnMsg = "API_CURSOR_ERROR";
			break;
		case API_MEMORY_ERROR:
			strReturnMsg = "API_MEMORY_ERROR";
			break;
		case API_TRANS_HAD_SET_ON:
			strReturnMsg = "API_TRANS_HAD_SET_ON";
			break;
		case API_STOP_FILETRANS:
			strReturnMsg = "API_STOP_FILETRANS";
			break;
		case API_NO_FREE_SESS:
			strReturnMsg = "API_NO_FREE_SESS";
			break;
		case API_COMMUNICATION_ERROR:
			strReturnMsg = "API_COMMUNICATION_ERROR";
			break;
		case API_SESS_ISUSEDQUIT:
			strReturnMsg = "API_SESS_ISUSEDQUIT";
			break;

		case APPC_NOT_BOUND:
			strReturnMsg = "APPC_NOT_BOUND";
			break;
		case APPC_ALREADY_ALLOC:
			strReturnMsg = "APPC_ALREADY_ALLOC";
			break;
		case APPC_NOT_ALLOC:
			strReturnMsg = "APPC_NOT_ALLOC";
			break;
			
		case API_PARAM_ERROR:
			strReturnMsg = "API_PARAM_ERROR";
			break;

		default:												// OTHER
			strReturnMsg.Format("%d", nReturn);
			break;
	}

	return strReturnMsg;
}
