#include "stdafx.h"
#include "CmnLibEx.h"
#include "MonitorClient.h"

//注册监控使用者版本信息，该信息将用来计算授权码
//MONITORCLIENT_API void HCMonitor2_Register( const char* pVersion );
void WINAPI hy_HCMonitor2_Register()
{
	HCMonitor2_Register(AUTH_VERSION);
}

//获取摄像头个数，-1表示失败（可能Monitor.exe没有启动或者其他原因 ）
//MONITORCLIENT_API int HCMonitor2_GetDeviceCount();
int WINAPI hy_HCMonitor2_GetDeviceCount()
{
	return HCMonitor2_GetDeviceCount();
}

//开始一个客户服务
//开始录制之前必须先开始客户服务
//MONITORCLIENT_API DWORD HCMonitor2_CustomerServiceBegin();
DWORD WINAPI hy_HCMonitor2_CustomerServiceBegin()
{
	return HCMonitor2_CustomerServiceBegin();
}

//指定的客户服务结束
//MONITORCLIENT_API void HCMonitor2_CustomerServiceEnd( DWORD dwCustomer );
void WINAPI hy_HCMonitor2_CustomerServiceEnd( DWORD dwCustomer )
{
	HCMonitor2_CustomerServiceEnd( dwCustomer );
}

//添加指定客户的信息
//！！！！！！！！！注意添加信息不会自动开始录像！！！！！！！！！而只是在已经开始的录像中增加信息
//MONITORCLIENT_API void HCMonitor2_AddInfo( DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult );
void WINAPI hy_HCMonitor2_AddInfo( DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult )
{
	HCMonitor2_AddInfo( dwCustomer, pCardNo, pTrans, pResult );
}

//开始录制
//iSlotNum为摄像头编号（从零开始，即0-第一个摄像头；1-第二个摄像头）
//dwCustomer 客户号（由CustomerServiceBegin申请到）
//pCardNo 卡号
//pTrans 交易
//pResult 交易结果
//MONITORCLIENT_API void HCMonitor2_StartCap( DWORD dwSlotNum, DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult );
void WINAPI hy_HCMonitor2_StartCap( DWORD dwSlotNum, DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult )
{
	HCMonitor2_StartCap( dwSlotNum, dwCustomer, pCardNo, pTrans, pResult );
}

//开始录制，录制一段时间后自动结束
//iSlotNum为摄像头编号（从零开始，即0-第一个摄像头；1-第二个摄像头）
//dwSeconds录制的时间，以秒为单位
//dwCustomer 客户号（由CustomerServiceBegin申请到）
//pCardNo 卡号
//pTrans 交易
//pResult 交易结果
//MONITORCLIENT_API void HCMonitor2_StartCapWithCapSeconds( DWORD dwSlotNum, DWORD dwSeconds, DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult );
void WINAPI hy_HCMonitor2_StartCapWithCapSeconds( DWORD dwSlotNum, DWORD dwSeconds, DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult )
{
	HCMonitor2_StartCapWithCapSeconds( dwSlotNum, dwSeconds, dwCustomer, pCardNo, pTrans, pResult );
}

//停止录制
//iSlotNum为摄像头编号（从零开始，即0-第一个摄像头；1-第二个摄像头）
//MONITORCLIENT_API void HCMonitor2_StopCap( DWORD dwSlotNum );
void WINAPI hy_HCMonitor2_StopCap( DWORD dwSlotNum )
{
	HCMonitor2_StopCap( dwSlotNum );
}

//再过一段时间后自动结束
//iSlotNum为摄像头编号（从零开始，即0-第一个摄像头；1-第二个摄像头）
//dwSeconds延时的时间，以秒为单位
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

//开始预录
//iSlotNum为摄像头编号（从零开始，即0-第一个摄像头；1-第二个摄像头）
////dwSeconds预录的时间，以秒为单位
//
//开始预录后，调用HCMonitor2_StartCap/HCMonitor2_StartCapWithCapSeconds则进入正常录制状态，预录的文件与录制的文件用分号隔开保存在数据库中。
//所以，在查询的时候要注意返回的文件名可能是多个文件组成的。
//
//开始预录后，如果调用HCMonitor2_StopCap/HCMonitor2_StopCapAfterSeconds，则在停止的时候，预录同时停止，且预录文件被删除
//MONITORCLIENT_API void HCMonitor2_StartPreRecording( DWORD dwSlotNum, DWORD dwSeconds );
void WINAPI hy_HCMonitor2_StartPreRecording( DWORD dwSlotNum, DWORD dwSeconds )
{
	HCMonitor2_StartPreRecording( dwSlotNum, dwSeconds );
}

//Monitor2程序可以退出了
//有些摄像头驱动有句柄泄漏, 没有办法处理的情况下只好让Monitor2.exe重新启动以释放资源
//调用者认为一小段时间内不会录像可以让Monitor2.exe重新启动的时候调用该函数.Monitor2.exe会再判断需要重新启动的情况下重新启动
//MONITORCLIENT_API void HCMonitor2_RestartAllowed();
void WINAPI hy_HCMonitor2_RestartAllowed()
{
	HCMonitor2_RestartAllowed();
}
