/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranAtmPProc_H_INCLUDED_)
#define _TranAtmPProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Library Variable
/////////////////////////////////////////////////////////////////////////////
	HMERECV	HMEATMPRecv;
	HMESEND	HMEATMPSend;

	//监控   BY HYL
	long    SendFileLength;							//文件长度
	long    DownFilelength;
	int     SendBufferLength;						//上传数据包长度
    long    SetBegin;								//偏移量
	char    DetailTblTemp[400];						//数据长度
 	CString strDownLoadFileName;
	char    FileEnd;
	char    szUploadFileNamePath[256];
	int     ADFileType;
	CString	ADFileList;
    CStringArray ADFileNameList;
	CString     ADFileUseTime;
	CString     ADFileEndTime;
	CString	SWFileList;
    CStringArray SWFileNameList;
	CString     SWUpdateFileUseTime;
	CString  	SWFileVersion;
	
/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Library Function
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//  fnAPL_AtmpCommand   by zjd
/////////////////////////////////////////////////////////////////////////////
	int		fnAPL_UnpackAtmpCmd(char* Buffer, char* MessageKind);
	int		fnAPL_PackAtmpCmd(char* SendBuffer, bool bRejectFlag);

	int		fnAPP_TranATMPSendDetailNotice();
	int		fnAPP_TranATMPSendDetail();
	int		fnAPP_TranATMPAdDownloadNotice();
	int		fnAPP_TranATMPAdDownload();
	int		fnAPP_TranATMPAdverDownLoadFileProc(CString TempNameList,bool* bOverFlag);
	int		fnAPP_TranATMPNoticeDownLoadFileProc(CString TempNameList,bool* bOverFlag);
	int		fnAPP_TranATMPCautionDownLoadFileProc(CString TempNameList,bool* bOverFlag);
	int		fnAPP_TranATMPSoftUpdateNotice();
	int		fnAPP_TranATMPSoftUpdate();
	int		fnAPP_TranATMPDownloadFileProc(CString TempNameList,bool* SWbOverFlag);

/////////////////////////////////////////////////////////////////////////////
#endif
