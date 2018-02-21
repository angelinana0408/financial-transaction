#include "AccApi.h"

//注册监控使用者版本信息，该信息将用来计算授权码
//MONITORCLIENT_API void HCMonitor2_Register( const char* pVersion );
void WINAPI Acc_SetTxCode(int callid,int txcode,int *ret)
{
	SetTxCode(int callid,int txcode,int *ret);
}
void SetNameValue(int callid,const char *name,const char *value,int *ret);

void GetNameValue(int callid,const char *name,char *value,char *szMsg,int *ret);

void GetNameValueByRec(int callid,int recNo,const char *name,char *value,char *szMsg,int *ret);

void GetTotalRecCount(int callid,int *rec,char *szMsg,int *ret);

void GetReturnCode(int callid,int *retcode,char *szMsg,int *ret);

void GetReturnMsg(int callid,char *msg,char *szMsg,int *ret);

void SendRequest(int callid,int *rec,char *retcode,char *szMsg,int *ret);

void DeleteTmpFile(int callid);
