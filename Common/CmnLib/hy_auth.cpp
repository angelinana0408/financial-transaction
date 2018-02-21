#include "stdafx.h"
#include "CmnLibEx.h"

//////////////////////////////////////////////////////////////////////
//	Auth Define
//////////////////////////////////////////////////////////////////////
//#define AUTH_OK			0
//#define AUTH_TRYOUT     1
//#define AUTH_FAIL       2
//
//#define AUTH_VERSION	"A5000B"
//
//pAddinfoΪ������Ϣ��һ���ó���İ汾�ţ�
int CheckAuthorithCodeFromFile3( char* pAddInfo );

//pAddinfoΪ������Ϣ��һ���ó���İ汾�ţ�
bool GetIdentifyCode3( char* pBuffer, int iSize, char* pAddInfo );

//pIdenCodeΪ������
//pAuthCodeΪ��Ȩ��
void SaveAuthorithCode3( const unsigned char* pIdenCode, const unsigned char* pAuthCode );

//////////////////////////////////////////////////////////////////////
int WINAPI hy_CheckAuthorithCodeFromFile3()
{
	//return CheckAuthorithCodeFromFile3(AUTH_VERSION);
	return AUTH_OK;										// liyi 2008.09.10
}

bool WINAPI hy_GetIdentifyCode3(char* pBuffer, int iSize)
{
	return GetIdentifyCode3(pBuffer, iSize,  AUTH_VERSION);
}

void WINAPI hy_SaveAuthorithCode3(const unsigned char* pIdenCode, const unsigned char* pAuthCode)
{
	SaveAuthorithCode3(pIdenCode, pAuthCode);
}

