//******************************************************************************
//
// File Name        : Cryptogram.h
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

#ifndef _CRYPTOGRAM_H_
#define _CRYPTOGRAM_H_

//#include <wincrypt.h>							// SDK��
#include "wincrypt.h"							// Non SDK��

//////////////////////////////////////////////////////////////////////
// Define
//////////////////////////////////////////////////////////////////////
//#define MY_ENCODING_TYPE			(PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
#define ENCRYPT_BUFF_SIZE			STRBUFFSIZE					// ��ȣȭ ���ۻ�����
#define KEYLENGTH					0x00800000					// ��ȣȭ�� ����� Ű�� ����
#define ENCRYPT_ALGORITHM			CALG_RC4					// ��ȣȭ �˰���
#define ENCRYPT_BLOCK_SIZE			8							// ��ȣȭ ��ϻ�����
#define HASHLENGTH					128							// ������ Ȯ�� �ؽ� ����
#define PWD_MAX_SIZE				128							// �н������� �ִ� ����
#define PWD_MAX_COUNT				10							// �н������� ����
#define CRYPT_PAD					0x80						// ��ȣȭ �е�(��ȣ�� ������)

// Error Define
#define NH_CRYPT_SUCCESS				0
#define NH_CRYPT_ERROR					-1
#define NH_CRYPT_ERROR_BASE				NH_CRYPT_ERROR
#define NH_CRYPT_INDATA_OVER			(NH_CRYPT_ERROR_BASE - 1)		// -2 ( �Է� �ڷᰡ �ʹ� ŭ )
#define NH_CRYPT_OUTBUFF_SMALL			(NH_CRYPT_ERROR_BASE - 2)		// -3 ( ��ȣȭ ������ ���� ���̺��� ������ ������ ���̰� ���� )
#define NH_CRYPT_OUTBUFF_ERR			(NH_CRYPT_ERROR_BASE - 3)		// -4 ( �Է� ���� ���̺��� ������ ������ ���̰� ���� )
#define NH_CRYPT_INFILE_NOT_FOUND		(NH_CRYPT_ERROR_BASE - 4)		// -5 ( ��ȣȭ�� ���� ���� )
#define NH_CRYPT_OUTFILE_CREATE_FAIL	(NH_CRYPT_ERROR_BASE - 5)		// -6 ( ��ȣȭ ���� ���� ���� )

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
int NH_CryptoStart(HCRYPTPROV& hCryptProv, HCRYPTHASH& hHash);

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
int NH_HashPWD(HCRYPTPROV& hCryptProv, HCRYPTKEY& hKey, HCRYPTHASH& hHash, LPSTR pszPassword);

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
int NH_CryptoEnd(HCRYPTPROV& hCryptProv, HCRYPTKEY& hKey, HCRYPTHASH& hHash);

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
int NH_EncryptString(HCRYPTPROV& hCryptProv, HCRYPTKEY& hKey, HCRYPTHASH& hHash, 
					 LPBYTE pbData, DWORD& dwDataLen, LPBYTE pbCheckHash, DWORD& dwCHLen);

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
int NH_DecryptString(HCRYPTPROV& hCryptProv, HCRYPTKEY& hKey, HCRYPTHASH& hHash,
					 LPBYTE pbData, DWORD& dwDataLen, 
					 DWORD& dwCHLen, BOOL& bEqual);

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
int LogEncrypt(LPCSTR pSrc, DWORD dwSrc, LPSTR pDest, DWORD& dwDest, LPCSTR pszPassword = NULL);

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
//int LogDecrypt(LPCSTR pSrc, LPSTR pDest, LPCSTR pszPassword = NULL, int nIndex = 0, HWND hWnd = NULL);

#endif	// _CRYPTOGRAM_H_
