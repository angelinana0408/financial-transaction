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
//此api步骤
//1、设置交易码 SetTxCode
// 2、设置请求字段值 SetNameValue
// 3、发送请求   SendRequest
// 4、取返回结果 GetReturnCode  GetTotalRecCount GetReturnMsg
// 5、取返回的具体字段 GetNameValue GetNameValueByRec
//6、 删除临时文件 DeleteTmpFile


#ifdef __cplusplus
extern "C"
{

	// 函数名: SetTxCode
	// 编程  : 王明松 2007-12-6 18:01:39
	// 返回  : void 
	// 输入参数  : int callid  语音callid
	// 输入参数  : int txcode  交易码
	// 输出参数  : int *ret    函数调用返回码 <0失败，0成功)
	// 描述  : 设置交易码到发送数据缓冲
	DLL_EXPORT void SetTxCode(int callid,int txcode,int *ret);

	// 函数名: SetNameValue
	// 编程  : 王明松 2007-12-6 18:02:55
	// 返回  : void 
	// 输入参数  : int callid 语音callid
	// 输入参数  : const char *name 字段名称
	// 输入参数  : const char *value 字段值
	// 输入参数  : int *ret 函数调用返回码 <0失败，0成功)
	// 描述  : 设置字段值到发送数据缓冲
	DLL_EXPORT void SetNameValue(int callid,const char *name,const char *value,int *ret);

	// 函数名: GetNameValue
	// 编程  : 王明松 2007-12-6 18:04:21
	// 返回  : void 
	// 输入参数  : int callid 语音callid
	// 输入参数  : const char *name 字段名称
	// 输出参数  : char *value 字段值
	// 输出参数  : char *szMsg 出错信息
	// 输出参数  : int *ret 函数调用返回码 <0失败，0成功)
	// 描述  : 从应答数据缓冲中读出字段值,用于应答是一个数据包的情况下
	DLL_EXPORT void GetNameValue(int callid,const char *name,char *value,char *szMsg,int *ret);

	// 函数名: GetNameValueByRec
	// 编程  : 王明松 2007-12-6 18:06:12
	// 返回  : void 
	// 输入参数  : int callid 语音callid
	// 输入参数  : int recNo   记录id 从0开始
	// 输入参数  : const char *name 字段名称
	// 输出参数  : char *value 字段值
	// 输出参数  : char *szMsg 出错信息
	// 输出参数  : int *ret 函数调用返回码 <0失败，0成功)
	// 描述  : 从应答数据文件中根据记录ID取出字段值，用于多包应答的情况下，即记录数>1的时候
	DLL_EXPORT void GetNameValueByRec(int callid,int recNo,const char *name,char *value,char *szMsg,int *ret);

	// 函数名: GetTotalRecCount
	// 编程  : 王明松 2007-12-6 18:09:57
	// 返回  : void 
	// 输入参数  : int callid 语音callid
	// 输出参数  : int *rec 记录数
	// 输出参数  : char *szMsg 出错信息
	// 输出参数  : int *ret 函数调用返回码 <0失败，0成功)
	// 描述  : 从应答数据中取出记录数
	DLL_EXPORT void GetTotalRecCount(int callid,int *rec,char *szMsg,int *ret);

	// 函数名: GetReturnCode
	// 编程  : 王明松 2007-12-6 18:11:56
	// 返回  : void 
	// 输入参数  : int callid 语音callid
	// 输出参数  : int *retcode 返回码
	// 输出参数  : char *szMsg 出错信息
	// 输出参数  : int *ret 函数调用返回码 <0失败，0成功)
	// 描述  : 从应答数据中取出应答返回码
	DLL_EXPORT void GetReturnCode(int callid,int *retcode,char *szMsg,int *ret);

	// 函数名: GetReturnMsg
	// 编程  : 王明松 2007-12-6 18:13:08
	// 返回  : void 
	// 输入参数  : int callid 语音callid
	// 输出参数  : char *msg 返回信息
	// 输出参数  : char *szMsg  出错信息
	// 输出参数  : int *ret 函数调用返回码 <0失败，0成功)
	// 描述  : 从应答数据中取出应答返回信息
	DLL_EXPORT void GetReturnMsg(int callid,char *msg,char *szMsg,int *ret);

	// 函数名: SendRequest
	// 编程  : 王明松 2007-12-6 18:15:09
	// 返回  : void 
	// 输入参数  : int callid 语音callid
	// 输出参数  : int *rec 记录数
	// 输出参数  : char *retcode 返回码
	// 输出参数  : char *szMsg  出错信息 ret=0时为返回信息
	// 输出参数  : int *ret 函数调用返回码 <0失败，0成功)
	// 描述  : 发送请求至账户管家进行处理并返回
	DLL_EXPORT void SendRequest(int callid,int *rec,char *retcode,char *szMsg,int *ret);

	// 函数名: DeleteTmpFile
	// 编程  : 王明松 2007-12-6 20:28:03
	// 返回  : void 
	// 参数  : int callid
	// 描述  : 删除交易callid所用的临时数据文件
	DLL_EXPORT void DeleteTmpFile(int callid);
}
#endif

#endif // !defined(AFX_ACCAPI_H__80FF4318_D19D_47AB_8915_3D9E754ABD7A__INCLUDED_)
