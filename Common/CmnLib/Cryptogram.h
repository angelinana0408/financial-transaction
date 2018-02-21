//******************************************************************************
//
// File Name        : Cryptogram.h
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

#ifndef _CRYPTOGRAM_H_
#define _CRYPTOGRAM_H_

//#include <wincrypt.h>							// SDK용
#include "wincrypt.h"							// Non SDK용

//////////////////////////////////////////////////////////////////////
// Define
//////////////////////////////////////////////////////////////////////
//#define MY_ENCODING_TYPE			(PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
#define ENCRYPT_BUFF_SIZE			STRBUFFSIZE					// 암호화 버퍼사이즈
#define KEYLENGTH					0x00800000					// 암호화에 사용할 키의 길이
#define ENCRYPT_ALGORITHM			CALG_RC4					// 암호화 알고리즘
#define ENCRYPT_BLOCK_SIZE			8							// 암호화 블록사이즈
#define HASHLENGTH					128							// 데이터 확인 해쉬 길이
#define PWD_MAX_SIZE				128							// 패스워드의 최대 길이
#define PWD_MAX_COUNT				10							// 패스워드의 갯수
#define CRYPT_PAD					0x80						// 암호화 패드(암호가 있을시)

// Error Define
#define NH_CRYPT_SUCCESS				0
#define NH_CRYPT_ERROR					-1
#define NH_CRYPT_ERROR_BASE				NH_CRYPT_ERROR
#define NH_CRYPT_INDATA_OVER			(NH_CRYPT_ERROR_BASE - 1)		// -2 ( 입력 자료가 너무 큼 )
#define NH_CRYPT_OUTBUFF_SMALL			(NH_CRYPT_ERROR_BASE - 2)		// -3 ( 암호화 데이터 편집 길이보다 내보낼 버퍼의 길이가 작음 )
#define NH_CRYPT_OUTBUFF_ERR			(NH_CRYPT_ERROR_BASE - 3)		// -4 ( 입력 버퍼 길이보다 내보낼 버퍼의 길이가 작음 )
#define NH_CRYPT_INFILE_NOT_FOUND		(NH_CRYPT_ERROR_BASE - 4)		// -5 ( 암호화된 파일 없음 )
#define NH_CRYPT_OUTFILE_CREATE_FAIL	(NH_CRYPT_ERROR_BASE - 5)		// -6 ( 복호화 파일 생성 실패 )

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
int NH_CryptoStart(HCRYPTPROV& hCryptProv, HCRYPTHASH& hHash);

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
int NH_HashPWD(HCRYPTPROV& hCryptProv, HCRYPTKEY& hKey, HCRYPTHASH& hHash, LPSTR pszPassword);

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
int NH_CryptoEnd(HCRYPTPROV& hCryptProv, HCRYPTKEY& hKey, HCRYPTHASH& hHash);

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
int NH_EncryptString(HCRYPTPROV& hCryptProv, HCRYPTKEY& hKey, HCRYPTHASH& hHash, 
					 LPBYTE pbData, DWORD& dwDataLen, LPBYTE pbCheckHash, DWORD& dwCHLen);

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
int NH_DecryptString(HCRYPTPROV& hCryptProv, HCRYPTKEY& hKey, HCRYPTHASH& hHash,
					 LPBYTE pbData, DWORD& dwDataLen, 
					 DWORD& dwCHLen, BOOL& bEqual);

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
int LogEncrypt(LPCSTR pSrc, DWORD dwSrc, LPSTR pDest, DWORD& dwDest, LPCSTR pszPassword = NULL);

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
//int LogDecrypt(LPCSTR pSrc, LPSTR pDest, LPCSTR pszPassword = NULL, int nIndex = 0, HWND hWnd = NULL);

#endif	// _CRYPTOGRAM_H_
