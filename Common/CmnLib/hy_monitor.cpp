#include "stdafx.h"
#include "CmnLibEx.h"
#include "MonitorClient.h"

//ע����ʹ���߰汾��Ϣ������Ϣ������������Ȩ��
//MONITORCLIENT_API void HCMonitor2_Register( const char* pVersion );
void WINAPI hy_HCMonitor2_Register()
{
	HCMonitor2_Register(AUTH_VERSION);
}

//��ȡ����ͷ������-1��ʾʧ�ܣ�����Monitor.exeû��������������ԭ�� ��
//MONITORCLIENT_API int HCMonitor2_GetDeviceCount();
int WINAPI hy_HCMonitor2_GetDeviceCount()
{
	return HCMonitor2_GetDeviceCount();
}

//��ʼһ���ͻ�����
//��ʼ¼��֮ǰ�����ȿ�ʼ�ͻ�����
//MONITORCLIENT_API DWORD HCMonitor2_CustomerServiceBegin();
DWORD WINAPI hy_HCMonitor2_CustomerServiceBegin()
{
	return HCMonitor2_CustomerServiceBegin();
}

//ָ���Ŀͻ��������
//MONITORCLIENT_API void HCMonitor2_CustomerServiceEnd( DWORD dwCustomer );
void WINAPI hy_HCMonitor2_CustomerServiceEnd( DWORD dwCustomer )
{
	HCMonitor2_CustomerServiceEnd( dwCustomer );
}

//���ָ���ͻ�����Ϣ
//������������������ע�������Ϣ�����Զ���ʼ¼�񣡣�����������������ֻ�����Ѿ���ʼ��¼����������Ϣ
//MONITORCLIENT_API void HCMonitor2_AddInfo( DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult );
void WINAPI hy_HCMonitor2_AddInfo( DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult )
{
	HCMonitor2_AddInfo( dwCustomer, pCardNo, pTrans, pResult );
}

//��ʼ¼��
//iSlotNumΪ����ͷ��ţ����㿪ʼ����0-��һ������ͷ��1-�ڶ�������ͷ��
//dwCustomer �ͻ��ţ���CustomerServiceBegin���뵽��
//pCardNo ����
//pTrans ����
//pResult ���׽��
//MONITORCLIENT_API void HCMonitor2_StartCap( DWORD dwSlotNum, DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult );
void WINAPI hy_HCMonitor2_StartCap( DWORD dwSlotNum, DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult )
{
	HCMonitor2_StartCap( dwSlotNum, dwCustomer, pCardNo, pTrans, pResult );
}

//��ʼ¼�ƣ�¼��һ��ʱ����Զ�����
//iSlotNumΪ����ͷ��ţ����㿪ʼ����0-��һ������ͷ��1-�ڶ�������ͷ��
//dwSeconds¼�Ƶ�ʱ�䣬����Ϊ��λ
//dwCustomer �ͻ��ţ���CustomerServiceBegin���뵽��
//pCardNo ����
//pTrans ����
//pResult ���׽��
//MONITORCLIENT_API void HCMonitor2_StartCapWithCapSeconds( DWORD dwSlotNum, DWORD dwSeconds, DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult );
void WINAPI hy_HCMonitor2_StartCapWithCapSeconds( DWORD dwSlotNum, DWORD dwSeconds, DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult )
{
	HCMonitor2_StartCapWithCapSeconds( dwSlotNum, dwSeconds, dwCustomer, pCardNo, pTrans, pResult );
}

//ֹͣ¼��
//iSlotNumΪ����ͷ��ţ����㿪ʼ����0-��һ������ͷ��1-�ڶ�������ͷ��
//MONITORCLIENT_API void HCMonitor2_StopCap( DWORD dwSlotNum );
void WINAPI hy_HCMonitor2_StopCap( DWORD dwSlotNum )
{
	HCMonitor2_StopCap( dwSlotNum );
}

//�ٹ�һ��ʱ����Զ�����
//iSlotNumΪ����ͷ��ţ����㿪ʼ����0-��һ������ͷ��1-�ڶ�������ͷ��
//dwSeconds��ʱ��ʱ�䣬����Ϊ��λ
//MONITORCLIENT_API void HCMonitor2_StopCapAfterSeconds( DWORD dwSlotNum, DWORD dwSeconds );
void WINAPI hy_HCMonitor2_StopCapAfterSeconds( DWORD dwSlotNum, DWORD dwSeconds )
{
	HCMonitor2_StopCapAfterSeconds( dwSlotNum, dwSeconds );
}

//MONITORCLIENT_API void HCMonitor2_SetVideoSize( DWORD dwSlotNum, DWORD dwWidth, DWORD dwHeight );
void WINAPI hy_HCMonitor2_SetVideoSize( DWORD dwSlotNum, DWORD dwWidth, DWORD dwHeight )
{
	HCMonitor2_SetVideoSize( dwSlotNum, dwWidth, dwHeight );
}

//��ʼԤ¼
//iSlotNumΪ����ͷ��ţ����㿪ʼ����0-��һ������ͷ��1-�ڶ�������ͷ��
////dwSecondsԤ¼��ʱ�䣬����Ϊ��λ
//
//��ʼԤ¼�󣬵���HCMonitor2_StartCap/HCMonitor2_StartCapWithCapSeconds���������¼��״̬��Ԥ¼���ļ���¼�Ƶ��ļ��÷ֺŸ������������ݿ��С�
//���ԣ��ڲ�ѯ��ʱ��Ҫע�ⷵ�ص��ļ��������Ƕ���ļ���ɵġ�
//
//��ʼԤ¼���������HCMonitor2_StopCap/HCMonitor2_StopCapAfterSeconds������ֹͣ��ʱ��Ԥ¼ͬʱֹͣ����Ԥ¼�ļ���ɾ��
//MONITORCLIENT_API void HCMonitor2_StartPreRecording( DWORD dwSlotNum, DWORD dwSeconds );
void WINAPI hy_HCMonitor2_StartPreRecording( DWORD dwSlotNum, DWORD dwSeconds )
{
	HCMonitor2_StartPreRecording( dwSlotNum, dwSeconds );
}

//Monitor2��������˳���
//��Щ����ͷ�����о��й©, û�а취����������ֻ����Monitor2.exe�����������ͷ���Դ
//��������ΪһС��ʱ���ڲ���¼�������Monitor2.exe����������ʱ����øú���.Monitor2.exe�����ж���Ҫ�����������������������
//MONITORCLIENT_API void HCMonitor2_RestartAllowed();
void WINAPI hy_HCMonitor2_RestartAllowed()
{
	HCMonitor2_RestartAllowed();
}
