/////////////////////////////////////////////////////////////////////////////
//	Total.cpp
/////////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\Profile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

WithName WITHNAME[] = {
//   01234567890123456789
    "0:CWD              :",
    "1:CWC              :",
    "2:CWD2             :",
    "3:CWD3             :",
    "4:CWD4             :",
    "5:CWD5             :",
    "6:CWD6             :",
    "7:CWD7             :",
    "8:CWD8             :",
    "9:CWD9             :",
    "10:CWD10           :",
    "11:CWD11           :",
    "12:CWD12           :",
    "13:CWD13           :",
    "14:CWD14           :",
    "15:CWD15           :",
    "16:CWD16           :"
};

InqName INQNAME[] = {
//   01234567890123456789
    "0:INQ              :",
    "1:INQ1             :",
    "2:INQ2             :",
    "3:INQ3             :",
    "4:INQ4             :",
    "5:INQ5             :",
    "6:INQ6             :",
    "7:INQ7             :",
    "8:INQ8             :",
    "9:INQ9             :",
    "10:INQ10           :",
    "11:INQ11           :",
    "12:INQ12           :",
    "13:INQ13           :",
    "14:INQ14           :",
    "15:INQ15           :",
    "16:INQ16           :"
};

TransName TRANSNAME[]={
//   01234567890123456789
    "0:TFR              :",
    "1:TFR1             :",
    "2:TFR2             :",
    "3:TFR3             :",
    "4:TFR4             :",
    "5:TFR5             :",
    "6:TFR6             :",
    "7:TFR7             :",
    "8:TFR8             :",
    "9:TFR9             :",
    "10:TFR10           :",
    "11:TFR11           :",
    "12:TFR12           :",
    "13:TFR13           :",
    "14:TFR14           :",
    "15:TFR15           :",
    "16:TFR16           :"
};

DepName DEPNAME[]={
//   01234567890123456789
    "0:DEP              :",
    "1:DPC              :",
    "2:DEP2             :",
    "3:DEP3             :",
    "4:DEP4             :",
    "5:DEP5             :",
    "6:DEP6             :",
    "7:DEP7             :",
    "8:DEP8             :",
    "9:DEP9             :",
    "10:DEP10           :",
    "11:DEP11           :",
    "12:DEP12           :",
    "13:DEP13           :",
    "14:DEP14           :",
    "15:DEP15           :",
    "16:DEP16           :"
};

/////////////////////////////////////////////////////////////////////////////
//	Init Withdraw Total
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitWithTotal()
{
	memset(&WITHTOTAL, 0, sizeof(WITHTOTAL));

	for (int i = 0; i < itemof(WITHTOTAL.DATA); i++)
		StrAllZero(WITHTOTAL.DATA[i]);

	PutWithTotal();
}

/////////////////////////////////////////////////////////////////////////////
//	Init Inquiry Total
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitInqTotal()
{
	memset(&INQTOTAL, 0, sizeof(INQTOTAL));

	for (int i = 0; i < itemof(INQTOTAL.DATA); i++)
		StrAllZero(INQTOTAL.DATA[i]);

	PutInqTotal();
}

/////////////////////////////////////////////////////////////////////////////
//	Init Transfer Total
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitTransTotal()
{
	memset(&TRANSTOTAL, 0, sizeof(TRANSTOTAL));

	for (int i = 0; i < itemof(TRANSTOTAL.DATA); i++)
		StrAllZero(TRANSTOTAL.DATA[i]);

	PutTransTotal();
}

/////////////////////////////////////////////////////////////////////////////
//	Init Deposit Total
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitDepTotal()
{
	memset(&DEPTOTAL, 0, sizeof(DEPTOTAL));

	for (int i = 0; i < itemof(DEPTOTAL.DATA); i++)
		StrAllZero(DEPTOTAL.DATA[i]);

	PutDepTotal();
}

/////////////////////////////////////////////////////////////////////////////
//	Withdraw Info
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetWithTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(WITHTOTAL.DATA); i++)
		GetPStr(WITHTOTAL_SEC, WITHNAME[i].name, '?', &WITHTOTAL.DATA[i], sizeof(WITHTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Inquiry Info
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetInqTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(INQTOTAL.DATA); i++)
		GetPStr(INQTOTAL_SEC, INQNAME[i].name, '?', &INQTOTAL.DATA[i], sizeof(INQTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Transfer Info
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetTransTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(TRANSTOTAL.DATA); i++)
		GetPStr(TRANSTOTAL_SEC, TRANSNAME[i].name, '?', &TRANSTOTAL.DATA[i], sizeof(TRANSTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Deposit Info
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetDepTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(DEPTOTAL.DATA); i++)
		GetPStr(DEPTOTAL_SEC, DEPNAME[i].name, '?', &DEPTOTAL.DATA[i], sizeof(DEPTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Withdraw Total
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutWithTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(WITHTOTAL.DATA); i++)
		PutPStr(WITHTOTAL_SEC, WITHNAME[i].name, &WITHTOTAL.DATA[i], sizeof(WITHTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Inquiry Total
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutInqTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(INQTOTAL.DATA); i++)
		PutPStr(INQTOTAL_SEC, INQNAME[i].name, &INQTOTAL.DATA[i], sizeof(INQTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Transfer Total
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutTransTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(TRANSTOTAL.DATA); i++)
		PutPStr(TRANSTOTAL_SEC, TRANSNAME[i].name, &TRANSTOTAL.DATA[i], sizeof(TRANSTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Deposit Total
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutDepTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(DEPTOTAL.DATA); i++)
		PutPStr(DEPTOTAL_SEC, DEPNAME[i].name, &DEPTOTAL.DATA[i], sizeof(DEPTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

