//******************************************************************************
//
// File Name        : Cryptogram.cpp
// Description      : ��ȣȭ �Լ�����
// Author           : ������
// Initial Date     : 2003.11.11
// Revision History : 
// Notes            : Ư���� ���� ����ϱ� ���� �Լ��̹Ƿ� �ٸ� ���� ����� 
//                    �����Ͽ��� ����ϱ� �ٶ�. SDK�� ��Ƽ� ����� ��(����)
//                    
//                    @ ����
//                      �̰��� �� �Ἥ �������� ��.
//                       #pragma comment( lib, "Advapi32.lib")
//                     
//                    @ SDK��
//                      �������
//                       #include <wincrypt.h>
//
//                    @ Non SDK��
//                      Directory�� Include�� �߰� �Ұ�
//                       -> .\LOGCRYPT\CMNLIB\CRYPT
//
//                      Directory�� Lib�� �߰� �Ұ�
//                       -> .\LOGCRYPT\CMNLIB\CRYPT
//
//                    @ �߰� ����
//                      ������ GetLastError()�� Ȯ�� �Ұ�.
// 
//                      ��ȣȭ�� 2���̻��� ���۸� ����ϵ��ϵǾ� �ִ�.
//                      ����, 14700Bytes ������ �α׸� ��ȣȭ, ��ȣȭ�ؾ��Ѵ�.
// 
//******************************************************************************
 
#include "stdafx.h"
#include "CmnLibEx.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
//#include <wincrypt.h>							// SDK��
#include "wincrypt.h"							// Non SDK��
#include "Cryptogram.h"

//////////////////////////////////////////////////////////////////////
// Define
//////////////////////////////////////////////////////////////////////
#define WM_PROGRESS_NOTIFY WM_USER + 1110

char szPasswordTable[PWD_MAX_COUNT][PWD_MAX_SIZE + 1] = 
{	"Nautilus Hyosung ATM Log Password List.",
	"Auto Teller Machine",
	"The Kalignite Platform",
	"Windows Platform",
	"DOS Platform",
	"ActiveX Module Interface",
	"Cryptogram Algorism",
	"aBcdefgHigklmnopqrstuvwxyz",
	"12345678901234567890",
	"Have a Good Day!!"
};

//==============================================================================
// 
// Function Name : NH_CryptoStart
// Description   : ��ȣȭ�� ���� ���� �Լ��� �ʼ��� ���� ó���� ȣ�� �Ѵ�.
// Parameters    : HCRYPTPROV&   : hCryptProv  : I/O : Address to which the function 
//                                                     copies a handle to the CSP.
//                                                     (Cryptographic Service Provider)
//                 HCRYPTHASH&   : hHash       : I/O : Address to which the function 
//                                                     copies a handle to the new 
//                                                     hash object.
// Return        : ���� : 0 (NH_CRYPT_SUCCESS)
//                 ���� : -1 (NH_CRYPT_ERROR)
// Release       : VER 0.9
// Initial Date  : 2003.11.11
// Notes         : 
// 
//==============================================================================
int NH_CryptoStart(HCRYPTPROV& hCryptProv, 
				   HCRYPTHASH& hHash)
{
	int nRet			= NH_CRYPT_SUCCESS;
 
	// ����Ʈ CSP�� �ڵ��� ���´�. CryptoAPI�� ���� ù��° �Լ� 
	if( !CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0) )
	{
		// Key Set�� ���ٸ� ������ش�.
		if( !CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET) )
		{
			TRACE("Error during CryptAcquireContext!");
			return (nRet = NH_CRYPT_ERROR);
		}
	}
	TRACE("A cryptographic provider has been acquired. \n");

	// hash ������Ʈ�� �����Ѵ�.
	if( !CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash) )
	{ 
		 TRACE("Error during CryptCreateHash!\n");
		 return (nRet = NH_CRYPT_ERROR);
	}  
	TRACE("A hash object has been created. \n");
	
	return nRet;
}

//==============================================================================
// 
// Function Name : NH_HashPWD
// Description   : ��ȣ�� �ɾ��� �н������� �ؽ����� ��� ����.
// Parameters    : HCRYPTPROV& : hCryptProv  : I/O : Address to which the function 
//                                                   copies a handle to the CSP.
//                                                   (Cryptographic Service Provider)
//                 HCRYPTKEY&  : hKey        : I/O : Address to which the function 
//                                                   copies the handle of the newly 
//                                                   generated key.
//                 HCRYPTHASH& : hHash       : I/O : Address to which the function 
//                                                   copies a handle to the new 
//                                                   hash object.
//                 LPSTR       : szPassword  :  I  : ��ȣȭ �� �н����� ��
// Return        : ���� : 0 (NH_CRYPT_SUCCESS)
//                 ���� : -1 (NH_CRYPT_ERROR)
// Release       : VER 0.9
// Initial Date  : 2003.11.11
// Notes         : 
// 
//==============================================================================
int NH_HashPWD(HCRYPTPROV& hCryptProv, 
			   HCRYPTKEY& hKey, 
			   HCRYPTHASH& hHash, 
			   LPSTR pszPassword)
{
	int nRet			= NH_CRYPT_SUCCESS;
	
	//--------------------------------------------------------------------
	//�н����忡 �ؽ��� ���Ѵ�.
	if( !CryptHashData(hHash, (BYTE *)pszPassword, strlen(pszPassword), 0) )
	{
		TRACE("Error during CryptHashData. \n");
		return (nRet = NH_CRYPT_ERROR);
	}
	TRACE("The password has been added to the hash. \n");
	
	//--------------------------------------------------------------------
	// �ؽ� ���� �̿��Ͽ� ����Ű�� �����Ѵ�.
	if( !CryptDeriveKey(hCryptProv, ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey) )
	{
		TRACE("Error during CryptDeriveKey!\n"); 
		return (nRet = NH_CRYPT_ERROR);
	}
	TRACE("An encryption key is derived from the password hash. \n"); 
	
	//--------------------------------------------------------------------
	//�ؽ� ������Ʈ�� �Ҹ��Ѵ�.
	CryptDestroyHash(hHash);
	hHash = NULL;
	
	return nRet;
}

//==============================================================================
// 
// Function Name : NH_CryptoEnd
// Description   : ��ȭ�� ���� �Ҷ� ����� ���ҽ��� ��ȯ�Ѵ�.
// Parameters    : HCRYPTPROV& : hCryptProv  : I/O : Address to which the function 
//                                                   copies a handle to the CSP.
//                                                   (Cryptographic Service Provider)
//                 HCRYPTKEY&  : hKey        : I/O : Address to which the function 
//                                                   copies the handle of the newly 
//                                                   generated key.
//                 HCRYPTHASH& : hHash       : I/O : Address to which the function 
//                                                   copies a handle to the new 
//                                                   hash object.
// Return        : ���� : 0 (NH_CRYPT_SUCCESS)
//                 ���� : -1 (NH_CRYPT_ERROR)
// Release       : VER 0.9
// Initial Date  : 2003.11.11
// Notes         : 
// 
//==============================================================================
int NH_CryptoEnd(HCRYPTPROV& hCryptProv, 
				 HCRYPTKEY& hKey, 
				 HCRYPTHASH& hHash)
{
	//--------------------------------------------------------------------
	// ����Ű�� �ı��Ѵ�.	
	if(hKey)
	{
		CryptDestroyKey(hKey);
		hKey = NULL;
	}
	//--------------------------------------------------------------------
	// �ؽ� ������Ʈ�� �Ҹ��Ѵ�.
	if(hHash)
	{
		CryptDestroyHash(hHash);
		hHash = NULL;
	}
	//--------------------------------------------------------------------
	// // CSP�� �ڵ��� �����Ѵ�. 	
	if(hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
		hCryptProv = NULL;
	}

	return NH_CRYPT_SUCCESS;
}

//==============================================================================
// 
// Function Name : NH_EncryptString
// Description   : ���ϴ� ���ڿ��� ��ȣȭ�Ѵ�.
// Parameters    : HCRYPTPROV& : hCryptProv   : I/O : Address to which the function 
//                                                    copies a handle to the CSP.
//                                                    (Cryptographic Service Provider)
//                 HCRYPTKEY&  : hKey         : I/O : Address to which the function 
//                                                    copies the handle of the newly 
//                                                    generated key.
//                 HCRYPTHASH& : hHash        : I/O : Address to which the function 
//                                                    copies a handle to the new 
//                                                    hash object.
//                 LPBYTE      : pbData       : I/O : ��ȣȭ �� ������
//                 DWORD&      : dwDataLen    : I/O : ��ȣȭ �� �������� ����
//                 LPBYTE      : pbCheckHash  : I/O : �������� ��ȿ�� Hash ��
//                 DWORD&      : dwCHLen      : I/O : �������� ��ȿ�� Hash ���� ����
//                                                     - �⺻�� : 20
// Return        : ���� : 0 (NH_CRYPT_SUCCESS)
//                 ���� : -1 (NH_CRYPT_ERROR)
// Release       : VER 0.9
// Initial Date  : 2003.11.11
// Notes         : 
// 
//==============================================================================
int NH_EncryptString(HCRYPTPROV& hCryptProv, 
					 HCRYPTKEY& hKey, 
					 HCRYPTHASH& hHash, 
					 LPBYTE pbData, 
					 DWORD& dwDataLen, 
					 LPBYTE pbCheckHash, 
					 DWORD& dwCHLen)
{
	int nRet			= NH_CRYPT_SUCCESS;

	DWORD dwBlockLen; 
	DWORD dwBufferLen; 
	
	//--------------------------------------------------------------------
	// �ѹ��� ��ȣȭ�� ����� ũ�⸦ �����Ѵ�.. 
	dwBlockLen = ENCRYPT_BUFF_SIZE - ( ENCRYPT_BUFF_SIZE % ENCRYPT_BLOCK_SIZE );
	
	if ( ENCRYPT_BLOCK_SIZE > 1 )
		dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE;
	else
		dwBufferLen = dwBlockLen;
    
	//--------------------------------------------------------------------
	//������ �ؽ��� ���ϱ� ���� �ؽ�������Ʈ�� �����Ѵ�.
	if( !CryptCreateHash(hCryptProv, CALG_SHA1, 0, 0, &hHash) )
	{ 
		TRACE("Error during CryptCreateHash!\n");
		return (nRet = NH_CRYPT_ERROR);
	}  
	TRACE("A hash object has been created. \n");
		
	//--------------------------------------------------------------------
	// ���� ���� �ؽ��� ���Ͽ� �ؽ� ������Ʈ�� �߰��Ѵ�.
	if( !CryptHashData(hHash, (BYTE *)pbData, dwDataLen, 0) )					
	{
		TRACE("Error during CryptHashData. \n"); 
		return (nRet = NH_CRYPT_ERROR);
	}
	TRACE("buffer has been added to the hash. \n");
		
	//--------------------------------------------------------------------
	// ���� ����� ��ȣȭ�Ѵ�.	
	if( !CryptEncrypt(hKey, 0, FALSE, 0, pbData, &dwDataLen, dwBufferLen) )
	{ 
		TRACE("Error during CryptEncrypt. \n"); 
		return (nRet = NH_CRYPT_ERROR);
	} 
	TRACE("CryptEncrypt Successed. \n");
	
	//--------------------------------------------------------------------
	// üũ�� �ؽ� ���� �����.
	BYTE* pbHashSize	= NULL;
	BYTE* pHashData		= NULL;
	DWORD dwHashLen		= 0;

	memset(pbCheckHash, 0x00, dwCHLen);
	
	if( !CryptGetHashParam(hHash, HP_HASHSIZE, pbHashSize, (unsigned long*)&dwHashLen, 0) ) 
	{
		TRACE("CryptGetHashParam failed to get size.\n");
		return (nRet = NH_CRYPT_ERROR);
	}
	
	if( !CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0) ) 
	{
		TRACE("CryptGetHashParam failed to get length.\n");
		return (nRet = NH_CRYPT_ERROR);
	}
	
	if( !CryptGetHashParam(hHash, HP_HASHVAL, pHashData, &dwHashLen, 0) )
	{
		TRACE("Error during reading hash value.\n");
		return (nRet = NH_CRYPT_ERROR);
	}
	
	pHashData = new BYTE[dwHashLen];
	if ( !pHashData )  
	{
		TRACE("Allocation failed.\n");
		return (nRet = NH_CRYPT_ERROR);
	}
	
	if( !CryptGetHashParam(hHash, HP_HASHVAL, pHashData, &dwHashLen, 0) )  
	{
		TRACE("Error during reading hash value.\n");
		return (nRet = NH_CRYPT_ERROR);
	}

	//--------------------------------------------------------------------
	//  �ؽ����� ��ȣȭ �Ѵ�.
	if( !CryptEncrypt(hKey, 0, TRUE, 0, pHashData, &dwHashLen, dwHashLen + ENCRYPT_BLOCK_SIZE) )
	{ 
		TRACE("Error during CryptEncrypt. \n"); 
		return (nRet = NH_CRYPT_ERROR);
	} 
	
	//--------------------------------------------------------------------
	// ��ȣȭ �� ����� ����� �����Ѵ�.
	if ( dwHashLen > dwCHLen )
	{
		TRACE("small Check Hash Key Buffer. \n"); 
		return (nRet = NH_CRYPT_ERROR);
	}
	
	dwCHLen = dwHashLen;
	memcpy(pbCheckHash, pHashData, dwHashLen);

	if ( pHashData )
	{
		delete [] pHashData;
		pHashData = NULL;
	}

	return nRet;
}

//==============================================================================
// 
// Function Name : NH_DecryptString
// Description   : ��ȣȭ �� ������ ��ȣȭ�Ѵ�.
// Parameters    : HCRYPTPROV& : hCryptProv   : I/O : Address to which the function 
//                                                    copies a handle to the CSP.
//                                                    (Cryptographic Service Provider)
//                 HCRYPTKEY&  : hKey         : I/O : Address to which the function 
//                                                    copies the handle of the newly 
//                                                    generated key.
//                 HCRYPTHASH& : hHash        : I/O : Address to which the function 
//                                                    copies a handle to the new 
//                                                    hash object.
//                 LPBYTE      : pbData       : I/O : ��ȣȭ �� ������
//                 DWORD&      : dwDataLen    : I/O : ��ȣȭ �� �������� ����
//                 DWORD&      : dwCHLen      :  I  : �������� ��ȿ�� Hash ���� ����
//                                                     - �⺻�� : 20
//                 BOOL&       : bEqual       :  O  : �������� ��ȿ�� ��
//                                                     - TRUE  : ��ȣȭ ����
//                                                     - FALSE : ��ȣȭ ����
// Return        : ���� : 0 (NH_CRYPT_SUCCESS)
//                 ���� : -1 (NH_CRYPT_ERROR)
// Release       : VER 0.9
// Initial Date  : 2003.11.11
// Notes         : 
// 
//==============================================================================
int NH_DecryptString(HCRYPTPROV& hCryptProv, 
					 HCRYPTKEY& hKey, 
					 HCRYPTHASH& hHash,
					 LPBYTE pbData, 
					 DWORD& dwDataLen, 
					 DWORD& dwCHLen, 
					 BOOL& bEqual)					 
{
	int nRet			= NH_CRYPT_SUCCESS;
	
	DWORD dwBlockLen; 
	DWORD dwBufferLen; 

	//--------------------------------------------------------------------
	// �ѹ��� ��ȣȭ�� ����� ũ�⸦ �����Ѵ�.. 
	dwBlockLen = ENCRYPT_BUFF_SIZE - ( ENCRYPT_BUFF_SIZE % ENCRYPT_BLOCK_SIZE );
	
	if ( ENCRYPT_BLOCK_SIZE > 1 )
		dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE;
	else
		dwBufferLen = dwBlockLen;
	
	//--------------------------------------------------------------------
	//������ �ؽ��� ���ϱ� ���� �ؽ�������Ʈ�� �����Ѵ�.
	if( !CryptCreateHash(hCryptProv, CALG_SHA1, 0, 0, &hHash) )
	{ 
		 TRACE("Error during CryptCreateHash!\n");
		 return (nRet = NH_CRYPT_ERROR);
	}  
	TRACE("A hash object has been created. \n");
		
	//--------------------------------------------------------------------
	// ���� ������ ��ȣȭ �Ѵ�.
	if( !CryptDecrypt(hKey, 0, TRUE, 0, pbData, &dwDataLen) )
	{
	   TRACE("Error during CryptDecrypt!"); 
	   return (nRet = NH_CRYPT_ERROR);
	}

	// Check Hash�� ���
	dwDataLen -= dwCHLen;
	BYTE pbCheckHash[HASHLENGTH]		= { 0, };
	memcpy(pbCheckHash, pbData + dwDataLen, dwCHLen);
	
	//--------------------------------------------------------------------
	//��ȣȭ �� ����Ÿ�� �ؽ����� ���Ѵ�.
	if( !CryptHashData(hHash, (BYTE *)pbData, dwDataLen, 0) )				
	{
		TRACE("Error during CryptHashData. \n"); 
		return (nRet = NH_CRYPT_ERROR);
	}
	TRACE("buffer has been added to the hash. \n");
		
	//--------------------------------------------------------------------
	// üũ �ؽ���
	BYTE* pbHashSize	= NULL;
	BYTE* pHashData		= NULL;
	DWORD dwHashLen		= 0;
	bEqual				= FALSE;

	//--------------------------------------------------------------------
	// �ؽ� ������Ʈ���� ���� �ؽ� ���� �����Ѵ�.
	if( !CryptGetHashParam(hHash, HP_HASHSIZE, pbHashSize, (unsigned long*)&dwHashLen, 0) )
	{
		TRACE("CryptGetHashParam failed to get size.\n");
		return (nRet = NH_CRYPT_ERROR);
	}
	
	if( !CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0) )
	{
		TRACE("CryptGetHashParam failed to get length.\n");
		return (nRet = NH_CRYPT_ERROR);
	}

	pHashData = new BYTE[dwHashLen];
	if ( !pHashData )  
	{
		TRACE("Allocation failed.\n");
		return (nRet = NH_CRYPT_ERROR);
	}

	if( !CryptGetHashParam(hHash, HP_HASHVAL, pHashData, &dwHashLen, 0) )
	{
		TRACE("Error during reading hash value.\n");
		return (nRet = NH_CRYPT_ERROR);
	}

	if ( memcmp(pbCheckHash, pHashData, dwHashLen) == 0 )
		bEqual = TRUE;

	if ( pHashData )
	{
		delete [] pHashData;
		pHashData = NULL;
	}

	return nRet;
}

//==============================================================================
// 
// Function Name : LogEncrypt
// Description   : ������ ��ȣȭ�Ͽ� ��ȯ�Ѵ�.
// Parameters    : LPCSTR     : pSrc          :  I  : ���� ������
//                 DWORD      : dwSrc         :  I  : ���� ������ ����
//                 LPSTR      : pDest         :  O  : ��ȣȭ ������
//                 DWORD&     : dwDest        : I/O : ��ȣȭ ������ ����
//                                                     - ((���������� ���� + 20 Bytes) * 2) + 2Bytes �̻� �ʿ�
//                 LPCSTR     : pszPassword   :  I  : ��ȣȭ �� �н�����
//                                                     - 128�� �̳��� �Ѵ�.
// Return        : ���� : 0 (NH_CRYPT_SUCCESS)
//                 ���� : -1 ( �Լ� ���� : NH_CRYPT_ERROR )
//                        -2 ( �Է� �ڷᰡ �ʹ� ŭ : NH_CRYPT_INDATA_OVER )
//                        -3 ( ��ȣȭ ������ ���� ���̺��� ������ ������ ���̰� ���� : NH_CRYPT_OUTBUFF_SMALL)
//                        -4 ( �Է� ���� ���̺��� ������ ������ ���̰� ���� : NH_CRYPT_OUTBUFF_ERR )
// Release       : VER 0.9
// Initial Date  : 2003.11.11
// Notes         : �̰��� HexDump(), MsgDump()�ȿ����� ���δ�.
//                 �̰��� ���� CString�� ������ �ʵ�.(Binary Data ����)
// 
//==============================================================================
int LogEncrypt(LPCSTR pSrc, DWORD dwSrc, LPSTR pDest, DWORD& dwDest, LPCSTR pszPassword /*= NULL*/)
{
	HCRYPTPROV hCryptProv; 
	HCRYPTKEY hKey; 
	HCRYPTHASH hHash; 
	int nRet				= NH_CRYPT_SUCCESS;
	int nPWDNo				= CRYPT_PAD;
	LPSTR pszPWD			= NULL;

	if ( dwSrc > dwDest )
	{
		memset(pDest, 0x00, dwDest);
		return ( nRet = NH_CRYPT_OUTBUFF_ERR );
	}
	
	//--------------------------------------------------------------------
	// �н� ���尡 ���� ���� ������ �߻��Ͽ��� ���� ���Ѵ�.
	if ( NULL == pszPassword )
	{
		// Seed the random-number generator with current time so that
		// the numbers will be different every time we run.
		srand( (unsigned)time( NULL ) );
		
		nPWDNo	= (rand()  % 10);
		pszPWD	= szPasswordTable[nPWDNo];
	}
	else
	{
		// �Է� �н����尡 ������ 128���� ��ȣȭ �� �յڷ� �ٴ´�.
		nPWDNo	= CRYPT_PAD;
		pszPWD	= (LPSTR)pszPassword;
	}
	
	//--------------------------------------------------------------------
	// ��ȣȭ ����
	if ( 0 != ( nRet = NH_CryptoStart(hCryptProv, hHash) ) )
	{
		TRACE("NH_CryptoStart failed.[0x%08X]\n", GetLastError());
		return nRet;
	}
	
	//--------------------------------------------------------------------
	// �н����� �ؽ�
	if ( 0 != ( nRet = NH_HashPWD(hCryptProv, hKey, hHash, pszPWD) ) )
	{
		TRACE("NH_HashPWD failed.[0x%08X]\n", GetLastError());
		// ��ȣȭ ����
		NH_CryptoEnd(hCryptProv, hKey, hHash);
		return nRet;
	}
	
	BYTE bData[ENCRYPT_BUFF_SIZE]	= { 0, };
	DWORD dwDataLen					= sizeof(bData);
	BYTE bCheckHash[HASHLENGTH]		= { 0, };
	DWORD dwCHLen					= sizeof(bCheckHash);
		
	if ( dwSrc <= dwDataLen )
	{
		memcpy(bData, pSrc, dwSrc);
		dwDataLen = dwSrc;
	}
	else
	{
		TRACE("Input Data Big.\n");
		return ( nRet = NH_CRYPT_INDATA_OVER );
	}

	//--------------------------------------------------------------------
	// ��ȣȭ
	if ( 0 != ( nRet = NH_EncryptString(hCryptProv, hKey, hHash, 
		                                bData, dwDataLen, bCheckHash, dwCHLen) ) )
	{
		TRACE("NH_EncryptString failed.[0x%08X]\n", GetLastError());
		// ��ȣȭ ����
		NH_CryptoEnd(hCryptProv, hKey, hHash);
		return nRet;
	}

	//--------------------------------------------------------------------
	// ��ȣȭ �� ����
	// ������� + ((��ȣȭ���� ���� + üũ�ؽ� ����) * 2) + ���̷�����
	if ( dwDest > ( 1 + ((dwDataLen + dwCHLen) * 2) + 1 ) )
	{
		int nPos	= 0;
		// ���
		pDest[nPos]	= (BYTE)nPWDNo;
		nPos += 1;
		// ��ȣȭ�� �ڷ�
		CString	szUPCrypto = MakeUnPack(bData, dwDataLen);
//		memcpy(pDest + nPos, bData, dwDataLen);
//		nPos += dwDataLen;
		memcpy(pDest + nPos, (LPSTR)(LPCTSTR)szUPCrypto, szUPCrypto.GetLength());
		nPos += szUPCrypto.GetLength();
		// üũ�ؽ�
		CString	szUPCheckCrypto = MakeUnPack(bCheckHash, dwCHLen);
//		memcpy(pDest + nPos, bCheckHash, dwCHLen);
//		nPos += dwCHLen;
		memcpy(pDest + nPos, (LPSTR)(LPCTSTR)szUPCheckCrypto, szUPCheckCrypto.GetLength());
		nPos += szUPCheckCrypto.GetLength();
		// ���Ϸ�
		pDest[nPos]	= (BYTE)nPWDNo;
		nPos += 1;
		// ����
		dwDest		= nPos;		// 1 + dwDataLen + dwCHLen + 1
	}
	else
	{
		TRACE("Output Buffer small.\n");
		// ���� �Ͽ����Ƿ� ���� �����͸� ���� ���ش�.
		if ( dwDest >= dwSrc )
		{
			memcpy(pDest, pSrc, dwSrc);
			dwDest = dwSrc;
			nRet = NH_CRYPT_OUTBUFF_SMALL;
		}
	}
	
	//--------------------------------------------------------------------
	// ��ȣȭ ����
	NH_CryptoEnd(hCryptProv, hKey, hHash);
	
	return nRet;
}

//==============================================================================
// 
// Function Name : LogDecrypt
// Description   : ������ ������ ��ȣȭ �Ѵ�.
// Parameters    : LPCSTR     : pSrc          :  I  : ��ȣȭ�� �� �α� ����
//                                                     - Full Path�� ����.
//                 LPSTR      : pDest         :  O  : ��ȣȭ�� Ǯ�� �α� ����
//                                                     - Full Path�� ����.
//                 LPCSTR     : pszPassword   :  I  : ��ȣȭ �� �н�����
//                                                     - 128�� �̳��� �Ѵ�.
//                 int        : nIndex        :  I  : ��ġ ��ȣ ��Ƽ�� �Ҷ� �ʿ�
//                 HWND       : hWnd          :  I  : ���� ��Ȳ�� �˸� ������ �ڵ�
// Return        : ���� : 0 (NH_CRYPT_SUCCESS)
//                 ���� : -1 ( �Լ� ���� : NH_CRYPT_ERROR )
//                        -5 ( ��ȣȭ�� ���� ���� : NH_CRYPT_INFILE_NOT_FOUND )
//                        -6 ( ��ȣȭ ���� ���� ���� : NH_CRYPT_OUTFILE_CREATE_FAIL )
// Release       : VER 0.9
// Initial Date  : 2003.11.11
// Notes         : ��ȣȭ�ϴ� �Լ��� ������ ����� ����.
// 
//==============================================================================
int WINAPI LogDecrypt(LPCSTR pSrc, LPSTR pDest, LPCSTR pszPassword /*= NULL*/, int nIndex /*= 0*/, HWND hWnd /*=NULL*/)
{
	int nRet				= NH_CRYPT_SUCCESS;

	HCRYPTPROV hCryptProv; 
	HCRYPTKEY hKey; 
	HCRYPTHASH hHash; 
	
	FILE* hSrcFile			= NULL; 
	FILE* hDestFile			= NULL; 
	
	//--------------------------------------------------------------------
	// ���� ����
	if( !(hSrcFile = fopen(pSrc, "rb")) ) 
	{
		TRACE("Error opening ciphertext file![0x%08X]\n", GetLastError());
		return ( nRet = NH_CRYPT_INFILE_NOT_FOUND);
	}	
	if( !(hDestFile = fopen(pDest, "wb")) )
	{
		TRACE("Error opening plaintext file![0x%08X]\n", GetLastError());
		return ( nRet = NH_CRYPT_OUTFILE_CREATE_FAIL);
	} 

	BYTE bRBuff[ENCRYPT_BUFF_SIZE]	= { 0, };				// File Read Buffer
	BYTE bCBuff[ENCRYPT_BUFF_SIZE*2]= { 0, };				// Crypto Buffer(2��� ��´�)
	int nCPos						= 0;					// Crypto Buffer�� ���ִ� �ڷ��� ũ��
	DWORD dwRByte					= 0;					// File���� ���� �ڷ��� Bytes ũ��
	BOOL bSCryptoTerm				= FALSE;				// ��ȣȭ ���� ���� ǥ�� �÷���
	BOOL bECryptoTerm				= FALSE;				// ��ȣȭ ���� ���� ǥ�� �÷���
	int nPWDNo						= CRYPT_PAD;			// ��ȣȭ Ű ����
	DWORD dwCalcBytes				= 0;
	
	memset(bRBuff, 0x00, sizeof(bRBuff));					// �ʱ�ȭ
	memset(bCBuff, 0x00, sizeof(bCBuff));
	
	while ( !feof(hSrcFile) )								// File�� ������ �Դ°�?
	{
		memset(bRBuff, 0x00, sizeof(bRBuff));

		// �� ũ�⸸ŭ ���Ͽ��� �о���δ�.
		dwRByte = fread(bRBuff, sizeof(BYTE), sizeof(bRBuff), hSrcFile);

		int nRPos		= 0;								// ���� ���� �ڷᷮ
		while ( nRPos < (int)dwRByte )						// ���� �ڷḦ �� �� ��� ���� ���´�.
		{
			//------------------------------------------------------------
			// ��ȣȭ ������ ������ ��ȣȭ�Ѵ�.
			//
			// 0. ������ ũ�⿡ �ٴٶ��µ� ��ȣȭ ������ �������� �ʾ��� 
			//    ���� �׳� ���Ͽ� ����. ( 90%�� ������ �Ѵ� )
			// 1. ��ȣȭ �������� ��� ���µ�.. Crypto Buffer�� �ڷᰡ ���
			//    �ִٸ� ���Ͽ� ����ϰ� Crypto Buffer�� ����.
			// 2. ��ȣȭ ������ ���̶�� ����
			// 3. ��ȣȭ ������ �����ٸ� ��ȣȭ �ϰ� ���Ͽ� ��� �� 
			//    Crypto Buffer�� Ŭ���� �Ѵ�.
			//
			switch( bRBuff[nRPos] ) 
			{
				// ���ǵ� �н����带 ���� �е�
				case 0x00 :  case 0x01 :  case 0x02 :
				case 0x03 :  case 0x04 :  case 0x05 :
				case 0x06 :  case 0x07 :  case 0x08 :
				case 0x09 :
					if ( bSCryptoTerm ) 
					{
						bECryptoTerm = TRUE;
						if ( nPWDNo == bRBuff[nRPos] )		// �е� ���� ó���� ��������
							nRPos++;						// ���� ��Ų��.
					}
					else 
					{
						bSCryptoTerm	= TRUE;				// ��ȣȭ ���� �ð�
						nPWDNo			= bRBuff[nRPos++];	// ��ȣȭ Ű ��

						if ( 0 != nCPos )
						{
							// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
							fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
							
							// ���۸� �ʱ�ȭ �Ѵ�.
							memset(bCBuff, 0x00, sizeof(bCBuff));
							nCPos	= 0;
							
							::PostMessage(hWnd, WM_PROGRESS_NOTIFY, dwCalcBytes, nIndex);
						}
					}
					break;
				// ���� ��ȣ�� ���� �е�
				case CRYPT_PAD : 
					if ( bSCryptoTerm ) 
					{
						bECryptoTerm = TRUE;
						if ( nPWDNo == bRBuff[nRPos] )		// �е� ���� ó���� ��������
							nRPos++;						// ���� ��Ų��.
					}
					else 
					{
						bSCryptoTerm	= TRUE;				// ��ȣȭ ���� �ð�
						nPWDNo			= CRYPT_PAD;		// ��ȣȭ Ű ��
						nRPos++;
						
						if ( 0 != nCPos )
						{
							// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
							fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
							
							// ���۸� �ʱ�ȭ �Ѵ�.
							memset(bCBuff, 0x00, sizeof(bCBuff));
							nCPos	= 0;
							
							::PostMessage(hWnd, WM_PROGRESS_NOTIFY, dwCalcBytes, nIndex);
						}
					}
					break;
				default:
					bCBuff[nCPos++]	= bRBuff[nRPos++];		// �� ����
			}

			// ��ü ���� ���ϱ� ���� ó��
			dwCalcBytes++;

			//------------------------------------------------------------
			// ��ȣȭ ������ ������ ��ȣȭ�Ѵ�.
			if ( ( ENCRYPT_BUFF_SIZE - (ENCRYPT_BUFF_SIZE / 10) ) < nCPos )
			{
				// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
				fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
				
				// ���۸� �ʱ�ȭ �Ѵ�.
				memset(bCBuff, 0x00, sizeof(bCBuff));
				nCPos	= 0;

				::PostMessage(hWnd, WM_PROGRESS_NOTIFY, dwCalcBytes, nIndex);
			}
			else if ( bSCryptoTerm && bECryptoTerm )
			{
				// ��ȣȭ Ű(��ȣ)�� �ʿ��ѵ� ���� ���� �׳� ���Ͽ� �����Ѵ�.
				if ( (CRYPT_PAD == nPWDNo) && (NULL == pszPassword) )
				{
					BYTE bTBuff[3] = { CRYPT_PAD, 0, 0};
					// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
					fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
					// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
					fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
					// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
					fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
				}
				else
				{
					LPSTR pszPWD			= NULL;
					BOOL bEqual				= FALSE;
					DWORD nPackCryptoLen	= 0;
					BYTE bPackCrypto[ENCRYPT_BUFF_SIZE] = { 0, };
					
					// Password ����
					if ( CRYPT_PAD == (BYTE)nPWDNo ) pszPWD = (LPSTR)pszPassword;
					else pszPWD = szPasswordTable[nPWDNo];

					//--------------------------------------------------------------------
					// ��ȣȭ ����
					if ( 0 != ( nRet = NH_CryptoStart(hCryptProv, hHash) ) )
					{
						TRACE("NH_CryptoStart failed.[0x%08X]\n", GetLastError());
						return nRet;
					}
					
					//----------------------------------------------------
					// �н����� �ؽ�
					if ( 0 != ( nRet = NH_HashPWD(hCryptProv, hKey, hHash, pszPWD) ) )
					{
						TRACE("NH_HashPWD failed.[0x%08X]\n", GetLastError());
						
						BYTE bTBuff[3] = { nPWDNo, 0, 0};
						// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
						fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
						// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
						fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
						// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
						fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
					}
					else
					{
						// ���� �ڷḦ ���Ѵ�.
						nPackCryptoLen = MakePack(bCBuff, bPackCrypto, nCPos);
						
						//----------------------------------------------------
						// �ڷḦ ��ȣȭ�Ѵ�.
						DWORD dwCHLen	= 20;
						NH_DecryptString(hCryptProv, hKey, hHash,
							             bPackCrypto, nPackCryptoLen, 
										 dwCHLen, bEqual);

						// ���� üũ �ؽ��� FALSE�̸� �׳� �����͸� �����ϰ� �ƴϸ� 
						// ��ȣȭ�� �ڷḦ �����Ѵ�.
						if ( bEqual )
						{
							// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
							char szTemp[ENCRYPT_BUFF_SIZE] = { 0, };
							int nszTempPos = 0;
							for(DWORD i = 0; i < nPackCryptoLen; i++)
							{
								// �������� 0x0A�� �ִ� ���� 0x0D0A�� �ٲپ� �־��
								// MS�� Text ������ �ν��Ѵ�.
								if ( 0x0A == bPackCrypto[i] )
								{
									if ( 0x0D != bPackCrypto[i] )
										szTemp[nszTempPos++] = 0x0D;
								}
								szTemp[nszTempPos++] = bPackCrypto[i];
							}
							fwrite(szTemp, sizeof(BYTE), nszTempPos, hDestFile);
						}
						else
						{
							TRACE("NH_DecryptString failed.[0x%08X]\n", GetLastError());

							BYTE bTBuff[3] = { nPWDNo, 0, 0};
							// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
							fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
							// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
							fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
							// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
							fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
						}
					}

					//---------------------------------------------------
					// ��ȣȭ ����
					NH_CryptoEnd(hCryptProv, hKey, hHash);
				}

				// ���۸� �ʱ�ȭ �Ѵ�.
				memset(bCBuff, 0x00, sizeof(bCBuff));
				nCPos	= 0;

				// ������ �ʱ�ȭ�Ѵ�.
				bSCryptoTerm	= FALSE;
				bECryptoTerm	= FALSE;

				::PostMessage(hWnd, WM_PROGRESS_NOTIFY, dwCalcBytes, nIndex);
			}
		}
		::PostMessage(hWnd, WM_PROGRESS_NOTIFY, dwCalcBytes, nIndex);
	}
	
	//--------------------------------------------------------------------
	// ���ۿ� ���� �ܿ� �ڷᰡ �ִٸ� ó���Ѵ�. �̶� ���� �״�� ����.
	if ( bSCryptoTerm && 0 != nCPos )
	{
		BYTE bTBuff[3] = { nPWDNo, 0, 0};
		// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
		fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
		// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
		fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
		// ��� ���Ͽ� ���� �޼����� �����Ѵ�.
		fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
	}
	else
	{
		char szTemp[ENCRYPT_BUFF_SIZE] = { 0, };
		int nszTempPos = 0;
		for(int i = 0; i < nCPos; i++)
		{
			// �������� 0x0A�� �ִ� ���� 0x0D0A�� �ٲپ� �־��
			// MS�� Text ������ �ν��Ѵ�.
			if ( 0x0A == bCBuff[i] )
			{
				if ( 0x0D != bCBuff[i] )
					szTemp[nszTempPos++] = 0x0D;
			}
			szTemp[nszTempPos++] = bCBuff[i];
		}
		fwrite(szTemp, sizeof(BYTE), nszTempPos, hDestFile);
	}
	
	::PostMessage(hWnd, WM_PROGRESS_NOTIFY, dwCalcBytes, nIndex);
	//--------------------------------------------------------------------
	// ���� ���ҽ� ��ȯ
	if(hSrcFile)
	{
		fclose(hSrcFile); 
		hSrcFile	= NULL;
	}
	if(hDestFile)
	{
		fclose(hDestFile); 
		hDestFile	= NULL;
	}
	
	return nRet;
}
