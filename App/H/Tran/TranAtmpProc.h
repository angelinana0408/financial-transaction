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

	//���   BY HYL
	long    SendFileLength;							//�ļ�����
	long    DownFilelength;
	int     SendBufferLength;						//�ϴ����ݰ�����
    long    SetBegin;								//ƫ����
	char    DetailTblTemp[400];						//���ݳ���
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
