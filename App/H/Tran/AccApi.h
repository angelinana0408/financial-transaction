// AccApi.h: interface for the CAccApi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCAPI_H__80FF4318_D19D_47AB_8915_3D9E754ABD7A__INCLUDED_)
#define AFX_ACCAPI_H__80FF4318_D19D_47AB_8915_3D9E754ABD7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



// #ifdef EXPORTS
	#define DLL_EXPORT __declspec(dllexport)
// #else
// 	#ifdef WIN_DLL
// 		#define DLL_EXPORT __declspec(dllimport)
// 	#else
// 		#define DLL_EXPORT 
// 	#endif
// #endif
//��api����
//1�����ý����� SetTxCode
// 2�����������ֶ�ֵ SetNameValue
// 3����������   SendRequest
// 4��ȡ���ؽ�� GetReturnCode  GetTotalRecCount GetReturnMsg
// 5��ȡ���صľ����ֶ� GetNameValue GetNameValueByRec
//6�� ɾ����ʱ�ļ� DeleteTmpFile


#ifdef __cplusplus
extern "C"
{

	// ������: SetTxCode
	// ���  : ������ 2007-12-6 18:01:39
	// ����  : void 
	// �������  : int callid  ����callid
	// �������  : int txcode  ������
	// �������  : int *ret    �������÷����� <0ʧ�ܣ�0�ɹ�)
	// ����  : ���ý����뵽�������ݻ���
	DLL_EXPORT void SetTxCode(int callid,int txcode,int *ret);

	// ������: SetNameValue
	// ���  : ������ 2007-12-6 18:02:55
	// ����  : void 
	// �������  : int callid ����callid
	// �������  : const char *name �ֶ�����
	// �������  : const char *value �ֶ�ֵ
	// �������  : int *ret �������÷����� <0ʧ�ܣ�0�ɹ�)
	// ����  : �����ֶ�ֵ���������ݻ���
	DLL_EXPORT void SetNameValue(int callid,const char *name,const char *value,int *ret);

	// ������: GetNameValue
	// ���  : ������ 2007-12-6 18:04:21
	// ����  : void 
	// �������  : int callid ����callid
	// �������  : const char *name �ֶ�����
	// �������  : char *value �ֶ�ֵ
	// �������  : char *szMsg ������Ϣ
	// �������  : int *ret �������÷����� <0ʧ�ܣ�0�ɹ�)
	// ����  : ��Ӧ�����ݻ����ж����ֶ�ֵ,����Ӧ����һ�����ݰ��������
	DLL_EXPORT void GetNameValue(int callid,const char *name,char *value,char *szMsg,int *ret);

	// ������: GetNameValueByRec
	// ���  : ������ 2007-12-6 18:06:12
	// ����  : void 
	// �������  : int callid ����callid
	// �������  : int recNo   ��¼id ��0��ʼ
	// �������  : const char *name �ֶ�����
	// �������  : char *value �ֶ�ֵ
	// �������  : char *szMsg ������Ϣ
	// �������  : int *ret �������÷����� <0ʧ�ܣ�0�ɹ�)
	// ����  : ��Ӧ�������ļ��и��ݼ�¼IDȡ���ֶ�ֵ�����ڶ��Ӧ�������£�����¼��>1��ʱ��
	DLL_EXPORT void GetNameValueByRec(int callid,int recNo,const char *name,char *value,char *szMsg,int *ret);

	// ������: GetTotalRecCount
	// ���  : ������ 2007-12-6 18:09:57
	// ����  : void 
	// �������  : int callid ����callid
	// �������  : int *rec ��¼��
	// �������  : char *szMsg ������Ϣ
	// �������  : int *ret �������÷����� <0ʧ�ܣ�0�ɹ�)
	// ����  : ��Ӧ��������ȡ����¼��
	DLL_EXPORT void GetTotalRecCount(int callid,int *rec,char *szMsg,int *ret);

	// ������: GetReturnCode
	// ���  : ������ 2007-12-6 18:11:56
	// ����  : void 
	// �������  : int callid ����callid
	// �������  : int *retcode ������
	// �������  : char *szMsg ������Ϣ
	// �������  : int *ret �������÷����� <0ʧ�ܣ�0�ɹ�)
	// ����  : ��Ӧ��������ȡ��Ӧ�𷵻���
	DLL_EXPORT void GetReturnCode(int callid,int *retcode,char *szMsg,int *ret);

	// ������: GetReturnMsg
	// ���  : ������ 2007-12-6 18:13:08
	// ����  : void 
	// �������  : int callid ����callid
	// �������  : char *msg ������Ϣ
	// �������  : char *szMsg  ������Ϣ
	// �������  : int *ret �������÷����� <0ʧ�ܣ�0�ɹ�)
	// ����  : ��Ӧ��������ȡ��Ӧ�𷵻���Ϣ
	DLL_EXPORT void GetReturnMsg(int callid,char *msg,char *szMsg,int *ret);

	// ������: SendRequest
	// ���  : ������ 2007-12-6 18:15:09
	// ����  : void 
	// �������  : int callid ����callid
	// �������  : int *rec ��¼��
	// �������  : char *retcode ������
	// �������  : char *szMsg  ������Ϣ ret=0ʱΪ������Ϣ
	// �������  : int *ret �������÷����� <0ʧ�ܣ�0�ɹ�)
	// ����  : �����������˻��ܼҽ��д�������
	DLL_EXPORT void SendRequest(int callid,int *rec,char *retcode,char *szMsg,int *ret);

	// ������: DeleteTmpFile
	// ���  : ������ 2007-12-6 20:28:03
	// ����  : void 
	// ����  : int callid
	// ����  : ɾ������callid���õ���ʱ�����ļ�
	DLL_EXPORT void DeleteTmpFile(int callid);
}
#endif

#endif // !defined(AFX_ACCAPI_H__80FF4318_D19D_47AB_8915_3D9E754ABD7A__INCLUDED_)
