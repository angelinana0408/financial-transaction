//******************************************************************************
//
// File Name        : Cryptogram.cpp
// Description      : 암호화 함수집합
// Author           : 김형민
// Initial Date     : 2003.11.11
// Revision History : 
// Notes            : 특정한 곳에 사용하기 위한 함수이므로 다른 곳에 적용시 
//                    변경하여서 사용하기 바람. SDK를 깔아서 사용할 것(권장)
//                    
//                    @ 공통
//                      이것을 꼭 써서 컴파일할 것.
//                       #pragma comment( lib, "Advapi32.lib")
//                     
//                    @ SDK용
//                      헤더파일
//                       #include <wincrypt.h>
//
//                    @ Non SDK용
//                      Directory의 Include에 추가 할것
//                       -> .\LOGCRYPT\CMNLIB\CRYPT
//
//                      Directory의 Lib에 추가 할것
//                       -> .\LOGCRYPT\CMNLIB\CRYPT
//
//                    @ 추가 사항
//                      에러는 GetLastError()로 확인 할것.
// 
//                      암호화는 2배이상의 버퍼를 사용하도록되어 있다.
//                      따라서, 14700Bytes 이하의 로그만 암호화, 복호화해야한다.
// 
//******************************************************************************
 
#include "stdafx.h"
#include "CmnLibEx.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
//#include <wincrypt.h>							// SDK용
#include "wincrypt.h"							// Non SDK용
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
// Description   : 암호화를 위한 시작 함수로 필수로 제일 처음에 호출 한다.
// Parameters    : HCRYPTPROV&   : hCryptProv  : I/O : Address to which the function 
//                                                     copies a handle to the CSP.
//                                                     (Cryptographic Service Provider)
//                 HCRYPTHASH&   : hHash       : I/O : Address to which the function 
//                                                     copies a handle to the new 
//                                                     hash object.
// Return        : 성공 : 0 (NH_CRYPT_SUCCESS)
//                 실패 : -1 (NH_CRYPT_ERROR)
// Release       : VER 0.9
// Initial Date  : 2003.11.11
// Notes         : 
// 
//==============================================================================
int NH_CryptoStart(HCRYPTPROV& hCryptProv, 
				   HCRYPTHASH& hHash)
{
	int nRet			= NH_CRYPT_SUCCESS;
 
	// 디폴트 CSP의 핸들을 얻어온다. CryptoAPI중 가장 첫번째 함수 
	if( !CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0) )
	{
		// Key Set이 없다면 만들어준다.
		if( !CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET) )
		{
			TRACE("Error during CryptAcquireContext!");
			return (nRet = NH_CRYPT_ERROR);
		}
	}
	TRACE("A cryptographic provider has been acquired. \n");

	// hash 오브젝트를 생성한다.
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
// Description   : 암호를 걸었던 패스워드의 해쉬값을 얻어 낸다.
// Parameters    : HCRYPTPROV& : hCryptProv  : I/O : Address to which the function 
//                                                   copies a handle to the CSP.
//                                                   (Cryptographic Service Provider)
//                 HCRYPTKEY&  : hKey        : I/O : Address to which the function 
//                                                   copies the handle of the newly 
//                                                   generated key.
//                 HCRYPTHASH& : hHash       : I/O : Address to which the function 
//                                                   copies a handle to the new 
//                                                   hash object.
//                 LPSTR       : szPassword  :  I  : 암호화 할 패스워드 값
// Return        : 성공 : 0 (NH_CRYPT_SUCCESS)
//                 실패 : -1 (NH_CRYPT_ERROR)
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
	//패스워드에 해쉬를 취한다.
	if( !CryptHashData(hHash, (BYTE *)pszPassword, strlen(pszPassword), 0) )
	{
		TRACE("Error during CryptHashData. \n");
		return (nRet = NH_CRYPT_ERROR);
	}
	TRACE("The password has been added to the hash. \n");
	
	//--------------------------------------------------------------------
	// 해쉬 값을 이용하여 세션키를 생성한다.
	if( !CryptDeriveKey(hCryptProv, ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey) )
	{
		TRACE("Error during CryptDeriveKey!\n"); 
		return (nRet = NH_CRYPT_ERROR);
	}
	TRACE("An encryption key is derived from the password hash. \n"); 
	
	//--------------------------------------------------------------------
	//해쉬 오브젝트를 소멸한다.
	CryptDestroyHash(hHash);
	hHash = NULL;
	
	return nRet;
}

//==============================================================================
// 
// Function Name : NH_CryptoEnd
// Description   : 암화를 종료 할때 사용한 리소스를 반환한다.
// Parameters    : HCRYPTPROV& : hCryptProv  : I/O : Address to which the function 
//                                                   copies a handle to the CSP.
//                                                   (Cryptographic Service Provider)
//                 HCRYPTKEY&  : hKey        : I/O : Address to which the function 
//                                                   copies the handle of the newly 
//                                                   generated key.
//                 HCRYPTHASH& : hHash       : I/O : Address to which the function 
//                                                   copies a handle to the new 
//                                                   hash object.
// Return        : 성공 : 0 (NH_CRYPT_SUCCESS)
//                 실패 : -1 (NH_CRYPT_ERROR)
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
	// 세션키를 파괴한다.	
	if(hKey)
	{
		CryptDestroyKey(hKey);
		hKey = NULL;
	}
	//--------------------------------------------------------------------
	// 해쉬 오브젝트를 소멸한다.
	if(hHash)
	{
		CryptDestroyHash(hHash);
		hHash = NULL;
	}
	//--------------------------------------------------------------------
	// // CSP의 핸들을 해제한다. 	
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
// Description   : 원하는 문자열을 암호화한다.
// Parameters    : HCRYPTPROV& : hCryptProv   : I/O : Address to which the function 
//                                                    copies a handle to the CSP.
//                                                    (Cryptographic Service Provider)
//                 HCRYPTKEY&  : hKey         : I/O : Address to which the function 
//                                                    copies the handle of the newly 
//                                                    generated key.
//                 HCRYPTHASH& : hHash        : I/O : Address to which the function 
//                                                    copies a handle to the new 
//                                                    hash object.
//                 LPBYTE      : pbData       : I/O : 암호화 할 데이터
//                 DWORD&      : dwDataLen    : I/O : 암호화 할 데이터의 길이
//                 LPBYTE      : pbCheckHash  : I/O : 데이터의 유효성 Hash 값
//                 DWORD&      : dwCHLen      : I/O : 데이터의 유효성 Hash 값의 길이
//                                                     - 기본값 : 20
// Return        : 성공 : 0 (NH_CRYPT_SUCCESS)
//                 실패 : -1 (NH_CRYPT_ERROR)
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
	// 한번에 암호화할 블록의 크기를 결정한다.. 
	dwBlockLen = ENCRYPT_BUFF_SIZE - ( ENCRYPT_BUFF_SIZE % ENCRYPT_BLOCK_SIZE );
	
	if ( ENCRYPT_BLOCK_SIZE > 1 )
		dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE;
	else
		dwBufferLen = dwBlockLen;
    
	//--------------------------------------------------------------------
	//파일의 해쉬를 취하기 위해 해쉬오브젝트를 선언한다.
	if( !CryptCreateHash(hCryptProv, CALG_SHA1, 0, 0, &hHash) )
	{ 
		TRACE("Error during CryptCreateHash!\n");
		return (nRet = NH_CRYPT_ERROR);
	}  
	TRACE("A hash object has been created. \n");
		
	//--------------------------------------------------------------------
	// 읽은 블럭의 해쉬를 취하여 해쉬 오브젝트에 추가한다.
	if( !CryptHashData(hHash, (BYTE *)pbData, dwDataLen, 0) )					
	{
		TRACE("Error during CryptHashData. \n"); 
		return (nRet = NH_CRYPT_ERROR);
	}
	TRACE("buffer has been added to the hash. \n");
		
	//--------------------------------------------------------------------
	// 읽은 블록을 암호화한다.	
	if( !CryptEncrypt(hKey, 0, FALSE, 0, pbData, &dwDataLen, dwBufferLen) )
	{ 
		TRACE("Error during CryptEncrypt. \n"); 
		return (nRet = NH_CRYPT_ERROR);
	} 
	TRACE("CryptEncrypt Successed. \n");
	
	//--------------------------------------------------------------------
	// 체크할 해쉬 값을 만든다.
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
	//  해쉬값을 암호화 한다.
	if( !CryptEncrypt(hKey, 0, TRUE, 0, pHashData, &dwHashLen, dwHashLen + ENCRYPT_BLOCK_SIZE) )
	{ 
		TRACE("Error during CryptEncrypt. \n"); 
		return (nRet = NH_CRYPT_ERROR);
	} 
	
	//--------------------------------------------------------------------
	// 암호화 된 블록을 대상을 복사한다.
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
// Description   : 암호화 된 문장을 복호화한다.
// Parameters    : HCRYPTPROV& : hCryptProv   : I/O : Address to which the function 
//                                                    copies a handle to the CSP.
//                                                    (Cryptographic Service Provider)
//                 HCRYPTKEY&  : hKey         : I/O : Address to which the function 
//                                                    copies the handle of the newly 
//                                                    generated key.
//                 HCRYPTHASH& : hHash        : I/O : Address to which the function 
//                                                    copies a handle to the new 
//                                                    hash object.
//                 LPBYTE      : pbData       : I/O : 암호화 할 데이터
//                 DWORD&      : dwDataLen    : I/O : 암호화 할 데이터의 길이
//                 DWORD&      : dwCHLen      :  I  : 데이터의 유효성 Hash 값의 길이
//                                                     - 기본값 : 20
//                 BOOL&       : bEqual       :  O  : 데이터의 유효성 값
//                                                     - TRUE  : 복호화 성공
//                                                     - FALSE : 복호화 실패
// Return        : 성공 : 0 (NH_CRYPT_SUCCESS)
//                 실패 : -1 (NH_CRYPT_ERROR)
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
	// 한번에 암호화할 블록의 크기를 결정한다.. 
	dwBlockLen = ENCRYPT_BUFF_SIZE - ( ENCRYPT_BUFF_SIZE % ENCRYPT_BLOCK_SIZE );
	
	if ( ENCRYPT_BLOCK_SIZE > 1 )
		dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE;
	else
		dwBufferLen = dwBlockLen;
	
	//--------------------------------------------------------------------
	//파일의 해쉬를 취하기 위해 해쉬오브젝트를 선언한다.
	if( !CryptCreateHash(hCryptProv, CALG_SHA1, 0, 0, &hHash) )
	{ 
		 TRACE("Error during CryptCreateHash!\n");
		 return (nRet = NH_CRYPT_ERROR);
	}  
	TRACE("A hash object has been created. \n");
		
	//--------------------------------------------------------------------
	// 읽은 내용을 복호화 한다.
	if( !CryptDecrypt(hKey, 0, TRUE, 0, pbData, &dwDataLen) )
	{
	   TRACE("Error during CryptDecrypt!"); 
	   return (nRet = NH_CRYPT_ERROR);
	}

	// Check Hash의 계산
	dwDataLen -= dwCHLen;
	BYTE pbCheckHash[HASHLENGTH]		= { 0, };
	memcpy(pbCheckHash, pbData + dwDataLen, dwCHLen);
	
	//--------------------------------------------------------------------
	//복호화 한 데이타의 해쉬값을 취한다.
	if( !CryptHashData(hHash, (BYTE *)pbData, dwDataLen, 0) )				
	{
		TRACE("Error during CryptHashData. \n"); 
		return (nRet = NH_CRYPT_ERROR);
	}
	TRACE("buffer has been added to the hash. \n");
		
	//--------------------------------------------------------------------
	// 체크 해쉬비교
	BYTE* pbHashSize	= NULL;
	BYTE* pHashData		= NULL;
	DWORD dwHashLen		= 0;
	bEqual				= FALSE;

	//--------------------------------------------------------------------
	// 해쉬 오브젝트에서 실제 해쉬 값을 추출한다.
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
// Description   : 문장을 암호화하여 반환한다.
// Parameters    : LPCSTR     : pSrc          :  I  : 원본 데이터
//                 DWORD      : dwSrc         :  I  : 원본 데이터 길이
//                 LPSTR      : pDest         :  O  : 암호화 데이터
//                 DWORD&     : dwDest        : I/O : 암호화 데이터 길이
//                                                     - ((원본데이터 길이 + 20 Bytes) * 2) + 2Bytes 이상 필요
//                 LPCSTR     : pszPassword   :  I  : 암호화 할 패스워드
//                                                     - 128자 이내로 한다.
// Return        : 성공 : 0 (NH_CRYPT_SUCCESS)
//                 실패 : -1 ( 함수 에러 : NH_CRYPT_ERROR )
//                        -2 ( 입력 자료가 너무 큼 : NH_CRYPT_INDATA_OVER )
//                        -3 ( 암호화 데이터 편집 길이보다 내보낼 버퍼의 길이가 작음 : NH_CRYPT_OUTBUFF_SMALL)
//                        -4 ( 입력 버퍼 길이보다 내보낼 버퍼의 길이가 작음 : NH_CRYPT_OUTBUFF_ERR )
// Release       : VER 0.9
// Initial Date  : 2003.11.11
// Notes         : 이것은 HexDump(), MsgDump()안에서만 쓰인다.
//                 이것은 절대 CString에 넣으면 않됨.(Binary Data 포함)
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
	// 패스 워드가 없을 경우는 난수를 발생하여서 값을 정한다.
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
		// 입력 패스워드가 있으면 128값이 암호화 값 앞뒤로 붙는다.
		nPWDNo	= CRYPT_PAD;
		pszPWD	= (LPSTR)pszPassword;
	}
	
	//--------------------------------------------------------------------
	// 암호화 시작
	if ( 0 != ( nRet = NH_CryptoStart(hCryptProv, hHash) ) )
	{
		TRACE("NH_CryptoStart failed.[0x%08X]\n", GetLastError());
		return nRet;
	}
	
	//--------------------------------------------------------------------
	// 패스워드 해쉬
	if ( 0 != ( nRet = NH_HashPWD(hCryptProv, hKey, hHash, pszPWD) ) )
	{
		TRACE("NH_HashPWD failed.[0x%08X]\n", GetLastError());
		// 암호화 종료
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
	// 암호화
	if ( 0 != ( nRet = NH_EncryptString(hCryptProv, hKey, hHash, 
		                                bData, dwDataLen, bCheckHash, dwCHLen) ) )
	{
		TRACE("NH_EncryptString failed.[0x%08X]\n", GetLastError());
		// 암호화 종료
		NH_CryptoEnd(hCryptProv, hKey, hHash);
		return nRet;
	}

	//--------------------------------------------------------------------
	// 암호화 값 편집
	// 헤더길이 + ((암호화값의 길이 + 체크해쉬 길이) * 2) + 테이러길이
	if ( dwDest > ( 1 + ((dwDataLen + dwCHLen) * 2) + 1 ) )
	{
		int nPos	= 0;
		// 헤더
		pDest[nPos]	= (BYTE)nPWDNo;
		nPos += 1;
		// 암호화된 자료
		CString	szUPCrypto = MakeUnPack(bData, dwDataLen);
//		memcpy(pDest + nPos, bData, dwDataLen);
//		nPos += dwDataLen;
		memcpy(pDest + nPos, (LPSTR)(LPCTSTR)szUPCrypto, szUPCrypto.GetLength());
		nPos += szUPCrypto.GetLength();
		// 체크해쉬
		CString	szUPCheckCrypto = MakeUnPack(bCheckHash, dwCHLen);
//		memcpy(pDest + nPos, bCheckHash, dwCHLen);
//		nPos += dwCHLen;
		memcpy(pDest + nPos, (LPSTR)(LPCTSTR)szUPCheckCrypto, szUPCheckCrypto.GetLength());
		nPos += szUPCheckCrypto.GetLength();
		// 테일러
		pDest[nPos]	= (BYTE)nPWDNo;
		nPos += 1;
		// 길이
		dwDest		= nPos;		// 1 + dwDataLen + dwCHLen + 1
	}
	else
	{
		TRACE("Output Buffer small.\n");
		// 실패 하였으므로 들어온 데이터를 복사 해준다.
		if ( dwDest >= dwSrc )
		{
			memcpy(pDest, pSrc, dwSrc);
			dwDest = dwSrc;
			nRet = NH_CRYPT_OUTBUFF_SMALL;
		}
	}
	
	//--------------------------------------------------------------------
	// 암호화 종료
	NH_CryptoEnd(hCryptProv, hKey, hHash);
	
	return nRet;
}

//==============================================================================
// 
// Function Name : LogDecrypt
// Description   : 파일을 넣으면 복호화 한다.
// Parameters    : LPCSTR     : pSrc          :  I  : 암호화가 된 로그 파일
//                                                     - Full Path를 쓴다.
//                 LPSTR      : pDest         :  O  : 암호화기 풀린 로그 파일
//                                                     - Full Path를 쓴다.
//                 LPCSTR     : pszPassword   :  I  : 암호화 할 패스워드
//                                                     - 128자 이내로 한다.
//                 int        : nIndex        :  I  : 위치 번호 멀티로 할때 필요
//                 HWND       : hWnd          :  I  : 진행 상황을 알릴 윈도우 핸들
// Return        : 성공 : 0 (NH_CRYPT_SUCCESS)
//                 실패 : -1 ( 함수 에러 : NH_CRYPT_ERROR )
//                        -5 ( 암호화된 파일 없음 : NH_CRYPT_INFILE_NOT_FOUND )
//                        -6 ( 복호화 파일 생성 실패 : NH_CRYPT_OUTFILE_CREATE_FAIL )
// Release       : VER 0.9
// Initial Date  : 2003.11.11
// Notes         : 복호화하는 함수로 파일을 만들어 낸다.
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
	// 파일 오픈
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
	BYTE bCBuff[ENCRYPT_BUFF_SIZE*2]= { 0, };				// Crypto Buffer(2배로 잡는다)
	int nCPos						= 0;					// Crypto Buffer의 차있는 자료의 크기
	DWORD dwRByte					= 0;					// File에서 읽은 자료의 Bytes 크기
	BOOL bSCryptoTerm				= FALSE;				// 암호화 구간 시작 표시 플래그
	BOOL bECryptoTerm				= FALSE;				// 암호화 구간 종료 표시 플래그
	int nPWDNo						= CRYPT_PAD;			// 암호화 키 종류
	DWORD dwCalcBytes				= 0;
	
	memset(bRBuff, 0x00, sizeof(bRBuff));					// 초기화
	memset(bCBuff, 0x00, sizeof(bCBuff));
	
	while ( !feof(hSrcFile) )								// File의 끝까지 왔는가?
	{
		memset(bRBuff, 0x00, sizeof(bRBuff));

		// 블럭 크기만큼 파일에서 읽어들인다.
		dwRByte = fread(bRBuff, sizeof(BYTE), sizeof(bRBuff), hSrcFile);

		int nRPos		= 0;								// 남은 읽은 자료량
		while ( nRPos < (int)dwRByte )						// 읽은 자료를 다 쓴 경우 빠져 나온다.
		{
			//------------------------------------------------------------
			// 암호화 구간이 끝나면 복호화한다.
			//
			// 0. 버퍼의 크기에 다다랐는데 암호화 구간이 시작하지 않았을 
			//    경우는 그냥 파일에 쓴다. ( 90%가 넘으면 한다 )
			// 1. 암호화 구간으로 들어 갔는데.. Crypto Buffer에 자료가 들어
			//    있다면 파일에 기록하고 Crypto Buffer를 비운다.
			// 2. 암호화 구간이 중이라면 진행
			// 3. 암호화 구간이 끝났다면 복호화 하고 파일에 기록 후 
			//    Crypto Buffer를 클리어 한다.
			//
			switch( bRBuff[nRPos] ) 
			{
				// 정의된 패스워드를 쓰는 패드
				case 0x00 :  case 0x01 :  case 0x02 :
				case 0x03 :  case 0x04 :  case 0x05 :
				case 0x06 :  case 0x07 :  case 0x08 :
				case 0x09 :
					if ( bSCryptoTerm ) 
					{
						bECryptoTerm = TRUE;
						if ( nPWDNo == bRBuff[nRPos] )		// 패드 값이 처음과 같을때만
							nRPos++;						// 증가 시킨다.
					}
					else 
					{
						bSCryptoTerm	= TRUE;				// 암호화 구간 시간
						nPWDNo			= bRBuff[nRPos++];	// 암호화 키 값

						if ( 0 != nCPos )
						{
							// 대상 파일에 원문 메세지를 저장한다.
							fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
							
							// 버퍼를 초기화 한다.
							memset(bCBuff, 0x00, sizeof(bCBuff));
							nCPos	= 0;
							
							::PostMessage(hWnd, WM_PROGRESS_NOTIFY, dwCalcBytes, nIndex);
						}
					}
					break;
				// 개인 암호를 쓰는 패드
				case CRYPT_PAD : 
					if ( bSCryptoTerm ) 
					{
						bECryptoTerm = TRUE;
						if ( nPWDNo == bRBuff[nRPos] )		// 패드 값이 처음과 같을때만
							nRPos++;						// 증가 시킨다.
					}
					else 
					{
						bSCryptoTerm	= TRUE;				// 암호화 구간 시간
						nPWDNo			= CRYPT_PAD;		// 암호화 키 값
						nRPos++;
						
						if ( 0 != nCPos )
						{
							// 대상 파일에 원문 메세지를 저장한다.
							fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
							
							// 버퍼를 초기화 한다.
							memset(bCBuff, 0x00, sizeof(bCBuff));
							nCPos	= 0;
							
							::PostMessage(hWnd, WM_PROGRESS_NOTIFY, dwCalcBytes, nIndex);
						}
					}
					break;
				default:
					bCBuff[nCPos++]	= bRBuff[nRPos++];		// 값 저장
			}

			// 전체 량을 구하기 위한 처리
			dwCalcBytes++;

			//------------------------------------------------------------
			// 암호화 구간이 끝나면 복호화한다.
			if ( ( ENCRYPT_BUFF_SIZE - (ENCRYPT_BUFF_SIZE / 10) ) < nCPos )
			{
				// 대상 파일에 원문 메세지를 저장한다.
				fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
				
				// 버퍼를 초기화 한다.
				memset(bCBuff, 0x00, sizeof(bCBuff));
				nCPos	= 0;

				::PostMessage(hWnd, WM_PROGRESS_NOTIFY, dwCalcBytes, nIndex);
			}
			else if ( bSCryptoTerm && bECryptoTerm )
			{
				// 복호화 키(암호)가 필요한데 없을 경우는 그냥 파일에 저장한다.
				if ( (CRYPT_PAD == nPWDNo) && (NULL == pszPassword) )
				{
					BYTE bTBuff[3] = { CRYPT_PAD, 0, 0};
					// 대상 파일에 원문 메세지를 저장한다.
					fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
					// 대상 파일에 원문 메세지를 저장한다.
					fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
					// 대상 파일에 원문 메세지를 저장한다.
					fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
				}
				else
				{
					LPSTR pszPWD			= NULL;
					BOOL bEqual				= FALSE;
					DWORD nPackCryptoLen	= 0;
					BYTE bPackCrypto[ENCRYPT_BUFF_SIZE] = { 0, };
					
					// Password 설정
					if ( CRYPT_PAD == (BYTE)nPWDNo ) pszPWD = (LPSTR)pszPassword;
					else pszPWD = szPasswordTable[nPWDNo];

					//--------------------------------------------------------------------
					// 암호화 시작
					if ( 0 != ( nRet = NH_CryptoStart(hCryptProv, hHash) ) )
					{
						TRACE("NH_CryptoStart failed.[0x%08X]\n", GetLastError());
						return nRet;
					}
					
					//----------------------------------------------------
					// 패스워드 해쉬
					if ( 0 != ( nRet = NH_HashPWD(hCryptProv, hKey, hHash, pszPWD) ) )
					{
						TRACE("NH_HashPWD failed.[0x%08X]\n", GetLastError());
						
						BYTE bTBuff[3] = { nPWDNo, 0, 0};
						// 대상 파일에 원문 메세지를 저장한다.
						fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
						// 대상 파일에 원문 메세지를 저장한다.
						fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
						// 대상 파일에 원문 메세지를 저장한다.
						fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
					}
					else
					{
						// 받은 자료를 팩한다.
						nPackCryptoLen = MakePack(bCBuff, bPackCrypto, nCPos);
						
						//----------------------------------------------------
						// 자료를 복호화한다.
						DWORD dwCHLen	= 20;
						NH_DecryptString(hCryptProv, hKey, hHash,
							             bPackCrypto, nPackCryptoLen, 
										 dwCHLen, bEqual);

						// 만약 체크 해쉬가 FALSE이면 그냥 데이터를 저장하고 아니면 
						// 복호화된 자료를 저장한다.
						if ( bEqual )
						{
							// 대상 파일에 원문 메세지를 저장한다.
							char szTemp[ENCRYPT_BUFF_SIZE] = { 0, };
							int nszTempPos = 0;
							for(DWORD i = 0; i < nPackCryptoLen; i++)
							{
								// 마지막에 0x0A만 있는 것을 0x0D0A로 바꾸어 주어야
								// MS용 Text 문서로 인식한다.
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
							// 대상 파일에 원문 메세지를 저장한다.
							fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
							// 대상 파일에 원문 메세지를 저장한다.
							fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
							// 대상 파일에 원문 메세지를 저장한다.
							fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
						}
					}

					//---------------------------------------------------
					// 암호화 종료
					NH_CryptoEnd(hCryptProv, hKey, hHash);
				}

				// 버퍼를 초기화 한다.
				memset(bCBuff, 0x00, sizeof(bCBuff));
				nCPos	= 0;

				// 값들을 초기화한다.
				bSCryptoTerm	= FALSE;
				bECryptoTerm	= FALSE;

				::PostMessage(hWnd, WM_PROGRESS_NOTIFY, dwCalcBytes, nIndex);
			}
		}
		::PostMessage(hWnd, WM_PROGRESS_NOTIFY, dwCalcBytes, nIndex);
	}
	
	//--------------------------------------------------------------------
	// 버퍼에 남은 잔여 자료가 있다면 처리한다. 이때 것은 그대로 쓴다.
	if ( bSCryptoTerm && 0 != nCPos )
	{
		BYTE bTBuff[3] = { nPWDNo, 0, 0};
		// 대상 파일에 원문 메세지를 저장한다.
		fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
		// 대상 파일에 원문 메세지를 저장한다.
		fwrite(bCBuff, sizeof(BYTE), nCPos, hDestFile);
		// 대상 파일에 원문 메세지를 저장한다.
		fwrite(bTBuff, sizeof(BYTE), 1, hDestFile);
	}
	else
	{
		char szTemp[ENCRYPT_BUFF_SIZE] = { 0, };
		int nszTempPos = 0;
		for(int i = 0; i < nCPos; i++)
		{
			// 마지막에 0x0A만 있는 것을 0x0D0A로 바꾸어 주어야
			// MS용 Text 문서로 인식한다.
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
	// 파일 리소스 반환
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
