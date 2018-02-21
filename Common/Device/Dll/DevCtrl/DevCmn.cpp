/////////////////////////////////////////////////////////////////////////////
//	DevCmn.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include "sys/timeb.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\nhmwi.h"
#include "..\..\H\Dll\Profile.h"
#include "..\..\H\Dll\Network.h"
#include "..\..\H\Dll\ComPort.h"
#include "..\..\H\Dll\LanPort.h"
#include "..\..\H\Dll\NmsWork.h"
#include "..\..\H\Dll\EtcWork.h"

#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	CDevCmn Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
CDevCmn::CDevCmn()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CDevCmn()");

	m_pProfile	= NULL;
	m_pNetWork	= NULL;
	m_pComPort	= NULL;
	m_pLanPort	= NULL;
	m_pNmsWork	= NULL;
	m_pEtcWork	= NULL;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CDevCmn():return");
}

CDevCmn::~CDevCmn()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~CDevCmn()");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~CDevCmn():return");
}


BEGIN_MESSAGE_MAP(CDevCmn, CWnd)
	//{{AFX_MSG_MAP(CDevCmn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
//	CDevCmn message handlers
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	FUNCTION
/////////////////////////////////////////////////////////////////////////////
// Set Owner
void CDevCmn::SetOwner(CNHMWI* pMwi)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner()");

	m_pMwi		= pMwi;
	m_pProfile	= new CProfile;

	m_pProfile->InitProfileCheck();								// Profile Exist Check & Create
    m_pProfile->GetDeviceProfile();								// Profile Read 
	m_pProfile->GetNetworkProfile();
	m_pProfile->GetTransProfile();
	m_pProfile->GetWithTotal();									// Read Total				
	m_pProfile->GetInqTotal();
	m_pProfile->GetTransTotal();
	m_pProfile->GetDepTotal();

	// Define Const
	memcpy(&AtmDefine, &m_pProfile->DEVICE, sizeof(AtmDefine));
	memcpy(&NetDefine, &m_pProfile->NETWORK, sizeof(NetDefine));
	memcpy(&TranDefine, &m_pProfile->TRANS, sizeof(TranDefine));

	m_pNetWork	= new CNetWork(m_pProfile->NETWORK);
																// 2005.06.15
	m_pComPort	= new CComPort(m_pProfile->DEVICE, m_pProfile->NETWORK);
	m_pLanPort	= new CLanPort(m_pProfile->DEVICE, m_pProfile->NETWORK);
	m_pNmsWork	= new CNmsWork(m_pProfile->DEVICE, m_pProfile->NETWORK);
	m_pEtcWork	= new CEtcWork(m_pProfile->DEVICE, m_pProfile->NETWORK);
																// 2005.12.23
	fnAPL_InitializeDS();

	fnAPC_SetOwner(this);										// 2010.10.20
	emvkrnl_RegistIFMCommFunc(fnAPC_IcSendApdu);

	fnPBC_Initialize();											// Initialize Pbc Kernel : 2010.10.20
	fnAPC_PbcRegistFunc();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner():return");
	return;
}

/////////////////////////////////////////////////////////////////////////////
//	JOURNAL FUNCTION(JPR) : JPR SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnJPR_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnJPR_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	JOURNAL FUNCTION(JPR) : JPR GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrJPR_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrJPR_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_JPR))
		strReturn = m_pMwi->JprGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrJPR_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Sensor Information
CString CDevCmn::fstrJPR_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrJPR_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_JPR))
		strReturn = m_pMwi->JprGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrJPR_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnJPR_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Paper Status
int	CDevCmn::fnJPR_GetPaperStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetPaperStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprGetPaperStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetPaperStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Material Information
int	CDevCmn::fnJPR_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprGetMaterialInfo();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	JOURNAL FUNCTION(JPR) : JPR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnJPR_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_Initialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	JprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnJPR_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_Deinitialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	JprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Emboss Print
int	CDevCmn::fnJPR_EmbossPrint()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_EmbossPrint()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	JprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprEmbossPrint();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_EmbossPrint():return(%d)", nReturn);
	return nReturn;
}

// Print(Print Data)
int	CDevCmn::fnJPR_Print(LPCTSTR szPrintData)
{
	CStringArray	strPrintArray;
	BYTE			DumpData[BASEBUFFSIZE];
	int				nDumpLength = 0;
	BYTE			SaveDumpData[BASEBUFFSIZE];
	int				iPos = 0;

	SplitString(szPrintData, ",", strPrintArray);
	memset(SaveDumpData, 0, sizeof(SaveDumpData));
	for (int i = 0; i < strPrintArray.GetSize(); i++)
	{
		memset(DumpData, 0, sizeof(DumpData));
		nDumpLength = MakePack(strPrintArray[i].GetBuffer(0), DumpData, strPrintArray[i].GetLength());
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_Print(DumpData[%s], nDumpLength[%d])", DumpData, nDumpLength);
		memcpy(&SaveDumpData[iPos], DumpData, nDumpLength);
		iPos += nDumpLength;
		SaveDumpData[iPos] = 0x0d;
		SaveDumpData[iPos + 1] = 0x0a;
		iPos += 2;
		m_pProfile->GetTransProfile();						// added by yusy 08.09.09
		m_pProfile->TRANS.JPRPrintCnt += 1;
		m_pProfile->TRANS.INKPrintCnt += 1;
		m_pProfile->PutTransProfile();
	}

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	JprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprPrint(szPrintData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_Print(...):return(%d)", nReturn);
	return nReturn;
}

// Print Log(Print Data)
int	CDevCmn::fnJPR_PrintLog(LPCTSTR szPrintData)
{
	CStringArray	strPrintArray;
	BYTE			DumpData[BASEBUFFSIZE];
	int				nDumpLength = 0;

	SplitString(szPrintData, ",", strPrintArray);
	for (int i = 0; i < strPrintArray.GetSize(); i++)
	{
		memset(DumpData, 0, sizeof(DumpData));
		nDumpLength = MakePack(strPrintArray[i].GetBuffer(0), DumpData, strPrintArray[i].GetLength());
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_PrintLog(DumpData[%s], nDumpLength[%d])", DumpData, nDumpLength);
	}

	int		nReturn = FALSE;
	return nReturn;
}

// Print(Print Data To File)  记录到文件
int	CDevCmn::fnJPR_PrintFile(LPCTSTR szPrintData)
{
	CStringArray	strPrintArray;
	BYTE			DumpData[BASEBUFFSIZE];
	int				nDumpLength = 0;
	char			szTempFileName[256];
	FILE*			TempFp;
	BYTE			SaveDumpData[BASEBUFFSIZE];
	int				iPos = 0;
	
	SplitString(szPrintData, ",", strPrintArray);
	memset(SaveDumpData, 0, sizeof(SaveDumpData));
	for (int i = 0; i < strPrintArray.GetSize(); i++)
	{
		memset(DumpData, 0, sizeof(DumpData));
		nDumpLength = MakePack(strPrintArray[i].GetBuffer(0), DumpData, strPrintArray[i].GetLength());
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_PrintFile(DumpData[%s], nDumpLength[%d])", DumpData, nDumpLength);
		memcpy(&SaveDumpData[iPos], DumpData, nDumpLength);
		iPos += nDumpLength;
		SaveDumpData[iPos] = 0x0d;
		SaveDumpData[iPos + 1] = 0x0a;
		iPos += 2;
	}
	////////////////////////////记录到电子流水中 start by zjd
	memset(szTempFileName, 0, sizeof(szTempFileName));
	_mkdir(_EJR_DETAILSEND);
	sprintf(szTempFileName, "%s\\%6.6s%8.8s.LOG", _EJR_DETAILSEND, 
		&(GetDate().GetBuffer(0)[2]),
		m_pProfile->NETWORK.AtmSerialNum);
// 2007.06.08
    if( ( TempFp = fopen( szTempFileName, "a+b" ) ) == NULL )
        TempFp = fopen( szTempFileName, "wb" );
	if (TempFp)
	{
		fwrite(SaveDumpData, iPos, 1, TempFp);
		fflush(TempFp);
		fclose(TempFp);
	}


   if(bAgentMode == 1)
   {
///////////////////////////记录到电子流水中 end
	   // 2008.03.13 by hyl
		CString szAgentJnlPath("");
		CString szAgentJnlTempPath("");
		CString szAgentTempPathName("");
		szAgentJnlPath = IniGetStr(_AGENT_ATMS,"COMMON","JRNLDIR","D:\\ATMS");
	//	szAgentJnlTempPath = IniGetStr(_AGENT_ATMS,"COMMON","JRNLDIR","D:\\ATMS\\TEMP");
		_mkdir(szAgentJnlPath);
		_mkdir(_AGENT_JNL_TEMPPATH);

		szAgentTempPathName.Format("%s\\%s", _AGENT_JNL_TEMPPATH,"Temp.jnl");

		if( ( TempFp = fopen( szAgentTempPathName, "a+b" ) ) == NULL )
			TempFp = fopen( szAgentTempPathName, "wb" );
		if (TempFp)
		{
			fwrite(SaveDumpData, iPos, 1, TempFp);
			fflush(TempFp);
			fclose(TempFp);
		}
///////////////////////////记录到AGENT电子流水中 end
   }


//	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_PrintFile(...):return()");
	return TRUE;
}

/*
int	CDevCmn::fnAPL_AgentCreateFile()
{ 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AgentCreateFile()");	  
    CString szAgentJnlPath("");
	CString szAgentTempFilePath("");
	FILE*		TempFp;
	struct _timeb AgentTime;
     _ftime(&AgentTime); 

	iFileNumber = 0;

    szAgentCreateFileTime.Format("%8.8s%6.6s%3.3s",GetDate().GetBuffer(0),GetTime().GetBuffer(0),GetSprintf("%03d",(int)AgentTime.millitm));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szAgentCreateFileTime[%s]",szAgentCreateFileTime);

	szAgentJnlPath = IniGetStr(_AGENT_ATMS,"COMMON","JRNLDIR","D:\\ATMS");
	szAgentTempFilePath = GetSprintf("%s\\%3.3s\\%8.8s\\%6.6s",szAgentJnlPath,
		"JNL",
		szAgentCreateFileTime.GetBuffer(0),
		"JNLDAT");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szAgentTempFilePath[%s]",szAgentTempFilePath);
	
    _mkdir(szAgentJnlPath);
	_mkdir(GetSprintf("%s\\%3.3s",szAgentJnlPath,"JNL"));
	_mkdir(GetSprintf("%s\\%3.3s\\%8.8s",szAgentJnlPath,"JNL",szAgentCreateFileTime.GetBuffer(0)));
	_mkdir(szAgentTempFilePath);

	while(iFileNumber <= 9)
	{
		szAgentCreateFilePathName.Format("%s\\%17.17s%d%s", szAgentTempFilePath, 
		szAgentCreateFileTime.GetBuffer(0),
		iFileNumber,
		".jnl");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szAgentCreateFilePathName[%s]",szAgentCreateFilePathName);

		if( ( TempFp = fopen( szAgentCreateFilePathName, "r" ) ) == NULL )
		{
             TempFp = fopen( szAgentCreateFilePathName, "wb" );
			 fclose(TempFp);
			 break;
		}

		if (TempFp)
		{
		     iFileNumber++;
			 continue;
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AgentCreateFile()return[%s]",szAgentCreateFilePathName);
//     return szAgentCreateFilePathName;
return TRUE;

}*/


// Print(Print Data To File)  记录到IDX文件
int	CDevCmn::fnJPR_AgentIdxPrintFile(LPCTSTR szPrintData)
{
	CStringArray	strPrintArray;
	SHFILEOPSTRUCT  FileOp;

	int				iPos = 0;
	BYTE			DumpData[BASEBUFFSIZE];
	BYTE			SaveDumpData[BASEBUFFSIZE];
	char            StrTo[256];
	char            StrFrom[256];

	CString         szAgentJnlPath("");
	CString         szAgentJnlTempPath("");
	CString         szAgentTempPathName("");
	CString         szAgentChangeName("");
	CString         szAgentTempFilePath("");
	CString         szAgentIdxFileName("");
	CString         szAgentIdxFilePath("");
	CString         szPrintIdxData("");
	FILE*	    	TempFp;
	FILE*	    	TempFp1;
	struct _timeb   AgentTime;

     _ftime(&AgentTime); 
	iFileNumber = 0;
	
	FileOp.wFunc = FO_MOVE;
	FileOp.fFlags= FOF_SILENT|FOF_NOCONFIRMATION|FOF_SILENT;
	
    szAgentCreateFileTime.Format("%8.8s%6.6s%3.3s",GetDate().GetBuffer(0),GetTime().GetBuffer(0),GetSprintf("%03d",(int)AgentTime.millitm));
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szAgentCreateFileTime[%s]",szAgentCreateFileTime);

    /////////////////////////生成目标文件路径和目标文件名
	szAgentJnlPath = IniGetStr(_AGENT_ATMS,"COMMON","JRNLDIR","D:\\ATMS");
	szAgentTempFilePath = GetSprintf("%s\\%3.3s\\%8.8s\\%6.6s",szAgentJnlPath,
							"JNL",
							szAgentCreateFileTime.GetBuffer(0),
							"JNLDAT");	
    _mkdir(szAgentJnlPath);
	_mkdir(GetSprintf("%s\\%3.3s",szAgentJnlPath,"JNL"));
	_mkdir(GetSprintf("%s\\%3.3s\\%8.8s",szAgentJnlPath,"JNL",szAgentCreateFileTime.GetBuffer(0)));
	_mkdir(szAgentTempFilePath);

	while(iFileNumber <= 9)
	{
		szAgentCreateFilePathName.Format("%s\\%17.17s%1.1d%s", szAgentTempFilePath, 
		szAgentCreateFileTime.GetBuffer(0),
		iFileNumber,
		".jnl");

		if (fopen( szAgentCreateFilePathName, "r" ))
		{
		     iFileNumber++;
			 continue;
		}
		else
			break;
	}
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_AgentIdxPrintFile()::CreateFilePathName[%s]",szAgentCreateFilePathName);

    ////////////////////////////源文件
	szAgentTempPathName.Format("%s\\%s", _AGENT_JNL_TEMPPATH,"Temp.jnl");
	szAgentChangeName.Format("%s\\%17.17s%1.1d%s",_AGENT_JNL_TEMPPATH,
		                      szAgentCreateFileTime.GetBuffer(0),
					          iFileNumber,
						      ".jnl");
	
	if((TempFp1 = fopen( szAgentTempPathName, "rb")) == NULL )
        TempFp1 = fopen( szAgentTempPathName, "wb" );
	if(TempFp1)
		 fclose(TempFp1);

	rename(szAgentTempPathName,szAgentChangeName);        //更改文件名
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Change File Name Success[%s] to [%s]",szAgentTempPathName,szAgentChangeName);	
    /////////////////////////////移动文件
    strcpy(StrFrom,GetSprintf("%s",szAgentChangeName));
	strcpy(StrTo,GetSprintf("%s",szAgentTempFilePath));
	FileOp.pFrom = StrFrom;
	FileOp.pTo = StrTo;	
	if(!SHFileOperation(&FileOp))
	{	
		memset(SaveDumpData, 0, sizeof(SaveDumpData));
		memset(DumpData, 0, sizeof(DumpData));	

		szPrintIdxData.Format("%17.17s%1.1d%s",szAgentCreateFileTime.GetBuffer(0),
							iFileNumber,
							&szPrintData[0]);
		memcpy(&(szPrintIdxData.GetBuffer(0)[52]),GetSprintf("%03d",fnAPL_AgentLineCount()),3);
		memcpy(&SaveDumpData,"[JNLSTART]",10);
		iPos += 10;
		memcpy(&SaveDumpData[iPos], GetSprintf("%s",szPrintIdxData), szPrintIdxData.GetLength());
		iPos += szPrintIdxData.GetLength();
		memcpy(&SaveDumpData[iPos],"[JNLEND]",8);
		iPos += 8;

		SaveDumpData[iPos] = 0x0d;
		SaveDumpData[iPos + 1] = 0x0a;
		iPos += 2;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_AgentIdxPrintFile return[%s]",szPrintIdxData.GetBuffer(0));
	
     	//////////////////////////记录到IDX文件中 by hyl
		szAgentIdxFilePath.Format("%s\\%s\\%8.8s\\%s", szAgentJnlPath, 
				"JNL",
				szAgentCreateFileTime.GetBuffer(0),
				"JNLINDEX");

		_mkdir(szAgentIdxFilePath);

		szAgentIdxFileName.Format("%s\\%8.8s%s", szAgentIdxFilePath, 
			szAgentCreateFileTime.GetBuffer(0),".idx");

		if( ( TempFp = fopen( szAgentIdxFileName, "a+b" ) ) == NULL )
			TempFp = fopen( szAgentIdxFileName, "wb" );
		if (TempFp)
		{
			fwrite(SaveDumpData, iPos, 1, TempFp);
			fflush(TempFp);
			fclose(TempFp);
		}
	}
	else 
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::AGENT JRNLDIR   MOVE PATH [%s] TO [%s] FAIL!",szAgentChangeName,szAgentTempFilePath);
	}

/////////////////////////////////////////////////////////////////////////////////////////////	
	
///////////////////////////记录到电子流水中 end

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_PrintFile(...):return()");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	SLIP PRINTER FUNCTION(SPR) : SPR SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnSPR_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPR_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Clear Retract Count
int	CDevCmn::fnSPR_ClearRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_ClearRetractCnt()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprClearRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_ClearRetractCnt():return(%d)", nReturn);
	return nReturn;
}

// Set Print Image Mode
int	CDevCmn::fnSPR_SetPrintImageMode(int nPrintImageMode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_SetPrintImageMode()");

	int		nReturn = FALSE;

	switch (AtmDefine.SprHandler)								// 2004.11.05
	{
		case SPR_T_TYPE:
		case SPR_D_TYPE:
			switch (AtmDefine.MachineKind)						// 2004.11.05
			{
				case NANOCASH:
					if (nPrintImageMode)
						nReturn = RegSetStr(_REGKEY_SPRSTATUS, "EnPrintImage", "1");
					else
						nReturn = RegSetStr(_REGKEY_SPRSTATUS, "EnPrintImage", "0");
					break;
				case MCD3:
				case TTW:
				case MCD4:
				case TTW2:
				case EATMGBA1:									// 2005.11.11
				case EATMULL1:									// 2005.11.11
				case EATMTTW:									// 2005.11.11
				case TTW2E:										// 2005.11.11
				case MCD4E:										// 2005.11.11
				case MX8000T:									// 2005.11.11
				case MX8000L:									// 2005.11.11
				case MX5600T:
				case MX5600L:
					if (nPrintImageMode)
						nReturn = RegSetStr(_REGKEY_SPRSTATUS, "PrintOption", "2");
					else
						nReturn = RegSetStr(_REGKEY_SPRSTATUS, "PrintOption", "0");
					break;
			}
			break;

		case SPR_TF_TYPE:
		case SPR_DF_TYPE:
			switch (AtmDefine.MachineKind)						// 2004.11.05
			{
				case NANOCASH:
					nReturn = RegSetStr(_REGKEY_SPRSTATUS, "EnPrintImage", "0");
					break;
				case MCD3:
				case TTW:
				case MCD4:
				case TTW2:
				case EATMGBA1:									// 2005.11.11
				case EATMULL1:									// 2005.11.11
				case EATMTTW:									// 2005.11.11
				case TTW2E:										// 2005.11.11
				case MCD4E:										// 2005.11.11
				case MX8000T:									// 2005.11.11
				case MX8000L:									// 2005.11.11
				case MX5600T:
				case MX5600L:
					nReturn = RegSetStr(_REGKEY_SPRSTATUS, "PrintOption", "0");
					break;
			}
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_SetPrintImageMode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SLIP PRINTER FUNCTION(SPR) : SPR GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrSPR_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSPR_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_SPR))
		strReturn = m_pMwi->SprGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSPR_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Sensor Information
CString CDevCmn::fstrSPR_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSPR_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_SPR))
		strReturn = m_pMwi->SprGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSPR_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnSPR_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Paper Status
int	CDevCmn::fnSPR_GetPaperStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetPaperStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprGetPaperStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetPaperStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Material Information
int	CDevCmn::fnSPR_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprGetMaterialInfo();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

// Get Retract Count
CString CDevCmn::fstrSPR_GetRetractCnt()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSPR_GetRetractCnt()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_SPR))
		strReturn = m_pMwi->SprGetRetractCnt();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSPR_GetRetractCnt():return(%s)", strReturn);
}
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SLIP PRINTER FUNCTION(SPR) : SPR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnSPR_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Initialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnSPR_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Deinitialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Print(Print Data)
int	CDevCmn::fnSPR_Print(LPCTSTR szPrintData)
{
	CStringArray	strPrintArray;
	BYTE			DumpData[BASEBUFFSIZE];
	int				nDumpLength = 0;

	SplitString(szPrintData, ",", strPrintArray);
	for (int i = 0; i < strPrintArray.GetSize(); i++)
	{
		memset(DumpData, 0, sizeof(DumpData));
		nDumpLength = MakePack(strPrintArray[i].GetBuffer(0), DumpData, strPrintArray[i].GetLength());
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Print(DumpData[%s], nDumpLength[%d])", DumpData, nDumpLength);
	}

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprPrint(szPrintData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Print(...):return(%d)", nReturn);
	return nReturn;
}
// Print(Print Data To File)  记录到文件
int	CDevCmn::fnSPR_PrintFile(LPCTSTR szPrintData)
{
	CStringArray	strPrintArray;
	BYTE			DumpData[BASEBUFFSIZE];
	int				nDumpLength = 0;
	char			szTempFileName[256];
	FILE*			TempFp;
	BYTE			SaveDumpData[BASEBUFFSIZE];
	int				iPos = 0;

	SplitString(szPrintData, ",", strPrintArray);
	memset(SaveDumpData, 0, sizeof(SaveDumpData));
	for (int i = 0; i < strPrintArray.GetSize(); i++)
	{
		memset(DumpData, 0, sizeof(DumpData));
		nDumpLength = MakePack(strPrintArray[i].GetBuffer(0), DumpData, strPrintArray[i].GetLength());
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_PrintFile(DumpData[%s], nDumpLength[%d])", DumpData, nDumpLength);
		memcpy(&SaveDumpData[iPos], DumpData, nDumpLength);
		iPos += nDumpLength;
		SaveDumpData[iPos] = 0x0d;
		SaveDumpData[iPos + 1] = 0x0a;
		iPos += 2;
	}
	////////////////////////////记录到电子流水中 start by zjd
	memset(szTempFileName, 0, sizeof(szTempFileName));
	_mkdir(_EJR_DETAILSEND);
	sprintf(szTempFileName, "%s\\%6.6s%8.8s.SPR.LOG", _EJR_DETAILSEND, 
		&(GetDate().GetBuffer(0)[2]),
		m_pProfile->NETWORK.AtmSerialNum);
// 2007.06.08
    if( ( TempFp = fopen( szTempFileName, "a+b" ) ) == NULL )
        TempFp = fopen( szTempFileName, "wb" );
	if (TempFp)
	{
		fwrite(SaveDumpData, iPos, 1, TempFp);
		fflush(TempFp);
		fclose(TempFp);
	}
///////////////////////////记录到电子流水中 end

//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_PrintFile(...):return(%d)", TRUE);
	return TRUE;
}

// Print(Image No)
int	CDevCmn::fnSPR_PrintImage(LPCTSTR szImageName)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_PrintImage(szImageName[%s])", szImageName);
	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	SprRecvTime = MID_DEVRSP_TIME;
	switch (AtmDefine.MwDevice)									// 2008.03.28
	{
		case MW_KAL:
			break;
		case MW_NEXTWARE:
			if (fnAPL_GetDefineDevice(DEV_SPR))
				nReturn = m_pMwi->SprPrintImage(szImageName);
			break;
		case MW_KALWOSA:
			break;
		case MW_NEXTWOSA:
			if (fnAPL_GetDefineDevice(DEV_SPR))
				nReturn = m_pMwi->SprPrintImage(szImageName);
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_PrintImage(...):return(%d)", nReturn);
	return nReturn;
}

// Eject(Wait Time)
int	CDevCmn::fnSPR_Eject(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Eject(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprEject(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Eject(...):return(%d)", nReturn);
	return nReturn;
}

// Wait Taken
int	CDevCmn::fnSPR_WaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_WaitTaken()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_WaitTaken():return(%d)", nReturn);
	return nReturn;
}

// Cancel Wait Taken
int	CDevCmn::fnSPR_CancelWaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_CancelWaitTaken()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprCancelWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_CancelWaitTaken():return(%d)", nReturn);
	return nReturn;
}

// Retract
int	CDevCmn::fnSPR_Retract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Retract()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprRetract();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Retract():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	MAGNETIC CARD UNIT FUNCTION(MCU) : MCU SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnMCU_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnMCU_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Clear Retract Count
int	CDevCmn::fnMCU_ClearRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ClearRetractCnt()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuClearRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ClearRetractCnt():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	MAGNETIC CARD UNIT FUNCTION(MCU) : MCU GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrMCU_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_MCU))
		strReturn = m_pMwi->McuGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Sensor Information
CString CDevCmn::fstrMCU_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_MCU))
		strReturn = m_pMwi->McuGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnMCU_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnMCU_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Material Information
int	CDevCmn::fnMCU_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnMCU_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuGetMaterialInfo();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

// Get IC Chip Power Status
int	CDevCmn::fnMCU_ICGetChipPowerStatus()
{																// 2010.10.20
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICGetChipPowerStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuICGetChipPowerStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICGetChipPowerStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Retract Count
CString CDevCmn::fstrMCU_GetRetractCnt()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetRetractCnt()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_MCU))
		strReturn = m_pMwi->McuGetRetractCnt();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetRetractCnt():return(%s)", strReturn);
}
	return strReturn;
}

// Get Card Data
CString CDevCmn::fstrMCU_GetCardData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetCardData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_MCU))
		strReturn = m_pMwi->McuGetCardData();

	CStringArray	strCardDataArray;
	BYTE			DumpData1[BASEBUFFSIZE];
	BYTE			DumpData2[BASEBUFFSIZE];
	BYTE			DumpData3[BASEBUFFSIZE];
	int				nDumpLength1 = 0;
	int				nDumpLength2 = 0;
	int				nDumpLength3 = 0;

	memset(DumpData1, 0, sizeof(DumpData1));
	memset(DumpData2, 0, sizeof(DumpData2));
	memset(DumpData3, 0, sizeof(DumpData3));
	SplitString(strReturn, ",", strCardDataArray);
	if (strCardDataArray.GetSize() >= 1)
		nDumpLength1 = MakePack(strCardDataArray[0].GetBuffer(0), DumpData1, strCardDataArray[0].GetLength());
	if (strCardDataArray.GetSize() >= 2)
		nDumpLength2 = MakePack(strCardDataArray[1].GetBuffer(0), DumpData2, strCardDataArray[1].GetLength());
	if (strCardDataArray.GetSize() >= 3)
		nDumpLength3 = MakePack(strCardDataArray[2].GetBuffer(0), DumpData3, strCardDataArray[2].GetLength());

	if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "CardDataFlag", 0) == 1)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetCardData:return(DumpData[%s,%s,%s], nDumpLength[%d,%d,%d])", DumpData1, DumpData2, DumpData3, nDumpLength1, nDumpLength2, nDumpLength3);
	}
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetCardData:return(DumpData[ , , ], nDumpLength[%d,%d,%d])", nDumpLength1, nDumpLength2, nDumpLength3);
	}
	return strReturn;
}

// Get IC Data
CString	CDevCmn::fstrMCU_GetICData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetICData()");

	CString	strReturn("");

	strReturn = m_pMwi->McuGetICData();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetICData():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	MAGNETIC CARD UNIT FUNCTION(MCU) : MCU OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnMCU_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Initialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnMCU_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Deinitialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Entry Enable(Wait Time)
int	CDevCmn::fnMCU_EntryEnable(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_EntryEnable(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuEntryEnable(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_EntryEnable(...):return(%d)", nReturn);
	return nReturn;
}

// Entry Disable
int	CDevCmn::fnMCU_EntryDisable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_EntryDisable()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuEntryDisable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_EntryDisable():return(%d)", nReturn);
	return nReturn;
}

// Read
int	CDevCmn::fnMCU_Read()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Read()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuRead();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Read():return(%d)", nReturn);
	return nReturn;
}

// Eject(Wait Time)
int	CDevCmn::fnMCU_Eject(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Eject(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuEject(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Eject(...):return(%d)", nReturn);
	return nReturn;
}

// Wait Taken
int	CDevCmn::fnMCU_WaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_WaitTaken()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_WaitTaken():return(%d)", nReturn);
	return nReturn;
}

// Cancel Wait Taken
int	CDevCmn::fnMCU_CancelWaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_CancelWaitTaken()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuCancelWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_CancelWaitTaken():return(%d)", nReturn);
	return nReturn;
}

// Retract
int	CDevCmn::fnMCU_Retract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Retract()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuRetract();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Retract():return(%d)", nReturn);
	return nReturn;
}

// Write(Track Data)
int	CDevCmn::fnMCU_Write(LPCTSTR szTrackData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Write(szTrackData[%s])", szTrackData);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuWrite(szTrackData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Write(...):return(%d)", nReturn);
	return nReturn;
}

// Initialize IC Chip:Move Point
int CDevCmn::fnMCU_ICChipInitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICChipInitialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuICChipInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICChipInitialize():return(%d)", nReturn);
	return nReturn;
}

// ICEntry Enable(Wait Time)
int	CDevCmn::fnMCU_ICEntryEnable(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICEntryEnable(nWaitSec[%d])", nWaitSec);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuICEntryEnable(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICEntryEnable(...):return(%d)", nReturn);
	return nReturn;
}

// ICEntry Disable
int	CDevCmn::fnMCU_ICEntryDisable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICEntryDisable()");

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuICEntryDisable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICEntryDisable(...):return(%d)", nReturn);
	return nReturn;
}

// IC Send Data(Protocol ID, Send Data, Wait Time)
int	CDevCmn::fnMCU_ICSendData(int nProtocolId, LPCTSTR szICSendData, int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICSendData(nProtocolId[%d], szICSendData[%s], nWaitSec[%d])", nProtocolId, szICSendData, nWaitSec);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuICSendData(nProtocolId, szICSendData, nWaitSec);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICSendData(...):return(%d)", nReturn);
	return nReturn;
}

// IC Chip Power
int	CDevCmn::fnMCU_ICChipPower(int nActType)
{																// 2010.10.20
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICChipPower(nActType[%d])", nActType);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuICChipPower(nActType);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICChipPower(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	EMV IC CARD UNIT FUNCTION(MCU) : EMV OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Emv Sel Application Selection								// 2010.10.20
int	CDevCmn::fnEMV_SelApplicationSelection(BYTE btSupportPSE, EMVst_AIDLIST* stpAidList, int* npCandiDateCnt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelApplicationSelection(btSupportPSE[%x], npCandiDateCnt[%d])", btSupportPSE, *npCandiDateCnt);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelApplicationSelection(stpAidList)", stpAidList, ((stpAidList->AidCnt) * sizeof(EMVst_AIDDATA)) + sizeof(int));

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Sel_ApplicationSelection(btSupportPSE, stpAidList, npCandiDateCnt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelApplicationSelection(npCandiDateCnt[%d]):return(%d)", *npCandiDateCnt, nReturn);
	return nReturn;
}

// Emv Sel Final Application Selection
int	CDevCmn::fnEMV_SelFinalAppSelection(WORD wAidLen, BYTE* btpAidVal, BYTE* btpSW)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelFinalAppSelection(wAidLen[%d], btpSW[%x])", wAidLen, *btpSW);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelFinalAppSelection(btpAidVal)", btpAidVal, wAidLen);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Sel_FinalAppSelection(wAidLen, btpAidVal, btpSW);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelFinalAppSelection(btpSW[%x]):return(%d)", *btpSW, nReturn);
	return nReturn;
}

// Emv Sel Make Display Application List
int	CDevCmn::fnEMV_SelMakeDisplayAppList(BYTE btSupportUserConfirm, WORD wCandiDateCnt, EMVst_APPLIST* stpAppList)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelMakeDisplayAppList(btSupportUserConfirm[%x], wCandiDateCnt[%d])", btSupportUserConfirm, wCandiDateCnt);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Sel_MakeDisplayAppList(btSupportUserConfirm, wCandiDateCnt, stpAppList);

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelMakeDisplayAppList(stpAppList):return", stpAppList, (sizeof(EMVst_APPDESC) * wCandiDateCnt) + (sizeof(int) * 2));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelMakeDisplayAppList():return(%d)", nReturn);
	return nReturn;
}

// Emv Sel Remove Candidate List
int	CDevCmn::fnEMV_SelRemoveCandidateList(WORD wCandidateCnt, WORD wAidLen, BYTE* btpAidVal)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelRemoveCandidateList(wCandidateCnt[%d], wAidLen[%d])", wCandidateCnt, wAidLen);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelRemoveCandidateList(btpAidVal)", btpAidVal, wAidLen);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		emvkrnl_Sel_RemoveCandidateList(wCandidateCnt, wAidLen, btpAidVal);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelRemoveCandidateList():return()");
	return nReturn;
}

// Emv Trans Initialize Application
int	CDevCmn::fnEMV_TransInitApplication()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransInitApplication()");

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Trans_InitApplication();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransInitApplication():return(%d)", nReturn);
	return nReturn;
}

// Emv Trans Read Application Data
int	CDevCmn::fnEMV_TransReadAppData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransReadAppData()");

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Trans_ReadAppData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransReadAppData():return(%d)", nReturn);
	return nReturn;
}

// Emv Trans Offline Data Authentification
int	CDevCmn::fnEMV_TransOfflineDataAuth(EMVst_CA_PUBKEY* stpCaPubKey)
{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransOfflineDataAuth(stpCaPubKey)", stpCaPubKey, sizeof(EMVst_CA_PUBKEY));

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Trans_OfflineDataAuth(stpCaPubKey);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransOfflineDataAuth():return(%d)", nReturn);
	return nReturn;
}

// Emv Trans Process Restrict
int	CDevCmn::fnEMV_TransProcessRestrict()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransProcessRestrict()");

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Trans_ProcessRestrict();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransProcessRestrict():return(%d)", nReturn);
	return nReturn;
}

// Emv Trans Cardholder Verify
int	CDevCmn::fnEMV_TransCardholderVerify(WORD wOnlinePinLen, BYTE* btpEncipheredOnlinePin)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransCardholderVerify(wOnlinePinLen[%d])", wOnlinePinLen);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransCardholderVerify(btpEncipheredOnlinePin)", btpEncipheredOnlinePin, wOnlinePinLen);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Trans_CardholderVerify(wOnlinePinLen, btpEncipheredOnlinePin);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransCardholderVerify():return(%d)", nReturn);
	return nReturn;
}

// Emv Trans Terminal Risk Mangement
int	CDevCmn::fnEMV_TransTerminalRiskMgmt(BYTE btSupportForceOnline)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransTerminalRiskMgmt(btSupportForceOnline[%x])", btSupportForceOnline);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Trans_TerminalRiskMgmt(btSupportForceOnline);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransTerminalRiskMgmt():return(%d)", nReturn);
	return nReturn;
}

// Emv Trans Terminal Action Analysis
int	CDevCmn::fnEMV_TransTerminalActionAnalysis()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransTerminalActionAnalysis()");

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Trans_TerminalActionAnalysis();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransTerminalActionAnalysis():return(%x)", nReturn);
	return nReturn;
}

// Emv Trans Card Action Analysis
int	CDevCmn::fnEMV_TransCardActionAnalysis(BYTE* btpOnlineFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransCardActionAnalysis(btpOnlineFlag[%x])", *btpOnlineFlag);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Trans_CardActionAnalysis(btpOnlineFlag);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransCardActionAnalysis(btpOnlineFlag[%x]):return(%d)", *btpOnlineFlag, nReturn);
	return nReturn;
}

// Emv Trans Online Process
int	CDevCmn::fnEMV_TransOnlineProcess(int nOnlineMode, BYTE btAcquirerCid)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransOnlineProcess(nOnlineMode[%d], btAcquirerCid[%x])", nOnlineMode, btAcquirerCid);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Trans_OnlineProcess(nOnlineMode, btAcquirerCid);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransOnlineProcess():return(%d)", nReturn);
	return nReturn;
}

// Emv Trans Completion
int	CDevCmn::fnEMV_TransCompletion()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransCompletion()");

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Trans_Completion();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TransCompletion():return(%x)", nReturn);
	return nReturn;
}

// Emv Tlv Get Tid Tag
int	CDevCmn::fnEMV_TlvGetTidTag(int nTid, BYTE* btpTag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetTidTag(nTid[%d], btpTag[%x])", nTid, *btpTag);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		emvkrnl_Tlv_GetTidTag(nTid, btpTag);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetTidTag(btpTag[%x]):return()", *btpTag);
	return nReturn;
}

// Emv Tlv Get Tid
int	CDevCmn::fnEMV_TlvGetTid(BYTE* btpTag, int nLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetTid(btpTag[%x], nLen[%d])", *btpTag, nLen);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Tlv_GetTid(btpTag, nLen);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetTid():return(%d)", nReturn);
	return nReturn;
}

// Emv Tlv Get Tag
int	CDevCmn::fnEMV_TlvGetTag(BYTE* btpBuf, BYTE* btpTag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetTag(btpBuf[%x], btpTag[%x])", *btpBuf, *btpTag);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Tlv_GetTag(btpBuf, btpTag);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetTag(btpTag[%x]):return(%d)", *btpTag, nReturn);
	return nReturn;
}

// Emv Tlv Get Value
int	CDevCmn::fnEMV_TlvGetVal(int nTid, int* npLen, BYTE* btpVal)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetVal(nTid[%d], npLen[%d])", nTid, *npLen);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Tlv_GetVal(nTid, npLen, btpVal);

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetVal(btpVal):return", btpVal, *npLen);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetVal(npLen[%d]):return(%d)", *npLen, nReturn);
	return nReturn;
}

// Emv Tlv Get Tid Length
int	CDevCmn::fnEMV_TlvGetTidLen(int nTid)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetTidLen(nTid[%d])", nTid);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Tlv_GetTidLen(nTid);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetTidLen():return(%d)", nReturn);
	return nReturn;
}

// Emv Tlv Get Length
int	CDevCmn::fnEMV_TlvGetLen(BYTE* btpBuf, int* npValLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetLen(btpBuf[%x], npValLen[%d])", *btpBuf, *npValLen);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Tlv_GetLen(btpBuf, npValLen);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvGetLen(btpBuf[%x], npValLen[%d]):return(%d)", *btpBuf, *npValLen, nReturn);
	return nReturn;
}

// Emv Tlv Store Value
int	CDevCmn::fnEMV_TlvStoreVal(int nTid, int nLen, BYTE* btpVal)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvStoreVal(nTid[%d], nLen[%d])", nTid, nLen);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvStoreVal(btpVal)", btpVal, nLen);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Tlv_StoreVal(nTid, nLen, btpVal);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvStoreVal():return(%d)", nReturn);
	return nReturn;
}

// Emv Tlv Store Value From Record
int	CDevCmn::fnEMV_TlvStoreValFromRecord(BYTE* btpRecord, int nType, int nInLen, int nSource)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvStoreValFromRecord(nType[%d], nInLen[%d], nSource[%d])", nType, nInLen, nSource);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvStoreValFromRecord(btpRecord)", btpRecord, nInLen);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Tlv_StoreValFromRecord(btpRecord, nType, nInLen, nSource);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TlvStoreValFromRecord():return(%d)", nReturn);
	return nReturn;
}

// Emv Utility BIN to N
int	CDevCmn::fnEMV_UtilBINtoN(int nVal, BYTE* btpData, int nLen)
{
	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		emvkrnl_Util_BINtoN(nVal, btpData, nLen);

	return nReturn;
}

// Emv Utility to YYYYMMDD
int	CDevCmn::fnEMV_UtiltoYYYYMMDD(BYTE* btpData, BYTE* btpOut, int nFormat)
{
	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		emvkrnl_Util_toYYYYMMDD(btpData, btpOut, nFormat);

	return nReturn;
}

// Emv Utility Get CN Real Length
int	CDevCmn::fnEMV_UtilGetCNRealLen(BYTE* btpData, int nLen)
{
	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = emvkrnl_Util_GetCNRealLen(btpData, nLen);

	return nReturn;
}

// Emv Utility Right Justify
int	CDevCmn::fnEMV_UtilRightJustify(BYTE* btpData, int nLen, BYTE* btpDest)
{
	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		emvkrnl_Util_RightJustify(btpData, nLen, btpDest);

	return nReturn;
}

// Emv Utility Decompress
int	CDevCmn::fnEMV_UtilDecompress(BYTE* btpSource, BYTE* btpDestination, int nCount)
{
	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		emvkrnl_Util_Decompress(btpSource, btpDestination, nCount);

	return nReturn;
}

// Emv Utility Compress
int	CDevCmn::fnEMV_UtilCompress(BYTE* btpSource, BYTE* btpDestination, int nCount)
{
	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		emvkrnl_Util_Compress(btpSource, btpDestination, nCount);

	return nReturn;
}

// Emv Utility Get APDU SW
int	CDevCmn::fnEMV_UtilGetAPDUSW(BYTE* btpSW12)
{
	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		emvkrnl_Util_GetAPDUSW(btpSW12);

	return nReturn;
}

// Emv Utility Get Version
int	CDevCmn::fnEMV_UtilGetVersion(BYTE* btpKernelVer)
{
	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		emvkrnl_Util_GetVersion(btpKernelVer);

	return nReturn;
}

// Emv Sel Descretionary Data List
int	CDevCmn::fnEMV_SelDescretionaryDataList(EMVst_APPTAGLIST* stpAppTagList)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelDescretionaryDataList()");

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		emvkrnl_Sel_DescretionaryDataList(stpAppTagList);

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelDescretionaryDataList(stpAppTagList):return", stpAppTagList, sizeof(EMVst_APPTAGLIST));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelDescretionaryDataList():return()");
	return nReturn;
}

// Emv Sel Set Only One Aid
int	CDevCmn::fnEMV_SelSetOnlyOneAID(BYTE btUcMatchUpYN)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelSetOnlyOneAID(btUcMatchUpYN[%x])", btUcMatchUpYN);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		emvkrnl_Sel_SetOnlyOneAID(btUcMatchUpYN);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelSetOnlyOneAID():return()");
	return nReturn;
}

// Emv Utility Set Kernel Log Path								// 2010.10.20
int	CDevCmn::fnEMV_UtilSetKernelLogPath(BYTE* btpPathName, UINT nPathLen)
{
	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		emvkrnl_Util_SetKernelLogPath(btpPathName, nPathLen);

	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	PBOC IC CARD UNIT FUNCTION(MCU) : PBOC OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Pboc Initialize
int	CDevCmn::fnPBC_Initialize()									// 2010.10.20
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_Initialize()");
	
	int	nReturn = FALSE;
	
	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_Init();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Pboc Initialize Add Application
int	CDevCmn::fnPBC_InitAddApplication(PBCst_APPLIST* stpApp)
{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitAddApplication(stpApp->AID)", stpApp->AID, sizeof(stpApp->AID));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitAddApplication(stpApp->AidLen)", &(stpApp->AidLen), sizeof(stpApp->AidLen));

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_AddApp(stpApp);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitAddApplication():return(%d)", nReturn);
	return nReturn;
}

// Pboc Initialize Get Applications
int	CDevCmn::fnPBC_InitGetApplication(int nIndex, PBCst_APPLIST* stpApp)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitGetApplication(nIndex[%d])", nIndex);
	
	int	nReturn = FALSE;
	
	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_GetApp(nIndex, stpApp);

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitGetApplication(stpApp->AID)", stpApp->AID, sizeof(stpApp->AID));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitGetApplication(stpApp->AidLen)", &(stpApp->AidLen), sizeof(stpApp->AidLen));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitGetApplication():return(%d)", nReturn);
	return nReturn;
}

// Pboc Initialize Del Application
int	CDevCmn::fnPBC_InitDelApplication(BYTE* btpAID, int nAidLen)
{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitDelApplication(btpAID)", btpAID, nAidLen);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitDelApplication(nAidLen[%d])", nAidLen);
	
	int	nReturn = FALSE;
	
	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_DelApp(btpAID, nAidLen);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitDelApplication():return(%d)", nReturn);
	return nReturn;
}

// Pboc Sel Application Selection
int	CDevCmn::fnPBC_SelApplicationSelection(BYTE btSlotIn, DWORD dwTransNoIn, BYTE btTryCntIn, PBCst_APPLIST* stpListOut, int* npAppNumOut, int nReadLogFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_SelApplicationSelection(btSlotIn[%x], dwTransNoIn[%x], btTryCntIn[%x], nReadLogFlag[%d])", btSlotIn, dwTransNoIn, btTryCntIn, nReadLogFlag);

	int	nReturn = FALSE;
	PBCst_APPLIST* stpListOutList = stpListOut;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = Emvlib_CreatAppList(btSlotIn, dwTransNoIn, btTryCntIn, stpListOut, npAppNumOut, nReadLogFlag);

	for (int i = 0; i < *(npAppNumOut); i++)
	{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_SelApplicationSelection(stpListOutList->AID)", stpListOutList->AID, sizeof(stpListOutList->AID));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_SelApplicationSelection(stpListOutList->AidLen)", &(stpListOutList->AidLen), sizeof(stpListOutList->AidLen));
	stpListOutList++;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_SelApplicationSelection(npAppNumOut[%d]):return(%d)", *npAppNumOut, nReturn);
	return nReturn;
}

// Pboc Sel Final Application Selection
int	CDevCmn::fnPBC_SelFinalAppSelection(int nSelIndexIn, int nReadLogFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_SelFinalAppSelection(nSelIndexIn[%d], nReadLogFlag[%d])", nSelIndexIn, nReadLogFlag);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = Emvlib_SelectApp(nSelIndexIn, nReadLogFlag);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_SelFinalAppSelection():return(%d)", nReturn);
	return nReturn;
}

// Pboc Trans Read Application Data
int	CDevCmn::fnPBC_TransReadAppData(int nGetTag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TransReadAppData(nGetTag[%x])", nGetTag);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_ReadAppData(nGetTag);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TransReadAppData():return(%d)", nReturn);
	return nReturn;
}

// Pboc Trans Offline Data Authentification
int	CDevCmn::fnPBC_TransOfflineDataAuth()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TransOfflineDataAuth()");

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_CardAuth();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TransOfflineDataAuth():return(%d)", nReturn);
	return nReturn;
}

// Pboc Trans Process
int	CDevCmn::fnPBC_TransProcess(BYTE* btpIfGoOnline)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TransProcess()");

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_ProcTrans(btpIfGoOnline);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TransProcess(btpIfGoOnline[%x]):return(%d)", *btpIfGoOnline, nReturn);
	return nReturn;
}

// Pboc Trans Completion
int	CDevCmn::fnPBC_TransCompletion(BYTE btResult, BYTE* btpRspCode, BYTE* btpAuthCode, int nAuthCodeLen, BYTE* btpAuthData, int nAuthDataLen, BYTE* btpScriptData, int nScriptLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TransCompletion(btResult[%x], btpRspCode[%x%x], nAuthCodeLen[%d], nAuthDataLen[%d], nScriptLen[%d])", btResult, *btpRspCode, *(btpRspCode + 1), nAuthCodeLen, nAuthDataLen, nScriptLen);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TransCompletion(btpAuthCode)", btpAuthCode, nAuthCodeLen);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TransCompletion(btpAuthData)", btpAuthData, nAuthDataLen);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TransCompletion(btpScriptData)", btpScriptData, nScriptLen);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_ProcTransComplete(btResult, btpRspCode, btpAuthCode, nAuthCodeLen, btpAuthData, nAuthDataLen, btpScriptData, nScriptLen);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TransCompletion():return(%d)", nReturn);
	return nReturn;
}

// Pboc EC Get Balance
int	CDevCmn::fnPBC_EcGetBalance(BYTE* btpBcdBalance)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_EcGetBalance()");

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_GetBalance(btpBcdBalance);

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_EcGetBalance(btpBcdBalance):return", btpBcdBalance, 6);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_EcGetBalance():return(%d)", nReturn);
	return nReturn;
}

// Pboc EC Get Icc Data
int	CDevCmn::fnPBC_EcGetIccData(WORD wTag, BYTE* btpDataOut, int* npOutLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_EcGetIccData(wTag[%x])", wTag);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_GetIccData(wTag, btpDataOut, npOutLen);

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_EcGetIccData(btpDataOut):return", btpDataOut, *npOutLen);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_EcGetIccData(npOutLen[%d]):return(%d)", *npOutLen, nReturn);
	return nReturn;
}

// Pboc EC Read Log Record
int	CDevCmn::fnPBC_EcReadLogRecord(int nRecordNo)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_EcReadLogRecord(nRecordNo[%d])", nRecordNo);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_ReadLogRecord(nRecordNo);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_EcReadLogRecord():return(%d)", nReturn);
	return nReturn;
}

// Pboc EC Log Item
int	CDevCmn::fnPBC_EcGetLogItem(WORD wTag, BYTE* btpTagData, int* npTagLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_EcGetLogItem(wTag[%x])", wTag);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_GetLogItem(wTag, btpTagData, npTagLen);

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_EcGetLogItem(btpTagData):return", btpTagData, *npTagLen);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_EcGetLogItem(npTagLen[%d]):return(%d)", *npTagLen, nReturn);
	return nReturn;
}

// Pboc Tlv Get Param
int	CDevCmn::fnPBC_TlvGetParam(PBCst_TERM* stpParam)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvGetParam()");

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		EmvLib_GetParam(stpParam);

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvGetParam(stpParam):return", stpParam, (sizeof(PBCst_TERM)));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvGetParam():return()");
	return nReturn;
}

// Pboc Tlv Set Param
int	CDevCmn::fnPBC_TlvSetParam(PBCst_TERM* stpParam)
{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvSetParam(stpParam)", stpParam, (sizeof(PBCst_TERM)));

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		EmvLib_SetParam(stpParam);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvSetParam():return()");
	return nReturn;
}

// Pboc Tlv Get Value
int	CDevCmn::fnPBC_TlvGetValue(WORD wTag, BYTE* btpDataOut, int* npDataLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvGetValue(wTag[%x])", wTag);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_GetTLV(wTag, btpDataOut, npDataLen);

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvGetValue(btpDataOut):return", btpDataOut, *npDataLen);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvGetValue(npDataLen[%d]):return(%d)", *npDataLen, nReturn);
	return nReturn;
}

// Pboc Tlv Store Value
int	CDevCmn::fnPBC_TlvStoreValue(WORD wTag, BYTE* btpDataIn, int nDataLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvStoreValue(wTag[%x], nDataLen[%d])", wTag, nDataLen);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvStoreValue(btpDataIn)", btpDataIn, nDataLen);

	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_SetTLV(wTag, btpDataIn, nDataLen);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvStoreValue():return(%d)", nReturn);
	return nReturn;
}

// Pboc Tlv Get Script Result
int	CDevCmn::fnPBC_TlvGetScriptResult(BYTE* btpResult, int* npRetLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvGetScriptResult()");
	
	int	nReturn = FALSE;
	
	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_GetScriptResult(btpResult, npRetLen);
	
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvGetScriptResult(btpResult):return", btpResult, *npRetLen);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_TlvGetScriptResult(npRetLen[%d]):return(%d)", *npRetLen, nReturn);
	return nReturn;
}

// Pboc Utility Get Version
int	CDevCmn::fnPBC_UtilGetVersion()
{
	int	nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_GetVer();

	return nReturn;
}

// Pboc Utility Clear Trans Log
int	CDevCmn::fnPBC_UtilClearTransLog()
{
	int	nReturn = FALSE;
	
	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		EmvLib_ClearTransLog();
	
	return nReturn;
}

// Pboc Init Add Ca Pub Key
int	CDevCmn::fnPBC_InitAddCaPubKey(PBCst_CA_PUBKEY* stpCaPubKey)
{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitAddCaPubKey(stpCaPubKey)", stpCaPubKey, (sizeof(PBCst_CA_PUBKEY)));
	
	int	nReturn = FALSE;
	
	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_AddCapk(stpCaPubKey);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitAddCaPubKey():return(%d)", nReturn);
	return nReturn;
}

// Pboc Init Get Ca Pub Key
int	CDevCmn::fnPBC_InitGetCaPubKey(int nIndex, PBCst_CA_PUBKEY* stpCaPubKey)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitGetCaPubKey(nIndex[%d])", nIndex);
	
	int	nReturn = FALSE;
	
	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_GetCapk(nIndex, stpCaPubKey);

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitGetCaPubKey(stpCaPubKey)", stpCaPubKey, (sizeof(PBCst_CA_PUBKEY)));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitGetCaPubKey():return(%d)", nReturn);
	return nReturn;
}

// Pboc Init Del Ca Pub Key
int	CDevCmn::fnPBC_InitDelCaPubKey(BYTE btKeyID, BYTE* btpRID)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitDelCaPubKey(btKeyID[%x])", btKeyID);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitDelCaPubKey(btpRID)", btpRID, 5);
	
	int	nReturn = FALSE;
	
	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_DelCapk(btKeyID, btpRID);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitDelCaPubKey():return(%d)", nReturn);
	return nReturn;
}

// Pboc Init Check Ca Pub Key
int	CDevCmn::fnPBC_InitCheckCaPubKey(BYTE* btpKeyID, BYTE* btpRID)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitCheckCaPubKey(btpKeyID[%x])", *btpKeyID);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitCheckCaPubKey(btpRID)", btpRID, 5);
	
	int	nReturn = FALSE;
	
	DeviceOperationFlag = TRUE;
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = EmvLib_CheckCapk(btpKeyID, btpRID);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBC_InitCheckCaPubKey():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	BRM FUNCTION(BRM) : BRM SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnBRM_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Clear Dispense Information
int	CDevCmn::fnBRM_ClearDispenseInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ClearDispenseInfo()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejTotCount", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount1", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount1", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejCount1", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount2", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount2", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejCount2", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount3", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount3", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejCount3", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount4", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount4", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejCount4", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount5", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount5", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejCount5", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RetractTotCount", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejInRejectBinCnt", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejInRetractBinCnt", 0);
																// 2005.12.08
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette1", "0 0 0 0 0 0");
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette2", "0 0 0 0 0 0");
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette3", "0 0 0 0 0 0");
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette4", "0 0 0 0 0 0");
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette5", "0 0 0 0 0 0");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ClearDispenseInfo():return(%d)", nReturn);
	return nReturn;
}

// Set Value Of Cash(Cash Value Data)
int	CDevCmn::fnBRM_SetValueOfCash(LPCTSTR szCashValueData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_SetValueOfCash(szCashValueData[%s])", szCashValueData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmSetValueOfCash(szCashValueData);	// 2005.02.24

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_SetValueOfCash(...):return(%d)", nReturn);
	return nReturn;
}

// Set MinMax Of CST(Min Count Data, Max Count Data)
int	CDevCmn::fnBRM_SetMinMaxCST(LPCTSTR szMinCntData, LPCTSTR szMaxCntData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_SetMinMaxCST(szMinCntData[%s], szMaxCntData[%s])", szMinCntData, szMaxCntData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmSetMinMaxCST(szMinCntData, szMaxCntData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_SetMinMaxCST(...):return(%d)", nReturn);
	return nReturn;
}

// Set Number Of Cash(Cash Count Data)
int	CDevCmn::fnBRM_SetNumberOfCash(LPCTSTR szCashCntData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_SetNumberOfCash(szCashCntData[%s])", szCashCntData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmSetNumberOfCash(szCashCntData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_SetNumberOfCash(...):return(%d)", nReturn);
	return nReturn;
}

// Set Avail Deposit(Deposit Type)
int	CDevCmn::fnBRM_SetAvailDeposit(int nDepType)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_SetAvailDeposit(nDepType[%d])", nDepType);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmSetAvailDeposit(nDepType);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_SetAvailDeposit(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	BRM FUNCTION(BRM) : BRM GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrBRM_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->BrmGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Sensor Information
CString CDevCmn::fstrBRM_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->BrmGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnBRM_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Brm Cim Get Device Status
int	CDevCmn::fnBRM_CimGetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_CimGetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmCimGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CimGetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Brm Cdu Get Device Status
int	CDevCmn::fnBRM_CduGetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_CduGetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmCduGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CduGetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Shutter Status
int	CDevCmn::fnBRM_GetShutterStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetShutterStatus()");

	static	int		nSaveReturn = SHUTTER_CLOSED;
	int		nReturn = SHUTTER_CLOSED;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetShutterStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetShutterStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Brm Cim Get Shutter Status
int	CDevCmn::fnBRM_CimGetShutterStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_CimGetShutterStatus()");

	static	int		nSaveReturn = SHUTTER_CLOSED;
	int		nReturn = SHUTTER_CLOSED;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmCimGetShutterStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CimGetShutterStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Brm Cdu Get Shutter Status
int	CDevCmn::fnBRM_CduGetShutterStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_CduGetShutterStatus()");

	static	int		nSaveReturn = SHUTTER_CLOSED;
	int		nReturn = SHUTTER_CLOSED;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmCduGetShutterStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CduGetShutterStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Retract Action
int	CDevCmn::fnBRM_GetRetractAction()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRetractAction()");

	static	int		nSaveReturn = RETRACT_NOTACTIVE;
	int		nReturn = RETRACT_NOTACTIVE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetRetractAction();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRetractAction():return(%d)", nReturn);
}
	return nReturn;
}

// Brm Cim Get Retract Action
int	CDevCmn::fnBRM_CimGetRetractAction()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_CimGetRetractAction()");

	static	int		nSaveReturn = RETRACT_NOTACTIVE;
	int		nReturn = RETRACT_NOTACTIVE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmCimGetRetractAction();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CimGetRetractAction():return(%d)", nReturn);
}
	return nReturn;
}

// Brm Cdu Get Retract Action
int	CDevCmn::fnBRM_CduGetRetractAction()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_CduGetRetractAction()");

	static	int		nSaveReturn = RETRACT_NOTACTIVE;
	int		nReturn = RETRACT_NOTACTIVE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmCduGetRetractAction();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CduGetRetractAction():return(%d)", nReturn);
}
	return nReturn;
}

// Get Position
int	CDevCmn::fnBRM_GetPosition()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetPosition()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetPosition();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetPosition():return(%d)", nReturn);
}
	return nReturn;
}

// Get Change Unit Lock
int	CDevCmn::fnBRM_GetChangeUnitLock()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetChangeUnitLock()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetChangeUnitLock();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetChangeUnitLock():return(%d)", nReturn);
}
	return nReturn;
}

// Get Clerk CST Status
int	CDevCmn::fnBRM_GetClerkCSTStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetClerkCSTStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetClerkCSTStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetClerkCSTStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Reject CST Status
int	CDevCmn::fnBRM_GetRejectCSTStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRejectCSTStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetRejectCSTStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRejectCSTStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Retract CST Status
int	CDevCmn::fnBRM_GetRetractCSTStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRetractCSTStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetRetractCSTStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRetractCSTStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Deposit CST Status(Cash Type)
int	CDevCmn::fnBRM_GetDepositCSTStatus(int nCashType)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetDepositCSTStatus(nCashType[%d])", nCashType);

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetDepositCSTStatus(nCashType);

	switch (nCashType)
	{
		case CASH_TYPE_DEPOSIT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetDepositCSTStatus(nCashType[%d]):return(%d)", nCashType, nReturn);
}
			break;
		case CASH_TYPE_DEPOSIT100:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetDepositCSTStatus(nCashType[%d]):return(%d)", nCashType, nReturn);
}
			break;
		case CASH_TYPE_DEPOSIT50:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetDepositCSTStatus(nCashType[%d]):return(%d)", nCashType, nReturn);
}
			break;
		case CASH_TYPE_RECYCLE100:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetDepositCSTStatus(nCashType[%d]):return(%d)", nCashType, nReturn);
}
			break;
		case CASH_TYPE_RECYCLE50:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetDepositCSTStatus(nCashType[%d]):return(%d)", nCashType, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Recycle Box Status
int	CDevCmn::fnBRM_GetRecycleBoxStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRecycleBoxStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetRecycleBoxStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRecycleBoxStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Autoload Condition
int	CDevCmn::fnBRM_GetAutoloadCondition()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAutoloadCondition()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetAutoloadCondition();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAutoloadCondition():return(%d)", nReturn);
}
	return nReturn;
}

// Get Material Information
int	CDevCmn::fnBRM_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetMaterialInfo();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

// Get Avail Deposit
int	CDevCmn::fnBRM_GetAvailDeposit()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAvailDeposit()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetAvailDeposit();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAvailDeposit():return(%d)", nReturn);
}
	return nReturn;
}

// Get Avail Withdraw
int	CDevCmn::fnBRM_GetAvailWithdraw()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAvailWithdraw()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetAvailWithdraw();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAvailWithdraw():return(%d)", nReturn);
}
	return nReturn;
}

// Get Accept Count Status
int	CDevCmn::fnBRM_GetAcceptCountStatus()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAcceptCountStatus()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetAcceptCountStatus();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAcceptCountStatus():return(%d)", nReturn);
	return nReturn;
}

// Get Count Result
CString CDevCmn::fstrBRM_GetCountResult()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetCountResult()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->BrmGetCountResult();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetCountResult():return(%s)", strReturn);
	return strReturn;
}

// Get Retract Result
CString CDevCmn::fstrBRM_GetRetractResult()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetRetractResult()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->BrmGetRetractResult();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetRetractResult():return(%s)", strReturn);
	return strReturn;
}

// Get Refill Result
CString CDevCmn::fstrBRM_GetRefillResult()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetRefillResult()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->BrmGetRefillResult();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetRefillResult():return(%s)", strReturn);
	return strReturn;
}

// Get Takeup Result
CString CDevCmn::fstrBRM_GetTakeupResult()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetTakeupResult()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->BrmGetTakeupResult();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetTakeupResult():return(%s)", strReturn);
	return strReturn;
}

// Get Number Of CST
int	CDevCmn::fnBRM_GetNumberOfCST()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfCST()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetNumberOfCST();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfCST():return(%d)", nReturn);
}
	return nReturn;
}

// Get CST Type(CST No)
int	CDevCmn::fnBRM_GetCSTType(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSTType(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetCSTType(nCSTNo);

	switch (nCSTNo)
	{
		case BRM_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSTType(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSTType(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSTType(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSTType(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSTType(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get CST Status(CST No)
int	CDevCmn::fnBRM_GetCSTStatus(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSTStatus(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetCSTStatus(nCSTNo);

	switch (nCSTNo)
	{
		case BRM_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Value Of Cash(CST No)
int	CDevCmn::fnBRM_GetValueOfCash(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetValueOfCash(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetValueOfCash(nCSTNo);

	switch (nCSTNo)
	{
		case BRM_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Number Of Cash(CST No)
int	CDevCmn::fnBRM_GetNumberOfCash(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfCash(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetNumberOfCash(nCSTNo);

	switch (nCSTNo)
	{
		case BRM_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Number Of Set Cash(CST No)
int	CDevCmn::fnBRM_GetNumberOfSetCash(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfSetCash(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetNumberOfSetCash(nCSTNo);

	switch (nCSTNo)
	{
		case BRM_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Dispense Of CST(Cash Amount)
CString	CDevCmn::fstrBRM_GetDispenseOfCST(int nCashAmt)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetDispenseOfCST(nCashAmt[%d])", nCashAmt);

	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->BrmGetDispenseOfCST(nCashAmt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetDispenseOfCST(...):return(%s)", strReturn);
	return strReturn;
}

// Get Number Of Reject(CST No)
int	CDevCmn::fnBRM_GetNumberOfReject(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfReject(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int				nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmGetNumberOfReject(nCSTNo);

	switch (nCSTNo)
	{
		case BRM_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case BRM_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Number Of Retract()
CString	CDevCmn::fstrBRM_GetNumberOfRetract()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetNumberOfRetract()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->BrmGetNumberOfRetract();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetNumberOfRetract():return(%s)", strReturn);
}
	return strReturn;
}

// Get Number Of Deposit(Cash Type)
CString	CDevCmn::fstrBRM_GetNumberOfDeposit(int nCashType)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetNumberOfDeposit(nCashType[%d])", nCashType);

	static	CString	strSaveReturn0("");
	static	CString	strSaveReturn1("");
	static	CString	strSaveReturn2("");
	static	CString	strSaveReturn3("");
	static	CString	strSaveReturn4("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->BrmGetNumberOfDeposit(nCashType);

	switch (nCashType)
	{
		case CASH_TYPE_DEPOSIT:
if (strReturn != strSaveReturn0)
{
	strSaveReturn0 = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetNumberOfDeposit(nCashType[%d]):return(%s)", nCashType, strReturn);
}
			break;
		case CASH_TYPE_DEPOSIT100:
if (strReturn != strSaveReturn1)
{
	strSaveReturn1 = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetNumberOfDeposit(nCashType[%d]):return(%s)", nCashType, strReturn);
}
			break;
		case CASH_TYPE_DEPOSIT50:
if (strReturn != strSaveReturn2)
{
	strSaveReturn2 = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetNumberOfDeposit(nCashType[%d]):return(%s)", nCashType, strReturn);
}
			break;
		case CASH_TYPE_RECYCLE100:
if (strReturn != strSaveReturn3)
{
	strSaveReturn3 = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetNumberOfDeposit(nCashType[%d]):return(%s)", nCashType, strReturn);
}
			break;
		case CASH_TYPE_RECYCLE50:
if (strReturn != strSaveReturn4)
{
	strSaveReturn4 = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetNumberOfDeposit(nCashType[%d]):return(%s)", nCashType, strReturn);
}
			break;
	}

	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	BRM FUNCTION(BRM) : BRM OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnBRM_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Initialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MAX_DEVRSP_TIME;								// 2008.11.05
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnBRM_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Deinitialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Open Shutter
int	CDevCmn::fnBRM_OpenShutter()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_OpenShutter()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmOpenShutter();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_OpenShutter():return(%d)", nReturn);
	return nReturn;
}

// Close Shutter
int	CDevCmn::fnBRM_CloseShutter()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CloseShutter()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmCloseShutter();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CloseShutter():return(%d)", nReturn);
	return nReturn;
}

// Ready
int	CDevCmn::fnBRM_Ready()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_Ready()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmReady();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_Ready():return(%d)", nReturn);
	return nReturn;
}

// Accept(Wait Time)
int	CDevCmn::fnBRM_Accept(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Accept(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmAcceptCash(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Accept(...):return(%d)", nReturn);
	return nReturn;
}

// Cancel Accept : for OKI-BRM
int	CDevCmn::fnBRM_CancelAcceptCash()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CancelAcceptCash()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmCancelAcceptCash();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CancelAcceptCash():return(%d)", nReturn);
	return nReturn;
}

// Ready Count
int	CDevCmn::fnBRM_ReadyCount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ReadyCount()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmReadyCount();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ReadyCount():return(%d)", nReturn);
	return nReturn;
}

// Count
int	CDevCmn::fnBRM_Count()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Count()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MAX_DEVRSP_TIME;								// 2008.11.05
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmCount();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Count():return(%d)", nReturn);
	return nReturn;
}

// Stack
int	CDevCmn::fnBRM_Stack()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Store()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MAX_DEVRSP_TIME;								// 2008.11.05
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmStack();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Store():return(%d)", nReturn);
	return nReturn;
}

// Reject(Wait Time):Reject All
int	CDevCmn::fnBRM_Reject(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Reject(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MAX_DEVRSP_TIME;								// 2008.11.05
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmReject(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Reject(...):return(%d)", nReturn);
	return nReturn;
}

// Wait Lift
int	CDevCmn::fnBRM_WaitLift()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_WaitLift()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmWaitLift();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_WaitLift():return(%d)", nReturn);
	return nReturn;
}

// Wait Taken
int	CDevCmn::fnBRM_WaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_WaitTaken()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_WaitTaken():return(%d)", nReturn);
	return nReturn;
}

// Cancel Wait Taken
int	CDevCmn::fnBRM_CancelWaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CancelWaitTaken()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmCancelWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CancelWaitTaken():return(%d)", nReturn);
	return nReturn;
}

// Retract
int	CDevCmn::fnBRM_Retract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Retract()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MAX_DEVRSP_TIME;								// 2008.11.05
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmRetract();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Retract():return(%d)", nReturn);
	return nReturn;
}

// Present2(Wait Time):Reject Part
int	CDevCmn::fnBRM_Present2(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Present2(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmPresent2(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Present2(...):return(%d)", nReturn);
	return nReturn;
}

// Ready Dispense
int	CDevCmn::fnBRM_ReadyDispense()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ReadyDispense()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmReadyDispense();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ReadyDispense():return(%d)", nReturn);
	return nReturn;
}

// Dispense(Cash Amount)
int	CDevCmn::fnBRM_Dispense(int nCashAmt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Dispense(nCashAmt[%d])", nCashAmt);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MAX_DEVRSP_TIME;								// 2008.11.05
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmDispenseAmount(nCashAmt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Dispense(...):return(%d)", nReturn);
	return nReturn;
}

// Present(Wait Time)
int	CDevCmn::fnBRM_Present(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Present(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmPresent(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Present(...):return(%d)", nReturn);
	return nReturn;
}

// Refill Money(Cash Count)
int	CDevCmn::fnBRM_RefillMoney(int nCashCnt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_RefillMoney(nCashCnt[%d])", nCashCnt);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmRefillMoney(nCashCnt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_RefillMoney(...):return(%d)", nReturn);
	return nReturn;
}

// Takeup Money(Cash Count)
int	CDevCmn::fnBRM_TakeupMoney(int nCashCnt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_TakeupMoney(nCashCnt[%d])", nCashCnt);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->BrmTakeupMoney(nCashCnt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_TakeupMoney(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CDU FUNCTION(CDU) : CDU SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnCDU_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Clear Dispense Information
int	CDevCmn::fnCDU_ClearDispenseInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_ClearDispenseInfo()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejTotCount", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount1", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount1", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejCount1", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount2", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount2", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejCount2", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount3", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount3", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejCount3", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount4", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount4", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejCount4", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount5", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount5", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejCount5", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RetractTotCount", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejInRejectBinCnt", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejInRetractBinCnt", 0);
																// 2005.12.08
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette1", "0 0 0 0 0 0");
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette2", "0 0 0 0 0 0");
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette3", "0 0 0 0 0 0");
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette4", "0 0 0 0 0 0");
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette5", "0 0 0 0 0 0");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_ClearDispenseInfo():return(%d)", nReturn);
	return nReturn;
}

// Set Value Of Cash(Cash Value Data)
int	CDevCmn::fnCDU_SetValueOfCash(LPCTSTR szCashValueData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_SetValueOfCash(szCashValueData[%s])", szCashValueData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduSetValueOfCash(szCashValueData);	// 2005.02.24

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_SetValueOfCash(...):return(%d)", nReturn);
	return nReturn;
}

// Set MinMax Of CST(Min Count Data, Max Count Data)
int	CDevCmn::fnCDU_SetMinMaxCST(LPCTSTR szMinCntData, LPCTSTR szMaxCntData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_SetMinMaxCST(szMinCntData[%s], szMaxCntData[%s])", szMinCntData, szMaxCntData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduSetMinMaxCST(szMinCntData, szMaxCntData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_SetMinMaxCST(...):return(%d)", nReturn);
	return nReturn;
}

// Set Number Of Cash(Cash Count Data)
int	CDevCmn::fnCDU_SetNumberOfCash(LPCTSTR szCashCntData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_SetNumberOfCash(szCashCntData[%s])", szCashCntData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduSetNumberOfCash(szCashCntData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_SetNumberOfCash(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CDU FUNCTION(CDU) : CDU GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrCDU_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCDU_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->CduGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCDU_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Sensor Information
CString CDevCmn::fstrCDU_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrCDU_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->CduGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCDU_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnCDU_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Shutter Status
int	CDevCmn::fnCDU_GetShutterStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetShutterStatus()");

	static	int		nSaveReturn = SHUTTER_CLOSED;
	int		nReturn = SHUTTER_CLOSED;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetShutterStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetShutterStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Retract Action
int	CDevCmn::fnCDU_GetRetractAction()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetRetractAction()");

	static	int		nSaveReturn = RETRACT_NOTACTIVE;
	int		nReturn = RETRACT_NOTACTIVE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetRetractAction();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetRetractAction():return(%d)", nReturn);
}
	return nReturn;
}

// Get Position
int	CDevCmn::fnCDU_GetPosition()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetPosition()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetPosition();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetPosition():return(%d)", nReturn);
}
	return nReturn;
}

// Get Reject CST Status
int	CDevCmn::fnCDU_GetRejectCSTStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetRejectCSTStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetRejectCSTStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetRejectCSTStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Recycle Box Status
int	CDevCmn::fnCDU_GetRecycleBoxStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetRecycleBoxStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetRecycleBoxStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetRecycleBoxStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Material Information
int	CDevCmn::fnCDU_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetMaterialInfo();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

// Get Avail Withdraw
int	CDevCmn::fnCDU_GetAvailWithdraw()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetAvailWithdraw()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetAvailWithdraw();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetAvailWithdraw():return(%d)", nReturn);
}
	return nReturn;
}

// Get Number Of CST
int	CDevCmn::fnCDU_GetNumberOfCST()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCST()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetNumberOfCST();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCST():return(%d)", nReturn);
}
	return nReturn;
}

// Get CST Status(CST No)
int	CDevCmn::fnCDU_GetCSTStatus(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetCSTStatus(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetCSTStatus(nCSTNo);

	switch (nCSTNo)
	{
		case CDU_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Value Of Cash(CST No)
int	CDevCmn::fnCDU_GetValueOfCash(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetValueOfCash(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetValueOfCash(nCSTNo);

	switch (nCSTNo)
	{
		case CDU_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Number Of Cash(CST No)
int	CDevCmn::fnCDU_GetNumberOfCash(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCash(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetNumberOfCash(nCSTNo);

	switch (nCSTNo)
	{
		case CDU_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Number Of Set Cash(CST No)
int	CDevCmn::fnCDU_GetNumberOfSetCash(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfSetCash(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetNumberOfSetCash(nCSTNo);

	switch (nCSTNo)
	{
		case CDU_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Dispense Of CST(Cash Amount)
CString	CDevCmn::fstrCDU_GetDispenseOfCST(int nCashAmt)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrCDU_GetDispenseOfCST(nCashAmt[%d])", nCashAmt);

	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->CduGetDispenseOfCST(nCashAmt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCDU_GetDispenseOfCST(...):return(%s)", strReturn);
	return strReturn;
}

// Get Number Of Dispense(CST No)
int	CDevCmn::fnCDU_GetNumberOfDispense(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfDispense(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		if ((nCSTNo >= CDU_CST_1)	&&
			(nCSTNo <= CDU_CST_4))
		{
			nReturn = IniGetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", 
									GetSprintf("CstCount%d", nCSTNo));
			if (nReturn < 0)
				nReturn = 0;
		}
		else
			nReturn = 0;
	}

	switch (nCSTNo)
	{
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfDispense(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfDispense(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfDispense(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfDispense(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Number Of Reject(CST No)
int	CDevCmn::fnCDU_GetNumberOfReject(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfReject(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	CString strReturn("");
	CStringArray strReturnArray;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		if (nCSTNo == CDU_CST_REJECT)							// 2006.02.09
		{
			nReturn = IniGetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejTotCount"); 
		}
		else
		if ((nCSTNo >= CDU_CST_1)	&&
			(nCSTNo <= CDU_CST_4))
		{														// 2004.11.05
			strReturn = IniGetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", 
									GetSprintf("Cassette%d", nCSTNo));
			SplitString(strReturn, " ", strReturnArray);
			if (strReturnArray.GetSize() >= 6)
				nReturn = Asc2Int(strReturnArray[5]);
			else
				nReturn = 0;
			if (nReturn < 0)
				nReturn = 0;
		}
		else
			nReturn = 0;
	}

	switch (nCSTNo)
	{
		case CDU_CST_REJECT:									// 2006.02.09
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CDU FUNCTION(CDU) : CDU OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnCDU_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Initialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnCDU_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Deinitialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Open Shutter
int	CDevCmn::fnCDU_OpenShutter()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_OpenShutter()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduOpenShutter();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_OpenShutter():return(%d)", nReturn);
	return nReturn;
}

// Close Shutter
int	CDevCmn::fnCDU_CloseShutter()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_CloseShutter()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduCloseShutter();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_CloseShutter():return(%d)", nReturn);
	return nReturn;
}

// Wait Taken
int	CDevCmn::fnCDU_WaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_WaitTaken()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_WaitTaken():return(%d)", nReturn);
	return nReturn;
}

// Cancel Wait Taken
int	CDevCmn::fnCDU_CancelWaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_CancelWaitTaken()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduCancelWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_CancelWaitTaken():return(%d)", nReturn);
	return nReturn;
}

// Retract
int	CDevCmn::fnCDU_Retract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Retract()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduRetract();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Retract():return(%d)", nReturn);
	return nReturn;
}

// Dispense(Cash Amount)
int	CDevCmn::fnCDU_Dispense(int nCashAmt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Dispense(nCashAmt[%d])", nCashAmt);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduDispenseAmount(nCashAmt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Dispense(...):return(%d)", nReturn);
	return nReturn;
}

// Present(Wait Time)
int	CDevCmn::fnCDU_Present(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Present(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	CshRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduPresent(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Present(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	PASSBOOK FUNCTION(PBM) : PBM SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnPBM_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnPBM_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Clear Retract Count
int	CDevCmn::fnPBM_ClearRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_ClearRetractCnt()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmClearRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_ClearRetractCnt():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	PASSBOOK FUNCTION(PBM) : PBM GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrPBM_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Sensor Information
CString CDevCmn::fstrPBM_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnPBM_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnPBM_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Material Information
int	CDevCmn::fnPBM_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnPBM_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmGetMaterialInfo();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

// Get Retract Count
CString CDevCmn::fstrPBM_GetRetractCnt()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetRetractCnt()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetRetractCnt();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetRetractCnt():return(%s)", strReturn);
}
	return strReturn;
}

// Get PB Ms Data
CString CDevCmn::fstrPBM_GetMsData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetMsData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetMsData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetMsData():return(%s)", strReturn);
	return strReturn;
}

// Get PB Bar Data
CString CDevCmn::fstrPBM_GetBarData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetBarData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetBarData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetBarData():return(%s)", strReturn);
	return strReturn;
}

// Get PB Line Data
CString CDevCmn::fstrPBM_GetLineData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetLineData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetLineData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetLineData():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	PASSBOOK FUNCTION(PBM) : PBM OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnPBM_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Initialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnPBM_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Deinitialize()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Entry Enable(Wait Time)
int	CDevCmn::fnPBM_EntryEnable(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_EntryEnable(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmEntryEnable(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_EntryEnable(...):return(%d)", nReturn);
	return nReturn;
}

// Entry Disable
int	CDevCmn::fnPBM_EntryDisable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_EntryDisable()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmEntryDisable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_EntryDisable():return(%d)", nReturn);
	return nReturn;
}

// PB Ms Read
int	CDevCmn::fnPBM_Read()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Read()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmRead();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Read():return(%d)", nReturn);
	return nReturn;
}

// PB Bar Line Read	
int	CDevCmn::fnPBM_BarLineRead()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_BarLineRead()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmBarLineRead();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_BarLineRead():return(%d)", nReturn);
	return nReturn;
}

// Print(Print Data)
int	CDevCmn::fnPBM_Print(LPCTSTR szPrintData)
{
	CStringArray	strPrintArray;
	BYTE			DumpData[BASEBUFFSIZE];
	int				nDumpLength = 0;

	SplitString(szPrintData, ",", strPrintArray);
	for (int i = 0; i < strPrintArray.GetSize(); i++)
	{
		memset(DumpData, 0, sizeof(DumpData));
		nDumpLength = MakePack(strPrintArray[i].GetBuffer(0), DumpData, strPrintArray[i].GetLength());
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Print(DumpData[%s], nDumpLength[%d])", DumpData, nDumpLength);
	}

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	PbmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmPrint(szPrintData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Print(...):return(%d)", nReturn);
	return nReturn;
}

// Eject(Wait Time)
int	CDevCmn::fnPBM_Eject(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Eject(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmEject(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Eject(...):return(%d)", nReturn);
	return nReturn;
}

// Wait Taken
int	CDevCmn::fnPBM_WaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_WaitTaken()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_WaitTaken():return(%d)", nReturn);
	return nReturn;
}

// Cancel Wait Taken
int	CDevCmn::fnPBM_CancelWaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_CancelWaitTaken()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmCancelWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_CancelWaitTaken():return(%d)", nReturn);
	return nReturn;
}

// Retract
int	CDevCmn::fnPBM_Retract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Retract()");

	int		nReturn = FALSE;

	DeviceOperationFlag = TRUE;
	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmRetract();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Retract():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CAMERA FUNCTION(CMR) : CMR SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnCMR_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMR_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CAMERA FUNCTION(CMR) : CMR GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrCMR_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCMR_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CMR))
		strReturn = m_pMwi->CmrGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCMR_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnCMR_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMR_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CAMERA FUNCTION(CMR) : CMR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnCMR_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnCMR_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Capture Face(File Name)
int	CDevCmn::fnCMR_CaptureFace(LPCTSTR szFileName)
{
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
	{
		switch (AtmDefine.CameraDevice)							// 2005.03.30
		{
			case PCI_CAMERA1_SET:
			case USB_CAMERA1_SET:
			case PCI_CAMERA2_SET:
			case USB_CAMERA2_SET:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_CaptureFace(szFileName[%s])", szFileName);
				nReturn = m_pMwi->CmrCaptureFace(szFileName);
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMR_CaptureFace(...):return(%d)", nReturn);
				break;
		}
	}

	return nReturn;
}

// Capture Hand(File Name)
int	CDevCmn::fnCMR_CaptureHand(LPCTSTR szFileName)
{
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
	{
		switch (AtmDefine.CameraDevice)							// 2005.03.30
		{
			case PCI_CAMERA2_SET:
			case USB_CAMERA2_SET:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_CaptureHand(szFileName[%s])", szFileName);
				nReturn = m_pMwi->CmrCaptureHand(szFileName);
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMR_CaptureHand(...):return(%d)", nReturn);
				break;
		}
	}

	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	DOOR FUNCTION(DOR) : DOR SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnDOR_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DOR))
		nReturn = m_pMwi->DorClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnDOR_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	DOOR FUNCTION(DOR) : DOR GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrDOR_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrDOR_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_DOR))
		strReturn = m_pMwi->DorGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrDOR_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnDOR_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnDOR_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DOR))
		nReturn = m_pMwi->DorGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Door Status
int	CDevCmn::fnDOR_GetDoorStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnDOR_GetDoorStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DOR))
		nReturn = m_pMwi->DorGetDoorStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_GetDoorStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	DOOR FUNCTION(DOR) : DOR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnDOR_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DOR))
		nReturn = m_pMwi->DorInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnDOR_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DOR))
		nReturn = m_pMwi->DorDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	LIGHT FUNCTION(LGT) : LGT SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnLGT_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_LGT))
		nReturn = m_pMwi->LgtClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnLGT_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Set Flicker(Index, Value)
int	CDevCmn::fnLGT_SetFlicker(int nIndex, int nValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_SetFlicker(nIndex[%x], nValue[%x])", nIndex, nValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_LGT))
		nReturn = m_pMwi->LgtSetFlicker(nIndex, nValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnLGT_SetFlicker(...):return(%d)", nReturn);
	return nReturn;
}

// Set Indicator(Value)
int	CDevCmn::fnLGT_SetIndicator(int nValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_SetIndicator(nValue[%x])", nValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_LGT))
		nReturn = m_pMwi->LgtSetIndicator(nValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnLGT_SetIndicator(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	LIGHT FUNCTION(LGT) : LGT GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrLGT_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrLGT_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_LGT))
		strReturn = m_pMwi->LgtGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrLGT_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnLGT_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnLGT_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_LGT))
		nReturn = m_pMwi->LgtGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	LIGHT FUNCTION(LGT) : LGT OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnLGT_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_LGT))
		nReturn = m_pMwi->LgtInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnLGT_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_LGT))
		nReturn = m_pMwi->LgtDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SPL FUNCTION(SPL) : SPL SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnSPL_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Set Segment(Value)
int	CDevCmn::fnSPL_SetSegment(LPCTSTR szValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetSegment(szValue[%s])", szValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetSegment(szValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetSegment(...):return(%d)", nReturn);
	return nReturn;
}

// Set Led(Value)
int	CDevCmn::fnSPL_SetLed(LPCTSTR szValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetLed(szValue[%s])", szValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetLed(szValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetLed(...):return(%d)", nReturn);
	return nReturn;
}

// Set Rpl(Value)
int	CDevCmn::fnSPL_SetRpl(LPCTSTR szValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetRpl(szValue[%s])", szValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetRpl(szValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetRpl(...):return(%d)", nReturn);
	return nReturn;
}

// Set Inform(Value)
int	CDevCmn::fnSPL_SetInform(LPCTSTR szValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetInform(szValue[%s])", szValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetInform(szValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetInform(...):return(%d)", nReturn);
	return nReturn;
}

// Set InService LED(Value)
int	CDevCmn::fnSPL_SetInServiceLED(LPCTSTR szValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetInServiceLED(szValue[%s])", szValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetInServiceLED(szValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetInServiceLED(...):return(%d)", nReturn);
	return nReturn;
}

// Set Superviosr LED(Value)
int	CDevCmn::fnSPL_SetSuperviosrLED(LPCTSTR szValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetSuperviosrLED(szValue[%s])", szValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetSuperviosrLED(szValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetSuperviosrLED(...):return(%d)", nReturn);
	return nReturn;
}

// Set Error LED(Value)
int	CDevCmn::fnSPL_SetErrorLED(LPCTSTR szValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetErrorLED(szValue[%s])", szValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetErrorLED(szValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetErrorLED(...):return(%d)", nReturn);
	return nReturn;
}

// Set Communications LED(Value)
int	CDevCmn::fnSPL_SetCommunicationsLED(LPCTSTR szValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetCommunicationsLED(szValue[%s])", szValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetCommunicationsLED(szValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetCommunicationsLED(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SPL FUNCTION(SPL) : SPL GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrSPL_GetErrorCode()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSPL_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_SPL))
		strReturn = m_pMwi->SplGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSPL_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnSPL_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Test Key
int	CDevCmn::fnSPL_GetTestKey()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetTestKey()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplGetTestKey();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetTestKey():return(%d)", nReturn);
}
	return nReturn;
}

// Get Load Switch
int	CDevCmn::fnSPL_GetLoadSwitch()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetLoadSwitch()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplGetLoadSwitch();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetLoadSwitch():return(%d)", nReturn);
}
	return nReturn;
}

// Get Call Key
int	CDevCmn::fnSPL_GetCallKey()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetCallKey()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

//	if (fnAPL_GetDefineDevice(DEV_SPL))
//		nReturn = m_pMwi->SplGetCallKey();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetCallKey():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SPL FUNCTION(SPL) : SPL OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnSPL_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnSPL_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	UPS FUNCTION(UPS) : UPS SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnUPS_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
	{
		switch (AtmDefine.PowerHandler)							// 2005.12.23
		{
			case NOT_SET:
				break;
			case PNC_TYPE:
				nReturn = m_pMwi->UpsClearErrorCode();
				break;
			case UPS_TYPE:
				nReturn = m_pMwi->UpsClearErrorCode();
				break;
			case EUL_TYPE:
				nReturn = m_pLanPort->UpsClearErrorCode();
				break;
			case EUC_TYPE:
				nReturn = m_pComPort->UpsClearErrorCode();
				break;
		}
	}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnUPS_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	UPS FUNCTION(UPS) : UPS GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrUPS_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrUPS_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_UPS))
	{
		switch (AtmDefine.PowerHandler)							// 2005.12.23
		{
			case NOT_SET:
				break;
			case PNC_TYPE:
				strReturn = m_pMwi->UpsGetErrorCode();
				break;
			case UPS_TYPE:
				strReturn = m_pMwi->UpsGetErrorCode();
				break;
			case EUL_TYPE:
				strReturn = m_pLanPort->UpsGetErrorCode();
				break;
			case EUC_TYPE:
				strReturn = m_pComPort->UpsGetErrorCode();
				break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrUPS_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnUPS_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnUPS_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
	{
		switch (AtmDefine.PowerHandler)							// 2005.12.23
		{
			case NOT_SET:
				break;
			case PNC_TYPE:
				nReturn = m_pMwi->UpsGetDeviceStatus();
				break;
			case UPS_TYPE:
				nReturn = m_pMwi->UpsGetDeviceStatus();
				break;
			case EUL_TYPE:
				nReturn = m_pLanPort->UpsGetDeviceStatus();
				break;
			case EUC_TYPE:
				nReturn = m_pComPort->UpsGetDeviceStatus();
				break;
		}
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Power Status
int	CDevCmn::fnUPS_GetPowerStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnUPS_GetPowerStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
	{
		switch (AtmDefine.PowerHandler)							// 2005.12.23
		{
			case NOT_SET:
				break;
			case PNC_TYPE:
				nReturn = m_pMwi->UpsGetPowerStatus();
				break;
			case UPS_TYPE:
				nReturn = m_pMwi->UpsGetPowerStatus();
				break;
			case EUL_TYPE:
				nReturn = m_pLanPort->UpsGetPowerStatus();
				break;
			case EUC_TYPE:
				nReturn = m_pComPort->UpsGetPowerStatus();
				break;
		}
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_GetPowerStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	UPS FUNCTION(UPS) : UPS OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnUPS_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
	{
		switch (AtmDefine.PowerHandler)							// 2005.12.23
		{
			case NOT_SET:
				break;
			case PNC_TYPE:
				nReturn = m_pMwi->UpsInitialize();
				break;
			case UPS_TYPE:
				nReturn = m_pMwi->UpsInitialize();
				break;
			case EUL_TYPE:
				nReturn = m_pLanPort->UpsInitialize();
				break;
			case EUC_TYPE:
				nReturn = m_pComPort->UpsInitialize();
				break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnUPS_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
	{
		switch (AtmDefine.PowerHandler)							// 2005.12.23
		{
			case NOT_SET:
				break;
			case PNC_TYPE:
				nReturn = m_pMwi->UpsDeinitialize();
				break;
			case UPS_TYPE:
				nReturn = m_pMwi->UpsDeinitialize();
				break;
			case EUL_TYPE:
				nReturn = m_pLanPort->UpsDeinitialize();
				break;
			case EUC_TYPE:
				nReturn = m_pComPort->UpsDeinitialize();
				break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Power Off(Wait Time)
int	CDevCmn::fnUPS_PowerOff(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_PowerOff(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
	{
		switch (AtmDefine.PowerHandler)							// 2005.12.23
		{
			case NOT_SET:
				break;
			case PNC_TYPE:
				nReturn = m_pMwi->UpsPowerOff(nWaitSec);
				break;
			case UPS_TYPE:
				nReturn = m_pMwi->UpsPowerOff(nWaitSec);
				break;
			case EUL_TYPE:
				if (PowerOffDetect & CLERK_POWER_OFF)			// 2005.12.28
					;
				else		
					nReturn = m_pLanPort->UpsPowerOff(nWaitSec);
				break;
			case EUC_TYPE:
				if (PowerOffDetect & CLERK_POWER_OFF)			// 2005.12.28
					;
				else		
					nReturn = m_pComPort->UpsPowerOff(nWaitSec);
				break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_PowerOff(...):return(%d)", nReturn);
	return nReturn;
}

// Power Restart(Wait Time, Restart Time)
int	CDevCmn::fnUPS_PowerRestart(int nWaitSec, int nRestartSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_PowerRestart(nWaitSec[%d], nRestartSec[%d])", nWaitSec, nRestartSec);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
	{
		switch (AtmDefine.PowerHandler)							// 2005.12.23
		{
			case NOT_SET:
				break;
			case PNC_TYPE:
				nReturn = m_pMwi->UpsPowerRestart(nWaitSec, nRestartSec);
				break;
			case UPS_TYPE:
				nReturn = m_pMwi->UpsPowerRestart(nWaitSec, nRestartSec);
				break;
			case EUL_TYPE:
				nReturn = m_pLanPort->UpsPowerRestart(nWaitSec, nRestartSec);
				break;
			case EUC_TYPE:
				nReturn = m_pComPort->UpsPowerRestart(nWaitSec, nRestartSec);
				break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_PowerRestart(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	PIN FUNCTION(PIN) : PIN SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnPIN_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnPIN_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Set Mac Key Name(Key Name)
int	CDevCmn::fnPIN_SetMacKeyName(LPCTSTR szKeyName)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_SetMacKeyName(szKeyName[%s])", szKeyName);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinSetMacKeyName(szKeyName);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_SetMacKeyName(...):return(%d)", nReturn);
	return nReturn;
}

// Set Pin Key Name(Key Name)
int	CDevCmn::fnPIN_SetPinKeyName(LPCTSTR szKeyName)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_SetPinKeyName(szKeyName[%s])", szKeyName);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinSetPinKeyName(szKeyName);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_SetPinKeyName(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	PIN FUNCTION(PIN) : PIN GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrPIN_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPIN_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PIN))
		strReturn = m_pMwi->PinGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPIN_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnPIN_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnPIN_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Pin Key Data
CString CDevCmn::fstrPIN_GetPinKeyData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPIN_GetPinKeyData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PIN))
		strReturn = m_pMwi->PinGetPinKeyData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPIN_GetPinKeyData():return(%s)", strReturn);
	return strReturn;
}

// Get Macing Data
CString CDevCmn::fstrPIN_GetMacingData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPIN_GetMacingData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PIN))
		strReturn = m_pMwi->PinGetMacingData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPIN_GetMacingData():return(%s)", strReturn);
	return strReturn;
}

// GetEncryptedData
CString CDevCmn::fnPIN_GetEncryptedData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_GetEncryptedData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PIN))
		strReturn = m_pMwi->PinGetEncryptedData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_GetEncryptedData():return(%s)", strReturn);
	return strReturn;
}

// GetDecryptedData
CString CDevCmn::fnPIN_GetDecryptedData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_GetDecryptedData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PIN))
		strReturn = m_pMwi->PinGetDecryptedData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_GetDecryptedData():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	PIN FUNCTION(PIN) : PIN OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnPIN_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnPIN_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Entry Enable(EnableMode, MinKeyCount, MaxKeyCount, AutoEnd, ActiveKeys, TerminatorKeys, CardData, Timeout)
int	CDevCmn::fnPIN_EntryEnable(int nEnableMode, int nMinKeyCount, int nMaxKeyCount, int bAutoEnd, LPCTSTR szActiveKeys, LPCTSTR szTerminatorKeys, LPCTSTR szCardData, int nTimeout)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_EntryEnable(nEnableMode[%d], nMinKeyCount[%d], nMaxKeyCount[%d], bAutoEnd[%d], szActiveKeys[%s], szTerminatorKeys[%s], szCardData[%s], nTimeout[%d])", nEnableMode, nMinKeyCount, nMaxKeyCount, bAutoEnd, szActiveKeys, szTerminatorKeys, szCardData, nTimeout);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinEntryEnable(nEnableMode, nMinKeyCount, nMaxKeyCount, bAutoEnd, szActiveKeys, szTerminatorKeys, szCardData, nTimeout);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnPIN_EntryEnable(...):return(%d)", nReturn);
	return nReturn;
}

// Entry Disable
int	CDevCmn::fnPIN_EntryDisable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_EntryDisable()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinEntryDisable();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnPIN_EntryDisable():return(%d)", nReturn);
	return nReturn;
}

// Load IV(Key Name)
int	CDevCmn::fnPIN_LoadIV(LPCTSTR szKeyName)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_LoadIV(szKeyName[%s])", szKeyName);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinLoadIV(szKeyName);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_LoadIV(...):return(%d)", nReturn);
	return nReturn;
}

// Load IV(Key Name, Key Value)
int	CDevCmn::fnPIN_LoadKey(LPCTSTR szKeyName, LPCTSTR szKeyValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_LoadKey(szKeyName[%s], szKeyValue[%s])", szKeyName, szKeyValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinLoadKey(szKeyName, szKeyValue);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_LoadKey(...):return(%d)", nReturn);
	return nReturn;
}

// Macing Data(Mac Data)
int	CDevCmn::fnPIN_MacingData(LPCTSTR szMacData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_MacingData(szMacData[%s])", szMacData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinMacingData(szMacData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_MacingData(...):return(%d)", nReturn);
	return nReturn;
}

// Initialize EPP
int	CDevCmn::fnPIN_InitializeEPP()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_InitializeEPP()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinInitializeEPP();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_InitializeEPP():return(%d)", nReturn);
	return nReturn;
}

// Import Key
int	CDevCmn::fnPIN_ImportKey(LPCTSTR szKeyName, LPCTSTR szKeyEncKeyName, LPCTSTR szValue, LPCTSTR szUse)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_ImportKey(szKeyName[%s], szKeyEncKeyName[%s], szValue[Not traced for security], szUse[%s])", szKeyName, szKeyEncKeyName, szUse);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinImportKey(szKeyName, szKeyEncKeyName, szValue, szUse);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_ImportKey(...):return(%d)", nReturn);
	return nReturn;
}

// EncryptECB
int	CDevCmn::fnPIN_EncryptECB(LPCTSTR szAlgorithm, LPCTSTR szKeyName, LPCTSTR szKeyEncKeyName, short sPadchar, LPCTSTR szCryptData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_EncryptECB(szAlgorithm[%s], szKeyName[%s], szKeyEncKeyName[%s], sPadchar[%d], szCryptData[%s])", szAlgorithm, szKeyName, szKeyEncKeyName, sPadchar, szCryptData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinEncryptECB(szAlgorithm, szKeyName, szKeyEncKeyName, sPadchar, szCryptData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_EncryptECB(...):return(%d)", nReturn);
	return nReturn;
}

// EncryptECB
int	CDevCmn::fnPIN_DecryptECB(LPCTSTR szAlgorithm, LPCTSTR szKeyName, LPCTSTR szKeyEncKeyName, short sPadchar, LPCTSTR szCryptData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_DecryptECB(szAlgorithm[%s], szKeyName[%s], szKeyEncKeyName[%s], sPadchar[%d], szCryptData[%s])", szAlgorithm, szKeyName, szKeyEncKeyName, sPadchar, szCryptData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinDecryptECB(szAlgorithm, szKeyName, szKeyEncKeyName, sPadchar, szCryptData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_DecryptECB(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SENSOR FUNCTION(SNS) : SNS SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnSNS_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSNS_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Clear Operator Switch
int	CDevCmn::fnSNS_ClearOperatorSwitch()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_ClearOperatorSwitch()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsClearOperatorSwitch();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_ClearOperatorSwitch():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SENSOR FUNCTION(SNS) : SNS GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrSNS_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSNS_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_SNS))
		strReturn = m_pMwi->SnsGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSNS_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnSNS_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSNS_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Operator Switch Status
int	CDevCmn::fnSNS_GetOperatorSwitch()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSNS_GetOperatorSwitch()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsGetOperatorSwitch();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_GetOperatorSwitch():return(%d)", nReturn);
}
	return nReturn;
}

// Get Internal Tamper Status
int	CDevCmn::fnSNS_GetInternalTamper()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSNS_GetInternalTamper()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsGetInternalTamper();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_GetInternalTamper():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SENSOR FUNCTION(SNS) : SNS OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnSNS_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnSNS_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	VFD FUNCTION(VFD) : VFD SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code	
int	CDevCmn::fnVFD_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_ClearErrorCode()");

	int nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Set Brightness Level
int	CDevCmn::fnVFD_SetBrightnessLevel(int nLevel)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_SetBrightnessLevel(nLevel[%d])", nLevel);

	int nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdSetBrightnessLevel(nLevel);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_SetBrightnessLevel(...):return(%d)", nReturn);
	return nReturn;
}

// Set Display At Power On
int	CDevCmn::fnVFD_SetDisplayAtPowerOn(int nSMemIndex, int nEMemIndex, int nScrolled, int nInterval)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_SetDisplayAtPowerOn(nSMemIndex[%d], nEMemIndex[%d], nScrolled%d], nInterval[%d])", nSMemIndex, nEMemIndex, nScrolled, nInterval);

	int nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdSetDisplayAtPowerOn(nSMemIndex, nEMemIndex, nScrolled, nInterval);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_SetDisplayAtPowerOn(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	VFD FUNCTION(VFD) : VFD GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrVFD_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrVFD_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_VFD))
		strReturn = m_pMwi->VfdGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrVFD_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnVFD_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	VFD FUNCTION(VFD) : VFD OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnVFD_Initialize(int nPortNum, int nBaudRate)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_Initialize(nPortNum[%d], nBaudRate[%d])", nPortNum, nBaudRate);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdInitialize(nPortNum, nBaudRate);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_Initialize(...):return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnVFD_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Initialize Display
int	CDevCmn::fnVFD_InitializeDisplay()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_InitializeDisplay()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdInitializeDisplay();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_InitializeDisplay():return(%d)", nReturn);
	return nReturn;
}

// Clear Display
int	CDevCmn::fnVFD_ClearDisplay()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_ClearDisplay()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdClearDisplay();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_ClearDisplay():return(%d)", nReturn);
	return nReturn;
}

// Scroll Display
int	CDevCmn::fnVFD_ScrollDisplay(int nShiftNum, int nRepeatNum, int nInterval)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_ScrollDisplay(nShiftNum[%d], nRepeatNum[%d], nInterval[%d])", nShiftNum, nRepeatNum, nInterval);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdScrollDisplay(nShiftNum, nRepeatNum, nInterval);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_ScrollDisplay(...):return(%d)", nReturn);
	return nReturn;
}

// Blink Display
int	CDevCmn::fnVFD_BlinkDisplay(int nPattern, int nNormalTime, int nReverseTime, int nRepeatNumber)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_BlinkDisplay(nPattern[%d], nNormalTime[%d], nReverseTime[%d], nRepeatNumber[%d])", nPattern, nNormalTime, nReverseTime, nRepeatNumber);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdBlinkDisplay(nPattern, nNormalTime, nReverseTime, nRepeatNumber);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_BlinkDisplay(...):return(%d)", nReturn);
	return nReturn;
}

// Curtain Display
int	CDevCmn::fnVFD_CurtainDisplay(int nDirection, int nSpeed, int nPattern)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_CurtainDisplay(nDirection[%d], nSpeed[%d], nPattern[%d])", nDirection, nSpeed, nPattern);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdCurtainDisplay(nDirection, nSpeed, nPattern);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_CurtainDisplay():return(%d)", nReturn);
	return nReturn;
}

// Real Time Text Display
int	CDevCmn::fnVFD_RealTimeTextDisplay(LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_RealTimeTextDisplay(szTextData[%s], nOffsetX[%d], nOffsetY[%d], nCharSzX[%d], nCharSzY[%d], nBoldStyle[%d], nItalic[%d], szFontName[%s])", szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBoldStyle, nItalic, szFontName);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdRealTimeTextDisplay(szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBoldStyle, nItalic, szFontName);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_RealTimeTextDisplay(...):return(%d)", nReturn);
	return nReturn;
}

// Real Time Bit Image Display
int	CDevCmn::fnVFD_RealTimeBitImageDisplay(LPCTSTR szFileName, int nOffsetX, int nOffsetY)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_RealTimeBitImageDisplay(szFileName[%s], nOffsetX[%d], nOffsetY[%d])", szFileName, nOffsetX, nOffsetY);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdRealTimeBitImageDisplay(szFileName, nOffsetX, nOffsetY);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_RealTimeBitImageDisplay(...):return(%d)", nReturn);
	return nReturn;
}

// Real Time Text Bit Image Display
int	CDevCmn::fnVFD_RealTimeTextBitImageDisplay(LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName, LPCTSTR szFileName, int nImgOffsetX, int nImgOffsetY)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_RealTimeTextBitImageDisplay(szTextData[%s], nOffsetX[%d], nOffsetY[%d], nCharSzX[%d], nCharSzY[%d], nBoldStyle[%d], nItalic[%d], szFontName[%s], szFileName[%s], nImgOffsetX[%d], nImgOffsetY[%d])", szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBoldStyle, nItalic, szFontName, szFileName, nImgOffsetX, nImgOffsetY);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdRealTimeTextBitImageDisplay(szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBoldStyle, nItalic, szFontName, szFileName, nImgOffsetX, nImgOffsetY);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_RealTimeTextBitImageDisplay(...):return(%d)", nReturn);
	return nReturn;
}

// FROM Text Save
int	CDevCmn::fnVFD_FROMTextSave(int nMemIndex, LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMTextSave(nMemIndex[%d], szTextData[%s], nOffsetX[%d], nOffsetY[%d], nCharSzX[%d], nCharSzY[%d], nBoldStyle[%d], nItalic[%d], szFontName[%s])", nMemIndex, szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBoldStyle, nItalic, szFontName);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdFROMTextSave(nMemIndex, szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBoldStyle, nItalic, szFontName);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMTextSave(...):return(%d)", nReturn);
	return nReturn;
}

// FROM Bit Image Save
int	CDevCmn::fnVFD_FROMBitImageSave(int nMemIndex, LPCTSTR szFileName, int nOffsetX, int nOffsetY)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMBitImageSave(nMemIndex[%d], szFileName[%s], nOffsetX[%d], nOffsetY[%d])", nMemIndex, szFileName, nOffsetX, nOffsetY);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdFROMBitImageSave(nMemIndex, szFileName, nOffsetX, nOffsetY);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMBitImageSave(...):return(%d)", nReturn);
	return nReturn;
}

// FROM Text Bit Image Save
int	CDevCmn::fnVFD_FROMTextBitImageSave(int nMemIndex, LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBolStyle, int nItalic, LPCTSTR szFontName, LPCTSTR szFileName, int nImgOffsetX, int nImgOffsetY)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMTextBitImageSave(nMemIndex[%d], szTextData[%s], nOffsetX[%d], nOffsetY[%d], nCharSzX[%d], nCharSzY[%d], nBolStyle[%d], nItalic[%d], szFontName[%s], szFileName[%s], nImgOffsetX[%d], nImgOffsetY[%d])", nMemIndex, szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBolStyle, nItalic, szFontName, szFileName, nImgOffsetX, nImgOffsetY);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdFROMTextBitImageSave(nMemIndex, szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBolStyle, nItalic, szFontName, szFileName, nImgOffsetX, nImgOffsetY);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMTextBitImageSave(...):return(%d)", nReturn);
	return nReturn;
}

// FROM Display
int	CDevCmn::fnVFD_FROMDisplay(int nMemIndex)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMDisplay(nMemIndex[%d])", nMemIndex);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdFROMDisplay(nMemIndex);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMDisplay(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	COMMON FUNCTION(CMN) : CMN SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Event Device(Device Id)
int	CDevCmn::fnCMN_ClearEventDevice(int nDevId)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMN_ClearEventDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nDevId = fnAPL_GetDefineDevice(nDevId);
	if ((AtmDefine.CashHandler != BRM_SET)	&&
		(nDevId & DEV_CSH))
		nDevId = (nDevId & (~DEV_CSH)) | DEV_CDU;

	nReturn = m_pMwi->ClearEventDevice(nDevId);

	if (nReturn & DEV_CSH)
		nReturn |= DEV_CSH;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMN_ClearEventDevice(...):return(%x)", nReturn);
	return nReturn;
}

// Clear Status Device(Device Id)
int	CDevCmn::fnCMN_ClearStatusDevice(int nDevId)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMN_ClearStatusDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nDevId = fnAPL_GetDefineDevice(nDevId);
	if ((AtmDefine.CashHandler != BRM_SET)	&&
		(nDevId & DEV_CSH))
		nDevId = (nDevId & (~DEV_CSH)) | DEV_CDU;

	nReturn = m_pMwi->ClearStatusDevice(nDevId);

	if (nReturn & DEV_CSH)
		nReturn |= DEV_CSH;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMN_ClearStatusDevice(...):return(%x)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	COMMON FUNCTION(CMN) : CMN GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Device(Device Id)
int	CDevCmn::fnCMN_GetErrorDevice(int nDevId)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetErrorDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nDevId = fnAPL_GetDefineDevice(nDevId);
	if ((AtmDefine.CashHandler != BRM_SET)	&&
		(nDevId & DEV_CSH))
		nDevId = (nDevId & (~DEV_CSH)) | DEV_CDU;

	nReturn = m_pMwi->GetErrorDevice(nDevId);

	if (nReturn & DEV_CSH)
		nReturn |= DEV_CSH;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetErrorDevice(nDevId[%x]):return(%x)", nDevId, nReturn);
	return nReturn;
}

// Get Timeout Device(Device Id)
int	CDevCmn::fnCMN_GetTimeoutDevice(int nDevId)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetTimeoutDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nDevId = fnAPL_GetDefineDevice(nDevId);
	if ((AtmDefine.CashHandler != BRM_SET)	&&
		(nDevId & DEV_CSH))
		nDevId = (nDevId & (~DEV_CSH)) | DEV_CDU;

	nReturn = m_pMwi->GetTimeoutDevice(nDevId);

	if (nReturn & DEV_CSH)
		nReturn |= DEV_CSH;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetTimeoutDevice(nDevId[%x]):return(%x)", nDevId, nReturn);
	return nReturn;
}

// Get Event Device(Device Id)
int	CDevCmn::fnCMN_GetEventDevice(int nDevId)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetEventDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nDevId = fnAPL_GetDefineDevice(nDevId);
	if ((AtmDefine.CashHandler != BRM_SET)	&&
		(nDevId & DEV_CSH))
		nDevId = (nDevId & (~DEV_CSH)) | DEV_CDU;

	nReturn = m_pMwi->GetEventDevice(nDevId);

	if (nReturn & DEV_CSH)
		nReturn |= DEV_CSH;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetEventDevice(nDevId[%x]):return(%x)", nDevId, nReturn);
	return nReturn;
}

// Get Status Device(Device Id)
int	CDevCmn::fnCMN_GetStatusDevice(int nDevId)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetStatusDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nDevId = fnAPL_GetDefineDevice(nDevId);
	if ((AtmDefine.CashHandler != BRM_SET)	&&
		(nDevId & DEV_CSH))
		nDevId = (nDevId & (~DEV_CSH)) | DEV_CDU;

	nReturn = m_pMwi->GetStatusDevice(nDevId);

	if (nReturn & DEV_CSH)
		nReturn |= DEV_CSH;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetStatusDevice(nDevId[%x]):return(%x)", nDevId, nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	COMMON FUNCTION(CMN) : CMN OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Check Device Action(Device Id, Check Time)
CString	CDevCmn::fstrCMN_CheckDeviceAction(int nDevId, int nCheckTime)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrCMN_CheckDeviceAction(nDevId[%x], nCheckTime[%d])", nDevId, nCheckTime);

	CString			strReturn("");

	nDevId = fnAPL_GetDefineDevice(nDevId);
	if ((AtmDefine.CashHandler != BRM_SET)	&&
		(nDevId & DEV_CSH))
		nDevId = (nDevId & (~DEV_CSH)) | DEV_CDU;

	strReturn = m_pMwi->CheckDeviceAction(nDevId, nCheckTime);

if (strReturn != CHK_NORMAL)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCMN_CheckDeviceAction(nDevId[%x], nCheckTime[%d]):return(%s)", nDevId, nCheckTime, strReturn);
}
	return strReturn;
}

// Scan Device Action(Device Id, Check Time, Event Kind)
int	CDevCmn::fnCMN_ScanDeviceAction(int nDevId, int nScanTime, int nEventKind)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMN_ScanDeviceAction(nDevId[%x], nScanTime[%d], nEventKind[%d])", nDevId, nScanTime, nEventKind);

	int		nReturn = FALSE;

	nDevId = fnAPL_GetDefineDevice(nDevId);
	if ((AtmDefine.CashHandler != BRM_SET)	&&
		(nDevId & DEV_CSH))
		nDevId = (nDevId & (~DEV_CSH)) | DEV_CDU;

	nReturn = m_pMwi->ScanDeviceAction(nDevId, nScanTime, nEventKind);

	if (nReturn & DEV_CSH)
		nReturn |= DEV_CSH;

if (nReturn)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_ScanDeviceAction(nDevId[%x], nScanTime[%d], nEventKind[%d]):return(%d)", nDevId, nScanTime, nEventKind, nReturn);
}
	return nReturn;
}

// Kal Trace Backup(File Name, File Size)
int	CDevCmn::fnCMN_ProcBackupKalTrace(LPCTSTR szFileName, int nFileSize)
{
	int		nReturn = FALSE;

	switch (AtmDefine.MwDevice)									// 2007.09.06
	{
		case MW_KAL:
			break;

		case MW_NEXTWARE:
			nFileSize =	RegGetInt(_REGKEY_NEXTWARE, "TraceFileSize");
			break;

		case MW_KALWOSA:
			break;

		case MW_NEXTWOSA:
			nFileSize =	RegGetInt(_REGKEY_NEXTWARE, "TraceFileSize");
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_ProcBackupKalTrace(szFileName[%s], nFileSize[%d])", szFileName, nFileSize);

//	nReturn = m_pMwi->ProcBackupKalTrace(szFileName, nFileSize);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_ProcBackupKalTrace(...):return(%d)", nReturn);
	return nReturn;
}


int CDevCmn::fnCMN_SaveLineTrace(void* lpData, int len, int SaveType)
{
///////////////////////// Memory Information /////////////////////////////
//	MEMORYSTATUS memStatus;										// 2004.11.05
//	memStatus.dwLength = sizeof(MEMORYSTATUS);
//	GlobalMemoryStatus(&memStatus);
//	DWORD uvm;
//	uvm = memStatus.dwTotalVirtual - memStatus.dwAvailVirtual;
//////////////////////////////////////////////////////////////////////////

    int			i=0,j=0;
    BYTE		ascii[256],ch;
	CString		strEvent, strTemp;
	char		szTempFileName[256];
	FILE*		TempFp;
	int			iPos = 0;

    SYSTEMTIME  lTime;
    GetLocalTime(&lTime);

	if (SaveType == 1)									// BYTE Mode
	{
		if(len==0 || lpData==NULL);
		else
		{
			BYTE *pBuf = (BYTE *)lpData;

			memset(ascii,0x00,sizeof(ascii));
			for(i=0;i<len;i++)
			{
				if((i%16)==0)   
				{
					strTemp.Format("%05d:%05d ",i,i+16);
					strEvent += strTemp;
				}

				strTemp.Format("%02x ",pBuf[i]);
				strEvent += strTemp;

				if(j==7)	strEvent += " ";

				ch = pBuf[i];

				if(ch>=0x20 && ch <= 0x7e)
						ascii[j] = ch;
				else    ascii[j] = '.';

				j++;
				if(j==16)
				{
					strTemp.Format("%-16.16s\r\n",ascii);
					strEvent += strTemp;
					memset(ascii,0x00,sizeof(ascii));
					j = 0;
				}
			}

			if(j)
			{
				for(;j<16;j++)
				{
					strEvent += "   ";
					if(j && (j%8)==0)
						strEvent += " ";
				}
				strTemp.Format("%-16.16s\r\n",ascii);
				strEvent += strTemp;
			}

		}
		
		strEvent += "-----------------------------------------------------------------------------\r\n";
	}
	else												// string Mode
		strEvent.Format("%s\r\n",lpData);

////////////////////////////记录到电子流水中 start by zjd
	memset(szTempFileName, 0, sizeof(szTempFileName));
	_mkdir(_EJR_DETAILSEND);
	sprintf(szTempFileName, "%s\\%6.6s%8.8s.LINE.LOG", _EJR_DETAILSEND, 
		&(GetDate().GetBuffer(0)[2]),
		m_pProfile->NETWORK.AtmSerialNum);
// 2007.06.08
    if( ( TempFp = fopen( szTempFileName, "a+b" ) ) == NULL )
        TempFp = fopen( szTempFileName, "wb" );
	if (TempFp)
	{
		fwrite(strEvent.GetBuffer(0), strEvent.GetLength(), 1, TempFp);
		fflush(TempFp);
		fclose(TempFp);
	}
///////////////////////////记录到电子流水中 end
	return TRUE;
}

// Agent Rest Wait Result
int CDevCmn::fnAPL_AgentRestWaitResult()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentRestWaitResult()");

	int				nReturn = FALSE;
	CString			strResult("");
	CStringArray	strResultArray;
	CString			strTemp("");
	ULONG WiatTime2 = TimerSet(AGENT_DEVRSP_TIME);				// 125 Sec : 2007.11.01
	while (TRUE)
	{	 
		Delay_Msg(100);											// Performance Adjust
		if( bAgentRestFlag == AGENT_DEVICE_SUCCESS )
		{
			nReturn = TRUE;
            break;
		}
		else
		if( bAgentRestFlag == AGENT_DEVICE_FAIL )
		{
			nReturn = FALSE;
            break;
		}
		else
		if (CheckTimer(WiatTime2))							// Time Out
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentRestWaitResult()CheckTimer");

            nReturn = FALSE;
            break;
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentRestWaitResult()bAgentRestFlag[%d]", bAgentRestFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentRestWaitResult()[%d]", nReturn);	
  bAgentRestFlag = 0;
   return nReturn;
 }

// Agent Rest Result 
int CDevCmn::fnAPL_ShareResetDeviceResult(int bShareRestReturnFlag)
{
	int	nReturn = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_DeviceRestResult()");
    if(bShareRestReturnFlag == 0)
		bAgentRestFlag = AGENT_DEVICE_FAIL;
	else
	if(bShareRestReturnFlag == 1)
		bAgentRestFlag = AGENT_DEVICE_SUCCESS;
	nReturn = bShareRestReturnFlag; 

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_DeviceRestResult()[%d]", nReturn);
	return nReturn;
}
