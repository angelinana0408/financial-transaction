#include "stdafx.h"
#include "CmnLibEx.h"

/////////////////////////////////////////////////
//
// 명령재사용관련
//
char* WINAPI Strcpy(void* dBuffc, void* sBuffc)
{
	return strcpy((char*)dBuffc, (char*)sBuffc);
}

char* WINAPI Strcpy(void* dBuffc, LPCTSTR sBuffc)
{
	return strcpy((char*)dBuffc, (char*)sBuffc);
}

char* WINAPI Strcpy(void* dBuffc, void* sBuffc, int slen)
{
	return strncpy((char*)dBuffc, (char*)sBuffc, slen);
}

char*	WINAPI Strcpy(void* dBuffc, LPCTSTR sBuffc, int slen)
{
	return strncpy((char*)dBuffc, (char*)sBuffc, slen);
}

char*	WINAPI StrcpyRight(void* dBuffc, void* sBuffc, int slen, int dlen, char InitChar)
{
	char*	dBuff = (char*)dBuffc;
	char*	sBuff = (char*)sBuffc;

	memset(dBuff, InitChar, dlen);
	if (dlen >= slen)
		memcpy(&dBuff[dlen - slen], sBuff, slen);
	else
		memcpy(dBuff, &sBuff[slen - dlen], dlen);
	return dBuff;
}

char*	WINAPI StrcpyRight(void* dBuffc, LPCTSTR sBuffc, int slen, int dlen, char InitChar)
{
	return StrcpyRight(dBuffc, (void*)sBuffc, slen, dlen, InitChar);
}

LPCTSTR WINAPI Strcpy(CString& dBuffc, void* sBuffc)
{
	dBuffc.Format("%s", sBuffc);
	return dBuffc;
}

LPCTSTR WINAPI Strcpy(CString& dBuffc, void* sBuffc, int slen)
{
	char	buf[256];

	sprintf(buf, "%%%d.%ds", slen);
	dBuffc.Format(buf, sBuffc);
	return dBuffc;
}

int WINAPI Strlen(void* sBuffc)
{
	return strlen((char*)sBuffc);
}

/////////////////////////////////////////////////
//
// Warning - SBuff와 kBuff의 크기가 동일하여야함.
// Ex) "123456" + "123456" = "246912"
//
char* WINAPI AddString(void* sBuffc, void* kBuffc, int klen, void* dBuffc)
{
	char*	sBuff = (char*)sBuffc;
	char*	kBuff = (char*)kBuffc;
	char*	dBuff = (char*)dBuffc;
	unsigned char CARRY, tmpCh;
	int		i = klen;
	char	tmp[1024];

	CARRY = 0;
	memset(tmp, 0x00, sizeof(tmp));
	memcpy(tmp, sBuff, klen);

	while (i > 0)
	{
		tmpCh = (BYTE)(tmp[i-1] & 0x0f);
		tmpCh += (BYTE)(kBuff[i-1] & 0x0f);
		tmpCh += CARRY;
		if (tmpCh >= 10)
		{
			tmpCh -= 10;
			CARRY = 1;
		}
		else CARRY = 0;
		tmp[i-1] = (char)(tmpCh | 0x30);
		i--;
	}

	if (dBuff)
	{
		memcpy(dBuff, tmp, klen); 
		return ((char*)dBuffc);
	}
	else
	{
		memcpy(sBuff, tmp, klen); 
		return ((char*)sBuffc);
	}
}

//////////////////////////////////////////////////
//
// Warning - SBuff와 kBuff의 크기가 동일하여야함.
// 반드시 Sbuff의 수가 커야함.
// Ex) "234567" - "123456" = "111111"
//
char*  WINAPI SubString(void* sBuffc, void* kBuffc, int klen, void* dBuffc)
{
	char*	sBuff = (char*)sBuffc;
	char*	kBuff = (char*)kBuffc;
	char*	dBuff = (char*)dBuffc;
	unsigned char CARRY, tmpCh;
	int		i = klen;
	char	tmp[1024];

	CARRY = 0;
	memset(tmp, 0x00, sizeof(tmp));
	memcpy(tmp, sBuff, klen);

	while (i > 0)
	{
		tmpCh = (BYTE)(tmp[i-1] & 0x0f);
		tmpCh += 10;
		tmpCh -= (BYTE)(kBuff[i-1] & 0x0f);
		tmpCh -= CARRY;
		if (tmpCh >= 10)
		{
			tmpCh -= 10;
			CARRY = 0;
		}
		else CARRY = 1;
		tmp[i-1] = (char)(tmpCh | 0x30);
		i--;
	}

	if (dBuff)
	{
		memcpy(dBuff, tmp, klen); 
		return ((char*)dBuffc);
	}
	else
	{
		memcpy(sBuff, tmp, klen); 
		return ((char*)sBuffc);
	}
}

////////////////////////////////////////
//
//	Asc type to Integer type
//  Ex) "123456" -> 123456
//
int WINAPI Asc2Int(void* lpvoid, int length)
{
    int		ivalue=0,bminus=FALSE;
    int		i;
	char*	asc = (char* )lpvoid;

	if (!length)
		return 0;

    for(i=0;i<length;i++)
    {
        if(asc[i]>='0' && asc[i]<='9')
        {
            ivalue = (ivalue*10) + (asc[i]-'0');
            continue;
        }
        if(asc[i]=='-') bminus = TRUE;
    }

    return (bminus) ? 0-ivalue : ivalue;
}

int WINAPI Asc2Int(LPCTSTR lpvoid, int length)
{
	return (Asc2Int((void*)lpvoid, length));
}

int WINAPI Asc2Int(LPCTSTR lpvoid)
{
	return (Asc2Int((void*)lpvoid, strlen(lpvoid)));
}

int	WINAPI Asc2IntHighOrder(LPCTSTR lpvoid, void* kBuffc, int inLen, int outLen)
{
	unsigned char*	kBuff = (unsigned char*)kBuffc;
	int				ivalue = Asc2Int((void*)lpvoid, inLen);
    unsigned char*	lpValue =(unsigned char *)&ivalue;
	int				k = 0;
	
	memset(kBuff, 0, outLen);

	for (int i = 0; i < outLen; i++)
	{
		k = outLen - i - 1;
		if (k < 4)
			kBuff[i] = lpValue[k];
		else
			kBuff[i] = 0x00;
	}

	return ivalue;
}

////////////////////////////////////////
//
//	Asc type to Integer type
//  Ex) "123456" -> 123456
//
double WINAPI Asc2Float(void* lpvoid, int length)
{
	double	ivalue=0;
	int		i;
	char*	asc = (char* )lpvoid;

	if (!length)
		return 0.;

	for(i=0;i<length;i++)
	{
		if(asc[i]>='0' && asc[i]<='9')
		{
			ivalue = (ivalue*10.) + (double)(asc[i]-'0');
		}
	}
	return ivalue;
}

double WINAPI Asc2Float(LPCTSTR lpvoid, int length)
{
	return (Asc2Float((void*)lpvoid, length));
}

double WINAPI Asc2Float(LPCTSTR lpvoid)
{
	return (Asc2Float((void*)lpvoid, strlen(lpvoid)));
}

//////////////////////////////////////////
CString WINAPI Asc2Amt(void* inbuff,int inLen,int outLen,char MarkChar)
{
	char	amount[1024];
	CString	cstramount("");

	memset(amount,0x00,sizeof(amount));
	EditAmount(inbuff,amount,inLen,outLen);
	MarkAmount(amount, outLen, MarkChar);

	Strcpy(cstramount, amount);
	return cstramount;
}

CString WINAPI Asc2Amt(LPCTSTR inbuff,int inLen,int outLen,char MarkChar)
{
	return Asc2Amt((void*)inbuff, inLen, outLen);
}

////////////////////////////////////////
//	Ex) "00123456" -> "  123456"
CString WINAPI Asc2String(void* lpvoid, int inLen, int outLen)
{
	char	str[1024];
	char	str2[1024];
	CString	cstrstr("");

	memset(str,0x00,sizeof(str));
	memset(str2,0x00,sizeof(str2));
	memcpy(str,lpvoid,inLen);
	
	for(int i = 0; i < inLen; i++)
	{
		if (str[i] == '.')
		{
			if (i)	
				str[i - 1] = '0';
			break;
		}

		if (str[i] != '0')	
			break;

		str[i] = ' ';
	}

	StrcpyRight(str2, str, inLen, outLen, ' ');
	Strcpy(cstrstr, str2);
	return cstrstr;
}

/////////////////////////////////////////////////
//
// Warning - Target Buffer의 크기가 
//           Conversion한 Data의 크기 보다 작다면
//           Error가 발생할 것이다.
// Ex) 340 -> "340"
//
char* WINAPI Int2Asc(int Value, void* Target, int len, int mode, char InitChar)
{
// mode : decimal,hex, binary..
	char	tmp[1024];
    char	*Dst  = (char* )Target;
	int		slen;

	memset(tmp, 0x00, sizeof(tmp));
	_itoa(Value, tmp, mode);
	Lower2Upper(tmp, strlen(tmp));
	memset(Dst, InitChar, len);
	slen = strlen(tmp);
	if (len >= slen)
		memcpy(&Dst[len-slen], tmp, slen);
	else memcpy(Dst, &tmp[slen-len], len);
	return ((char*)Target);
}

CString WINAPI Int2Asc(int Value, int len, int mode, char InitChar)
{
	CString	cstrstr("");
    char buf[1024];
    memset(buf, 0x00, sizeof(buf));
	if (len)
		Int2Asc(Value, buf, len, mode, InitChar);
	else														// len=0이면 그대로
	{
		_itoa(Value, buf, mode);
		Lower2Upper(buf, strlen(buf));
	}

	Strcpy(cstrstr, buf);
	return cstrstr;
}

////////////////////////////////////////
int	WINAPI SplitString(CString cstrSrc, CString cstrSplit, CStringArray& acstrRet)
{
	CString cstrTemp = cstrSrc;
	CString cstrData("");
	int		nIndex=0, nCnt=0;

	acstrRet.RemoveAll();										// 20030703 : 초기화하지 않으면 원래배열의 뒤에 추가됨
	
	while (TRUE)												// 20031030 : 무조건처리로변경
	{
		nIndex = cstrTemp.Find(cstrSplit);
		if(nIndex == -1)
		{
			acstrRet.Add(cstrTemp);								// 20031030 : 무조건처리로변경
			nCnt++;												// 20031030 : 마지막처리추가
			return nCnt;
		}
		else
		{
			cstrData = cstrTemp.Left(nIndex);
			cstrTemp = cstrTemp.Right(cstrTemp.GetLength() - (nIndex + cstrSplit.GetLength()));  
			acstrRet.Add(cstrData);
			nCnt++;
		}
	}
	return nCnt;
}

////////////////////////////////////////
// 한글절사
CString WINAPI GetString(void* inbuff,int inLen,int outLen)
{
	BYTE	str[1024];
	CString	cstrstr("");

    BYTE    *instr  = (BYTE* )inbuff;
	int		i;

	memset(str,0x00,sizeof(str));

	if(inLen <= outLen)
	{
		memset(str,0x20,outLen);
		outLen = inLen;
	}

	for (i=0;i<outLen;i++)
	{
		if (instr[i] & 0x80)									// hangul
		{
			if (i+1 < outLen)
			{
				if (instr[i+1] & 0x80)							// hangul garbage clear
				{
					str[i] = instr[i];	i++;
					str[i] = instr[i];
				}
				else
				{
					str[i] = ' ';	i++;
					str[i] = instr[i];
				}
			}
			else
			{
				str[i] = ' ';									// hangul left clear
			}
		}
		else
		{
			if (instr[i] < ' ')									// spec. clear
				str[i] = ' ';
			else 
				str[i] = instr[i];
		}
	}

	Strcpy(cstrstr, str);
	return cstrstr;
}
	
////////////////////////////////////////
// 한글절사(전각=>반각)
CString WINAPI GetString2Half(void* inbuff,int inLen,int outLen)
{
	BYTE	str[1024];
	CString	cstrstr("");

    BYTE    *instr  = (BYTE* )inbuff;
	int		i;

	memset(str,0x00,sizeof(str));

	if(inLen <= outLen)
	{
		memset(str,0x20,outLen);
		outLen = inLen;
	}

	for (i=0;i<outLen;i++)
	{
		if (instr[i] & 0x80)									// hangul
		{
			if (i+1 < outLen)
			{
				if (instr[i+1] & 0x80)							// hangul garbage clear
				{
					if ((instr[i] == 0xa1) &&					// 전각Space
						(instr[i+1] == 0xa1))
					{
						str[i] = ' ';	i++;
						str[i] = ' ';
					}
					else
					if (instr[i] == 0xa3)						// 전각문자
					{
						str[i] = instr[i+1] & 0x7f;
						if (str[i] < ' ')						// spec. clear
							str[i] = ' ';
						i++;
						str[i] = ' ';
					}
					else										// 나머지
					{
						str[i] = instr[i];	i++;
						str[i] = instr[i];
					}
				}
				else
				{
					str[i] = ' ';	i++;
					str[i] = instr[i];
				}
			}
			else
			{
				str[i] = ' ';									// hangul left clear
			}
		}
		else
		{
			if (instr[i] < ' ')									// spec. clear
				str[i] = ' ';
			else 
				str[i] = instr[i];
		}
	}

	Strcpy(cstrstr, str);
	return cstrstr;
}
	
//////////////////////////////////////////
//
//  Ex1) "000100" -> return index 3
//  Ex2) "    20" -> return index 4
//
int WINAPI Compress(void* sBuff, int slen)
{
	BYTE	*bsBuff = (BYTE *)sBuff;

	for (int i = 0; i < slen; i++)
	{
		if ((bsBuff[i] != 0x30) && (bsBuff[i] != 0x20) && (bsBuff[i] != 0x00))
			break;
	}

	// if all data are 0x30, 0x20, 0x00
	if (i == slen)
		return 0xff;
	return i;
}

//////////////////////////////////////////
//  jhlee 
int WINAPI EditAmount(void* inbuff,void* outbuff,int inLen,int outLen)
{
    char    *instr  = (char* )inbuff;
    char    *outstr = (char* )outbuff;

    int		i,j,k,alen;
    int		isminus = 0,isfound=0;

    char    src[1024];

    for(i=0,alen=0;i<inLen;i++)
    {
        if(instr[i]=='-')
        {
            isminus = 1;
            continue;
        }

        if(!isfound)
        {
            if(instr[i]>='1' && instr[i]<='9') isfound = 1;
        }

        if(isfound)
        {
            if(instr[i]>='0' && instr[i]<='9')
            {
                src[alen++] = instr[i];
            }
        }
    }

    memset(outstr,0x20,outLen);
    if(alen==0)
    {
        outstr[outLen-1]='0';
        return 1;
    }

    j=outLen-1;
    i=alen-1;
    k = 0;

    while(i>=0)
    {
        outstr[j--]=src[i--];
        if(j<0) break;
        k++;

        if((k%3)==0 && i>=0)
        {
            outstr[j--]=',';
            k=0;
        }

    }

    if(j && isminus)    outstr[j]='-';

    return j;
}

int WINAPI MarkAmount(void* inbuff,int inLen, char Value)
{
    char    *instr  = (char* )inbuff;
	int i;
	for(i=inLen-1;i>=0;i--)
	{
		if(instr[i]==' ')
		{
			instr[i]=Value;
			break;
		}
	}
	return i;
}

int WINAPI EditString(void* inBufc, int inLen, char* fmt, void* outBufc, char MoveChar, char HiddenChar, char HidenMoveChar, int HiddenFlg)
{
	char*	inBuf = (char*)inBufc;
	char*	outBuf = (char*)outBufc;
	int	flen = strlen(fmt);
	int i,j=0;

	memset(outBuf,' ',flen);
	for(i=0;i<flen;i++)
	{
		if (j == inLen)
			break;

		if (fmt[i] == MoveChar)
			outBuf[i] = inBuf[j++];
		else 
		if ((HiddenFlg) &&
			(fmt[i] == HiddenChar))
			{
				outBuf[i] = HidenMoveChar;
				j++;
			}
		else
		if ((!HiddenFlg) &&
			(fmt[i] == HiddenChar))
			outBuf[i] = inBuf[j++];
		else outBuf[i] = fmt[i];	
	}
	return i;
}

int	WINAPI EditString(void* inBufc, int inLen, char* fmt, CString& outBufc, char MoveChar, char HiddenChar, char HidenMoveChar, int HiddenFlg)
{
    char buf[1024];

	memset(buf, 0, sizeof(buf));
	int i = EditString(inBufc, inLen, fmt, buf, MoveChar, HiddenChar, HidenMoveChar, HiddenFlg);
	Strcpy(outBufc, buf);
	return i;
}

int	WINAPI EditString(LPCTSTR inBufc, char* fmt, CString& outBufc, char MoveChar, char HiddenChar, char HidenMoveChar, int HiddenFlg)
{
    char buf[1024];

	memset(buf, 0, sizeof(buf));
	int i = EditString((LPTSTR)inBufc, strlen(inBufc), fmt, buf, MoveChar, HiddenChar, HidenMoveChar, HiddenFlg);
	Strcpy(outBufc, buf);
	return i;
}

CString	WINAPI EditString(void* inBufc, int inLen, char* fmt, char MoveChar, char HiddenChar, char HidenMoveChar, int HiddenFlg)
{
	CString	cstrstr("");

	EditString(inBufc, inLen, fmt, cstrstr, MoveChar, HiddenChar, HidenMoveChar, HiddenFlg);
	return cstrstr;
}

CString	WINAPI EditString(LPCTSTR inBufc, char* fmt, char MoveChar, char HiddenChar, char HidenMoveChar, int HiddenFlg)
{
	CString	cstrstr("");

	EditString((LPTSTR)inBufc, strlen(inBufc), fmt, cstrstr, MoveChar, HiddenChar, HidenMoveChar, HiddenFlg);
	return cstrstr;
}

int WINAPI Sprintf(void* dst,int len,char* fmt,...)
{
    char buf[1024];
    memset(buf, 0x00, sizeof(buf));
    int ret = vsprintf((LPSTR)buf, (LPSTR)fmt, (va_list)(&fmt+1));

	if(ret>len)
	{
		memcpy(dst,buf,len);
	}
	else
	{
		memcpy(dst,buf,ret);
	}

	return ret;
}

CString WINAPI GetSprintf(char* fmt,...)
{
	CString	cstrstr("");
    char buf[1024];
    memset(buf, 0x00, sizeof(buf));
    vsprintf((LPSTR)buf, (LPSTR)fmt, (va_list)(&fmt+1));

	Strcpy(cstrstr, buf);
	return cstrstr;
}

int WINAPI FindChar(void* inbuf,int len, char chr)
{
    char    *instr  = (char* )inbuf;

	for(int i=0;i<len;i++)	
	{
		if((instr[i]==chr) ||
		   (instr[i]==0x00))
			return i;
	}
	return len;
}

int WINAPI FindNotNum(void* inbuf,int len)
{
    char    *instr  = (char* )inbuf;

	for(int i=0;i<len;i++)	
	{
		if((instr[i]<'0') ||
		   (instr[i]>'9') ||
		   (instr[i]==0x00))
			return i;
	}
	return len;
}

int WINAPI IsChar(void* inbuf,int len, char chr)
{
    char    *instr  = (char* )inbuf;

	for(int i=0;i<len;i++)	if(instr[i]!=chr)	return FALSE;
	return TRUE;
}

int WINAPI IsSpace(void* inbuf,int len)
{
    char    *instr  = (char* )inbuf;

	for(int i=0;i<len;i++)	if(instr[i]!=' ')	return FALSE;
	return TRUE;
}

int WINAPI IsZero(void* inbuf,int len)
{
    char    *instr  = (char* )inbuf;

	for(int i=0;i<len;i++)	if(instr[i]!='0')	return FALSE;
	return TRUE;
}

int WINAPI IsNum(void* inbuf,int len)
{
    char    *instr  = (char* )inbuf;

	for(int i=0;i<len;i++)	if((instr[i]<'0') || (instr[i]>'9'))	return FALSE;
	return TRUE;
}

int WINAPI IsUnPack(void* inbuf,int len)
{
    char    *instr  = (char* )inbuf;

	for(int i=0;i<len;i++)	if((instr[i]<0x30) || (instr[i]>0x3f))	return FALSE;
	return TRUE;
}

// '000103' => '   103'
int WINAPI Zero2Space(void* sBuff, int slen)
{
	int ret = Compress(sBuff, slen);

	if (ret != 0xff)
		memset(sBuff, ' ', ret);
	else memset(sBuff, ' ', slen);
	return ret;
}

// 'abc' => 'ABC'
int WINAPI Lower2Upper(void* sBuff, int slen)
{
	BYTE	*bsBuff = (BYTE *)sBuff;

	for (int i=0;i<slen;i++) if((bsBuff[i]>='a') && (bsBuff[i]<='z')) bsBuff[i] &= 0xdf;
	return TRUE;
}

// 'ABC' => 'abc'
int WINAPI Upper2Lower(void* sBuff, int slen)
{
	BYTE	*bsBuff = (BYTE *)sBuff;

	for (int i=0;i<slen;i++) if((bsBuff[i]>='A') && (bsBuff[i]<='Z')) bsBuff[i] |= 0x20;
	return TRUE;
}

//////////////////////////////////////////
//	A Byte type to a Ascii string
//  Ex) 0x1f -> "1f"
void WINAPI Byte2Asc(BYTE byteptr, void* strptrc, int mode)
{
	char*	strptr = (char*)strptrc;

	if ((mode == 8) || (mode == 10))
		Int2Asc((int)byteptr, strptr, 3, mode, '0');
	else if (mode == 16)
		Int2Asc((int)byteptr, strptr, 2, mode, '0');
	else Int2Asc((int)byteptr, strptr, 8, mode, '0');
}

CString WINAPI Byte2Asc(BYTE byteptr, int mode)
{
	CString	cstrstr("");
    char buf[1024];
    memset(buf, 0x00, sizeof(buf));
    Byte2Asc(byteptr, buf, mode);

	Strcpy(cstrstr, buf);
	return cstrstr;
}

void WINAPI Byte2Ascn(BYTE *byteptr, void* strptrc, int blen, int mode)
{
	char*	strptr = (char*)strptrc;

	for (int i = 0; i < blen ; i++)
	{
		Byte2Asc(*byteptr, strptr, mode);
		byteptr++;
		
		if ((mode == 8) || (mode == 10))
			strptr += 3;
		else if (mode == 16)
			strptr += 2;
		else strptr += 8;
	}
}

CString WINAPI Byte2Ascn(BYTE *byteptr, int blen, int mode)
{
	CString	cstrstr("");
    char buf[STRBUFFSIZE];
    memset(buf, 0x00, sizeof(buf));
    Byte2Ascn(byteptr, buf, blen, mode);

	Strcpy(cstrstr, buf);
	return cstrstr;
}

// 12 34 56 78 => 78 56 34 12 => int로 변경함
int WINAPI ByteHighOrder2Int(BYTE* Src, int inLen)
{
	int m_int = 0;

	for (int i = 0; i < inLen; i++)
	{
		m_int = m_int << 8;
		m_int |= Src[i];
	}

	return m_int;
}

void WINAPI Asc2Byte(void* ascptrc, BYTE *byteptr)
{
	char*	ascptr = (char*)ascptrc;
	BYTE	tmp[2] ;

	if (*(ascptr) > '9')
	{
		if (*(ascptr) < 'a')
			tmp[0] = ((((BYTE)(*(ascptr))) - 0x37) << 4);
		else tmp[0] = ((((BYTE)(*(ascptr))) - 0x57) << 4);
	}
	else tmp[0] = ((((BYTE)(*(ascptr))) - '0') << 4);
	ascptr++;

	if (*(ascptr) > '9')
	{
		if (*(ascptr) < 'a')
			tmp[1] = (((BYTE)(*(ascptr))) - 0x37);
		else tmp[1] = (((BYTE)(*(ascptr))) - 0x57);
	}
	else tmp[1] = (((BYTE)(*(ascptr))) - '0');
	ascptr++;

	*byteptr = tmp[0] | tmp[1]; 
}

////////////////////////////////////////////////////////////////////////////
//	2 Digit Ascii type String to BYTE type string
//	Ex) 2 Digit Asc type "a23376f7a5" -> BYTE 0xa2,0x33,0x76,0xf7,0xa5 conversion
void WINAPI Asc2Byten(void* ascptrc, BYTE *byteptr, int blen)
{
	char*	ascptr = (char*)ascptrc;

	for (int i = 0; i < blen ; i++)
	{
		Asc2Byte(ascptr, byteptr);
		ascptr += 2;
		byteptr++;
	}
}

BYTE WINAPI MakeBcc(void* asc, int length)
{
	BYTE	*basc = (BYTE *)asc;
	int i = 1;

	BYTE cc = basc[0];
	while (i < length)
		cc ^= basc[i++];
	return (cc);
}

int WINAPI MakePack(void* Src, void* Dest, int Slen, char OperMode, int LowNibbleFlg)
{
	BYTE	*bSrc = (BYTE *)Src;
	BYTE	*bDest = (BYTE *)Dest;
	BYTE	Value = 0;
	int		i=0, j=0;

	for(i = 0; i < Slen; i++)
	{
		if ((bSrc[i] >= 'a') && (bSrc[i] <= 'f'))
			Value = ((bSrc[i] - 0x57) & 0x0f);
		else
		if ((bSrc[i] >= 'A') && (bSrc[i] <= 'F'))
			Value = ((bSrc[i] - 0x37) & 0x0f);
		else Value = (bSrc[i] & 0x0f);

		if (!LowNibbleFlg)
		{
			if (!(i & 1))
			{
				if (OperMode == '|')
					bDest[j] |= (Value << 4);
				else 
				if (OperMode == '^')
					bDest[j] ^= (Value << 4);
				else
					bDest[j] = (bDest[j] & 0x0f) | (Value << 4);
			}
			else
			{
				if (OperMode == '|')
					bDest[j] |= (Value);
				else 
				if (OperMode == '^')
					bDest[j] ^= (Value);
				else
					bDest[j] = (bDest[j] & 0xf0) | (Value);
				j++;
			}
		}
		else
		{
			if (i & 1)
			{
				if (OperMode == '|')
					bDest[j] |= (Value << 4);
				else 
				if (OperMode == '^')
					bDest[j] ^= (Value << 4);
				else
					bDest[j] = (bDest[j] & 0x0f) | (Value << 4);
			}
			else
			{
				if (OperMode == '|')
					bDest[j] |= (Value);
				else 
				if (OperMode == '^')
					bDest[j] ^= (Value);
				else
					bDest[j] = (bDest[j] & 0xf0) | (Value);
				j++;
			}
		}
	}
	return j;
}

int WINAPI MakeUnPack(void* Src, void* Dest, int Slen, int AscFlg)
{
	BYTE	*bSrc = (BYTE *)Src;
	BYTE	*bDest = (BYTE *)Dest;
	int		i=0, j=0;

	for(i = 0; i < Slen; i++)
	{
		if (!AscFlg)											// 0x30 - 0x3f
		{
			bDest[j++] = ((bSrc[i] & 0xf0) >> 4) | '0';
			bDest[j++] = (bSrc[i] & 0x0f) | '0';
		}
		else													// 0x30 - 0x39, 0x41 - 0x46
		{	
			bDest[j] = ((bSrc[i] & 0xf0) >> 4) | '0';
			if (bDest[j] >= 0x3a)
				bDest[j] += 0x07;
			j++;

			bDest[j] = (bSrc[i] & 0x0f) | '0';
			if (bDest[j] >= 0x3a)
				bDest[j] += 0x07;
			j++;
		}
	}
	return j;
}

int WINAPI MakeUnPack(void* Src, CString& Dest, int Slen, int AscFlg)
{
    char str[STRBUFFSIZE];

	memset(str, 0, sizeof(str));
	int i = MakeUnPack(Src, str, Slen, AscFlg);
	Strcpy(Dest, str);
	return i;
}

CString WINAPI MakeUnPack(void* Src, int Slen, int AscFlg)
{
	CString	cstrstr("");

	MakeUnPack(Src, cstrstr, Slen, AscFlg);
	return cstrstr;
}

void WINAPI GetDate(void* gDate)
{
	char	*cgDate = (char* )gDate;
	SYSTEMTIME	ti;

	GetLocalTime(&ti);
	Int2Asc(ti.wYear,	&cgDate[0], 4, 10, '0');
	Int2Asc(ti.wMonth,	&cgDate[4], 2, 10, '0');
	Int2Asc(ti.wDay,	&cgDate[6], 2, 10, '0');
}

CString WINAPI GetDate()
{
	CString	cstrstr("");
    char buf[1024];
    memset(buf, 0x00, sizeof(buf));
    GetDate(buf);

	Strcpy(cstrstr, buf);
	return cstrstr;
}

void WINAPI GetTime(void* gTime)
{
	char	*cgTime = (char* )gTime;
	SYSTEMTIME	ti;

	GetLocalTime(&ti);
	Int2Asc(ti.wHour,	&cgTime[0], 2, 10, '0');
	Int2Asc(ti.wMinute,	&cgTime[2], 2, 10, '0');
	Int2Asc(ti.wSecond,	&cgTime[4], 2, 10, '0');
}

CString WINAPI GetTime()
{
	CString	cstrstr("");
    char buf[1024];
    memset(buf, 0x00, sizeof(buf));
    GetTime(buf);

	Strcpy(cstrstr, buf);
	return cstrstr;
}

void WINAPI GetDateTime(void* gDate, void* gTime)
{
	GetDate(gDate);
	GetTime(gTime);
}

void WINAPI PutDate(void* pDate)
{
	char	*cpDate = (char* )pDate;
	SYSTEMTIME	ti;

	GetLocalTime(&ti);
	ti.wYear   = Asc2Int(&cpDate[0],4);
	ti.wMonth  = Asc2Int(&cpDate[4],2);
	ti.wDay    = Asc2Int(&cpDate[6],2);
	SetLocalTime(&ti);
}

void WINAPI PutDate(LPCTSTR pDate)
{
	PutDate((void*)pDate);
}

void WINAPI PutTime(void* pTime)
{
	char	*cpTime = (char* )pTime;
	SYSTEMTIME	ti;

	GetLocalTime(&ti);
	ti.wHour   = Asc2Int(&cpTime[0],2);
	ti.wMinute = Asc2Int(&cpTime[2],2);
	ti.wSecond = Asc2Int(&cpTime[4],2);
	SetLocalTime(&ti);
}

void WINAPI PutTime(LPCTSTR pTime)
{
	PutTime((void*)pTime);
}

void WINAPI PutDateTime(void* pDate, void* pTime)
{
	PutDate(pDate);
	PutTime(pTime);
}

void WINAPI PutDateTime(LPCTSTR pDate, LPCTSTR pTime)
{
	PutDateTime((void*)pDate, (void*)pTime);
}

//////////////////////////////////////////////////////////////////////
//	EBCDIC
//////////////////////////////////////////////////////////////////////
BYTE   WINAPI A2E(BYTE Src)
{
	BYTE	Dest;
	CodeCvt((BYTE*)&Src, (BYTE*)&Dest, 1, ASCEBC);
	return Dest;
}

CString WINAPI A2E(void* vSrc, int Length)
{
	BYTE*	bSrc = (BYTE*)vSrc;
	char	str[STRBUFFSIZE];
	CString	cstrstr("");

	for (int i = 0; i < Length; i++)
		str[i] = A2E(bSrc[i]);

	Strcpy(cstrstr, str, Length);
	return cstrstr;
}

CString WINAPI A2E(LPCTSTR vSrc, int Length)
{
	return (A2E((void*)vSrc, Length));
}

CString WINAPI A2E(void* vSrc)
{
	return (A2E(vSrc, strlen((char*)vSrc)));
}

CString WINAPI A2E(LPCTSTR vSrc)
{
	return (A2E((void*)vSrc, strlen(vSrc)));
}

BYTE WINAPI E2A(BYTE Src)
{
	BYTE	Dest;
	CodeCvt((BYTE*)&Src, (BYTE*)&Dest, 1, EBCASC);
	if ((Dest < ' ')	||										// spec. clear
		(Dest >= 0x80))											// han, graphics clear
		Dest = ' ';
	return Dest;
}

CString WINAPI E2A(void* vSrc, int Length)
{
	BYTE*	bSrc = (BYTE*)vSrc;
	char	str[STRBUFFSIZE];
	CString	cstrstr("");

	for (int i = 0; i < Length; i++)
		str[i] = E2A(bSrc[i]);

	Strcpy(cstrstr, str, Length);
	return cstrstr;
}

CString WINAPI E2A(LPCTSTR vSrc, int Length)
{
	return (E2A((void*)vSrc, Length));
}

CString WINAPI E2A(void* vSrc)
{
	return (E2A(vSrc, strlen((char*)vSrc)));
}

CString WINAPI E2A(LPCTSTR vSrc)
{
	return (E2A((void*)vSrc, strlen(vSrc)));
}

//////////////////////////////////////////////////////////////////////
int  WINAPI JuminCheck(void* inbuf)
{
	BYTE*	binbuf = (BYTE*)inbuf;
	int		Weight[12] = { 2, 3, 4, 5, 6, 7, 8, 9, 2, 3, 4, 5 };
	int		Module = 11;
	int		Remainder = 0;
	int		Sum = 0;

	for (int i = 0; i < 12; i++)
		Sum += ((binbuf[i] & 0x0f) * Weight[i]);
	Remainder = Sum % Module;
	if (Remainder == 0) 
		Remainder = 1;
	else
	if (Remainder == 1) 
		Remainder = 0;
	else Remainder = Module - Remainder;

	if (binbuf[12] == (Remainder | '0'))
		return TRUE;
	else return FALSE;
}

int  WINAPI JuminCheck(LPCTSTR inbuf)
{
	return JuminCheck((void*)inbuf);
}

int  WINAPI AccountCheck(void* inbuf)
{
	BYTE*	binbuf = (BYTE*)inbuf;
	int		Weight[12] = { 7, 6, 5, 4, 3, 2, 7, 6, 5, 4, 3, 0 };
	int		Module = 11;
	int		Remainder = 0;
	int		Sum = 0;

	for (int i = 0; i < 12; i++)
		Sum += ((binbuf[i] & 0x0f) * Weight[i]);
	Remainder = Sum % Module;
	if (Remainder == 0) 
		Remainder = 0;
	else
	if (Remainder == 1) 
		Remainder = 1;
	else Remainder = Module - Remainder;

	if (binbuf[11] == (Remainder | '0'))
		return TRUE;
	else return FALSE;
}

int  WINAPI AccountCheck(LPCTSTR inbuf)
{
	return AccountCheck((void*)inbuf);
}

// Des Encrypt Decrypt(Mode 0:Decrypt, 1:Encrypt)
int WINAPI DesEncryptDecrypt(UCHAR*  szKey, UCHAR* szData, int Mode)
{
	int     valindex;
	int		i, j, k, iter;
	UCHAR   bufout[64];
	UCHAR 	kwork[56];
	UCHAR   worka[48];
	UCHAR   kn[48];
	UCHAR   buffer[64];
	UCHAR   key[64];
	UCHAR   nbrofshift;
	UCHAR   temp1;
	UCHAR   temp2;

																// Initialize The Tables
	static UCHAR	s1[4][16] = {								// Table - S1
		14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
		0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
		4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
		15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13  };

	static UCHAR	s2[4][16] = {								// Table - S2
		15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
		3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
		0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
		13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9  };

	static UCHAR	s3[4][16] = {								// Table - S3
		10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
		13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
		13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
		1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12  };

	static UCHAR	s4[4][16] = {								// Table - S4
		7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
		13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
		10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
		3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14  };

	static UCHAR	s5[4][16] = {								// Table - S5
		2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
		14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
		4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
		11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3  };

	static UCHAR	s6[4][16] = {								// Table - S6
		12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
		10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
		9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
		4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13  };

	static UCHAR	s7[4][16] = {								// Table - S7
		4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
		13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
		1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
		6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12  };

	static UCHAR	s8[4][16] = {								// Table - S8
		13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
		1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
		7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
		2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11  };

	static UCHAR	shift[16] = {								// Table - Shift
		1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1	      };


	static UCHAR	binary[64] = {								// Table - Binary
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1,
		0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1,
		1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1,
		1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1		  };

																// Main Process
	for (i = 0; i < 8; i++)										// Convert From 64-Bit Key Into 64-Byte Key
	{
		key[8*i]   = ((j = *(szKey + i)) / 128) % 2;
		key[8*i+1] = (j / 64) % 2;
		key[8*i+2] = (j / 32) % 2;
		key[8*i+3] = (j / 16) % 2;
		key[8*i+4] = (j / 8) % 2;
		key[8*i+5] = (j / 4) % 2;
		key[8*i+6] = (j / 2) % 2;
		key[8*i+7] = (j % 2);
	}

	for (i = 0; i < 8; i++)										// Convert From 64-Bit Data Into 64-Byte Data
	{
		buffer[8*i]   = ((j = *(szData + i)) / 128) % 2;
		buffer[8*i+1] = (j / 64) % 2;
		buffer[8*i+2] = (j / 32) % 2;
		buffer[8*i+3] = (j / 16) % 2;
		buffer[8*i+4] = (j / 8) % 2;
		buffer[8*i+5] = (j / 4) % 2;
		buffer[8*i+6] = (j / 2) % 2;
		buffer[8*i+7] = (j % 2);
	}

	bufout[ 0] = buffer[57];									// Initial Permutation of Data
	bufout[ 1] = buffer[49];
	bufout[ 2] = buffer[41];
	bufout[ 3] = buffer[33];
	bufout[ 4] = buffer[25];
	bufout[ 5] = buffer[17];
	bufout[ 6] = buffer[ 9];
	bufout[ 7] = buffer[ 1];
	bufout[ 8] = buffer[59];
	bufout[ 9] = buffer[51];
	bufout[10] = buffer[43];
	bufout[11] = buffer[35];
	bufout[12] = buffer[27];
	bufout[13] = buffer[19];
	bufout[14] = buffer[11];
	bufout[15] = buffer[ 3];
	bufout[16] = buffer[61];
	bufout[17] = buffer[53];
	bufout[18] = buffer[45];
	bufout[19] = buffer[37];
	bufout[20] = buffer[29];
	bufout[21] = buffer[21];
	bufout[22] = buffer[13];
	bufout[23] = buffer[ 5];
	bufout[24] = buffer[63];
	bufout[25] = buffer[55];
	bufout[26] = buffer[47];
	bufout[27] = buffer[39];
	bufout[28] = buffer[31];
	bufout[29] = buffer[23];
	bufout[30] = buffer[15];
	bufout[31] = buffer[ 7];
	bufout[32] = buffer[56];
	bufout[33] = buffer[48];
	bufout[34] = buffer[40];
	bufout[35] = buffer[32];
	bufout[36] = buffer[24];
	bufout[37] = buffer[16];
	bufout[38] = buffer[ 8];
	bufout[39] = buffer[ 0];
	bufout[40] = buffer[58];
	bufout[41] = buffer[50];
	bufout[42] = buffer[42];
	bufout[43] = buffer[34];
	bufout[44] = buffer[26];
	bufout[45] = buffer[18];
	bufout[46] = buffer[10];
	bufout[47] = buffer[ 2];
	bufout[48] = buffer[60];
	bufout[49] = buffer[52];
	bufout[50] = buffer[44];
	bufout[51] = buffer[36];
	bufout[52] = buffer[28];
	bufout[53] = buffer[20];
	bufout[54] = buffer[12];
	bufout[55] = buffer[ 4];
	bufout[56] = buffer[62];
	bufout[57] = buffer[54];
	bufout[58] = buffer[46];
	bufout[59] = buffer[38];
	bufout[60] = buffer[30];
	bufout[61] = buffer[22];
	bufout[62] = buffer[14];
	bufout[63] = buffer[ 6];

	kwork[ 0] = key[56];										// Initial Permutation of Key
	kwork[ 1] = key[48];
	kwork[ 2] = key[40];
	kwork[ 3] = key[32];
	kwork[ 4] = key[24];
	kwork[ 5] = key[16];
	kwork[ 6] = key[ 8];
	kwork[ 7] = key[ 0];
	kwork[ 8] = key[57];
	kwork[ 9] = key[49];
	kwork[10] = key[41];
	kwork[11] = key[33];
	kwork[12] = key[25];
	kwork[13] = key[17];
	kwork[14] = key[ 9];
	kwork[15] = key[ 1];
	kwork[16] = key[58];
	kwork[17] = key[50];
	kwork[18] = key[42];
	kwork[19] = key[34];
	kwork[20] = key[26];
	kwork[21] = key[18];
	kwork[22] = key[10];
	kwork[23] = key[ 2];
	kwork[24] = key[59];
	kwork[25] = key[51];
	kwork[26] = key[43];
	kwork[27] = key[35];
	kwork[28] = key[62];
	kwork[29] = key[54];
	kwork[30] = key[46];
	kwork[31] = key[38];
	kwork[32] = key[30];
	kwork[33] = key[22];
	kwork[34] = key[14];
	kwork[35] = key[ 6];
	kwork[36] = key[61];
	kwork[37] = key[53];
	kwork[38] = key[45];
	kwork[39] = key[37];
	kwork[40] = key[29];
	kwork[41] = key[21];
	kwork[42] = key[13];
	kwork[43] = key[ 5];
	kwork[44] = key[60];
	kwork[45] = key[52];
	kwork[46] = key[44];
	kwork[47] = key[36];
	kwork[48] = key[28];
	kwork[49] = key[20];
	kwork[50] = key[12];
	kwork[51] = key[ 4];
	kwork[52] = key[27];
	kwork[53] = key[19];
	kwork[54] = key[11];
	kwork[55] = key[ 3];

	for (iter = 1; iter < 17; iter++)							// 16 Iterations
	{
		for (i = 0; i < 32; i++)
			buffer[i] = bufout[32+i];
																// Calculation of F(R, K)
		worka[ 0] = buffer[31];									// Permute - E
		worka[ 1] = buffer[ 0];
		worka[ 2] = buffer[ 1];
		worka[ 3] = buffer[ 2];
		worka[ 4] = buffer[ 3];
		worka[ 5] = buffer[ 4];
		worka[ 6] = buffer[ 3];
		worka[ 7] = buffer[ 4];
		worka[ 8] = buffer[ 5];
		worka[ 9] = buffer[ 6];
		worka[10] = buffer[ 7];
		worka[11] = buffer[ 8];
		worka[12] = buffer[ 7];
		worka[13] = buffer[ 8];
		worka[14] = buffer[ 9];
		worka[15] = buffer[10];
		worka[16] = buffer[11];
		worka[17] = buffer[12];
		worka[18] = buffer[11];
		worka[19] = buffer[12];
		worka[20] = buffer[13];
		worka[21] = buffer[14];
		worka[22] = buffer[15];
		worka[23] = buffer[16];
		worka[24] = buffer[15];
		worka[25] = buffer[16];
		worka[26] = buffer[17];
		worka[27] = buffer[18];
		worka[28] = buffer[19];
		worka[29] = buffer[20];
		worka[30] = buffer[19];
		worka[31] = buffer[20];
		worka[32] = buffer[21];
		worka[33] = buffer[22];
		worka[34] = buffer[23];
		worka[35] = buffer[24];
		worka[36] = buffer[23];
		worka[37] = buffer[24];
		worka[38] = buffer[25];
		worka[39] = buffer[26];
		worka[40] = buffer[27];
		worka[41] = buffer[28];
		worka[42] = buffer[27];
		worka[43] = buffer[28];
		worka[44] = buffer[29];
		worka[45] = buffer[30];
		worka[46] = buffer[31];
		worka[47] = buffer[ 0];
																// KS Function Begin
		if (Mode == 1)											// Encrypt
		{
			nbrofshift = shift[iter-1];
			for (i = 0; i < nbrofshift; i++)
			{
				temp1 = kwork[0];
				temp2 = kwork[28];
				for (j = 0; j < 27; j++)
				{
					kwork[j]    = kwork[j+ 1];
					kwork[j+28] = kwork[j+29];
				}
				kwork[27] = temp1;
				kwork[55] = temp2;
			}
		}
		else													// Decrypt
		if (iter > 1)
		{
			nbrofshift = shift[17-iter];
			for (i = 0; i < nbrofshift; i++)
			{
				temp1 = kwork[27];
				temp2 = kwork[55];
				for (j = 27; j > 0; j--)
				{
					kwork[j]    = kwork[j- 1];
					kwork[j+28] = kwork[j+27];
				}
				kwork[ 0] = temp1;
				kwork[28] = temp2;
			}
		}

		kn[ 0] = kwork[13];										// Permute kwork - PC2
		kn[ 1] = kwork[16];
		kn[ 2] = kwork[10];
		kn[ 3] = kwork[23];
		kn[ 4] = kwork[ 0];
		kn[ 5] = kwork[ 4];
		kn[ 6] = kwork[ 2];
		kn[ 7] = kwork[27];
		kn[ 8] = kwork[14];
		kn[ 9] = kwork[ 5];
		kn[10] = kwork[20];
		kn[11] = kwork[ 9];
		kn[12] = kwork[22];
		kn[13] = kwork[18];
		kn[14] = kwork[11];
		kn[15] = kwork[ 3];
		kn[16] = kwork[25];
		kn[17] = kwork[ 7];
		kn[18] = kwork[15];
		kn[19] = kwork[ 6];
		kn[20] = kwork[26];
		kn[21] = kwork[19];
		kn[22] = kwork[12];
		kn[23] = kwork[ 1];
		kn[24] = kwork[40];
		kn[25] = kwork[51];
		kn[26] = kwork[30];
		kn[27] = kwork[36];
		kn[28] = kwork[46];
		kn[29] = kwork[54];
		kn[30] = kwork[29];
		kn[31] = kwork[39];
		kn[32] = kwork[50];
		kn[33] = kwork[44];
		kn[34] = kwork[32];
		kn[35] = kwork[47];
		kn[36] = kwork[43];
		kn[37] = kwork[48];
		kn[38] = kwork[38];
		kn[39] = kwork[55];
		kn[40] = kwork[33];
		kn[41] = kwork[52];
		kn[42] = kwork[45];
		kn[43] = kwork[41];
		kn[44] = kwork[49];
		kn[45] = kwork[35];
		kn[46] = kwork[28];
		kn[47] = kwork[31];
																// KS Function End

		for (i = 0; i < 48; i++)								// worka XOR kn
			worka[i] = worka[i] ^ kn[i];
																// 8 S-Functions
		valindex = s1[(2 * worka[ 0] + worka[ 5])][(2 * (2 * (2 * worka[ 1] + worka[ 2]) + worka[ 3]) + worka[ 4])];
		valindex = valindex * 4;
		kn[ 0] = binary[0+valindex];
		kn[ 1] = binary[1+valindex];
		kn[ 2] = binary[2+valindex];
		kn[ 3] = binary[3+valindex];

		valindex = s2[(2 * worka[ 6] + worka[11])][(2 * (2 * (2 * worka[ 7] + worka[ 8]) + worka[ 9]) + worka[10])];
		valindex = valindex * 4;
		kn[ 4] = binary[0+valindex];
		kn[ 5] = binary[1+valindex];
		kn[ 6] = binary[2+valindex];
		kn[ 7] = binary[3+valindex];

		valindex = s3[(2 * worka[12] + worka[17])][(2 * (2 * (2 * worka[13] + worka[14]) + worka[15]) + worka[16])];
		valindex = valindex * 4;
		kn[ 8] = binary[0+valindex];
		kn[ 9] = binary[1+valindex];
		kn[10] = binary[2+valindex];
		kn[11] = binary[3+valindex];

		valindex = s4[(2 * worka[18] + worka[23])][(2 * (2 * (2 * worka[19] + worka[20]) + worka[21]) + worka[22])];
		valindex = valindex * 4;
		kn[12] = binary[0+valindex];
		kn[13] = binary[1+valindex];
		kn[14] = binary[2+valindex];
		kn[15] = binary[3+valindex];

		valindex = s5[(2 * worka[24] + worka[29])][(2 * (2 * (2 * worka[25] + worka[26]) + worka[27]) + worka[28])];
		valindex = valindex * 4;
		kn[16] = binary[0+valindex];
		kn[17] = binary[1+valindex];
		kn[18] = binary[2+valindex];
		kn[19] = binary[3+valindex];

		valindex = s6[(2 * worka[30] + worka[35])][(2 * (2 * (2 * worka[31] + worka[32]) + worka[33]) + worka[34])];
		valindex = valindex * 4;
		kn[20] = binary[0+valindex];
		kn[21] = binary[1+valindex];
		kn[22] = binary[2+valindex];
		kn[23] = binary[3+valindex];
		
		valindex = s7[(2 * worka[36] + worka[41])][(2 * (2 * (2 * worka[37] + worka[38]) + worka[39]) + worka[40])];
		valindex = valindex * 4;
		kn[24] = binary[0+valindex];
		kn[25] = binary[1+valindex];
		kn[26] = binary[2+valindex];
		kn[27] = binary[3+valindex];
		
		valindex = s8[(2 * worka[42] + worka[47])][(2 * (2 * (2 * worka[43] + worka[44]) + worka[45]) + worka[46])];
		valindex = valindex * 4;
		kn[28] = binary[0+valindex];
		kn[29] = binary[1+valindex];
		kn[30] = binary[2+valindex];
		kn[31] = binary[3+valindex];

		worka[ 0] = kn[15];										// Permute - P
		worka[ 1] = kn[ 6];
		worka[ 2] = kn[19];
		worka[ 3] = kn[20];
		worka[ 4] = kn[28];
		worka[ 5] = kn[11];
		worka[ 6] = kn[27];
		worka[ 7] = kn[16];
		worka[ 8] = kn[ 0];
		worka[ 9] = kn[14];
		worka[10] = kn[22];
		worka[11] = kn[25];
		worka[12] = kn[ 4];
		worka[13] = kn[17];
		worka[14] = kn[30];
		worka[15] = kn[ 9];
		worka[16] = kn[ 1];
		worka[17] = kn[ 7];
		worka[18] = kn[23];
		worka[19] = kn[13];
		worka[20] = kn[31];
		worka[21] = kn[26];
		worka[22] = kn[ 2];
		worka[23] = kn[ 8];
		worka[24] = kn[18];
		worka[25] = kn[12];
		worka[26] = kn[29];
		worka[27] = kn[ 5];
		worka[28] = kn[21];
		worka[29] = kn[10];
		worka[30] = kn[ 3];
		worka[31] = kn[24];

		for (i = 0; i < 32; i++)
		{
			bufout[i+32] = bufout[i] ^ worka[i];
			bufout[i]    = buffer[i];
		}
	}															// End of Iter

	for (i = 0; i < 32; i++)									// Prepare Output
	{
		j = bufout[i];
		bufout[i]    = bufout[32+i];
		bufout[32+i] = j;
	}

	buffer[ 0] = bufout[39];									// Inverse Initial Permutation
	buffer[ 1] = bufout[ 7];
	buffer[ 2] = bufout[47];
	buffer[ 3] = bufout[15];
	buffer[ 4] = bufout[55];
	buffer[ 5] = bufout[23];
	buffer[ 6] = bufout[63];
	buffer[ 7] = bufout[31];
	buffer[ 8] = bufout[38];
	buffer[ 9] = bufout[ 6];
	buffer[10] = bufout[46];
	buffer[11] = bufout[14];
	buffer[12] = bufout[54];
	buffer[13] = bufout[22];
	buffer[14] = bufout[62];
	buffer[15] = bufout[30];
	buffer[16] = bufout[37];
	buffer[17] = bufout[ 5];
	buffer[18] = bufout[45];
	buffer[19] = bufout[13];
	buffer[20] = bufout[53];
	buffer[21] = bufout[21];
	buffer[22] = bufout[61];
	buffer[23] = bufout[29];
	buffer[24] = bufout[36];
	buffer[25] = bufout[ 4];
	buffer[26] = bufout[44];
	buffer[27] = bufout[12];
	buffer[28] = bufout[52];
	buffer[29] = bufout[20];
	buffer[30] = bufout[60];
	buffer[31] = bufout[28];
	buffer[32] = bufout[35];
	buffer[33] = bufout[ 3];
	buffer[34] = bufout[43];
	buffer[35] = bufout[11];
	buffer[36] = bufout[51];
	buffer[37] = bufout[19];
	buffer[38] = bufout[59];
	buffer[39] = bufout[27];
	buffer[40] = bufout[34];
	buffer[41] = bufout[ 2];
	buffer[42] = bufout[42];
	buffer[43] = bufout[10];
	buffer[44] = bufout[50];
	buffer[45] = bufout[18];
	buffer[46] = bufout[58];
	buffer[47] = bufout[26];
	buffer[48] = bufout[33];
	buffer[49] = bufout[ 1];
	buffer[50] = bufout[41];
	buffer[51] = bufout[ 9];
	buffer[52] = bufout[49];
	buffer[53] = bufout[17];
	buffer[54] = bufout[57];
	buffer[55] = bufout[25];
	buffer[56] = bufout[32];
	buffer[57] = bufout[ 0];
	buffer[58] = bufout[40];
	buffer[59] = bufout[ 8];
	buffer[60] = bufout[48];
	buffer[61] = bufout[16];
	buffer[62] = bufout[56];
	buffer[63] = bufout[24];

	for (i = 0, j = 0; i < 8; i++)
	{
		*(szData + i) = 0x00;

		for (k = 0; k < 7; k++)
			*(szData + i) = ((*(szData + i)) + buffer[j+k]) * 2;

		*(szData + i) = *(szData + i) + buffer[j+7];

		j += 8;
	}

	return TRUE;
}

